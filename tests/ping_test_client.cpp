#include <iostream>
#include <string>

#include "../include/e-c-sock/client.h"

const int BUFFER_SIZE = 1024;
const std::string LOCAL_HOST = "127.0.0.1";
const int PORT1 = 11111;
const int PORT2 = 11112;

int main() {
    connection_manager socket_board = create_connection_manager(); 

    // server 1
    std::cout << "Adding connection | Status : "
    << add_connection(&socket_board, 0, LOCAL_HOST.c_str(), PORT1) << "\n";

    std::cout << "Connections count : " << socket_board.n_active << "\n"; 

    // server 2
    std::cout << "Adding connection | Status : "
    << add_connection(&socket_board, 1, LOCAL_HOST.c_str(), PORT2) << "\n";

    std::cout << "Connections count : " << socket_board.n_active << "\n"; 

    // -------------------------------------------------------------------------

    const std::string sendData = "Client Ping\n"; 

    int sendStatus = send(&socket_board, 0, sendData.c_str(), sendData.length(), 0); 
    std::cout << "Sent " << sendStatus << " bytes out of " 
    << sendData.length() << "\n"; 

    sendStatus = send(&socket_board, 1, sendData.c_str(), sendData.length(), 0); 
    std::cout << "Sent " << sendStatus << " bytes out of " 
    << sendData.length() << "\n"; 

    // -------------------------------------------------------------------------

    char buf[BUFFER_SIZE]; 

    int recvStatus = recieve(&socket_board, 0, &buf[0], sizeof(buf) - 1, 0); 
    std::cout << "Recieved " << recvStatus << " bytes out of " 
    << sizeof(buf) << " maximum \n"; 
    buf[recvStatus] = '\0'; 
    
    std::cout << "Message: " << buf << "\n";

    recvStatus = recieve(&socket_board, 1, &buf[0], sizeof(buf) - 1, 0); 
    std::cout << "Recieved " << recvStatus << " bytes out of " 
    << sizeof(buf) << " maximum \n"; 
    buf[recvStatus] = '\0'; 
    
    std::cout << "Message: " << buf << "\n";

    // -------------------------------------------------------------------------

    std::cout << "Closing connection | Status : "
            << remove_connection(&socket_board, 0) 
            << "\n"; 

            
    std::cout << "Closing connection | Status : "
            << remove_connection(&socket_board, 1) 
            << "\n"; 

    return 0; 
}