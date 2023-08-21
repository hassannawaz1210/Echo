#include "mongodb.h"

MongoDB::MongoDB(): uri( mongocxx::uri(URI)) , client(mongocxx::client(uri)), db( client[dbName])
{}

void MongoDB::Insert(std::string username, ImVec4& color)
{
	auto collection = db[usersCollec];
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder
		<< "username" << username
		<< "colorX" << color.x
		<< "colorY" << color.y
		<< "colorZ" << color.z
		<< "colorW" << color.w
		<< bsoncxx::builder::stream::finalize;
	collection.insert_one(doc_value.view());
}

void MongoDB::Delete(std::string username)
{
	auto collection = db[usersCollec];
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder
		<< "username" << username
		<< bsoncxx::builder::stream::finalize;
	collection.delete_one(doc_value.view());
}

void MongoDB::ReadAll(std::vector<std::string> &usernames, std::vector<ImVec4>& colors)
{
	auto collection = db[usersCollec];
	auto cursor = collection.find(bsoncxx::builder::stream::document{} << bsoncxx::builder::stream::finalize);
	bsoncxx::document::view doc;
	float x, y, z, w;
	for (auto&& doc : cursor) {
		usernames.emplace_back(bsoncxx::stdx::string_view{ doc["username"].get_string().value });
		x = doc["colorX"].get_double().value;
		y = doc["colorY"].get_double().value;
		z = doc["colorZ"].get_double().value;
		w = doc["colorW"].get_double().value;
		colors.emplace_back(ImVec4(x, y, z, w));
	}

}

//int main()
//{
//	mongocxx::instance instance;
//	MongoDB mongo;
//
//
//	return 0;
//
//}