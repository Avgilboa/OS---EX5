// /*
// ** pollserver.c -- a cheezy multiperson chat server
// */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include "pipeLine.hpp"

#define PORT 8000
#define MAX_CLIENTS 10

int main(int argc, char *argv[]) {
    pipeLine pl;
    std::thread t1([&pl]{
        pl.run();
    });
    // if(argc != 2 || argc < 2 || argc > 10){
    //         printf("Usage : enter number of client between 2-10");
    //         exit(1);
    //     }
    
    std::thread t2([&pl] {
        int sockfd, clientfd, maxfd;
        struct sockaddr_in server_addr, client_addr;
        socklen_t client_len;
        fd_set readfds;
        int i, n;
        char buffer[256];

        // Create a socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("Error: Unable to create socket");
            exit(1);
        }

        // Configure server address
        bzero((char *) &server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        // Bind the socket to the server address
        if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
            perror("Error: Unable to bind socket");
            exit(1);
        }

        // Listen for incoming connections
        listen(sockfd, MAX_CLIENTS);

        // Initialize the file descriptor set
        FD_ZERO(&readfds);

        // Add the listening socket to the set
        FD_SET(sockfd, &readfds);
        maxfd = sockfd;

        // Main loop
        while (1) {
            fd_set tmpfds = readfds;
            // Block until there's activity on one of the file descriptors
            select(maxfd + 1, &tmpfds, NULL, NULL, NULL);

            // Check if there's a new connection
            if (FD_ISSET(sockfd, &tmpfds)) {
                client_len = sizeof(client_addr);
                clientfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);
                if (clientfd < 0) {
                    perror("Error: Unable to accept connection");
                    exit(1);
                    }
                printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                FD_SET(clientfd, &readfds);
                if (clientfd > maxfd) {
                    maxfd = clientfd;
                }
            }

            // Check for data from connected clients
            for (i = 0; i <= maxfd; i++) {
                if (i != sockfd && FD_ISSET(i, &tmpfds)) {
                    bzero(buffer, 256);
                    n = read(i, buffer, 255);
                    if (n < 0)
                    perror("Error: Unable to read from socket");
                else if (n == 0) {
                    printf("Connection closed by client\n");
                    close(i);
                    FD_CLR(i, &readfds);
                } else {
                        My_string data = My_string(buffer);
                        std::lock_guard<std::mutex> lock(queueMutex);
                        pl.myQueue.enqueue(data);
                        printf("Received message from client: %s\n", buffer);
                    // Process data here
                    }
                    }
                    }
                }
                close(sockfd);
    });
    t2.join();
     t1.join();
    return 0;
    }
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <netdb.h>
// #include <poll.h>
// #include <thread>
// #include "pipeLine.hpp"

// #define PORT "9034"   // Port we're listening on

// // Get sockaddr, IPv4 or IPv6:
// void *get_in_addr(struct sockaddr *sa)
// {
//     if (sa->sa_family == AF_INET) {
//         return &(((struct sockaddr_in*)sa)->sin_addr);
//     }

//     return &(((struct sockaddr_in6*)sa)->sin6_addr);
// }

// // Return a listening socket
// int get_listener_socket(void)
// {
//     int listener;     // Listening socket descriptor
//     int yes=1;        // For setsockopt() SO_REUSEADDR, below
//     int rv;

//     struct addrinfo hints, *ai, *p;

//     // Get us a socket and bind it
//     memset(&hints, 0, sizeof hints);
//     hints.ai_family = AF_UNSPEC;
//     hints.ai_socktype = SOCK_STREAM;
//     hints.ai_flags = AI_PASSIVE;
//     if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
//         fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
//         exit(1);
//     }
    
//     for(p = ai; p != NULL; p = p->ai_next) {
//         listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
//         if (listener < 0) { 
//             continue;
//         }
//         std::cout<<"listen\n";
//         // Lose the pesky "address already in use" error message
//         setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

//         if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
//             close(listener);
//             continue;
//         }

//         break;
//     }

//     // If we got here, it means we didn't get bound
//     if (p == NULL) {
//         return -1;
//     }

//     freeaddrinfo(ai); // All done with this

//     // Listen
//     if (listen(listener, 10) == -1) {
//         return -1;
//     }

//     return listener;
// }

// // Add a new file descriptor to the set
// void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
// {
//     // If we don't have room, add more space in the pfds array
//     if (*fd_count > *fd_size) {
//         exit(1);

//     }

//     (*pfds)[*fd_count].fd = newfd;
//     (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read

//     (*fd_count)++;
// }

// // Remove an index from the set
// void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
// {
//     // Copy the one from the end over this one
//     pfds[i] = pfds[*fd_count-1];

//     (*fd_count)--;
// }

// // Main
// int main(int argc, char *argv[])
// {
//     pipeLine pl;
//     std::thread t1([&pl]{
//         pl.run();
//     });
//     if(argc != 2 || argc < 2 || argc > 10){
//             printf("Usage : enter number of client between 2-10");
//             exit(1);
//         }
    
//     std::thread t2([&pl,&argv] {
//         std::cout<<"hi!"<<std::endl;
//         int listener;     // Listening socket descriptor

//         int newfd;        // Newly accept()ed socket descriptor
//         struct sockaddr_storage remoteaddr; // Client address
//         socklen_t addrlen;

//         char buf[256];    // Buffer for client data

//         char remoteIP[INET6_ADDRSTRLEN];

//         // Start off with room for 5 connections
//         // (We'll realloc as necessary)
//         int fd_count = 0;
//         int fd_size = atoi(argv[1]);
//         struct pollfd *pfds = new struct pollfd[fd_size];

//         // Set up and get a listening socket
//         listener = get_listener_socket();

//         if (listener == -1) {
//             fprintf(stderr, "error getting listening socket\n");
//             exit(1);
//         }

//         // Add the listener to set
//         pfds[0].fd = listener;
//         pfds[0].events = POLLIN; // Report ready to read on incoming connection

//         fd_count = 1; // For the listener

//         // Main loop
//         for(;;) {
//             int poll_count = poll(pfds, fd_count, -1);

//             if (poll_count == -1) {
//                 perror("poll");
//                 exit(1);
//             }

//             // Run through the existing connections looking for data to read
//             for(int i = 0; i < fd_count; i++) {

//                 // Check if someone's ready to read
//                 if (pfds[i].revents & POLLIN) { // We got one!!

//                     if (pfds[i].fd == listener) {
//                         // If listener is ready to read, handle new connection

//                         addrlen = sizeof remoteaddr;
//                         newfd = accept(listener,
//                             (struct sockaddr *)&remoteaddr,
//                             &addrlen);

//                         if (newfd == -1) {
//                             perror("accept");
//                         } else {
//                             add_to_pfds(&pfds, newfd, &fd_count, &fd_size);

//                             printf("pollserver: new connection from %s on "
//                                 "socket %d\n",
//                                 inet_ntop(remoteaddr.ss_family,
//                                     get_in_addr((struct sockaddr*)&remoteaddr),
//                                     remoteIP, INET6_ADDRSTRLEN),
//                                 newfd);
//                         }
//                     } else {
//                         // If not the listener, we're just a regular client
//                         int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);

//                         int sender_fd = pfds[i].fd;

//                         if (nbytes <= 0) {
//                             // Got error or connection closed by client
//                             if (nbytes == 0) {
//                                 // Connection closed
//                                 printf("pollserver: socket %d hung up\n", sender_fd);
//                             } else {
//                                 perror("recv");
//                             }

//                             close(pfds[i].fd); // Bye!

//                             del_from_pfds(pfds, i, &fd_count);

//                         } else {
//                             // We got some good data from a client
//                             My_string data = My_string(buf);
//                             std::lock_guard<std::mutex> lock(queueMutex);
//                             pl.myQueue.enqueue(data);
//                             for(int j = 0; j < fd_count; j++) {
//                                 //here check if palindrom
//                                 // Send to everyone!
                                
//                                 int dest_fd = pfds[j].fd;

//                             }
//                         }
//                     } // END handle data from client
//                 } // END got ready-to-read from poll()
//             } // END looping through file descriptors
//         } // END for(;;)--and you thought it would never end!
//     });
//     t2.join();
//     t1.join();
//     return 0;
// }

