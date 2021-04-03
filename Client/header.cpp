#include"header.hpp"
int input(struct sockaddr_in *srv, string &filename) {
    string command, ip_address;
    int port;
    while(1) {
        cin >> command;
        if(command == "link") {
            cin >> ip_address;
            srv->sin_addr.s_addr = inet_addr(ip_address.c_str()); //connect: connect to IP address
            cin >> port;
            srv->sin_port = htons(port); //connect: socket 'fd' to port
            return 1;
        }
        else if(command == "send") {
            cin >> filename;
            return 2;
        }
        else if(command == "leave")
            return 3;
        else if(command == "help")
            return 4;
        else
            cout << "Need 'link' [an IP address] [a port], 'send' [a file], or 'leave'.\n";
    }
}
Node::Node(unsigned char c, int f) {
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
bool Compare::operator()(Node* first, Node* second) {
    return first->freq > second->freq;
}
void huffman(string filename, int fd) {
    //get char and frequency mapping
    fstream file(filename, ios::in | ios::binary);
    map<unsigned char, int> ch_freq;
    map<unsigned char, int>::iterator it;
    unsigned char ch;
    while(!file.eof()) {
        ch = file.get();
        ch_freq[ch]++;
    }
    //push all map into priority queue
    //priority_queue<Node*, vector<Node*>, Compare> queue;
    priority_queue<Node*, vector<Node*>, greater<Node*>> fixed_ch_code;
    for(it = ch_freq.begin(); it != ch_freq.end(); it++) {
        Node* temp = new Node(it->first, it->second);
        //queue.push(temp);
        fixed_ch_code.push(temp);
    }
    //calculate fixed binary
    map<unsigned char, string> fixed_table;
    int count = 0, len;
    string binary;
    for(len = 0; fixed_ch_code.size() > pow(2,len); len++);
    while(!fixed_ch_code.empty()) {
        binary = "";
        int temp = count;
        for(int i = 0; i < len; i++) {
            if(temp & 1 == 1)
                binary = "1" + binary;
            else
                binary = "0" + binary;
            temp >>= 1;
        }
        fixed_table[fixed_ch_code.top()->letter] = binary;
        count++;
        fixed_ch_code.pop();
    }
    file.close();
    fstream fixed_file;
    string related_filename = "code-" + filename + ".txt";
    fixed_file.open(related_filename, ios::out | ios::binary);
    encode(filename, fixed_table);
    fixed_file << "Fixed-length Huffman coding:\n";
    for(map<unsigned char, string>::iterator ita = fixed_table.begin(); ita != fixed_table.end(); ita++) {
        fixed_file << ita->first << "\t" << ch_freq.find(ita->first)->second << "\t" << ita->second << endl;
    }
    
    //send related_file name
    char buf[512]; //used by write()
    if(write(fd, related_filename.c_str(), sizeof(buf)) < 0) { 
        perror("write");
        exit(1);
    }
    
    //send related_file size
    fstream related_file(related_filename, ios::in);
    related_file.seekg(0 , related_file.end);
    int filesize = related_file.tellg();
    related_file.seekg(0 , related_file.beg);
    if(write(fd, &filesize, sizeof(filesize)) < 0) { 
        perror("write");
        exit(1);
    }
    //send related_file
    while(!related_file.eof()) {
        related_file.read(buf, sizeof(buf));
        if(write(fd, buf, related_file.gcount()) < 0) {
            perror("write");
            exit(1);
        }
    }
}
void encode(string filename, map<unsigned char, string> &table) {
    fstream org_file(filename, ios::in);
    fstream com_file(filename + ".zip", ios::out);
    int i = 0;
    int count = 0; //count temp binary length
    int total = 0, len;
    cout << "Start encode...\n";
    while(!org_file.eof()) {
        unsigned char ch = org_file.get();
        len = table[ch].length()-1;
        for(i = 0; i <= len; i++) {
            total += (table[ch][i] - '0') * pow(2, 7-count);
            count++;
            if(count == 8) {
                com_file.put(static_cast<unsigned char> (total));
                total = 0;
                count = 0;
            }
        }
    }
    com_file.put(static_cast<unsigned char> (total));
    org_file.close();
    com_file.close();
    cout << "Encode complete.\n";
}

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
    map<ch, string> ch_code;
    //travel_get_code(ch_code);
    */

/*void travel_huff_code(Node* node, string flag) {
    if(node == nullptr)
        return;
    node->code = node->code + flag;
    travel_huff_code(node->left, "0");
    travel_huff_code(node->right, "1");
}*/