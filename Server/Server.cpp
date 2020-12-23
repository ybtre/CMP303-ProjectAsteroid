#include <iostream>
#include <list>
#include <SFML/Network.hpp>

// define current local address
#define SERVERIP sf::IpAddress::getLocalAddress()

// define port to listen to for clients
#define SERVERPORT 5555


int main() {
    int lives = 300;
    int score = 0;
    // float player1_X, player2_X;
    // float player1_Y, player2_Y;

    std::cout << "Sever Running" << std::endl;
    std::cout << "Server IP: " << SERVERIP << " Port: " << SERVERPORT << std::endl;

    sf::TcpListener TCPListener;
    sf::SocketSelector tcp_selector;
    sf::Socket::Status status = {};
    std::vector<sf::TcpSocket*> tcp_clients;

    sf::SocketSelector udp_selector;
    std::vector<sf::UdpSocket*> udp_clients;

    sf::Packet sendInitLives_Packet;
    TCPListener.listen(SERVERPORT);
    tcp_selector.add(TCPListener);

    sf::Packet receivedPacket;
    bool done = false;
    while (!done) {
        if (tcp_selector.wait()) {
            if (tcp_selector.isReady(TCPListener)) {
                sf::TcpSocket* socket = new sf::TcpSocket;
                TCPListener.accept(*socket);
                sf::Packet initPacket;
                if (status == sf::Socket::Done) {
                    initPacket << lives << score;
                    socket->send(initPacket);
                    //TODO: add ID to each client
                    std::cout << "Sent " << lives << " initial lives and " << score << " Score" << std::endl;
                }
                tcp_clients.push_back(socket);
                tcp_selector.add(*socket);
            }
            else {
                for (auto i = 0; i < tcp_clients.size(); i++) {
                    if (tcp_selector.isReady(*tcp_clients[i])) {
                        sf::Packet packet, sendPacket;
                        if (tcp_clients[i]->receive(packet) == sf::Socket::Done) {
                            packet >> lives >> score;
                            sendPacket << lives << score;
                            std::cout << "Received " << lives << " from client and " << score << " Score" << std::endl; 
                            for (auto j = 0; j < tcp_clients.size(); j++) {
                                if (i != j) {
                                    tcp_clients[j]->send(sendPacket);
                                    std::cout << "Sending " << lives << " Lives and " << score << " Score" << " to client" << std::endl;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for(std::vector<sf::TcpSocket*>::iterator it = tcp_clients.begin(); it != tcp_clients.end(); ++it) {
        delete *it;
    }
    return 0;
}
