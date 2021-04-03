#include"header.hpp"
int main() {
    int fd; //socket descriptor
    struct sockaddr_in srv; //used by connect()
    char buf[512]; //used by write()

    cout << "Welcome\nYou can 'link' [an IP address] [a port], 'send' [a file], 'leave', or 'help'.\n";
    cout << "===\nWaiting...\n";
    //Get the file descriptor
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0) {
        cout << "Error\n";
        exit(1);
    }

    //determine what to do according to input 
    bool do_you_link = false;
    while(1) {
        string filename;
        int flag = input(&srv, filename); //1: link, 2: send, 3:leave
        if(flag == 1 && !do_you_link) {
            do_you_link = true;
            //Connect the file descriptor to the server’s IP and port
            srv.sin_family = AF_INET;
            if(connect(fd, (struct sockaddr*) &srv, sizeof(srv)) < 0) {
                perror("connect"); 
                exit(1);
            }
            cout << "The server with IP address \"" << inet_ntoa(srv.sin_addr) << "\" has accepted your connection.\n";
            cout << "===\nwaiting...\n";
        }
        else if(flag == 2) {
            if(!do_you_link) {
                cout << "link first!!!\n";
                cout << "===\nwaiting...\n";
                continue;
            }
            //send flag
            if(write(fd, "send", sizeof(buf)) < 0) { 
                perror("write");
                exit(1);
            }
            //open file
            fstream file;
            file.open(filename, ios::in | ios::binary);
            if(!file.is_open()) {
                if(write(fd, "error", sizeof(buf)) < 0) { 
                    perror("write");
                    exit(1);
                }
                perror("open");
                exit(1);
            }
            //send filename
            if(write(fd, filename.c_str(), sizeof(buf)) < 0) { 
                perror("write");
                exit(1);
            }
            //send filesize
            file.seekg(0 , file.end);
            int filesize = file.tellg();
            file.seekg(0 , file.beg);
            if(write(fd, &filesize, sizeof(filesize)) < 0) { 
                perror("write");
                exit(1);
            }
            file.close();
            //do huffman and send compress-related file
            huffman(filename, fd);
            

            //send com_filesize
            fstream com_file("compressed-" + filename, ios::in);
            if(!com_file.is_open())
                cout << "WTF!!!!!!\n";
            com_file.seekg(0 , com_file.end);
            int com_filesize = com_file.tellg();
            com_file.seekg(0 , com_file.beg);
            if(write(fd, &com_filesize, sizeof(filesize)) < 0) { 
                perror("write");
                exit(1);
            }
            //send com_file
            while(!com_file.eof()) {
                com_file.read(buf, sizeof(buf));
                if(write(fd, buf, com_file.gcount()) < 0) {
                    perror("write");
                    exit(1);
                }
            }
            cout << "Original file length: " << filesize << " bytes, compressed file length: " << com_filesize
                 << " bytes (ratio:" << 100.0 * com_filesize / filesize << "%)\n";
            time_t t = time(NULL);
            struct tm time = *localtime(&t);
            cout << "Time to upload:" << time.tm_year + 1900 << "/" << time.tm_mon + 1 << "/" << time.tm_mday
                 << " " << time.tm_hour << ":" << time.tm_min << endl;
            cout << "Using fixed-length codeword (3 bits)\n";
            cout << "===\nwaiting...\n";
            com_file.close();
        }
        else if(flag == 3){
            if(write(fd, "leave", sizeof(buf)) < 0) { 
                perror("write");
                exit(1);
            }
            cout << "Bye bye.\n";
            break;
        }
        else if(flag == 4) {
            cout << "You can 'link' [an IP address] [a port], 'send' [a file], or 'leave'.\n";
            cout << "===\nwaiting...\n";
        }
    }
    close(fd);
    return 0;
}