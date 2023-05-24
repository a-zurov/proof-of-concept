#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>

#define PORT_THIS   5555
#define BUFF_LEN    512

#define MAX_CLIENTS 8

char buff[BUFF_LEN];

struct ClientsInfo {
    int  num_clients = {0};
    struct sockaddr_in info[MAX_CLIENTS];
};

void PrintAddr(const sockaddr_in& addr, const char* text);
void ProcessRequest(int nSocket, ClientsInfo& clients);

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

    // Bind sockaddr to a socket
    err = bind(nSocket, (struct sockaddr*)&addr_this, sizeof(addr_this));
    
    if (err < 0) {
        perror("cannot bind socket");
        exit(EXIT_FAILURE);
    }

    PrintAddr(addr_this, "UDP server started");
        
    fcntl(nSocket, F_SETFL, O_NONBLOCK);
    
    ClientsInfo clients;
    
    ProcessRequest(nSocket, clients);

    close(nSocket);
}

void ProcessRequest(int nSocket, ClientsInfo& clients)
{
    int base_delay = 1000;
    int base_period = 5000;
    struct timeval tv;
    int r, period = base_period;
    const char* msg;
       
    struct sockaddr_in addr_from;
    socklen_t size;
    int nBytes;

    for (int k = 0; true; ++k) {

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
            
            if (clients.num_clients >= MAX_CLIENTS) 
            {
                printf("cannot register new client\n");
            }
            else 
            {   // Register client 
                clients.info[clients.num_clients++] = addr_from;
            }
        }

        if (k > period) {
            
            r = rand();
            gettimeofday(&tv, 0); // current time
            msg = ctime(&(tv.tv_sec));
            sprintf(buff, "%10d  %s", r, msg);
            printf("%s\n", buff);
            
            // Send to all registered clients 
            for (int j = 0; j < clients.num_clients; j++) {
                sendto(nSocket
                    , buff
                    , strlen(buff) + 1
                    , 0
                    , (sockaddr*)&(clients.info[j])
                    , sizeof(clients.info[j]));
            }
            // Calculate next sendto time
            k = 0;
            period = r % base_period + 1;
        }
        usleep(base_delay);
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
