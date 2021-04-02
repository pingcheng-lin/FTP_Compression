#include"header.hpp"
int main() {
    int fd;
    struct sockaddr_in srv; //used by bind()
    struct sockaddr_in cli; //used by accept()
    int newfd; //returned by accept()
    int cli_len = sizeof(cli); //used by accept()
    char buf[512]; //used by read()
    int nbytes = 0; //used by read()

    
    //Get the file descriptor
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0) {
        cout << "Error\n";
        exit(1);
    }


    srv.sin_family = AF_INET; //use the Internet address family
    srv.sin_port = htons(1234); //bind socket 'fd' to port 80
    srv.sin_addr.s_addr = htonl(INADDR_ANY);
    //Connect the file descriptor with the IP address and port
    if(bind(fd, (struct sockaddr*) &srv, sizeof(srv)) < 0) {
        perror("bind"); 
        exit(1);
    }
    
    //Wait for requests from clients
    if(listen(fd, 5) < 0) {
        perror("listen");
        exit(1);
    }
    else {
        cout << "Listening...\n";
    }
    
    //Accept requests
    newfd = accept(fd, (struct sockaddr*) &cli, (socklen_t*)&cli_len);
    if(newfd < 0) {
        perror("accept");
        exit(1);
    }
    cout << "A client \"" << inet_ntoa(cli.sin_addr) << "\" has connected via port num "  << ntohs(cli.sin_port) << " using SOCK_STREAM (TCP)\n";
    
    while(1) {
        //Read data from the socket
        fstream file, related_file;
        string filename, related_filename;
        int filesize, related_filesize;
        //read flag
        if(read(newfd, buf, sizeof(buf)) < 0) { 
            perror("read");
            exit(1);
        }
        else if(strcmp(buf, "leave") == 0) {
            cout << "The client \"" << inet_ntoa(cli.sin_addr) << "\" with port " << ntohs(cli.sin_port) << " has terminated the connection.\n";
            break;
        }
        //read filename
        if(read(newfd, buf, sizeof(buf)) < 0) { 
            perror("read");
            exit(1);
        }
        else if(strcmp(buf, "error") == 0) {
            cout << "File open error!!!\n";
            exit(1);
        }
        else {
            filename = buf;
            file.open(filename, ios::out);//reset file
        }


        //read related_file name
        if(read(newfd, buf, sizeof(buf)) < 0) { 
            perror("read");
            exit(1);
        }
        else {
            related_filename = buf;
            related_file.open(related_filename, ios::out);//reset file
        }
        //read related_file size
        if(read(newfd, &related_filesize, sizeof(related_filesize)) < 0) { 
            perror("read");
            exit(1);
        }
        cout << "filesize: " << related_filesize << endl;
        //read related_file
        for(int i = related_filesize; i > 0; i-=nbytes)
            if((nbytes = read(newfd, buf, sizeof(buf))) < 0) { 
                perror("read");
                exit(1);
            }
            else {
                related_file.write(buf, nbytes);
                memset(buf, 0, 512*sizeof(buf[0]));
            }
        related_file.close();


        //read filesize
        if(read(newfd, &filesize, sizeof(filesize)) < 0) { 
            perror("read");
            exit(1);
        }
        cout << "filesize: " << filesize << endl;
        //read file
        for(int i = filesize; i > 0; i-=nbytes)
            if((nbytes = read(newfd, buf, sizeof(buf))) < 0) { 
                perror("read");
                exit(1);
            }
            else {
                file.write(buf, nbytes);
                memset(buf, 0, 512*sizeof(buf[0]));
            }
                
        cout << "The client sends a file \"" << filename << "\" with size of " << filesize << " bytes.\n";
        cout << "The Huffman coding data are stored in \"" << related_filename << "\".\n";
        
        cout << "===\n";
        file.close();
    }
    return 0;
}