#include <iostream>

#include "e-c-sock/client.h"

int main() {
    connection_manager socket_board = create_connection_manager(); 

    std::cout << "Adding connection | Status : "
            << add_connection(&socket_board, 0, "127.0.0.1", 11111)
            << "\n";

    std::cout << "Connections count : " << socket_board.n_active << "\n"; 

    std::cout << "Closing connection | Status : "
            << remove_connection(&socket_board, 0) 
            << "\n"; 

    return 0; 
}