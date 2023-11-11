#pragma once
#include "global_functions.h"
#define PADDING 5


void run()
{
	SDL_Window* window = createWindow();
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);

	configureGUI();
	setGUIStyle(window, gl_context);

	bool running = true;
	loop(running, window);

	destroyGUI();
	destroyWindow(window, gl_context);
}

SDL_Window* createWindow()
{
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		std::cout << "Error: %s\n" << SDL_GetError();
		exit(0);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	// ============ Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_WindowFlags sdl_window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
	SDL_Window* window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, sdl_window_flags);
	SDL_SetWindowBordered(window, SDL_TRUE);
	SDL_GL_SetSwapInterval(1); // Enable vsync
	return window;
	
}


void configureGUI()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
}

void setGUIStyle(SDL_Window*& window, SDL_GLContext& gl_context)
{

	// Setup Dear ImGui style
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 12.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		style.WindowBorderSize = 0.0f;
		style.FrameRounding = 12.f;
		style.GrabRounding = 3.f;
		style.ChildRounding = 3.f;
		style.PopupRounding = 3.f;
		style.FramePadding = ImVec2(10.f, 10.f);
		style.WindowPadding = ImVec2(15.f, 15.f);
		style.ItemSpacing = ImVec2(5.0f, 10.0f);
		style.IndentSpacing = 10.f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

		// Setup Platform/Renderer backends
		const char* glsl_version = "#version 130";
		ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
		ImGui_ImplOpenGL3_Init(glsl_version);

		io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//IM_ASSERT(font != nullptr);
	}
}

void handleEvents(bool &running, SDL_Window*& window)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
		{
			running = false;
		}
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
		{
			running = false;
		}
	}
}

void loop(bool& running, SDL_Window*& window)
{
	//gui variables
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//gui window variables
	ConversationWindow conversationWindow;
	PeopleWindow peopleWindow;
	ConnectionWindow connectionWindow;

	//sdl window position variables
	int windowPosX = 0, windowPosY = 0;
	int windowWidth = 0, windowHeight = 0;

	//
	User user;

	//
	ClientNetwork clientNetwork;
	
	//
	typedef std::vector<std::pair<std::string, std::pair<ImVec4, std::string>>> Message;
	Message chatMessages;

	//
	std::thread thread_1(&ClientNetwork::ReceiveMessages, &clientNetwork, &clientNetwork.messagesSocket, &chatMessages, &user);
	std::thread thread_2(&ClientNetwork::ReceiveOnlineList, &clientNetwork, &clientNetwork.onlineListSocket, &peopleWindow.onlineUsers);
	thread_1.detach();
	thread_2.detach();


	//main loop
	while (running)
	{
		handleEvents(running, window);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		//================ WINDOWS
		//ImGui::ShowDemoWindow(&running);

		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		SDL_GetWindowPosition(window, &windowPosX, &windowPosY);
		//CHAT WINDOW
		conversationWindow.SetSize(float(0.8 * windowWidth) - PADDING, float(windowHeight));
		conversationWindow.SetPos(float(windowPosX), float(windowPosY));
		conversationWindow.Display(chatMessages, clientNetwork, user);

		//PEOPLE WINDOW
		peopleWindow.SetSize(float(0.2 * windowWidth), float(windowHeight));
		peopleWindow.SetPos(float(windowPosX) + float(0.8 * windowWidth), float(windowPosY));
		peopleWindow.Display();

		//CONNECTION WINDOW


		//ISSUE: the following code is not working properly
		bool isDisconnected = (clientNetwork.onlineListSocket.getRemoteAddress() == sf::IpAddress::None);
		if (isDisconnected) clientNetwork.isConnected = false;

		if (!clientNetwork.isConnected)
		{
			//connectionWindow.SetPos(float(windowPosX) + (INITIAL_WINDOW_WIDTH / 2) - (CONNECTION_WINDOW_WIDTH / 2), float(windowPosY) + (INITIAL_WINDOW_HEIGHT / 2) - (CONNECTION_WINDOW_HEIGHT / 2));
			connectionWindow.SetSize(CONNECTION_WINDOW_WIDTH, CONNECTION_WINDOW_HEIGHT);
			connectionWindow.Display(clientNetwork, chatMessages, user);
		}

		//================ WINDOWS END

		// Rendering
		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		// Update and Render additional Platform Windows
	   // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	   //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}

		SDL_GL_SwapWindow(window);
	}
	
}

void destroyGUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
void destroyWindow(SDL_Window*& window, SDL_GLContext& gl_context)
{
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(0);
}