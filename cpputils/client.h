#ifndef SOCKET_TO_PY
#define SOCKET_TO_PY

// gcc preprocessor macros
//
// if windows detected, use winsock libraries 
// otherwise assume POSIX-style sockets
#ifdef _WIN32
    #include <winsock.h>
    #include <ws2tcpip.h>
    typedef SOCKET Socket; 
    #define ERROR_SOCKET INVALID_SOCKET
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    typedef int Socket; 
    #define ERROR_SOCKET -1
#endif

// any issues in this function will return the respective error socket values
Socket connect() {
    // for windows, one needs to initialize Winsock
    int initStatus;
    #ifdef _WIN32
        WSADATA wsaData; 
        initStatus = WSAStartup(MAKEWORD(2,2), &wsaData); 
    #else
        initStatus = 0; 
    #endif

    if (initStatus != 0) {
        return ERROR_SOCKET; 
    }
    else {
        return socket(AF_INET, SOCK_STREAM, 0);
    }
}

int send() {

}

int recieve() {

}

void disconnect() {

}

#endif