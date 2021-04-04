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
    Node(unsigned char c, int f);
    Node(Node* first, Node* second);
    unsigned char letter;
    int freq;
    string code;
    Node *right = nullptr, *left = nullptr;
};
class Compare { //compare the frequency to determine the priority for variable length
public:
    bool operator()(Node* first, Node* second);
};
int input(struct sockaddr_in *srv, string &filename);
string huffman(string filename, int fd);
void travel_huff_code(Node* root, string parent_code, string flag);
void travel_get_code(Node* node, map<unsigned char, string> &table);
void build_var_table(priority_queue<Node*, vector<Node*>, Compare> &var_node, map<unsigned char, string> &table);
void build_fix_table(priority_queue<Node*, vector<Node*>, greater<Node*>> &fix_node, map<unsigned char, string> &table);
void encode(string filename, map<unsigned char, string> &fix_table);
