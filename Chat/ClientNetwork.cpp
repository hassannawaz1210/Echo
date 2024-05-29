#pragma once
#include "ClientNetwork.h"

ClientNetwork::ClientNetwork()
{
    logl("Chat Client Started");
}


bool ClientNetwork::Connect(const char* address, unsigned short port)
{
    if (messagesSocket.connect(address, port) != sf::Socket::Done || onlineListSocket.connect(address, port) != sf::Socket::Done)
    {
        logl("Could not connect to the server\n");
        return false;
        //exit(0);
    }

    else
    {
        isConnected = true;
        logl("Connected to the server\n");
        return true;
    }
}

void ClientNetwork::ReceiveMessages(sf::TcpSocket* socket, Message* chatMessages, User* user)
{
    while (true)
    {
        if (isConnected)
        {
            if (socket->receive(last_packet) == sf::Socket::Done)
            {
                std::string received_string = "", sender_address = "", username = "";
                float colorX = 0, colorY = 0, colorZ = 0, colorW = 0;
                unsigned short sender_port = 0;
                last_packet >> username >> colorX >> colorY >> colorZ >> colorW >> received_string >> sender_address >> sender_port;

                if (received_string.length() > 0)
                {
                    chatMessages->push_back(std::make_pair(username, std::make_pair(ImVec4(colorX, colorY, colorZ, colorW), received_string)));
                }
                logl("From (" << sender_address << ":" << sender_port << "): " << username << " " << received_string);
            }

            ////if socket is disconnected from the server somehow
            //else if (socket->receive(last_packet) == sf::Socket::Disconnected)
            //{
            //    logl("I RANNNNNNNNNNNNNNNNNNNNNNNNNNNNN");
            //    isConnected = false;
            //    chatMessages->push_back(std::make_pair("SERVER: ", std::make_pair(ImVec4(1, 1, 1, 1), (char*)"Disconnected from the server")));
            //    logl("Disconnected from the server");
            //}
        }
        std::this_thread::sleep_for((std::chrono::milliseconds)250);
    }
}

void ClientNetwork::ReceiveOnlineList(sf::TcpSocket* socket, std::vector<User>* users)
{
    int size = 0;
    sf::Packet packet;
    User user;
    while (true)
    {
        if (isConnected)
        {
            if (socket->receive(packet) == sf::Socket::Done)
            {
                users->clear();
                packet >> size;
                for (int i = 0; i < size; i++)
                {
                    socket->receive(packet);
                    packet >> user.username >> user.color.x >> user.color.y >> user.color.z;
                    packet.clear();
                    users->push_back(user);
                }

            }
        }
        std::this_thread::sleep_for((std::chrono::milliseconds)250);
    }
}

void ClientNetwork::SendPacket(sf::Packet& packet)
{
    if (packet.getDataSize() > 0 && messagesSocket.send(packet) != sf::Socket::Done)
    {
        logl("Could not send packet");
    }
}


void ClientNetwork::Disconnect()
{
    messagesSocket.disconnect();
    isConnected = false;
}

void ClientNetwork::DownloadMessages(Message& chatMessages)
{
	sf::Packet packet;
    while (messagesSocket.receive(packet) == sf::Socket::Done)
    {
		std::string received_string = "", sender_address = "", username = "";
		float colorX = 0, colorY = 0, colorZ = 0, colorW = 0;
		unsigned short sender_port = 0;
		packet >> username >> colorX >> colorY >> colorZ >> colorW >> received_string >> sender_address >> sender_port;

        if (received_string.length() > 0)
        {
			chatMessages.push_back(std::make_pair(username, std::make_pair(ImVec4(colorX, colorY, colorZ, colorW), received_string)));
		}
		//logl("From (" << sender_address << ":" << sender_port << "): " << username << " " << received_string);
		packet.clear();
	}   

}