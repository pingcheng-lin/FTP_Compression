#include"header.h"
int input(struct sockaddr_in *srv, char *filename) {
    char command[5], ip_address[15];
    int port;
    while(1) {
        scanf("%s", command);
        if(strcmp("link", command) == 0) {
            scanf("%s", ip_address);
            srv->sin_addr.s_addr = inet_addr(ip_address); //connect: connect to IP address
            scanf("%d", &port);
            srv->sin_port = htons(port); //connect: socket 'fd' to port
            return 1;
        }
        else if(strcmp("send", command) == 0) {
            scanf("%s", filename);
            return 2;
        }
        else if(strcmp("leave", command) == 0)
            return 3;
        else if(strcmp("help", command) == 0)
            return 4;
        else
            printf("Need 'link' [an IP address] [a port], 'send' [a file], or 'leave'.\n");
    }
}

void huffman() {

}