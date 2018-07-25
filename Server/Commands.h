#pragma once
#include <regex>

bool Server_Command(std::string command) {
	if (regex_match(command, std::regex("\/exit", std::regex_constants::icase))) {
		globalMutex.lock();
		quit = true;
		globalMutex.unlock();
		return true;
	}
	else if (regex_match(command, std::regex("\/help", std::regex_constants::icase))) {
		std::cout << "/exit                                  -	Server shutdown" << std::endl;
		std::cout << "/help                                  -	List commands" << std::endl;
		std::cout << "/List Item [Item ID]                   -	List Item data" << std::endl;
		std::cout << "/List Items                            -	List Items data" << std::endl;
		std::cout << "/List Player Inventory [Player Name]   -	List Player Inventory" << std::endl;
		std::cout << "/List Player [Player Name]             -	List Player data" << std::endl;
		std::cout << "/List Players                          -	List Players data" << std::endl;
		std::cout << "/Kick [Player Name]                    -	Kick Player" << std::endl;
		return true;
	}
	else if (regex_match(command, std::regex("\/List Item \\w+", std::regex_constants::icase))) {
		std::string item_ID = command.substr(11, command.size() - 10);

		for (Item &item : items)
		{
			if (item.id == std::stoi(item_ID)) {
				std::cout << item.name << " data:" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << "ID: " << item.id << std::endl;
				std::cout << "Owner: " << item.owner << std::endl;
				std::cout << "Type: " << item.type << std::endl;
				std::cout << "Name: " << item.name << std::endl;
				std::cout << "X Position: " << item.collisionRect.getPosition().x << std::endl;
				std::cout << "Y Position: " << item.collisionRect.getPosition().y << std::endl;
				std::cout << "Grabbed: " << item.grabbed << std::endl;
				std::cout << "InInventory: " << item.inInventory << std::endl;
				if (item.inInventory) {
					std::cout << "X Inventory Position: " << item.inventoryPosition.x << std::endl;
					std::cout << "Y Inventory Position: " << item.inventoryPosition.y << std::endl;
				}
				std::cout << "Inventory Size X: " << item.inventorySize.x << std::endl;
				std::cout << "Inventory Size Y: " << item.inventorySize.y << std::endl;
				std::cout << "Equipped: " << item.equipped << std::endl;
				std::cout << "Visible: " << item.visible << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				return true;
			}
		}
		std::cout << "No item found with : " << item_ID << " id." << std::endl;
		return true;
	}
	else if (regex_match(command, std::regex("\/List ?Items", std::regex_constants::icase))) {
		std::cout << "Item List:" << std::endl;
		std::cout << "--------------------------------------------------" << std::endl;
		for (Item &item : items)
		{
			std::cout << item.id << ' ' << item.name << std::endl;
		}
		std::cout << "--------------------------------------------------" << std::endl;
		return true;
	}
	else if (regex_match(command, std::regex("\/List Player Inventory \\w+", std::regex_constants::icase))) {
		std::string player_Name = command.substr(23, command.size() - 22);

		for (Player &p : players) {
			if (p.username == player_Name) {
				std::cout << p.username << " inventory:" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;

				for (Item &item : items)
					if (item.owner == p.id) {
						std::cout << "ID: " << item.id << std::endl;
						std::cout << "Type: " << item.type << std::endl;
						std::cout << "Name: " << item.name << std::endl;
					}

				std::cout << std::endl;

				for (int y = 0; y < inventoryY; ++y) {
					for (int x = 0; x < inventoryX; ++x)
						std::cout << p.inventoryFreeSpace[x][y] << ' ';
					std::cout << std::endl;
				}
				std::cout << "--------------------------------------------------" << std::endl;
				return true;
			}
		}
		std::cout << "No player named: " << player_Name << std::endl;
		return true;
	}
	else if (regex_match(command, std::regex("\/List Player \\w+", std::regex_constants::icase))) {
		std::string player_Name = command.substr(13, command.size() - 12);

		for (Player &p : players) {
			if (p.username == player_Name) {
				std::cout << p.username << " data:" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				std::cout << "ID: " << p.id << std::endl;
				std::cout << "IP: " << p.socket->getRemoteAddress() << std::endl;
				std::cout << "Logged in: " << p.logged_in << std::endl;
				std::cout << "Username: " << p.username << std::endl;
				std::cout << "Position: " << p.collisionRect.getPosition().x << ' ' << p.collisionRect.getPosition().y << std::endl;
				std::cout << "Direction: " << p.direction << std::endl;
				std::cout << "Time: " << p.time << std::endl;
				std::cout << "Health: " << p.health << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
				return true;
			}
		}
		std::cout << "No player named: " << player_Name << std::endl;
		return true;
	}
	else if (regex_match(command, std::regex("\/List ?Players", std::regex_constants::icase))) {
		std::cout << "Player List:" << std::endl;
		std::cout << "--------------------------------------------------" << std::endl;
		for (Player &player : players)
		{
			std::cout << player.socket->getRemoteAddress() << ' ' << player.username << std::endl;
		}
		std::cout << "--------------------------------------------------" << std::endl;
		return true;
	}
	else if (regex_match(command, std::regex("\/Kick \\w+", std::regex_constants::icase))) {
		std::string Player_Name = command.substr(6, command.size() - 5);
		sf::Packet packet;

		
		for (unsigned int i = 0; i < players.size(); ++i) {
			if (players[i].username == Player_Name) {
				packet << Kicked_out;
				players[i].socket->send(packet);
				std::cout << CurrentTime() << players[i].username << " has been kicked out." << std::endl;
				Client_Disconnecting(i);
				return true;
			}
		}
		std::cout << "No player named: " << Player_Name << std::endl;
		return true;
	}
	return false;
}