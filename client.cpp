// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#define PORT 8000
 
int main(int argc, char const* argv[])
{
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;

    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
 
    if ((client_fd
         = connect(sock, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    while (1) // loop to send all the requests.
    {
        std::string command; //Command to send to server
        getline(std::cin, command); //Get command from user

        if (command == "quit") //If user wants to exit
        {
            std::cout << "good by" << std::endl;
            close(sock); //Close connection
            exit(0); //Exit program
        }

        if (send(sock, command.c_str(), command.length(), 0) == -1) //Sending while checking for errors.
        {
            perror("send");
            exit(1);
        }
    }
    printf("Hello message sent\n");
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
 
    // closing the connected socket
    close(client_fd);
    return 0;
}
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <errno.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>
// #include <arpa/inet.h>
// #include <sys/wait.h>
// #include <signal.h>
// #include <iostream>


// #define PORT "9034" // the port client will be connecting to 

// #define MAXDATASIZE 100 // max number of bytes we can get at once 

// using namespace std;


// int main(int argc, char *argv[])
// {
//     int sockfd, numbytes;  
//     char buf[MAXDATASIZE];
//     struct sockaddr_in addr;
//     if (argc != 2) {
//         fprintf(stderr,"usage: client hostname\n");
//         exit(1);
//     }
//     if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
//             perror("client: socket");
//     }
//     addr.sin_family = AF_INET;
//     addr.sin_port = htons(PORT);
//     addr.sin_addr.s_addr= htonl(INADDR_ANY);
//     inet_aton(argv[1], &(addr.sin_addr));

//     // loop through all the results and connect to the first we can
//         if (connect(sockfd, (struct sockaddr *)&addr ,sizeof(addr)) == -1) {
//             close(sockfd);
//             perror("client: connect");
//         }

//     while (1) // loop to send all the requests.
//     {
//         string command; //Command to send to server
//         getline(cin, command); //Get command from user

//         if (command == "quit") //If user wants to exit
//         {
//             cout << "good by" << endl;
//             close(sockfd); //Close connection
//             exit(0); //Exit program
//         }

//         if ((numbytes = send(sockfd, command.c_str(), command.length(), 0)) == -1) //Sending while checking for errors.
//         {
//             perror("send");
//             exit(1);
//         }
//     }

//     return 0;
// }