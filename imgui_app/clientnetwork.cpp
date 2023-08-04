#include "clientnetwork.hpp"

ClientNetwork::ClientNetwork()
{
     logl("Chat Client Started");
}

void ClientNetwork::Connect(const char *address, unsigned short port)
{
     if (socket.connect(address, port) != sf::Socket::Done)
     {
          logl("Could not connect to the server\n");
          //exit(0);
     }
     else
     {
          isConnected = true;
          logl("Connected to the server\n");
     }
}

void ClientNetwork::ReceivePackets(sf::TcpSocket *socket, std::vector<char*> *chatMessages)
{
     while (true)
     {
          if (socket->receive(last_packet) == sf::Socket::Done)
          {
               std::string received_string, sender_address, username;
               float colorX, colorY, colorZ, colorW;
               unsigned short sender_port;
               last_packet >> username >> colorX >> colorY >> colorZ >> colorW >> received_string >> sender_address >> sender_port;

               if (received_string.length() >= 1)
               {
                   char* tempMessage = new char[256];
                   //adding the whole message to username string
                   username += ": " + received_string;

                   strcpy_s(tempMessage, 256, username.c_str());
                   chatMessages->push_back(tempMessage);
               }
               logl("From (" << sender_address << ":" << sender_port << "): " << received_string);
          }

          std::this_thread::sleep_for((std::chrono::milliseconds)250);
     }
}

void ClientNetwork::SendPacket(sf::Packet &packet)
{
     if (packet.getDataSize() > 0 && socket.send(packet) != sf::Socket::Done)
     {
          logl("Could not send packet");
     }
}


void ClientNetwork::Disconnect()
{
    socket.disconnect();
}