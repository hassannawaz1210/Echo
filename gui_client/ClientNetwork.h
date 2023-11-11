#pragma once

#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include "SFML/Network.hpp"
#include "imgui.h"

//
#include "User.h"


#define logl(x) std::cout << x << std::endl
#define log(x) std::cout << x

typedef std::vector<std::pair<std::string, std::pair<ImVec4, std::string>>> Message;

class ClientNetwork {

public:

    sf::Packet last_packet;
    sf::TcpSocket messagesSocket;
    sf::TcpSocket onlineListSocket;
    bool isConnected = false;


    ClientNetwork();
    bool Connect(const char*, unsigned short);
    void ReceiveMessages(sf::TcpSocket*, Message*, User*);
    void ReceiveOnlineList(sf::TcpSocket*, std::vector<User>*);
    void SendPacket(sf::Packet&);
    void Disconnect();
    void DownloadMessages(Message &);
};
