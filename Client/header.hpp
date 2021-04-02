#include <netinet/in.h> //struct sockaddr_in, AF_INET, SOCK_STREAM, IPPROTO_TCP, INADDR_ANY
#include <unistd.h> //read, write
#include <arpa/inet.h> //inet_addr
#define MAX_FILENAME_SIZE 30
#include <map>
#include <queue>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath> //pow
#include <cstdlib> //exit
#include <cstring> //strcmp
#include <ctime> //current time
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
class Compare {
public:
    bool operator()(Node* first, Node* second);
};
int input(struct sockaddr_in *srv, string &filename);
void huffman(string filename, int fd);
//void travel_huff_code(Node* root, string flag);
