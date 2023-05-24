#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define PORT_THIS   5556
#define PORT_TO     5555
#define BUFF_LEN    512

char buff[BUFF_LEN];

void PrintAddr(const sockaddr_in& addr, const char* text);

int main(void)
{
    int nSocket, err;

    // Create IP socket for UDP
    nSocket = socket(PF_INET    // PF_INET = AF_INET
        , SOCK_DGRAM            // Supports datagrams = UDP
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

    struct sockaddr_in addr_this;           // Address format:
    addr_this.sin_family = AF_INET;         // IPv4 Internet protocol
    addr_this.sin_port = htons(PORT_THIS);  // port in network byte order
    addr_this.sin_addr.s_addr = INADDR_ANY; // (0.0.0.0) means any address for binding

    // Bind sockaddr to the socket
    err = bind(nSocket, (struct sockaddr*)&addr_this, sizeof(addr_this));

    if (err < 0) {
        perror("cannot bind socket");
        exit(EXIT_FAILURE);
    }

    // Enter message
    printf("Enter message: ");
    fgets(buff, BUFF_LEN, stdin);

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

    int nBytes;
    // Send UDP message
    nBytes = sendto(nSocket
        , buff
        , strlen(buff) + 1
        , 0
        , (struct sockaddr*)&addr_to
        , sizeof(addr_to));

    if (nBytes < 0) {
        perror("cannot send data");
        close(nSocket);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("sending %d bytes\n\n", nBytes);
    }

    // Manipulate our socket descriptor: it's NONBLOCK now..
    fcntl(nSocket
        , F_SETFL // set status flags to the value specified by argument below
        , O_NONBLOCK);

    // Receive response cycle
    struct sockaddr_in addr_from;
    socklen_t size = sizeof(addr_from);

    for (int k = 0; k < 1000; ++k) {

        nBytes = recvfrom(nSocket
            , buff
            , BUFF_LEN
            , 0
            , (sockaddr*)&addr_from
            , &size);

        if (nBytes > 0)
        {
            PrintAddr(addr_from, "Receive message from");
            printf("received %d bytes:\n%s\n", nBytes, buff);
            k = 0;
        }
        else
        {
            printf(".");
        }

        usleep(100);
    }

    printf("Receive wait timeout expired\n");

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
