#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string.h>
#include <SFML/Network.hpp>

#define MAX_RAW_DATA 256 // Max bytes supported on Raw Data mode

#define logl(x) std::cout << x << std::endl
#define log(x) std::cout << x

class User
{
public:
    std::string username;
    float colorX;
    float colorY;
    float colorZ;

    User()
    {
        username = "";
        colorX = 0;
        colorY = 0;
        colorZ = 0;
    }
};

class Message
{
    public:
	std::string username;
	std::string message;
	float colorX;
	float colorY;
	float colorZ;
	float colorW;
	std::string sender_address;
	unsigned short sender_port;

    Message()
    {
		username = "";
		message = "";
		colorX = 0;
		colorY = 0;
		colorZ = 0;
		colorW = 0;
		sender_address = "";
		sender_port = 0;
	}
};

class ServerNetwork
{
     sf::TcpListener listener;
     std::vector<sf::TcpSocket*> clientSockets;
     std::vector<sf::TcpSocket *> onlineListSockets;
     std::vector<User> usernames;
     std::vector<Message> messages;
     unsigned short listen_port;

public:
     ServerNetwork(unsigned short);
     void ConnectClients(std::vector<sf::TcpSocket *> *);
     void DisconnectClient(sf::TcpSocket *, size_t);

     void ReceivePackets(sf::TcpSocket *, size_t);
     void SendOnlinePeopleList();
     void SendPrevMessages(sf::TcpSocket *);
     void ReceiveUserDetails(sf::TcpSocket*);

     void BroadcastPacket(sf::Packet &, sf::IpAddress, unsigned short);

     void ManagePackets();
     void Run();
};
