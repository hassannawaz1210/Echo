#pragma once

//essential includes
#include "imgui.h"
#include "Backends/imgui_impl_opengl3.h"
#include "Backends/imgui_impl_sdl2.h"
#include "imgui_internal.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>

//my includes
#include "PeopleWindow.h"
#include "ConnectionWindow.h"
#include "ConversationWindow.h"
#include "User.h"
#include "ClientNetwork.h"


#define INITIAL_WINDOW_WIDTH 1280
#define INITIAL_WINDOW_HEIGHT 720
#define TITLE "BasedChat"


void run();

SDL_Window* createWindow();
void configureGUI();
void setGUIStyle(SDL_Window*& , SDL_GLContext&);

void loop(bool&, SDL_Window*& window);
void handleEvents(bool&, SDL_Window*&);

void destroyGUI();
void destroyWindow(SDL_Window*&, SDL_GLContext&);