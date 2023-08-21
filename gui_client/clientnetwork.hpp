#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include "SFML/Network.hpp"
#include "imgui.h"
#include "mongodb.h"


#define logl(x) std::cout << x << std::endl
#define log(x) std::cout << x

class User;
typedef std::vector<std::pair<std::string, std::pair<ImVec4, char*>>> Message;


class ClientNetwork{
public:
     sf::Packet last_packet;
     sf::TcpSocket socket;
     bool isConnected = false;
     ClientNetwork();
     bool Connect(const char *, unsigned short);
     void ReceivePackets(sf::TcpSocket *, Message *, MongoDB *, User*);
     void SendPacket(sf::Packet &);
     void Disconnect();
};
