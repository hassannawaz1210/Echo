cmake_minimum_required(VERSION 3.10)
project(Chat)

set(CMAKE_CXX_STANDARD 14)

# Add the path to the dependencies
get_filename_component(SOLUTION_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
#list(APPEND CMAKE_MODULE_PATH "${SOLUTION_DIR}/Dependencies")

# ImGui
set(IMGUI_DIR "${SOLUTION_DIR}/Dependencies/ImGui")
set(imgui
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
	${IMGUI_DIR}/imgui.h
	${IMGUI_DIR}/imgui_internal.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/Backends/imgui_impl_opengl3.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Backends/imgui_impl_opengl3.h
	${CMAKE_CURRENT_SOURCE_DIR}/Backends/imgui_impl_opengl3_loader.h
	${CMAKE_CURRENT_SOURCE_DIR}/Backends/imgui_impl_sdl2.h
	${CMAKE_CURRENT_SOURCE_DIR}/Backends/imgui_impl_sdl2.cpp
)

# Source files
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")
add_executable(Chat ${SOURCES} ${HEADERS} ${imgui})

# SFML
set(SFML_INCLUDE "${SOLUTION_DIR}/Dependencies/SFML/include")
target_link_libraries(Chat "${SOLUTION_DIR}/Dependencies/SFML/lib/sfml-system-d.lib")
target_link_libraries(Chat "${SOLUTION_DIR}/Dependencies/SFML/lib/sfml-network-d.lib")

# SDL2
set(SDL2_INCLUDE "${SOLUTION_DIR}/Dependencies/sdl2/include")
target_link_libraries(Chat "${SOLUTION_DIR}/Dependencies/sdl2/lib/x64/SDL2.lib")
target_link_libraries(Chat "${SOLUTION_DIR}/Dependencies/sdl2/lib/x64/SDL2main.lib")

#opengl32
target_link_libraries(Chat "opengl32.lib")

# Include the directories of the libraries
target_include_directories(Chat PRIVATE ${SFML_INCLUDE})
target_include_directories(Chat PRIVATE ${SDL2_INCLUDE})
target_include_directories(Chat PRIVATE ${IMGUI_DIR})

# copying necessary files to project folder
configure_file(${SOLUTION_DIR}/Dependencies/sdl2/lib/x64/SDL2.dll 
			   ${CMAKE_BINARY_DIR}/SDL2.dll 
			   COPYONLY)
			   
configure_file(${SOLUTION_DIR}/Dependencies/SFML/lib/sfml-network-d-2.dll 
			   ${CMAKE_BINARY_DIR}/sfml-network-d-2.dll 
			   COPYONLY)
			   
configure_file(${SOLUTION_DIR}/Dependencies/SFML/lib/sfml-network-2.dll 
			   ${CMAKE_BINARY_DIR}/sfml-network-2.dll 
			   COPYONLY)

configure_file(${SOLUTION_DIR}/Dependencies/SFML/lib/sfml-system-d-2.dll 
			   ${CMAKE_BINARY_DIR}/sfml-system-d-2.dll 
			   COPYONLY)

configure_file(${SOLUTION_DIR}/Dependencies/SFML/lib/sfml-system-2.dll 
			   ${CMAKE_BINARY_DIR}/sfml-system-2.dll 
			   COPYONLY)
			   
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Fonts
     DESTINATION ${CMAKE_BINARY_DIR})
	 
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
     DESTINATION ${CMAKE_BINARY_DIR})
	 
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Chat)