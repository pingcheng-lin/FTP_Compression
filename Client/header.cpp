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
    letter = 0;
    freq = first->freq + second->freq;
    code = "";
    left = first;
    right = second;
}
bool Compare::operator()(Node* first, Node* second) {
    return first->freq > second->freq;
}
string huffman(string filename, int fd) {
    //get char and frequency mapping from original file
    fstream file(filename, ios::in | ios::binary);
    map<unsigned char, int> ch_freq;
    unsigned char ch;
    while(!file.eof()) {
        ch = file.get();
        ch_freq[ch]++;
    }
    file.close();

    //push all map into priority queue
    map<unsigned char, int>::iterator it;
    priority_queue<Node*, vector<Node*>, Compare> var_node;
    priority_queue<Node*, vector<Node*>, greater<Node*>> fix_node;
    for(it = ch_freq.begin(); it != ch_freq.end(); it++) {
        Node* temp = new Node(it->first, it->second);
        var_node.push(temp);
        fix_node.push(temp);
    }

    //Determine variable or fixed
    map<unsigned char, string> table;
    string rel_filename;
    string var_or_fix;
    cout << "If you want variable-length huffman, enter 'var'.\n";
    cout << "If you want fixed-length huffman, enter 'fix'.\n";
    while(1) {
        cout << "=> ";
        cin >> var_or_fix;
        if(var_or_fix == "var") {
            build_var_table(var_node, table);
            rel_filename = "varcode-" + filename + ".txt";
            break;
        }
        else if(var_or_fix == "fix") {
            build_fix_table(fix_node, table);
            rel_filename = "fixcode-" + filename + ".txt";
            break;
        }
        else
            cout << "Wrong input, enter 'var' or 'fix'.\n";
    }

    //compress the original file
    int count = encode(filename, table); //last byte's bits

    //create a related file
    fstream rel_file(rel_filename, ios::out | ios::binary);
    if(var_or_fix == "var")
        rel_file << "Variable-length Huffman coding:\n";
    else if(var_or_fix == "fix")
        rel_file << "Fixed-length Huffman coding:\n";
    rel_file << "Last byte's bits: " << count << endl;
    rel_file << "Char\tFrequency\tCodeword\n";
    for(map<unsigned char, string>::iterator ita = table.begin(); ita != table.end(); ita++)
        rel_file << ita->first << "\t" << ch_freq.find(ita->first)->second << "\t" << ita->second << endl;
    rel_file.close();

    //send related_file name
    char buf[512]; //used by write()
    if(write(fd, rel_filename.c_str(), sizeof(buf)) < 0) { 
        perror("write");
        exit(1);
    }

    //send related_file size
    rel_file.open(rel_filename, ios::in);
    rel_file.seekg(0 , rel_file.end);
    int filesize = rel_file.tellg();
    rel_file.seekg(0 , rel_file.beg);
    if(write(fd, &filesize, sizeof(filesize)) < 0) { 
        perror("write");
        exit(1);
    }

    //send related_file
    while(!rel_file.eof()) {
        rel_file.read(buf, sizeof(buf));
        if(write(fd, buf, rel_file.gcount()) < 0) {
            perror("write");
            exit(1);
        }
    }
    rel_file.close();
    return var_or_fix;
}    
void travel_huff_code(Node* node, string parent_code, string flag) {
    if(node == nullptr)
        return;
    node->code = parent_code + flag;
    parent_code = node->code;
    travel_huff_code(node->left, parent_code, "0");
    travel_huff_code(node->right, parent_code, "1");
}
void travel_get_code(Node* node, map<unsigned char, string> &table) {
    if(node == nullptr)
        return;
    if(node->left == nullptr && node->right == nullptr) {
        table[node->letter] = node->code;
        return;
    }
    travel_get_code(node->left, table);
    travel_get_code(node->right, table);
}
void build_var_table(priority_queue<Node*, vector<Node*>, Compare> &var_node, map<unsigned char, string> &table) {
    //build huffman tree
    Node* root;
    while(1) {
        Node* first = var_node.top();
        var_node.pop();
        Node* second = var_node.top();
        var_node.pop();
        Node* new_node = new Node(first, second);
        if(var_node.empty()) {
            root = new_node;
            break;
        }
        var_node.push(new_node);
    }
    //disribute code
    travel_huff_code(root, "", "");
    //collect code
    travel_get_code(root, table);
}
void build_fix_table(priority_queue<Node*, vector<Node*>, greater<Node*>> &fix_node, map<unsigned char, string> &table) {
    int count = 0, len;
    string binary;
    //calculate needed binary string length
    for(len = 0; fix_node.size() > pow(2,len); len++);
    //create binary string according to letter priority
    while(!fix_node.empty()) {
        binary = "";
        int temp = count;
        for(int i = 0; i < len; i++) {
            if(temp & 1 == 1)
                binary = "1" + binary;
            else
                binary = "0" + binary;
            temp >>= 1;
        }
        table[fix_node.top()->letter] = binary;
        count++;
        fix_node.pop();
    }
}
int encode(string filename, map<unsigned char, string> &table) {
    fstream org_file(filename, ios::in | ios::binary);
    fstream com_file(filename + ".zip", ios::out | ios::binary);
    int i = 0;
    int count = 0; //count 8 time for a byte
    int total = 0, len; //total: a byte; len: table's binary string length
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
    org_file.close();
    com_file.close();
    cout << "Encode complete.\n";
    return count;
}