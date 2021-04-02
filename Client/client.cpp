#include"header.hpp"
int main() {
    int fd; //socket descriptor
    struct sockaddr_in srv; //used by connect()
    char buf[512]; //used by write()
    int nbytes; //used by write()
    printf("Welcome\nYou can 'link' [an IP address] [a port], 'send' [a file], 'leave', or 'help'.\n");
    printf("===\nWaiting...\n");
    //Get the file descriptor
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0) {
        printf("Error");
        exit(1);
    }
    
    while(1) {
        char filename[MAX_FILENAME_SIZE];
        int flag = input(&srv, filename); //1: link, 2: send, 3:leave
        if(flag == 1) {
            //Connect the file descriptor to the server’s IP and port
            srv.sin_family = AF_INET;
            if(connect(fd, (struct sockaddr*) &srv, sizeof(srv)) < 0) {
                perror("connect"); 
                exit(1);
            }
            printf("The server with IP address \"%s\" has accepted your connection.\n", inet_ntoa(srv.sin_addr));
            printf("===\nwaiting...\n");
        }
        if(flag == 2) {
            //Write data to the socket
            FILE *file;
            int num, total = 0;
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
            //send filesize
            fseek(file, 0 , SEEK_END);
            int filesize = ftell(file);
            fseek(file, 0 , SEEK_SET);
            if((nbytes = write(fd, &filesize, sizeof(filesize))) < 0) { 
                perror("write");
                exit(1);
            }
            //do huffman
            huffman(file);
            fseek(file, 0 , SEEK_SET);
            //send file
            while(!feof(file)) {
                num = fread(buf, sizeof(char), sizeof(buf), file);
                total += num;
                if((nbytes = write(fd, buf, num)) < 0) {
                    perror("write");
                    exit(1);
                }
            }
            printf("Original file length: %d bytes, compressed file length: %d bytes (ratio:)\n", filesize, total);
            time_t t = time(NULL);
            struct tm time = *localtime(&t);
            printf("Time to upload: %d/%02d/%02d %02d:%02d\n", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min);
            printf("Using fixed-length codeword (3 bits)\n");
            printf("===\nwaiting...\n");
        }
        if(flag == 3){
            if((nbytes = write(fd, "leave", sizeof(buf))) < 0) { 
                perror("write");
                exit(1);
            }
            printf("Bye bye.\n");
            break;
        }
        if(flag == 4) {
            printf("You can 'link' [an IP address] [a port], 'send' [a file], or 'leave'.\n");
            printf("===\nwaiting...\n");
        }
    }
    return 0;
}