#include <iostream>
#include <list>
#include <SFML/Network.hpp>

// define current local address
#define SERVERIP sf::IpAddress::getLocalAddress()

// define port to listen to for clients
#define SERVERPORT 5555

void TCPListen(sf::TcpListener& tcp_listener) {
    tcp_listener.listen(SERVERPORT);
    std::cout << "Listening to Port: " << SERVERPORT << std::endl;
}

void TCPAccept(sf::TcpListener& tcp_listener, sf::TcpSocket& tcp_socket) {
    sf::Thread thread([&tcp_listener, &tcp_socket]() {
          tcp_listener.accept(tcp_socket);
            std::cout << "TCP Accepted Socket in separate Thread" << std::endl << std::endl;
    });
    thread.launch();
}

int main() {
    int lives = 300;
    float player1_X, player2_X;
    float player1_Y, player2_Y;


    sf::TcpSocket TCPSocket;
    TCPSocket.setBlocking(false);
    std::cout << SERVERIP << std::endl;
    sf::TcpListener TCPListener;
    sf::Socket::Status status = {};
    sf::Packet sendInitLives_Packet;
    TCPListen(TCPListener);
    bool done = false;
    while (!done) {

        switch (status) {
        case sf::Socket::Done:
            TCPAccept(TCPListener, TCPSocket);

            sendInitLives_Packet << lives;
            TCPSocket.send(sendInitLives_Packet);
            break;
        case sf::Socket::NotReady:
            std::cout << status << std::endl;
            break;
        case sf::Socket::Partial:
            std::cout << status << std::endl;
            break;
        case sf::Socket::Disconnected:
            std::cout << "disconnected client" << std::endl;
            break;
        case sf::Socket::Error:
            std::cout << status << std::endl;
            break;
        }
        // if (TCPListener.listen(SERVERPORT) != sf::Socket::Done) {
        //     std::cout << "Could not listen to Port: " << SERVERPORT << std::endl;
        // }
        // else {
        //     std::cout << "Listening to Port: " << SERVERPORT << std::endl;
        // }
        // if (TCPListener.accept(TCPSocket) != sf::Socket::Done) {
        //     std::cout << "TCP could not Accept Socket" << std::endl;
        // }
        // else {
        //     std::cout << "TCP Accepted Socket" << std::endl << std::endl;
        // }

        // sf::Packet sendPacket;
        // sendPacket << lives;
        // TCPSocket.send(sendPacket);
    }

    // bool done = false;
    // while(!done) {
    sf::Packet receivedPacket;
    // TCPSocket.receive(receivedPacket);
    // if(receivedPacket >> player1_X >> player1_Y) {
    //     player2_X = player1_X;
    //     player2_Y = player1_Y;
    //     std::cout << "Player 2 X: " << player2_X << " ";
    //     std::cout << "Player 2 Y: " << player2_Y << std::endl;
    // }
    TCPSocket.receive(receivedPacket);
    if (receivedPacket >> lives) {
        std::cout << "received and set " << lives << " lives" << std::endl;
    }
    // }


    // TCPSocket.send(test.c_str(), test.length() + 1);
    // TCPSocket.receive(buffer, sizeof(buffer), received);
    // std::cout << buffer << std::endl;
    //
    // bool done = false;
    // while(!done) {
    //     std::getline(std::cin, test);
    //     TCPSocket.send(test.c_str(), test.length() + 1);
    //
    //     TCPSocket.receive(buffer, sizeof(buffer), received);
    //     if(received > 0) {
    //         std::cout<< "Received: " << buffer << std::endl;
    //     }
    // }

#pragma region UDP
    // sf::UdpSocket UDPSocket;
    // // UDPSocket.setBlocking(false);
    // char buffer[200];
    // std::size_t received;
    // // std::map<unsigned short, sf::IpAddress> pcID;
    // sf::IpAddress rIp;
    // unsigned short rPort;
    // std::string text = "Server Text";
    // unsigned short port;
    // std::cout << "set port number: "; // 192.168.100.27
    // std::cin >> port;
    //
    // UDPSocket.bind(port); //listen to that port
    // char answer = 'b';
    // while (answer != 'q') {
    //     sf::IpAddress tempIp;
    //     unsigned short tempPort;
    //     UDPSocket.receive(buffer, sizeof(buffer), received, tempIp, tempPort);
    //     if (received > 0) {
    //         rIp = tempIp;
    //         rPort = tempPort;
    //     }
    //     std::cin >> answer;
    // }
    //
    // bool done = false;
    // while (!done) {
    //     std::getline(std::cin, text);
    //     // std::map<unsigned short, sf::IpAddress>::iterator tempIterator;
    //     // for(tempIterator = pcID.begin(); tempIterator != pcID.end(); tempIterator++) {
    //     //     UDPSocket.send(text.c_str(), text.length()+1, tempIterator->second, tempIterator->first);
    //     // }
    //     UDPSocket.send(text.c_str(), text.length() + 1, rIp, rPort);
    // }
#pragma endregion UDP

    system("pause");
    return 0;
}
