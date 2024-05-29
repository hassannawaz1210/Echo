#pragma once
#include "imgui_internal.h"

class User
{
public:
	char username[256];
	ImVec4 color;

	User()
	{
		memset(username, 0, 256);
		color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	int length()
	{
		int count = 0;
		while (username[count] != '\0') count++;

		return count;
	}
};