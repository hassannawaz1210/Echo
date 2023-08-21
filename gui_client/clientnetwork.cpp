#include "clientnetwork.hpp"

ClientNetwork::ClientNetwork()
{
     logl("Chat Client Started");
}

bool ClientNetwork::Connect(const char *address, unsigned short port)
{
     if (socket.connect(address, port) != sf::Socket::Done)
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

void ClientNetwork::ReceivePackets(sf::TcpSocket *socket, Message* chatMessages, MongoDB* mongoDB, User *user)
{
     while (true)
     {
         if (isConnected)
         {
              if (socket->receive(last_packet) == sf::Socket::Done)
              {
                   std::string received_string = "", sender_address = "", username ="";
                   float colorX = 0, colorY = 0, colorZ = 0, colorW = 0;
                   unsigned short sender_port = 0;
                   last_packet >> username >> colorX >> colorY >> colorZ >> colorW >> received_string >> sender_address >> sender_port;

                   if (received_string.length() >= 1)
                   {
                       char* tempMessage = new char[256];
                       strcpy_s(tempMessage, 256, received_string.c_str());
                       chatMessages->push_back(std::make_pair(username, std::make_pair(ImVec4(colorX, colorY, colorZ, colorW), tempMessage)));

                   }
                   logl("From (" << sender_address << ":" << sender_port << "): " << username << " " << received_string);
              }

              //if socket is disconnected from the server somehow
              else if (socket->receive(last_packet) == sf::Socket::Disconnected)
              {
                  logl("I RANNNNNNNNNNNNNNNNNNNNNNNNNNNNN");
                  isConnected = false;
                  chatMessages->push_back(std::make_pair("SERVER: ", std::make_pair(ImVec4(1, 1, 1, 1), (char*)"Disconnected from the server")));
                  logl("Disconnected from the server");
              }
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
    isConnected = false;
}