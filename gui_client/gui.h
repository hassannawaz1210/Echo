#pragma once

#include "imgui.h"
#include "Backends/imgui_impl_sdl2.h"
#include "Backends/imgui_impl_opengl3.h"
#include "../Dependencies/ImGui/imgui_internal.h"
#include <utility> 
#include <vector>
#include <string>
#include <iostream>
//#include <map>
#include "clientnetwork.hpp"
#include "mongodb.h"

#define CONNECTION_WINDOW_WIDTH 370
#define CONNECTION_WINDOW_HEIGHT 305

typedef std::vector<std::pair<std::string, std::pair<ImVec4, char*>>> Message;



class User
{
public:
	char username[256];
	ImVec4 color;
	User();
	int usernameLength();
};

class ChatWindow
{
	ImGuiWindowFlags window_flags = 0;
	ImFont* font;
	char message[256] = "";
	bool isEnabled = false;
public:
	ChatWindow();
	void SetSize(const float, const float);
	void SetPos(const float, const float);
	void Display(Message&, ClientNetwork&, User&, MongoDB&);
	bool operator<(const User&);

};

class PeopleWindow
{
	ImGuiWindowFlags window_flags = 0;
	int online = 0;
	bool isEnabled = false;
	std::vector<std::string> onlineUsers;
	std::vector<ImVec4> usersColors;
public:
	PeopleWindow();
	void SetSize(const float, const float);
	void SetPos(const float, const float);
	void Display(MongoDB&);
};

class ConnectionWindow
{
	ImGuiWindowFlags window_flags = 0;
	ImGuiColorEditFlags color_pick_flags = 0;
public:
	char serverIP[256] = "127.0.0.1";
	unsigned short port = 2525;
	//
	ConnectionWindow();
	void SetSize(const float, const float);
	void SetPos(const float, const float);
	void Display(ClientNetwork&, Message&, User&, MongoDB&);

};