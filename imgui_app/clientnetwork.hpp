#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include "SFML/Network.hpp"

#define logl(x) std::cout << x << std::endl
#define log(x) std::cout << x

class ClientNetwork{
public:
     sf::Packet last_packet;
     sf::TcpSocket socket;
     bool isConnected = false;
     ClientNetwork();
     void Connect(const char *, unsigned short);
     void ReceivePackets(sf::TcpSocket *, std::vector<char*>*);
     void SendPacket(sf::Packet &);
     void Disconnect();
};
