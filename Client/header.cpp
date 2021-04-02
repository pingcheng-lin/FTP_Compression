#include"header.hpp"
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

class compare {
public:
    bool operator()(Node* first, Node* second) {
        return first->freq > second->freq;
    }
};
Node::Node(char c, int f) {
    letter = c;
    freq = f;
    code = "";
    right = nullptr;
    left = nullptr;
}
Node::Node(Node* first, Node* second) {
    letter = first->letter + second->letter;
    freq = first->freq + second->freq;
    code = "";
    right = first;
    left = second;
}
void huffman(string filename) {
    //get char and frequency mapping
    fstream file;
    file.open(filename, ios::in | ios::binary);
    map<char, int> ch_freq;
    map<char, int>::iterator it;
    char ch;
    while(!file.eof()) {
        ch = file.get();
        ch_freq[ch]++;
    }
    //push all map into priority queue
    priority_queue<Node*, vector<Node*>, compare> queue;
    for(it = ch_freq.begin(); it != ch_freq.end(); it++) {
        Node* temp = new Node(it->first, it->second);
        queue.push(temp);
    }
    //calculate fixed binary
    priority_queue<Node*, vector<Node*>, compare> fixed_ch_code = queue;
    map<char, string> fixed_table;
    int count = 0, len;
    for(len = 0; fixed_ch_code.size() > pow(2,len); len++);
        string binary = "";
        int temp = count;
        for(int i = 0; i < len; i++) {
            if(temp & 1 == 1)
                binary = "1" + binary;
            else
                binary = "0" + binary;
            temp >>= 1;
        }
        fixed_table[fixed_ch_code.top()->letter] = binary;
        cout << fixed_ch_code.top()->letter << "=>" << binary << endl;
        count++;
        fixed_ch_code.pop();
    }
    file.seekg(0, file.beg);
    
    //build huffman tree
    /*Node* root;
    while(1) {
        Node* first = queue.top();
        queue.pop();
        Node* second = queue.top();
        queue.pop();
        Node* new_node = new Node(first, second);
        queue.push(new_node);
        if(queue.empty()) {
            root = new_node;
            break;
        }
    }
    travel_huff_code(root, "");
    map<char, string> char_code;
    //travel_get_code(char_code);
    */

}

void travel_huff_code(Node* node, string flag) {
    if(node == nullptr)
        return;
    node->code = node->code + flag;
    travel_huff_code(node->left, "0");
    travel_huff_code(node->right, "1");
}