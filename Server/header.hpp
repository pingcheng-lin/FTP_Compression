#include <netinet/in.h> //struct sockaddr_in, AF_INET, SOCK_STREAM, IPPROTO_TCP, INADDR_ANY
#include <unistd.h> //read, write
#include <arpa/inet.h> //inet_ntoa
#define MAX_FILENAME_SIZE 30
#include <iostream>
#include <cstdlib> //exit
#include <cstring> //strcpy
#include <fstream>
using namespace std;
void decode(string filename, int filesize, string related_filename);