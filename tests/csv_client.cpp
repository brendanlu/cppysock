#include <iostream>
#include <string>

#include "../include/e-c-sock/client.h"

const int BUFFER_SIZE = 1024;
const std::string LOCAL_HOST = "127.0.0.1";
const int PORT = 11111;

int main() {
    connection_manager socket_board = create_connection_manager(); 

    std::cout << "Adding connection | Status : "
    << add_connection(&socket_board, 0, LOCAL_HOST.c_str(), PORT) << "\n";

    std::cout << "Connections count : " << socket_board.n_active << "\n"; 

    // -------------------------------------------------------------------------

    std::string headers = "Header1,Header2\n"; 
    int sendStatus = send(&socket_board, 0, headers.c_str(), headers.length(), 0); 

    

    // -------------------------------------------------------------------------

    std::cout << "Closing connection | Status : "
            << remove_connection(&socket_board, 0) 
            << "\n"; 

    return 0; 
}
