#pragma once

#include "imgui.h"
#include "Backends/imgui_impl_sdl2.h"
#include "Backends/imgui_impl_opengl3.h"
#include "../Dependencies/ImGui/imgui_internal.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "clientnetwork.hpp"

#define CONNECTION_WINDOW_WIDTH 370
#define CONNECTION_WINDOW_HEIGHT 305


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
	void Display(std::vector<char*>&, ClientNetwork&, User&);

};

class PeopleWindow
{
	ImGuiWindowFlags window_flags = 0;
	int online = 0;
	bool isEnabled = false;
public:
	PeopleWindow();
	void SetSize(const float, const float);
	void SetPos(const float, const float);
	void Display();
};

class ConnectionWindow
{
	ImGuiWindowFlags window_flags = 0;
	ImGuiColorEditFlags color_pick_flags = 0;
public:
	char serverIP[256] = "";
	unsigned short port = 0;
	//
	ConnectionWindow();
	void SetSize(const float, const float);
	void SetPos(const float, const float);
	void Display(ClientNetwork&, User&);

};