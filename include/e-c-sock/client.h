#ifndef SIMPLE_SOCKET_H
#define SIMPLE_SOCKET_H

#include <stdlib.h>

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
    #include <winsock2.h>
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

// this serves as the socket connections context in the calling state
// by design, there should only be one connection manager initialised in the
// calling state 
//
// it is the programmers responsibility to match all calls to add_connection
// to remove_connection
//
// ensure that connection_manager.n_active is 0 at the end of the session
typedef struct 
{
    int n_active;
    socket_t sockets[MAX_SOCKET_CONNECTIONS]; 
} connection_manager; 


// in the calling state, make sure there is ONLY ONE connection_manager
inline connection_manager create_connection_manager()
{
    connection_manager cm; 

    cm.n_active = 0; 
    for (int i=0; i<MAX_SOCKET_CONNECTIONS; ++i) {
        cm.sockets[i] = ERROR_SOCKET; 
    }

    return cm; 
}

// any issues will cause the function to return a nonzero error code for now
// TODO: maybe create error logfile if things go wrong for error tracking
inline int add_connection(connection_manager* manager, int connection_id, 
                    const char* ip, int port) 
{
    // check valid connection_id relative to the maximum number of allowed 
    // connections
    if (connection_id < 0 || connection_id > MAX_SOCKET_CONNECTIONS - 1) {
        return -1; 
    }

    // check we are not overwriting existing connected socket
    if (manager->sockets[connection_id] != ERROR_SOCKET) {
        return -1; 
    }

    // initialize winsock API on windows
    // nothing needed for POSIX-style sockets
    #ifdef _WIN32
        if (manager->n_active == 0) {
            WSADATA wsaData; 
            // check for appropriate initialization
            if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
                return -1;
            }
            // check winsock version 2.2 is available
            if (LOBYTE(wsaData.wVersion) != 2 || 
                HIBYTE(wsaData.wVersion) != 2) {
                WSACleanup(); 
                return -1;
            }
        }
    #endif

    // initialise the socket according to the macro defined values
    manager->sockets[connection_id] = socket(
        CONFIG_IPV, CONFIG_SOCKET_TYPE, CONFIG_PROTOCOL
    );

    // configure the connection location in the winsock struct
    // the struct to configure the network address differs for ipv4 and ipv6
    //
    // inet_pton returns positive number for success, and 0 or negative for fail
    int addrStatus; 
    
    #if CONFIG_IPV == AF_INET // ipv4
        struct sockaddr_in addrConfig; 
        memset(&addrConfig, 0, sizeof(addrConfig)); 
        addrConfig.sin_family = AF_INET; 
        addrConfig.sin_port = htons(port); 
        addrStatus = inet_pton(AF_INET, ip, &(addrConfig.sin_addr)); 
    #else // assume ipv6
        struct sockaddr_in6 addrConfig; 
        memset(&addrConfig, 0, sizeof(addrConfig)); 
        addrConfig.sin6_family = AF_INET6;
        addrConfig.sin6_port = htons(port); 
        addrStatus = inet_pton(AF_INET6, ip, &(addrConfig.sin6_addr)); 
    #endif
        
    if (addrStatus <= 0) {
        // on error make sure we free winsock resources if no other connections
        #ifdef _WIN32
            if (manager->n_active == 0) {
                WSACleanup(); 
            }
        #endif

        return -1; 
    }

    // now make a connection
    //
    // the local (outgoing port) is chosen automatically
    // so the configured port is the one being targeted
    //
    // the success return value should be 0 for winsock and posix-style (?)
    if (connect(manager->sockets[connection_id], (struct sockaddr*)&addrConfig, 
        sizeof(addrConfig)) != 0) {
            #ifdef _WIN32
                if (manager->n_active == 0) {
                    WSACleanup(); 
                }
            #endif
            return -1;     
    }
    else {
        manager->n_active += 1; 
        return 0; 
    }
}

inline int send(connection_manager* manager, int connection_id, const void* data, 
            int len, int flag)  
{
    return send(manager->sockets[connection_id], (const char*)data, len, flag);  
}

inline int recieve(connection_manager* manager, int connection_id, char* buf, 
            int len, int flag)
{
    return recv(manager->sockets[connection_id], buf, len, flag); 
}

// close a socket in the connection manager
// if it is the last socket, do appropriate cleanup 
inline int remove_connection(connection_manager* manager, int connection_id) 
{
    int closeStatus; 
    #ifdef _WIN32
        closeStatus = closesocket(manager->sockets[connection_id]);   
    #else
        closeStatus = close(manager->sockets[connection_id]); 
    #endif

    if (closeStatus == 0) {
        // decrement the connections count
        manager->n_active -= 1; 

        // reset
        manager->sockets[connection_id] = ERROR_SOCKET; 

        // on windows cleanup the winsock system resources if 0 connections
        #ifdef _WIN32
            if (manager->n_active == 0) {
                WSACleanup(); 
            }
        #endif
    }

    // if this is not 0 the caller can assume their socket was not closed
    //
    // this can always be checked by querying n_active
    return closeStatus; 

}

#endif