#pragma once
#include "ConversationWindow.h"
#include <string>

void SendMessage(char*, Message&, ClientNetwork&, User&);


ConversationWindow::ConversationWindow()
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

void ConversationWindow::SetSize(const float width, const float height)
{
    ImGui::SetNextWindowSize(ImVec2(width, height));
}

void ConversationWindow::SetPos(const float x, const float y)
{
    ImGui::SetNextWindowPos(ImVec2(x, y));
}

bool ClearChat(const char* message)
{
    if (strncmp(message, "/clear", 6) == 0)
    {
        return true;
    }
    return false;
}


void ConversationWindow::Display(Message& chatMessages, ClientNetwork& clientNetwork, User& user)
{
    //disabling the chat inputs if the user is not connected
    //if (clientNetwork.isConnected && !isEnabled)
    //{
    //    window_flags ^= ImGuiWindowFlags_NoInputs;
    //    window_flags ^= ImGuiWindowFlags_NoNav;               //temporarily turned off
    //    isEnabled = true;
    //}

    ImGui::Begin("Chat", NULL, window_flags);

    // ========== TEXTS REGION ===========
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(71.0f, 71.0f, 71.0f, 0.1f));
    ImGui::BeginChild("textsRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

    //first is username, second is message
    for (const auto& username : chatMessages)
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
        ImGui::PushFont(font);
        if (user.username == username.first)
            ImGui::TextColored(username.second.first, "You: ");
        else
            ImGui::TextColored(username.second.first, (username.first + ":").c_str());
        ImGui::SameLine();
        ImGui::TextColored(username.second.first, username.second.second.c_str());
        ImGui::PopFont();
    }
    ImGui::EndChild();
    ImGui::PopStyleColor(1);

    // =========== TEXT REGION END ===========
     
    // ++++++++++++ INPUT TEXT REGION ++++++++++++

    ImGui::PushItemWidth(float(ImGui::GetContentRegionAvail().x * 0.85));
    if (ImGui::InputTextWithHint("##chat_box", "enter text here", message, 256, ImGuiInputTextFlags_EnterReturnsTrue, NULL))
    {
        //std::cout << "socket value: " << sf::Socket::Disconnected << "\n";
        SendMessage(message, chatMessages, clientNetwork, user);
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();

    //SEND BUTTON
    if (ImGui::Button("Send")) // Check if the Send button is pressed
    {
        SendMessage(message, chatMessages, clientNetwork, user);
        //ImGui::SetKeyboardFocusHere(ImGui::GetID("##chat_box"));
    }

    ImGui::SameLine();
    if (ImGui::Button("Disconnect"))
    {
        if (clientNetwork.isConnected)
        {
            clientNetwork.Disconnect();
            chatMessages.push_back(std::make_pair(std::string("SERVER"), std::make_pair(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), (char*)"You have disconnected from the server.")));
        }

        else
            chatMessages.push_back(std::make_pair(std::string("SERVER"), std::make_pair(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), (char*)"You are not connected to any server.")));
    }


    ImGui::End();
}

void SendMessage(char* message, Message& chatMessages, ClientNetwork& clientNetwork, User& user)
{
    chatMessages.push_back(std::make_pair(std::string(user.username), std::make_pair(user.color, std::string(message))));

    if (clientNetwork.isConnected)
    {
        sf::Packet input_packet;
        input_packet << user.username;
        input_packet << user.color.x << user.color.y << user.color.z << user.color.w;
        input_packet << message;

        clientNetwork.SendPacket(input_packet);

      //  clientNetwork.DownloadMessages(chatMessages);
    }

    //!FAILS: because of "you:" in the message
    /*if (ClearChat(message))
    {
        chatMessages.clear();
    }*/


    //clear message buffer
    memset(message, '\0', sizeof(message));
    //set keyboard focus back to input text
    ImGui::SetKeyboardFocusHere(-1);
}
