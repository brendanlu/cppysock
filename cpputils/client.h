#include <stdlib.h>
#ifndef SIMPLE_SOCKET
#define SIMPLE_SOCKET

#define MAX_SOCKET_CONNECTIONS 5

// AF_INET:  IPv4
// AF_INET6: IPv6
//
// SOCK_STREAM: "Stream socket_ts"
//  Maintains open connection and uses TCP
// SOCK_DGRAM:  "Datafram socket_ts"
//  Connectionless; fire and forget and uses UDP
//
// IPPROTO_TCP: Specify TCP protocol
//  There are others for internet protocols, raw, and UDP
#define CONFIG_IPV             AF_INET
#define CONFIG_SOCKET_TYPE     SOCK_STREAM
#define CONFIG_PROTOCOL        IPPROTO_TCP

// gcc preprocessor macros
//
// if windows detected, use winsock libraries 
// otherwise assume POSIX-style sockets
#ifdef _WIN32
    #include <winsock.h>
    #include <ws2tcpip.h>
    typedef SOCKET socket_t; 
    #define ERROR_SOCKET INVALID_SOCKET
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    typedef int socket_t; 
    #define ERROR_SOCKET -1
#endif

typedef struct 
{
    int nActive;
    socket_t sockets[MAX_SOCKET_CONNECTIONS]; 
} connection_manager; 


// in the calling state, make sure there is ONLY ONE connection_manager
connection_manager create_connection_manager()
{
    connection_manager cm; 

    cm.nActive = 0; 
    for (int i=0; i<MAX_SOCKET_CONNECTIONS; ++i) {
        cm.sockets[i] = ERROR_SOCKET; 
    }

    return cm; 
}

// any issues will cause the function to return an error-indicating socket
// TODO: maybe create error logfile if things go wrong
int add_connection(connection_manager* manager, int connection_id, 
                    char* ip, int port) 
{
    if (connection_id < 0 || connection_id > MAX_SOCKET_CONNECTIONS - 1) {
        return -1; 
    }

    if (manager->nActive == 0) {
        // initialize winsock API on windows
        // nothing needed for POSIX-style sockets
        #ifdef _WIN32
            WSADATA wsaData; 
            // check for appropriate initialization
            if (WSAStartup(MAKEWORD(2,2), &wsaData)!= 0) {
                return -1;
            }
            // check winsock version 2.2 is available
            if (LOBYTE(wsaData.wVersion) != 2 || 
                HIBYTE(wsaData.wVersion) != 2) {
                WSACleanup(); 
                return -1;
            }
        #endif
    }

    manager->sockets[connection_id] = socket(
        CONFIG_IPV, CONFIG_SOCKET_TYPE, CONFIG_PROTOCOL
    );

    #ifdef _WIN32
        // configure the connection location in the winsock struct
        sockaddr_in addrConfig; 
        addrConfig.sin_family = CONFIG_IPV; 
    #endif
}

int send() 
{

}

int recieve()
{

}

// close a socket in the connection manager
// if it is the last socket, do appropriate cleanup 
int remove_connection(connection_manager* manager, int connection_id) 
{
    int temp; 
    #ifdef _WIN32
        temp = closesocket(manager->sockets[connection_id]);   
    #else
        temp = close(manager->sockets[connection_id]); 
    #endif

    if (manager->nActive == 1) {
        #ifdef _WIN32
            WSACleanup(); 
        #endif
    }

    return temp; 
}

#endif