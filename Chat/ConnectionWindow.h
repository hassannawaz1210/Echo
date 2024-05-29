#pragma once
#include "imgui_internal.h"

//
#include "ClientNetwork.h"
#include "User.h"

#define CONNECTION_WINDOW_WIDTH 370
#define CONNECTION_WINDOW_HEIGHT 305

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
	void Display(ClientNetwork&, Message&, User&);

};