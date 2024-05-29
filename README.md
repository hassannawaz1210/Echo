# Chat-App
Desktop Chat Application ( SDL + Sfml-Network + ImGui ) 

# Prerequisites
- Git installed on your system
- CMake installed on your system
- Visual Studio (or any IDE compatible with C++) installed on your system

# Setting up and running the project
### 1. ***Clone the Repository***:
Open your terminal (Command Prompt, Git Bash, or Terminal on MacOS/Linux) and clone the repository using the following command:
```
git clone github.com/hassannawaz1210/Chat-App
```
### 2. ***Navigate to the 'Chat' Folder***:
Once the repository is cloned, navigate into the 'Chat' folder using the following command:
```
cd Chat
```
### 3. ***Create a Build Folder***:
Inside the 'Chat' folder, create a 'build' folder. You can do this manually or by running the following command:
```
mkdir build
```
### 4. ***Generate Project Files***:
Open Command Prompt or Terminal and navigate to the 'build' folder. Then, run the following command to generate project files using CMake:
```
cd build
cmake ..
```
### 5. ***Open the Project in Visual Studio***:
Once the project files are generated, you will find them inside the 'build' folder.

### *** Important Note***:
Dont change the directory structure.
### ***Note***:
You have to build both projects(Server and Chat) separately.
### ***Note***:
Project is only running in Debug configuration only (for now)  
### ***Note***: 
There will be two copies of '**Fonts**' and '**assets**'. One copy inside Chat folder is necessary for Cmake to run. The other copy inside build folder is necessary for project to be built.
If you are certain that you wont be running Cmake again, you can get rid of the copy inside Chat folder.

# Screens
![image](https://github.com/hassannawaz1210/Chat-App/assets/119103060/e9cbb536-0089-41f8-acf1-a26beb0ad557)
