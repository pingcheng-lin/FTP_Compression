#include <netinet/in.h> //struct sockaddr_in, AF_INET, SOCK_STREAM, IPPROTO_TCP, INADDR_ANY
#include <stdio.h>
#include <stdlib.h> //exit
#include <unistd.h> //read, write
#include <arpa/inet.h> //inet_ntoa
int main() {
    int fd;
    struct sockaddr_in srv; /* used by bind() */
    struct sockaddr_in cli; /* used by accept() */
    int newfd; /* returned by accept() */
    int cli_len = sizeof(cli); /* used by accept() */
    char buf[512]; /* used by read() */
    int nbytes; /* used by read() */

    /* Get the file descriptor */
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0) {
        printf("Error");
       exit(1);
    }

    srv.sin_family = AF_INET; /* use the Internet address family */
    srv.sin_port = htons(21); /* bind socket 'fd' to port 80 */
    srv.sin_addr.s_addr = htonl(INADDR_ANY);
    /* Connect the file descriptor with the IP address and port */
    if(bind(fd, (struct sockaddr*) &srv, sizeof(srv)) < 0) {
        perror("bind"); 
        exit(1);
    }
    /* Wait for requests from clients */
    if(listen(fd, 5) < 0) {
        perror("listen");
        exit(1);
    }
    else {
        printf("Listening...\n");
    }
    /* Accept requests */
    newfd = accept(fd, (struct sockaddr*) &cli, &cli_len);
    if(newfd < 0) {
        perror("accept");
        exit(1);
    }
    printf("A client \"%s\" has connected via port num %d using SOCK_STREAM (TCP)\n", inet_ntoa(cli.sin_addr), ntohs(srv.sin_port));
    /* Read data from the socket */
    FILE *file = fopen(????);
    fwrite(buf, sizeof(char), sizeof(buf), file);
    if((nbytes = read(newfd, buf, sizeof(buf))) < 0) {
        perror("read");
        exit(1);
    }
    else {
        printf("The client sends a file \"1.txt\" with size of %d bytes.\n", nbytes);
        printf("The Huffman coding data are stored in \"1-code.txt\".\n");
    }
    printf("The client \"%s\" with port %d has terminated the connection.\n", inet_ntoa(cli.sin_addr), ntohs(srv.sin_port));
    return 0;
}