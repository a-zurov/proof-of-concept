#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT_THIS   5555
#define BUFF_LEN    512

int   readFromClient(int fd, char* buf);
void  writeToClient(int fd, char* buf);

int  main(void)
{
    int     nSocket, nNewSocket, err;

    char    buf[BUFF_LEN];

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

    struct  sockaddr_in  addr_this;
    addr_this.sin_family = AF_INET;
    addr_this.sin_port = htons(PORT_THIS);
    addr_this.sin_addr.s_addr = htonl(INADDR_ANY);

    err = bind(nSocket, (struct sockaddr*)&addr_this, sizeof(addr_this));
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

    // The select() uses the specified file descriptor mask
    // to determine which file descriptors to monitor for an event.
    // For simplicity, we do not calculate the highest value
    // and then we will explore all descriptors upto FD_SETSIZE.
    fd_set  active_set, read_set;

    FD_ZERO(&active_set);
    FD_SET(nSocket, &active_set);

    sockaddr_in  client;
    socklen_t  size;

    while (true) {

        read_set = active_set;
        if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0) {
            perror("select failure");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < FD_SETSIZE; ++j) {
            if (FD_ISSET(j, &read_set)) {
                if (j == nSocket) {

                    size = sizeof(sockaddr_in);
                    nNewSocket = accept(nSocket, (sockaddr*)&client, &size);
                    if (nNewSocket < 0) {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    fprintf(stdout, "Server: connect from host %s, port %hu.\n"
                        , inet_ntoa(client.sin_addr)
                        , ntohs(client.sin_port));

                    FD_SET(nNewSocket, &active_set);
                }
                else {

                    err = readFromClient(j, buf);

                    if (err < 0)
                    {
                        close(j);
                        FD_CLR(j, &active_set);
                    }
                    else
                    {
                        if (strstr(buf, "stop"))
                        {
                            close(j);
                            FD_CLR(j, &active_set);
                        }
                        else
                        {
                            writeToClient(j, buf);
                        }
                    }
                }
            }
        }
    }
}

int  readFromClient(int fd, char* buf)
{
    int  nBytes;

    nBytes = read(fd, buf, BUFF_LEN);
    if (nBytes < 0) {
        perror("read failure");
        return -1;
    }
    else if (nBytes == 0) {
        // end of data
        return -1;
    }
    else {
        fprintf(stdout, "Server got message: %s\n", buf);
        return 0;
    }
}

void  writeToClient(int fd, char* buf)
{
    int  nBytes;
    unsigned char* s;

    for (s = (unsigned char*)buf; *s; s++) *s = toupper(*s);
    nBytes = write(fd, buf, strlen(buf) + 1);
    fprintf(stdout, "Write back: %s\nnbytes=%d\n", buf, nBytes);

    if (nBytes < 0) {
        perror("write failure");
    }
}
