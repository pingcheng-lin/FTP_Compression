#include <netinet/in.h> //struct sockaddr_in, AF_INET, SOCK_STREAM, IPPROTO_TCP, INADDR_ANY
#include <stdio.h>
#include <stdlib.h> //exit
#include <unistd.h> //read, write
#include <arpa/inet.h> //inet_addr
#include <string.h> //strcmp
void input_link(struct sockaddr_in *srv);
int input();
void sendd(char *file);

int main() {
    int fd; /* socket descriptor */
    struct sockaddr_in srv; /* used by connect() */
    char buf[512]; /* used by write() */
    int nbytes; /* used by write() */
    int ing = 1;
    printf("Welcome\nYou can 'link' [an IP address] [a port], 'send' [a file], or 'leave'.\n===\n");
    
    
    /* Get the file descriptor */
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0) {
        printf("Error");
        exit(1);
    }
    
    
    /* Connect the file descriptor to the server’s IP and port */
    srv.sin_family = AF_INET;
    input_link(&srv);
    if(connect(fd, (struct sockaddr*) &srv, sizeof(srv)) < 0) {
        perror("connect"); 
        exit(1);
    }
    printf("The server with IP address \"%s\" has accepted your connection.\n", inet_ntoa(srv.sin_addr));
    

    /* Write data to the socket */
    char filename[30];
    input(filename);
    FILE *file = fopen(filename, "r");
    int num, total = 0;
    if((nbytes = write(fd, filename, sizeof(buf))) < 0) { //send filename
        perror("write");
        exit(1);
    }
    fseek(file, 0 , SEEK_END);
    int filesize = ftell(file);
    fseek(file, 0 , SEEK_SET);
    printf("%d\n", filesize);
    if((nbytes = write(fd, &filesize, sizeof(filesize))) < 0) { //send filesize
        perror("write");
        exit(1);
    }
    while(!feof(file)) { //send file
        num = fread(buf, sizeof(char), sizeof(buf), file);
        total += num;
        if((nbytes = write(fd, buf, num)) < 0) {
            perror("write");
            exit(1);
        }
    }
    printf("Original file length: %d bytes, compressed file length: %d bytes (ratio:)\n", filesize, total); 
    printf("Time to upload:\n");
    printf("Using fixed-length codeword (3 bits)\n");
    printf("Bye bye.\n");
    return 0;
}

void input_link(struct sockaddr_in *srv) {
    char command[5];
    char ip_address[15];
    int port;
    scanf("%s", command);
    while(strcmp("link", command) != 0) {
        scanf("%s", command);
    }
    scanf("%s", ip_address);
    scanf("%d", &port);
    srv->sin_port = htons(port); /* connect: socket 'fd' to port 80 */
    srv->sin_addr.s_addr = inet_addr(ip_address); /* connect: connect to IP address “140.117.11.87” */
}
int input(char *filename) {
    char command[5];
    scanf("%s", command);
    while(1) {
        if(strcmp("send", command) == 0) {
            scanf("%s", filename);
            return 1;
        }
        else if(strcmp("leave", command) == 0)
            return 0;
        else
            printf("Need 'link' [an IP address] [a port], 'send' [a file], or 'leave'.");
    }
    
}
void sendd(char *file) {
    printf("1\n");
}