#include <netinet/in.h> //struct sockaddr_in, AF_INET, SOCK_STREAM, IPPROTO_TCP, INADDR_ANY
#include <stdio.h>
#include <stdlib.h> //exit
#include <unistd.h> //read, write
#include <arpa/inet.h> //inet_addr
#include <string.h> //strcmp
#include <time.h> //current time
#include <queue>
#define MAX_FILENAME_SIZE 30
#include<map>
#include<queue>
#include<iostream>
#include<string>
#include<fstream>
#include<bitset>
#include<cmath>
using namespace std;
class Node {
public:
    Node(char c, int f);
    Node(Node* first, Node* second);
    char letter;
    int freq;
    string code;
    Node *right = nullptr, *left = nullptr;
};
int input(struct sockaddr_in *srv, char *filename);
void huffman(string filename);
void travel_huff_code(Node* root, string flag);
