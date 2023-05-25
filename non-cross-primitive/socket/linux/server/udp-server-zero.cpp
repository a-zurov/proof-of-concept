#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define PORT_THIS   5555
#define BUFF_LEN    512

char buff[BUFF_LEN];

void PrintAddr(const sockaddr_in& addr, const char* text);
void ProcessRequest(int nSock);
void strUp(char* p);

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

    struct sockaddr_in addr_this;
    addr_this.sin_family = AF_INET;         // IPv4 Internet protocol
    addr_this.sin_port = htons(PORT_THIS);  // port in network byte order
    addr_this.sin_addr.s_addr = INADDR_ANY; // (0.0.0.0) means any address for binding

    // Bind struct sockaddr_in to a socket
    err = bind(nSocket, (struct sockaddr*)&addr_this, sizeof(addr_this));

    if (err < 0) {
        perror("cannot bind socket");
        exit(EXIT_FAILURE);
    }

    PrintAddr(addr_this, "UDP server started");

    ProcessRequest(nSocket);

    close(nSocket);
}

void ProcessRequest(int nSocket)
{
    struct sockaddr_in addr_from;
    socklen_t size;
    int nBytes;

    while (true) {

        size = sizeof(addr_from);
        // Read message
        nBytes = recvfrom(nSocket
            , buff
            , BUFF_LEN
            , 0
            , (struct sockaddr*)&addr_from
            , &size);

        if (nBytes > 0) {

            if (strstr(buff, "stop")) return;

            PrintAddr(addr_from, "Receive message from");
            printf("received %d bytes:\n%s\n", nBytes, buff);

            // Prepare response
            strUp(buff);
            usleep(10000);

            // Send response
            nBytes = sendto(nSocket
                , buff
                , strlen(buff) + 1
                , 0
                , (struct sockaddr*)&addr_from
                , sizeof(addr_from));

            if (nBytes < 0)
            {
                perror("cannot send data");
                close(nSocket);
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("sending %d bytes\n", nBytes);
            }
        }
    }
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

void strUp(char* p)
{
    while (*p) {
        *p = toupper(*p);
        ++p;
    }
}
