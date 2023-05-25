#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PORT_TO     5555
#define BUFF_LEN    512

char buff[BUFF_LEN];

void PrintAddr(const sockaddr_in& addr, const char* text);

int main(void)
{
    int nSocket, err;

    // Create IP socket for TCP
    nSocket = socket(PF_INET  // PF_INET = AF_INET
        , SOCK_STREAM       // Supports TCP connection
        , 0);

    if (nSocket < 0) {
        perror("socket was not created");
        exit(EXIT_FAILURE);
    }

    // Socket options
    int opt = 1;
    setsockopt(nSocket
        , SOL_SOCKET
        , SO_REUSEADDR
        , (char*)&opt
        , sizeof(opt));


    struct hostent* hostinfo;
    hostinfo = gethostbyname("127.0.0.1");
    if (hostinfo == nullptr) {
        perror("unknown host\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr_to;                         // Address format:
    addr_to.sin_family = hostinfo->h_addrtype;          // address family
    addr_to.sin_port = htons(PORT_TO);                  // port in network byte order
    addr_to.sin_addr = *(in_addr*)(hostinfo->h_addr);   // IP host address

     // Create TCP connection
    err = connect(nSocket
        , (struct sockaddr*)&addr_to
        , sizeof(addr_to));

    if (err < 0) {
        perror("cannot create connection");
        exit(EXIT_FAILURE);
    }

    // Enter message
    printf("Enter message: ");
    fgets(buff, BUFF_LEN, stdin);

    // Send TCP data
    int nBytes;
    nBytes = send(nSocket
        , buff
        , strlen(buff) + 1
        , 0);

    if (nBytes < 0) {
        perror("cannot send data");
        close(nSocket);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("sending message of %d bytes\n", nBytes);
    }

    nBytes = recv(nSocket, buff, BUFF_LEN, 0);

    printf("received %d bytes :\n%s\n", nBytes, buff);

    close(nSocket);
}

void PrintAddr(const struct sockaddr_in& addr, const char* text)
{
    if (text) printf("%s\n", text);
    int family = addr.sin_family;
    int port = addr.sin_port;
    printf("family %d\n", family);
    printf("port %d\n", ntohs(port));
    printf("ip addr %s\n\n", inet_ntoa(addr.sin_addr));
}
