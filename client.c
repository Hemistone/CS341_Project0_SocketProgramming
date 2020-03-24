#include <arpa/inet.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PACKETCHUNKSIZE 10 * 1024 * 1024 - 8  //Bytes
#define CHUNKSIZE 100000

// Used eprintf() for debugging to avoid printing letters through console

char *host;
char *port;
int op;
int shift;
char *buffer[];  //Bytes

static void parseCommandLine(int argc, char *argv[]) {
    int opt;
    //printf("argc: %d\n", argc);
    if (argc != 9) {
        fprintf(stderr, "Error - set right arguments : [-h host] [-p port] [-o operation] [-s shift]");
        return;
    }
    while ((opt = getopt(argc, argv, "h:p:o:s:")) != -1) {
        switch (opt) {
            case 'h':
                host = strdup(optarg);
                //printf("host: %s\n", host);
                break;
            case 'p':
                port = strdup(optarg);
                //printf("port: %s\n", port);
                break;
            case 'o':
                op = atoi(optarg);
                //printf("op: %d\n", op);
                break;
            case 's':
                shift = atoi(optarg);  // TO DO: To handle negative values, you have to handle the values on your CLIENT
                //printf("shift: %d\n", shift);
                break;
            default:
                fprintf(stderr, "Unknown input option detacted");
                return;
        }
    }
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

// Cited Beej's Guide to Network Programming
// https://beej.us/guide/bgnet/html//index.html#what-is-a-socket
int loadSocket(void) {
    struct addrinfo hints, *res, *p;
    int status;
    int socketFd, numbytes;
    char ipstr[INET_ADDRSTRLEN];
    char buffer[CHUNKSIZE];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if ((status = getaddrinfo(host, port, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }
    for (p = res; p != NULL; p = p->ai_next) {
        // socket() simply returns to you a socket descriptor that you can use in later system calls, or -1 on error.
        // The global variable errno is set to the error’s value (see the errno man page for more details,
        // and a quick note on using errno in multithreaded programs).
        if ((socketFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) != -1) {  // Check if socket() worked well
            if (connect(socketFd, p->ai_addr, p->ai_addrlen) == -1) {
                printf("client: Connection Failed\n");
                close(socketFd);
            } else {
                printf("client: Connection Successful\n");
            }
        } else {
            printf("client: socekt failed\n");
        }

        break;
    }
    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), ipstr, sizeof ipstr);
    printf("client: connecting to %s\n", ipstr);
    freeaddrinfo(res);

    if ((numbytes = recv(socketFd, buffer, CHUNKSIZE - 1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buffer[numbytes] = '\0';

    printf("client: received '%s'\n", buffer);

    close(socketFd);

    return socketFd;
}

int main(int argc, char *argv[]) {
    //char *input = read_stdin();
    char buffer[CHUNKSIZE];
    fgets(buffer, CHUNKSIZE, stdin);
    //printf("%s\n", buffer);
    parseCommandLine(argc, argv);
    int sockfd = loadSocket();
    //printf("sockfd: %d\n", sockfd);

    return 0;
}