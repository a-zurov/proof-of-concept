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
void strUp(char* p);

int main(void)
{
    int nSocket, err;

    // Create TCP IP socket
    nSocket = socket(PF_INET    // PF_INET = AF_INET
        , SOCK_STREAM           // Supports TCP stream
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

    // Bind sockaddr to a socket
    err = bind(nSocket
        , (struct sockaddr*)&addr_this
        , sizeof(addr_this));

    if (err < 0) {
        perror("cannot bind socket");
        exit(EXIT_FAILURE);
    }

    // Define queue
    err = listen(nSocket, 3);
    if (err < 0) {
        perror("server listen queue failure");
        exit(EXIT_FAILURE);
    }

    PrintAddr(addr_this, "TCP server started");

    socklen_t size;
    sockaddr_in addr_from;
    int nNewSocket = accept(nSocket
        , (struct sockaddr*)&addr_from
        , &size);

    int nBytes;
    // Read message
    nBytes = recv(nNewSocket
        , buff
        , BUFF_LEN
        , 0);

    printf("%d bytes:\n%s\n", nBytes, buff);

    // Prepare response
    strUp(buff);
    usleep(10000);

    // Send response
    nBytes = send(nNewSocket, buff, strlen(buff) + 1, 0);

    if (nBytes < 0)
    {
        perror("cannot send data");
        close(nSocket);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("sending message of %d bytes\n", nBytes);
    }

    close(nNewSocket);
    close(nSocket);
}

void PrintAddr(const struct sockaddr_in& addr, const char* text)
{
    if (text) printf("%s\n", text);
    int f = addr.sin_family;
    int p = addr.sin_port;
    printf("type %d\n", f);
    printf("port %d\n", ntohs(p));
    printf("ip addr %s\n", inet_ntoa(addr.sin_addr));
}

void strUp(char* p)
{
    while (*p) {
        *p = toupper(*p);
        ++p;
    }
}
