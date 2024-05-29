cmake_minimum_required(VERSION 3.10)
project(server)

set(CMAKE_CXX_STANDARD 14)

# Add the path to the dependencies
get_filename_component(SOLUTION_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

# Source files
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")
add_executable(Chat ${SOURCES} ${HEADERS})

# SFML
set(SFML_INCLUDE "${SOLUTION_DIR}/Dependencies/SFML/include")
target_link_libraries(Chat "${SOLUTION_DIR}/Dependencies/SFML/lib/sfml-system-d.lib")
target_link_libraries(Chat "${SOLUTION_DIR}/Dependencies/SFML/lib/sfml-network-d.lib")

# Include the directories of the libraries
target_include_directories(Chat PRIVATE ${SFML_INCLUDE})

# copying necessary files to project folder
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

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Chat)