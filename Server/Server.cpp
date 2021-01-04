#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <list>
#include <set>
#include <SFML/Network.hpp>

// define current local address
#define SERVERIP sf::IpAddress::getLocalAddress()

// define port to listen to for clients
#define SERVERPORT 5555
#define UDPPORT 5556


int main() {
    SYSTEMTIME st, lt;
    
    GetSystemTime(&st);
    
    int lives = 300;
    int score = 0;
    float player1_X, player2_X;
    float player1_Y, player2_Y;
    int initSetClientID, recvClientID;
    int lastpos_SendTime = 0;

    std::cout << "Sever Running" << std::endl;
    std::cout << "Server IP: " << SERVERIP << " TCP Port: " << SERVERPORT << " UDP Port: " << UDPPORT << std::endl;

    sf::UdpSocket UDPSocket;
    // UDPSocket.setBlocking(true);
    // unsigned short port;
    // std::cout << "Set port number (different from 5555): ";
    // std::cin >> port;
    UDPSocket.bind(UDPPORT);
    UDPSocket.setBlocking(false);

    sf::TcpListener TCPListener;
    sf::SocketSelector tcp_selector;
    sf::Socket::Status status = {};
    std::vector<sf::TcpSocket*> tcp_clients;

    std::set<unsigned short> UDPPorts;

    TCPListener.listen(SERVERPORT);
    tcp_selector.add(TCPListener);

    bool done = false;
    while (!done) {
#pragma region TCP
        if (tcp_selector.wait()) {
            if (tcp_selector.isReady(TCPListener)) {
                if(tcp_clients.size() <= 4) {
                    sf::TcpSocket* socket = new sf::TcpSocket;
                    TCPListener.accept(*socket);
                    tcp_clients.push_back(socket);
                    initSetClientID = tcp_clients.size();
                    
                    std::cout << "///////// Accepted new client /////////" << std::endl;

                    sf::Packet initPacket;
                    if (status == sf::Socket::Done) {
                        initPacket << lives << score << initSetClientID;
                        socket->send(initPacket);
                        // TODO: add ID to each client
                        std::cout << "Sent " << lives << " initial lives and "
                                << "init client ID" << initSetClientID
                                << " and " << score << " Score to new client" << std::endl;
                        initPacket.clear();
                    }
                    tcp_selector.add(*socket);
                } else if (tcp_clients.size() > 4) {
                    std::cout << "Max Number of 2 Clients reached!" << std::endl;
                }
            }
            else {
                for (auto i = 0; i < tcp_clients.size(); i++) {
                    if (tcp_selector.isReady(*tcp_clients[i])) {
                        sf::Packet packet, sendPacket;
                        if (tcp_clients[i]->receive(packet) == sf::Socket::Done) {
                            packet >> lives >> score;
                            sendPacket << lives << score;
                            std::cout << "Received " << lives << " from client and " << score << " Score" << std::endl;
                            packet.clear();
                            for (auto j = 0; j < tcp_clients.size(); j++) {
                                if (i != j) {
                                    tcp_clients[j]->send(sendPacket);
                                    std::cout << "Sending " << lives << " Lives and " << score << " Score" <<
                                        " to client" << std::endl;
                                    sendPacket.clear();
                                }
                            }
                        }
                    }
                }
            }
        }
#pragma endregion TCP

#pragma region UDP
        sf::IpAddress recvIP;
        unsigned short recvPort;
        sf::Packet recvPositionsTimeClientID, sendPositionsTimeClientID;
        // if (UDPSocket.receive(recvPositionsTimeClientID, recvIP, recvPort) == sf::Socket::Done) {
            UDPSocket.receive(recvPositionsTimeClientID, recvIP, recvPort);
            // std::cout << "Received positions" << std::endl;
            if (recvPort != 0) {
                UDPPorts.insert(recvPort);
            }

            recvPositionsTimeClientID >> player1_X >> player1_Y >> player2_X >> player2_Y >> lastpos_SendTime >> recvClientID;
            // std::cout << "Received Time: " << lastpos_SendTime << std::endl;
            std::cout << "Received ClientID: " << recvClientID << std::endl;
            // std::cout << "Received positions: "
            //     << player1_X << " p1X; "
            //     << player1_Y << " p1Y; "
            //     << player2_X << " p2X; "
            //     << player2_Y << " p2Y;" << std::endl;
            recvPositionsTimeClientID.clear();

            sendPositionsTimeClientID << player1_X << player1_Y << player2_X << player2_Y << lastpos_SendTime << recvClientID;
            for (auto port : UDPPorts) {
                if (port != recvPort) {
                    UDPSocket.send(sendPositionsTimeClientID, recvIP, port);
                    std::cout << "Sending positions, time and client ID" << std::endl;
                    sendPositionsTimeClientID.clear();
                }
            }
        // }
    }
#pragma endregion UDP

    for(std::vector<sf::TcpSocket*>::iterator it = tcp_clients.begin(); it != tcp_clients.end(); ++it) {
        delete *it;
    }
    return
        0;
}
