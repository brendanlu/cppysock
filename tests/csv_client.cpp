#include <iostream>
#include <string>

#include "../include/e-c-sock/client.h"

const int BUFFER_SIZE = 1024;
const std::string LOCAL_HOST = "127.0.0.1";
const int PORT = 11111;
const int NSENDROWS = 10; 

int main() {
    connection_manager socket_board = create_connection_manager(); 

    std::cout << "Adding connection | Status : "
    << add_connection(&socket_board, 0, LOCAL_HOST.c_str(), PORT) << "\n";

    std::cout << "Connections count : " << socket_board.n_active << "\n"; 

    // -------------------------------------------------------------------------

    std::string headers = "Header1,Header2\n"; 
    int sendStatus = send(&socket_board, 0, headers.c_str(), headers.length(), 0); 

    if (sendStatus == headers.length()) {std::cout<<"Sent headers\n";}

    std::string data; 
    for (int i=1; i<NSENDROWS+1; ++i) {
        data = std::to_string(i) + "," + std::to_string(i) + "\n";
        sendStatus = send(&socket_board, 0, data.c_str(), data.length(), 0);

        if (sendStatus == data.length()) {
            std::cout<<"Sent data row " << i << "\n";
        }
    }

    // -------------------------------------------------------------------------

    std::cout << "Closing connection | Status : "
            << remove_connection(&socket_board, 0) 
            << "\n"; 

    return 0; 
}
