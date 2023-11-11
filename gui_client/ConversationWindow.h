#pragma once
#include "imgui_internal.h"
#include <vector>
#include <string>

//
#include "User.h"
#include "ClientNetwork.h"

typedef std::vector<std::pair<std::string, std::pair<ImVec4, std::string>>> Message;

class ConversationWindow
{
	ImGuiWindowFlags window_flags = 0;
	ImFont* font;
	char message[256] = "";

public:
	ConversationWindow();
	void SetSize(const float, const float);
	void SetPos(const float, const float);
	void Display(Message&, ClientNetwork&, User&);
//	bool operator<(const User&);

};

