#pragma once

#include "imgui.h"
#include "Backends/imgui_impl_sdl2.h"
#include "Backends/imgui_impl_opengl3.h"
#include "../Dependencies/ImGui/imgui_internal.h"
#include <vector>

class ChatWindow
{
	ImGuiWindowFlags window_flags = 0;
	char message[256] = "";
public:
	ChatWindow();
	void SetSize(const float, const float);
	void SetPos(const float, const float);
	void Display(std::vector<char*>);

};

class PeopleWindow
{
	ImGuiWindowFlags window_flags = 0;
	int online = 0;
public:
	PeopleWindow();
	void SetSize(const float, const float);
	void SetPos(const float, const float);
	void Display();
};