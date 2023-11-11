#pragma once
#include "PeopleWindow.h"

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

void PeopleWindow::Display()
{
    ImGui::Begin("People", NULL, window_flags);
    ImGui::Text("Online: %d", online);

    //ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
      //  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
   // mongoDB.ReadAll(onlineUsers, usersColors);
    online = int(onlineUsers.size());
    for (const auto& user: onlineUsers)
    {
        float x = user.color.x;
        float y = user.color.y;
        float z = user.color.z;

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
        ImGui::TextColored(ImVec4(x, y, z, 1), user.username);
    }

   // onlineUsers.clear();

    ImGui::End();
}