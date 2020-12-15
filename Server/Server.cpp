#include <iostream>
#include <SFML/Network.hpp>

#define SERVERIP "25.79.143.24"
#define SERVERPORT 5555

int main()
{
    sf::TcpSocket TCPSocket;
    std::cout << "test" << std::endl;
    sf::TcpListener TCPListener;
    
    TCPListener.listen(SERVERPORT);
    TCPListener.accept(TCPSocket);

    return 0;
}
