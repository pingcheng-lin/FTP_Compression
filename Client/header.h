#include <netinet/in.h> //struct sockaddr_in, AF_INET, SOCK_STREAM, IPPROTO_TCP, INADDR_ANY
#include <stdio.h>
#include <stdlib.h> //exit
#include <unistd.h> //read, write
#include <arpa/inet.h> //inet_addr
#include <string.h> //strcmp
#include <time.h> //current time
#define MAX_FILENAME_SIZE 30
struct Node {
    char word;
    int freq;
    struct Node *right, *left;
};
int input(struct sockaddr_in *srv, char *filename);
void huffman();