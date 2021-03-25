#include <netinet/in.h> //struct sockaddr_in, AF_INET, SOCK_STREAM, IPPROTO_TCP, INADDR_ANY
#include <stdio.h>
#include <stdlib.h> //exit
#include <unistd.h> //read, write
#include <arpa/inet.h> //inet_addr

void linkk(char *IP, int port);
void leave();
void sendd(char *file);

int main() {
    int fd; /* socket descriptor */
    struct sockaddr_in srv; /* used by connect() */
    char buf[512]; /* used by write() */
    int nbytes; /* used by write() */

    /* Get the file descriptor */
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd == -1) {
        printf("Error");
        return -1;
    }

    /* Connect the file descriptor to the server’s IP and port */
    srv.sin_family = AF_INET;
    srv.sin_port = htons(80); /* connect: socket ‘fd’ to port 80 */
    srv.sin_addr.s_addr = inet_addr("140.117.11.87"); /* connect: connect to IP address “140.117.11.87” */
    if(connect(fd, (struct sockaddr*) &srv, sizeof(srv)) < 0) {
        perror("connect"); exit(1);
    }

    /* Write data to the socket */
    if((nbytes = write(fd, buf, sizeof(buf))) < 0) {
        perror("write");
        exit(1);
    }

    return 0;
}

void linkk(char *IP, int port) {
    printf("1\n");
}

void leave() {
    printf("1\n");
}

void sendd(char *file) {
    printf("1\n");
}