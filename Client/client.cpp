#include"header.hpp"
int main() {
    int fd; //socket descriptor
    struct sockaddr_in srv; //used by connect()
    char buf[512]; //used by write()
    int nbytes; //used by write()


    cout << "Welcome\nYou can 'link' [an IP address] [a port], 'send' [a file], 'leave', or 'help'.\n";
    cout << "===\nWaiting...\n";
    //Get the file descriptor
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0) {
        cout << "Error\n";
        exit(1);
    }
    bool do_you_link = false;
    while(1) {
        char filename[MAX_FILENAME_SIZE];
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
            //Write data to the socket
            FILE *file;
            //send flag
            if((nbytes = write(fd, "send", sizeof(buf))) < 0) { 
                perror("write");
                exit(1);
            }
            //open file
            if((file = fopen(filename, "r")) == NULL) {
                if((nbytes = write(fd, "error", sizeof(buf))) < 0) { 
                    perror("write");
                    exit(1);
                }
                perror("fopen");
                exit(1);
            }
            //send filename
            if((nbytes = write(fd, filename, sizeof(buf))) < 0) { 
                perror("write");
                exit(1);
            }

            //do huffman and send compress-related file
            huffman(filename, fd);
            fseek(file, 0 , SEEK_SET);

            //send filesize
            fseek(file, 0 , SEEK_END);
            int filesize = ftell(file);
            fseek(file, 0 , SEEK_SET);
            if((nbytes = write(fd, &filesize, sizeof(filesize))) < 0) { 
                perror("write");
                exit(1);
            }
            //send file
            while(!feof(file)) {
                int num = fread(buf, sizeof(char), sizeof(buf), file);
                if((nbytes = write(fd, buf, num)) < 0) {
                    perror("write");
                    exit(1);
                }
            }
            cout << "Original file length: " << filesize << " bytes, compressed file length:" << filesize << " bytes (ratio:)\n";
            time_t t = time(NULL);
            struct tm time = *localtime(&t);
            cout << "Time to upload:" << time.tm_year + 1900 << "/" << time.tm_mon + 1 << "/" << time.tm_mday
                 << " " << time.tm_hour << ":" << time.tm_min << endl;
            cout << "Using fixed-length codeword (3 bits)\n";
            cout << "===\nwaiting...\n";
        }
        else if(flag == 3){
            if((nbytes = write(fd, "leave", sizeof(buf))) < 0) { 
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
    return 0;
}