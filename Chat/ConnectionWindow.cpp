#pragma once
#include "ConnectionWindow.h"

ConnectionWindow::ConnectionWindow()
{
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize;

    color_pick_flags |= ImGuiColorEditFlags_AlphaPreview;
    color_pick_flags |= ImGuiColorEditFlags_NoInputs;
    color_pick_flags |= ImGuiColorEditFlags_NoLabel;
    color_pick_flags |= ImGuiColorEditFlags_DisplayHSV;
}

void ConnectionWindow::SetSize(const float width, const float height)
{
    ImGui::SetNextWindowSize(ImVec2(width, height));
}

void ConnectionWindow::SetPos(const float x, const float y)
{
    ImGui::SetNextWindowPos(ImVec2(x, y));
}

void ConnectionWindow::Display(ClientNetwork& clientNetwork, Message& chatMessages, User& user)
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
           // mongoDB.Insert(std::string(user.username), user.color);

            //sending user's details to server
            sf::Packet packet;
            std::string username(user.username);
            packet << username << user.color.x << user.color.y << user.color.z;
            clientNetwork.SendPacket(packet);
        }
        else
        {
            chatMessages.push_back(std::make_pair(std::string("SERVER: "), std::make_pair(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), (char*)"Failed to connect to the server.")));
        }
    }

    ImGui::End();
}