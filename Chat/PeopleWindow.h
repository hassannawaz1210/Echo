#pragma once
#include <imgui.h>
#include "imgui_internal.h"
#include <vector>
#include <string>

//
#include "User.h"


class PeopleWindow
{
	ImGuiWindowFlags window_flags = 0;
	int online = 0;
public:
	std::vector<User> onlineUsers;

	PeopleWindow();
	void SetSize(const float, const float);
	void SetPos(const float, const float);
	void Display();
};
