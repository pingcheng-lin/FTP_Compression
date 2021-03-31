#include"header.h"
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
        printf("Error");
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
        printf("Listening...\n");
    }
    
    //Accept requests
    newfd = accept(fd, (struct sockaddr*) &cli, &cli_len);
    if(newfd < 0) {
        perror("accept");
        exit(1);
    }
    printf("A client \"%s\" has connected via port num %d using SOCK_STREAM (TCP)\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
    
    while(1) {
        //Read data from the socket
        FILE *file;
        char filename[MAX_FILENAME_SIZE];
        int filesize;
        //read flag
        if(read(newfd, buf, sizeof(buf)) < 0) { 
            perror("read");
            exit(1);
        }
        else if(strcmp(buf, "leave") == 0) {
            printf("The client \"%s\" with port %d has terminated the connection.\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
            break;
        }
        //read filename
        if(read(newfd, buf, sizeof(buf)) < 0) { 
            perror("read");
            exit(1);
        }
        else if(strcmp(buf, "error") == 0) {
            printf("File open error!!!\n");
            exit(1);
        }
        else {
            strcpy(filename, buf);
            printf("%s\n", filename);
            file = fopen(filename, "w");//reset file
        }
        //read filesize
        if(read(newfd, &filesize, sizeof(filesize)) < 0) { 
            perror("read");
            exit(1);
        }
        printf("filesize:%d\n", filesize);
        //read file
        for(int i = filesize; i > 0; i-=nbytes)
            //printf("%d=%d\n", i, nbytes);
            if((nbytes = read(newfd, buf, sizeof(buf))) < 0) { 
                perror("read");
                exit(1);
            }
            else{
                fwrite(buf, sizeof(char), nbytes, file);
                memset(buf, 0, 512*sizeof(buf[0]));
            }
                
        printf("The client sends a file \"%s\" with size of %d bytes.\n", filename, filesize);
        printf("The Huffman coding data are stored in \"txt\".\n");
        
        printf("===\n");
        fclose(file);
    }
    return 0;
}