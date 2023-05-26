#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT_THIS 5555
#define BUFF_LEN 512

int readFromClient(int fd, char* buf);
void writeToClient(int fd, char* buf);

int main(void)
{
    int nSocket, nNewSocket, err;

    char buff[BUFF_LEN];

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

    // Poll
    pollfd active_set[100];

    active_set[0].fd = nSocket;
    active_set[0].events = POLLIN;  // requested event
    active_set[0].revents = 0;      // returned event

    int nTotal = 1;
    struct sockaddr_in client;
    socklen_t size;

    while (true) {

        int ret = poll(active_set
            , nTotal
            , -1); // block

        if (ret < 0) {
            perror("Server: poll failure");
            exit(EXIT_FAILURE);
        }

        if (ret > 0) {
            for (int j = 0; j < nTotal; ++j) {

                if (active_set[j].revents & POLLIN) {

                    printf("get POLLIN at fd %d\n", active_set[j].fd);

                    active_set[j].revents &= ~POLLIN;

                    if (j == 0) {

                        size = sizeof(client);
                        nNewSocket = accept(active_set[j].fd
                            , (struct sockaddr*)&client
                            , &size);

                        if (nNewSocket < 0) {
                            perror("accept");
                            exit(EXIT_FAILURE);
                        }
                        printf("Server: connect from host %s port %hu\n"
                            , inet_ntoa(client.sin_addr)
                            , ntohs(client.sin_port));

                        if (nTotal < 100)
                        {
                            active_set[nTotal].fd = nNewSocket;
                            active_set[nTotal].events = POLLIN;
                            active_set[nTotal].revents = 0;
                            ++nTotal;
                        }
                        else
                        {
                            printf("no more sockets for client");
                            close(nNewSocket);
                        }
                    }
                    else
                    {
                        err = readFromClient(active_set[j].fd, buff);

                        if (err < 0 || strstr(buff, "stop"))
                        {
                            printf("close fd %d\n", (active_set[j].fd));
                            close(active_set[j].fd);

                            // Replace closed
                            if (j < nTotal - 1) {
                                active_set[j] = active_set[nTotal - 1];
                                --nTotal;
                                --j;
                            }
                        }
                        else
                        {
                            writeToClient(active_set[j].fd, buff);
                        }
                    }
                }
            }
        }
    }
}


int readFromClient(int fd, char* buf)
{
    int nBytes;
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
        printf("Server got message: %s\n", buf);
        return 0;
    }
}

void writeToClient(int fd, char* buf)
{
    int nBytes;
    unsigned char* s;

    for (s = (unsigned char*)buf; *s; s++) *s = toupper(*s);

    nBytes = write(fd, buf, strlen(buf) + 1);

    if (nBytes < 0) {
        perror("write failure");
    }
    else {
        printf("Write back: %s bytes = %d\n", buf, nBytes);
    }
}
