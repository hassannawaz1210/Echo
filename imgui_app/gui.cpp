#include "gui.h"

///////////////////////////////////////////////////
// Chat Window Implementation

ChatWindow::ChatWindow()
{
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
}

void ChatWindow::SetSize(const float width, const float height)
{
    ImGui::SetNextWindowSize(ImVec2(width, height));
}

void ChatWindow::SetPos(const float x, const float y)
{
    ImGui::SetNextWindowPos(ImVec2(x , y));
}

void ChatWindow::Display(std::vector<char*> chatMessages)
{
    ImGui::Begin("Chat", NULL, window_flags);

    //TEXTS REGION
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(71.0f, 71.0f, 71.0f, 0.1f));
    ImGui::BeginChild("textsRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Indent(5.0f);
    for (int i = 0; i < chatMessages.size(); i++)
    {
        ImGui::TextWrapped(chatMessages[i]);
    }
    ImGui::Unindent(5.0f);

    ImGui::EndChild();
    ImGui::PopStyleColor(1);

    //INPUT TEXT REGION

    ImGui::PushItemWidth(float(ImGui::GetContentRegionAvail().x * 0.945));
    if (ImGui::InputTextWithHint("##chat_box", "enter text here", message, 256, ImGuiInputTextFlags_EnterReturnsTrue, NULL))
    {
        char* tempMessage = new char[256];
        strcpy_s(tempMessage, 256, message);
        chatMessages.push_back(tempMessage);
        memset(message, '\0', sizeof(message));
        ImGui::SetKeyboardFocusHere(-1);
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();

    //SEND BUTTON
    if (ImGui::Button("Send")) // Check if the Send button is pressed
    {
        char* tempMessage = new char[256]; // Create a temporary buffer to store the message
        strcpy_s(tempMessage, 256, message); // Copy the message to the temporary buffer
        chatMessages.push_back(tempMessage); // Add the message to the chatMessages vector
        memset(message, '\0', sizeof(message));
        ImGui::SetKeyboardFocusHere(ImGui::GetID("##chat_box"));
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
    ImGui::BeginChild("peopleRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::EndChild();
    ImGui::End();
}

///////////////////////////////////////////////////