#pragma once

static sf::Mutex globalMutex;

static sf::TcpListener listener;
static sf::SocketSelector selector;

static bool quit = false;

static struct User {
	unsigned int id;
	std::string username;
	std::string password;

	User(unsigned int id, const char username[], const char password[])
		:id(id), username(username), password(password)
	{}

	User(unsigned int id, std::string username, std::string password)
		:id(id), username(username), password(password)
	{}
};
static std::vector<User> users;