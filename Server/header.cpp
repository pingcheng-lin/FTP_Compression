#include"header.hpp"
void decode(string filename, int filesize, int com_filesize, string related_filename) {
    string com_filename = filename + ".zip";
    fstream com_file(com_filename, ios::in | ios::binary);
    fstream rel_file(related_filename, ios::in | ios::binary);
    fstream file(filename, ios::out | ios::binary);
    map<string, unsigned char> code_char;
    string recycle;
    getline(rel_file, recycle);
    unsigned char temp_char;
    string temp_str;
    int trash;
    cout << "Getting related file information...\n";
    while(!rel_file.eof()) {
        temp_char = rel_file.get();
        rel_file.get(); //get \t
        rel_file >> trash; //get freq
        rel_file.get(); //get \t
        rel_file >> temp_str;
        rel_file.get(); //get \n
        code_char[temp_str] = temp_char;
    }
    cout << "Decoding...\n";
    map<string, unsigned char>::iterator iter;
    int count = 0; //count byte
    string binary = "";
    while(1) { //test one bit at a time
        unsigned char t = com_file.get();
        for(int i = 7; i >= 0; i--) {
            if(((t<<(7-i)) >> 7) & 1)
                binary = binary + "1";
            else
                binary = binary + "0";
            iter = code_char.find(binary);
            if (iter != code_char.end()) {
                file.put(code_char[binary]);
                count++;
                binary = "";
                if(count  == filesize)
                    break;
            }
        }
        if(count  == filesize)
            break;
    }
    com_file.close();
    rel_file.close();
    file.close();
    cout << "Decode complete.\n";
    cout << "The client sends a file \"" << filename << "\" with size of " << filesize << " bytes.\n";
    cout << "The Huffman coding data are stored in \"" << related_filename << "\".\n";
    cout << "===\n";
}