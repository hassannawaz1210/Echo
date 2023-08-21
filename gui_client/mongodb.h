#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <imgui.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/oid.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp> 
#include <mongocxx/uri.hpp>

constexpr char dbName[] = "chat_app";
constexpr char usersCollec[] = "usernames";
constexpr char URI[] = "mongodb://localhost:27017";


using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

class MongoDB
{
	mongocxx::uri uri;
	mongocxx::client client;
	mongocxx::database db;
	
public:

	MongoDB();
	void Insert(std::string, ImVec4&);
	void Delete(std::string);
	void ReadAll(std::vector<std::string>&, std::vector<ImVec4>&);

};