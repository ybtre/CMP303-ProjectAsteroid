For this module we had to network a game using any networking API. During the classes we were taught winsock and SFML. For my coursework I decided to use my year 1 asteroids game as a base and add networking using SFML on top.

The Network Architecture I chose for the projcet was Server-Client. Reasoning being that it scales easier than P2P. It is more secure, because the Server has to verify the data from the players and more stable if a Client crashes, thus not affecting the other Clients if the host-client crashes by mistake in P2P. Server can currently handle up to 4 clients, but the game is limited to 2 sprites to make testing faster.

For the Transport layer protocols I chose TCP for handling the player Lives, The total Score, each client's ID and the game Time. Those are things which I do not want to lose track of in case of Packet drops. For the player positions, rotation and packet sent time I used UDP. A packet not arriving containing that data does not impact the game there is
interpolation for the player positions.


##### Application layer Protocols

###### Client (every packet is cleared after being received/sent):
- Receives TCP Init packet on first connection with the server containing int values for Life/Score/ClientID
- Receives TCP packet containing Lives/Score/Server GameTime
- Receives UDP Packet containing player position/rotation/time when other client sent packet and pushes them into separate vectors for later use with interpolation
- Sends UDP packet every 1/3s containing player position/rotation/Client game time
- Sends TCP packet every 1/3s containing Lives and Score


###### Server (every packet is cleared after being received/sent):
- Establishes a Listener and a vector of sf::TcpSocket*
- On first connection with client accept *socket and add socket to vector
- If new client sends TCP Init packet containing int values for Life/Score/ClientID
- On existing TCP clients sends packet containing Lives/Score/Server GameTime every 1/3s
- On existing UDP clients receive packet containing player position/rotation variables (floats) and a float containing when the client sent the packet
- On UDP receive add the Port of the sender into an std::set<unsigned short>
- On UDP send go through std::set of ports and send packet to everyone but the original sender


######  Network Code Structure:
- Non-blocking sockets for TCP and UDP past the initial connection
- Server.exe handles both TCP and UDP communication and clients
- - TCP communication is handled through a vector containing all connected client sockets
- - UDP communication is handled through an std::set containing all the ports the server has received a UDP packet from
- Client.exe handles sending and receiving information given to it by the server


######  Lessons Learned
- Newfound appreciation for network programmers
- Need to work on my network architecture skills
- Need to start from a better organised game and work up.
- Handling multiple clients can cause time desync between clients - need a very good time handling method/architecture
- A lot about timing and clocks in games (which was also the origin of the majority of the issues I had)
