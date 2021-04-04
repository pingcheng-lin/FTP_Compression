# FTP program with data compression
## Support 4 commands
- `link IP_addr port_num`
- `leave`
- `help`
- `send filename`
    - support non-textual files
    - Using Huffman coding to compress the file
        - Including variable and fixed length compression
## What is the process?
1. Setup the connection
2. Server will read flag to see if client want to send a file or leave
3. Client opens original file, sends it's name and size
4. Client builds character-frequency table according to original file
5. Client does huffman encoding and build a character-binary table
    - If variable, build huffman tree and travel through
    - If fixed, simply assign binary string
6. Client encode original file to a compressed file
7. Client sends related file to server
    - Information of character, character's frequency, character's binary code
8. Client sends compressed file to server
9. Server uses compressed file and related file to decode
10. Loop until client leave