#include "servernetwork.hpp"

bool newConncetion = true;

ServerNetwork::ServerNetwork(unsigned short port) : listen_port(port)
{
     logl("Chat Server Started");

     if (listener.listen(listen_port) != sf::Socket::Done)
     {
          logl("Could not listen");
     }
}

void ServerNetwork::ReceiveUserDetails(sf::TcpSocket* new_client)
{
    sf::Packet packet;
    User user;
    packet.clear();
    if (new_client->receive(packet) == sf::Socket::Done)
    {
        packet >> user.username >> user.colorX >> user.colorY >> user.colorZ;
        usernames.push_back(user);
        packet.clear();
    }

    std::cout << user.username << std::endl;
    std::cout << user.colorX << std::endl;
    std::cout << user.colorY << std::endl;
    std::cout << user.colorZ << std::endl;
}


void ServerNetwork::ConnectClients(std::vector<sf::TcpSocket *> *client_array)
{
     while (true)
     {
          sf::TcpSocket *new_client = new sf::TcpSocket();
          sf::TcpSocket *online_list_socket  = new sf::TcpSocket();
          if (listener.accept(*new_client) == sf::Socket::Done && listener.accept(*online_list_socket) == sf::Socket::Done)
          {
               new_client->setBlocking(false);
               online_list_socket->setBlocking(false);

               client_array->push_back(new_client);
               onlineListSockets.push_back(online_list_socket);

               logl("Added client " << new_client->getRemoteAddress() << ":" << new_client->getRemotePort() << " on slot " << client_array->size());

               //receive user details upon connection
               ReceiveUserDetails(new_client);

               //send previous messages to the new client
               std::thread sendPrevMessages(&ServerNetwork::SendPrevMessages, this, new_client);
               sendPrevMessages.detach();

               //sending updated onkine people list to all clients
               std::thread sendOnlineList(&ServerNetwork::SendOnlinePeopleList, this);
               sendOnlineList.detach();

          }
          else
          {
               logl("Server listener error, restart the server");
               delete (new_client);
               break;
          }
     }
}

void ServerNetwork::SendPrevMessages(sf::TcpSocket* socket)
{
	 sf::Packet packet;
     for (const auto& message : messages)
     {
		  packet << message.username << message.colorX << message.colorY << message.colorZ << message.colorW << message.message << message.sender_address << message.sender_port;
		  socket->send(packet);
		  packet.clear();       
	 }
}

void ServerNetwork::DisconnectClient(sf::TcpSocket *socket_pointer, size_t position)
{
     logl("Client " << socket_pointer->getRemoteAddress() << ":" << socket_pointer->getRemotePort() << " disconnected, removing");
     socket_pointer->disconnect();
     delete (socket_pointer);
     clientSockets.erase(clientSockets.begin() + position);
     usernames.erase(usernames.begin() + position);
     onlineListSockets.erase(onlineListSockets.begin() + position);

     //sending updated onkine people list to all clients
     std::thread sendOnlineList(&ServerNetwork::SendOnlinePeopleList, this);
     sendOnlineList.detach();

}

void ServerNetwork::BroadcastPacket(sf::Packet &packet, sf::IpAddress exclude_address, unsigned short port)
{
     for (size_t iterator = 0; iterator < clientSockets.size(); iterator++)
     {
          sf::TcpSocket *client = clientSockets[iterator];
          if (client->getRemoteAddress() != exclude_address || client->getRemotePort() != port)
          {
               if (client->send(packet) != sf::Socket::Done)
               {
                    logl("Could not send packet on broadcast");
               }
          }
     }
}

void ServerNetwork::SendOnlinePeopleList()
{
    std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
    sf::Packet packet;

    for (const auto& socket : onlineListSockets)
    {
        //sending the list's size first
        int size = int(usernames.size());
        packet << size;
        socket->send(packet);
        packet.clear();

        //sending the actual names
        for (const auto& user : usernames)
        {
            packet << user.username << user.colorX << user.colorY << user.colorZ;
            socket->send(packet);
            packet.clear();
        }
    }
    std::cout << "IM KILLED" << std::endl;
}

void ServerNetwork::ReceivePackets(sf::TcpSocket *client, size_t iterator)
{
     sf::Packet packet;
     if (client->receive(packet) == sf::Socket::Disconnected)
     {
          DisconnectClient(client, iterator);
          return;
     }

     if (packet.getDataSize() > 0)
     {
          std::string received_message, username;
          float colorX, colorY, colorZ, colorW;
          packet >> username >> colorX >> colorY >> colorZ >> colorW >> received_message;

          Message message;
          message.username = username;
          message.colorX = colorX;
          message.colorY = colorY;
          message.colorZ = colorZ;
          message.colorW = colorW;
          message.message = received_message;
          message.sender_address = client->getRemoteAddress().toString();
          message.sender_port = client->getRemotePort();
          messages.push_back(message);
          logl(client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " " << username << ": " << received_message);

          packet.clear();
          packet << username << colorX << colorY << colorZ << colorW << received_message << client->getRemoteAddress().toString() << client->getRemotePort();

          BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
     }
}

void ServerNetwork::ManagePackets()
{
     while (true)
     {
          for (size_t iterator = 0; iterator < clientSockets.size(); iterator++)
          {
               ReceivePackets(clientSockets[iterator], iterator);
          }

          std::this_thread::sleep_for((std::chrono::milliseconds)250);
     }
}

void ServerNetwork::Run()
{

     std::thread connetion_thread(&ServerNetwork::ConnectClients, this, &clientSockets);

     ManagePackets();
}
