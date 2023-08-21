#include "gui.h"

///////////////////////////////////////////////////
// User Implementation
User::User()
{
    memset(username, 0, 256);
    color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
}

int User::usernameLength()
{
    int count = 0;
    while (username[count] != '\0') count++;
    
    return count;
}


///////////////////////////////////////////////////


///////////////////////////////////////////////////
// Chat Window Implementation

ChatWindow::ChatWindow()
{
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
 //   window_flags |= ImGuiWindowFlags_NoInputs;        //temporarily turned off
 //   window_flags |= ImGuiWindowFlags_NoNav;

    font = nullptr;
    font = ImGui::GetIO().Fonts->AddFontFromFileTTF("Fonts/Ubuntu-Bold.ttf", 14.0f);
    IM_ASSERT(font != nullptr);

}

void ChatWindow::SetSize(const float width, const float height)
{
    ImGui::SetNextWindowSize(ImVec2(width, height));
}

void ChatWindow::SetPos(const float x, const float y)
{
    ImGui::SetNextWindowPos(ImVec2(x , y));
}

bool ClearChat(const char *message)
{
    if (strncmp(message, "/clear", 6) == 0)
    {
        return true;
    }
    return false;
}

void SendMessage(const char* message, std::vector<std::pair<std::string, std::pair<ImVec4, char*>>> &chatMessages, ClientNetwork &clientNetwork, User& user)
{
    char* tempMessage = new char[256];
    strcpy_s(tempMessage, 256, message);
    chatMessages.push_back(std::make_pair(std::string(user.username), std::make_pair(user.color, tempMessage)));

    if (clientNetwork.isConnected)
    {
        sf::Packet input_packet;
        input_packet << user.username;
        input_packet << user.color.x << user.color.y << user.color.z << user.color.w;
        input_packet << message;

        clientNetwork.SendPacket(input_packet);
    }

    //FAILS because of "you:" in the message
    /*if (ClearChat(message))
    {
        chatMessages.clear();
    }*/
}


void ChatWindow::Display(Message&chatMessages, ClientNetwork& clientNetwork, User& user, MongoDB& mongoDB)
{
    //disabling the chat inputs if the user is not connected
    //if (clientNetwork.isConnected && !isEnabled)
    //{
    //    window_flags ^= ImGuiWindowFlags_NoInputs;
    //    window_flags ^= ImGuiWindowFlags_NoNav;               //temporarily turned off
    //    isEnabled = true;
    //}

    ImGui::Begin("Chat", NULL, window_flags);

    //TEXTS REGION
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(71.0f, 71.0f, 71.0f, 0.1f));
    ImGui::BeginChild("textsRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);


    //first is username, second is message
    //const reference to avoid copying

    for(const auto& username : chatMessages)
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
			ImGui::PushFont(font);
            if(user.username == username.first)
            ImGui::TextColored(username.second.first, "You: ");
            else
            ImGui::TextColored(username.second.first, (username.first + ":").c_str());
			ImGui::SameLine();
            ImGui::TextColored(username.second.first, username.second.second);
			ImGui::PopFont();
		}
    ImGui::EndChild();
    ImGui::PopStyleColor(1);

    //INPUT TEXT REGION

    ImGui::PushItemWidth(float(ImGui::GetContentRegionAvail().x * 0.8));
    if (ImGui::InputTextWithHint("##chat_box", "enter text here", message, 256, ImGuiInputTextFlags_EnterReturnsTrue, NULL))
    {
        std::cout << "socket value: " << sf::Socket::Disconnected << "\n";
        SendMessage(message, chatMessages, clientNetwork, user);
        //clear message buffer
        memset(message, '\0', sizeof(message));
        //set keyboard focus to input text
        ImGui::SetKeyboardFocusHere(-1);
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();

    //SEND BUTTON
    if (ImGui::Button("Send")) // Check if the Send button is pressed
    {
        SendMessage(message, chatMessages, clientNetwork, user);
        //clear message buffer
        memset(message, '\0', sizeof(message));
        ImGui::SetKeyboardFocusHere(ImGui::GetID("##chat_box"));
    }

    ImGui::SameLine();
    if (ImGui::Button("Disconnect"))
    {
        if (clientNetwork.isConnected)
        {
		    clientNetwork.Disconnect();
            mongoDB.Delete(user.username);
            chatMessages.push_back(std::make_pair(std::string("SERVER"), std::make_pair(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), (char*)"You have disconnected from the server.")));
        }

        else
        chatMessages.push_back(std::make_pair(std::string("SERVER"), std::make_pair(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), (char*)"You are not connected to any server.")));
    }


    ImGui::End();
}

///////////////////////////////////////////////////



///////////////////////////////////////////////////
// People Window Implementation

PeopleWindow::PeopleWindow()
{
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void PeopleWindow::SetSize(const float width, const float height)
{
    ImGui::SetNextWindowSize(ImVec2(width, height));
}

void PeopleWindow::SetPos(const float x, const float y)
{
    ImGui::SetNextWindowPos(ImVec2(x, y));
}

void PeopleWindow::Display(MongoDB& mongoDB)
{
    ImGui::Begin("People", NULL, window_flags);
    ImGui::Text("Online: %d", online);
    ImGui::BeginChild("peopleRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);

    //ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
      //  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    mongoDB.ReadAll(onlineUsers, usersColors);

    for (int i = 0; i < onlineUsers.size(); i++)
    {
		//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
		ImGui::TextColored(usersColors[i], onlineUsers[i].c_str());
	}

    onlineUsers.clear();
    usersColors.clear();

    ImGui::EndChild();
    ImGui::End();
}

///////////////////////////////////////////////////


///////////////////////////////////////////////////
// Connection Window
ConnectionWindow::ConnectionWindow()
{
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize;

    color_pick_flags |=  ImGuiColorEditFlags_AlphaPreview;
    color_pick_flags |= ImGuiColorEditFlags_NoInputs;
    color_pick_flags |= ImGuiColorEditFlags_NoLabel;
  //  color_pick_flags |= ImGuiColorEditFlags_DisplayHSV;
}

void ConnectionWindow::SetSize(const float width, const float height)
{
    ImGui::SetNextWindowSize(ImVec2(width, height));
}

void ConnectionWindow::SetPos(const float x, const float y)
{
    ImGui::SetNextWindowPos(ImVec2(x, y));
}

void ConnectionWindow::Display(ClientNetwork &clientNetwork, Message& chatMessages, User& user, MongoDB& mongoDB)
{
    ImGui::Begin("Connection", NULL, window_flags);

    ImGui::Text("Username:");
    ImGui::PushItemWidth(float(ImGui::GetContentRegionAvail().x));
    ImGui::InputTextWithHint("##username", "enter username", user.username, sizeof(user.username));
    ImGui::PopItemWidth();

    ImGui::Text("Server IP:");
    ImGui::PushItemWidth(float(ImGui::GetContentRegionAvail().x));
    ImGui::InputTextWithHint("##serverIP", "enter server IP", serverIP, sizeof(serverIP));
    ImGui::PopItemWidth();

    
    ImGui::Text("Port:");
    ImGui::PushItemWidth(float(ImGui::GetContentRegionAvail().x));
    ImGui::InputScalar("##bruh", ImGuiDataType_U16, &port, 0);
    ImGui::PopItemWidth();
        
 
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    //  ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoSidePreview);
    ImGui::ColorEdit4("##userColor", (float*)&user.color, color_pick_flags);
    ImGui::SameLine();
    ImGui::Text("Pick a Color");
    ImGui::SameLine();
    //ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 130);
    if (ImGui::Button("Connect"))
    {
        if (clientNetwork.Connect(serverIP, port))
        {
            std::cout << user.username << "'s console." << std::endl;
            chatMessages.push_back(std::make_pair(std::string("SERVER: "), std::make_pair(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), (char*)"Connected to the server.")));
            mongoDB.Insert(std::string(user.username), user.color);
        }
        else
        {
			chatMessages.push_back(std::make_pair(std::string("SERVER: "), std::make_pair(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), (char*)"Failed to connect to the server.")));
		}
	}
   
    ImGui::End();
}


///////////////////////////////////////////////////