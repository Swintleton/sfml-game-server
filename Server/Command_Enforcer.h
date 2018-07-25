#pragma once
#include "Broadcaster.h"

void move_Player_Failed(Player &player, sf::Vector2f oldPos) {
	sf::Packet packet;
	//set back old Position
	player.collisionRect.setPosition(oldPos);
	//send to client old Position
	packet << Move_Player << static_cast<sf::Int64>(oldPos.x) << static_cast<sf::Int64>(oldPos.y);
	player.socket->send(packet);
}

void move_Player(Player &player, sf::Packet &packet, float deltaTime) {
	//Read packet
	sf::Int64 x = player.collisionRect.getPosition().x;
	sf::Int64 y = player.collisionRect.getPosition().y;
	sf::Uint8 direction = player.direction;
	packet >> x >> y >> direction;

	//Save current position
	sf::Vector2f oldPos = player.collisionRect.getPosition();

	//Set new position
	player.collisionRect.setPosition(x, y);
	//Set new direction
	player.direction = direction;

	//Check for Collisions
	sf::RectangleShape w;
	for (unsigned int i = 0; i < walls.size(); ++i) {
		w = walls[i].collisionRect;
		//Put the collision rectangle up or down
		if (player.collisionRect.getPosition().y < w.getPosition().y + 30)
			w.setPosition(playerBeforeWall[i]);
		if (player.collisionRect.getPosition().y > w.getPosition().y - 30)
			w.setPosition(playerBehindWall[i]);

		//Checking for collision
		if (player.collisionRect.getGlobalBounds().intersects(w.getGlobalBounds())) {
			move_Player_Failed(player, oldPos);
			return;
		}
	}

	//Set limit
	float limit = 2;
	if (x != oldPos.x && y != oldPos.y)
		limit = std::round(player.speed * deltaTime + 1);
	else
		limit = std::round(player.speed * deltaTime + 0.6f);

	//Check move is possible: Compare the distance between the new and old position
	if (std::sqrt(std::pow(std::abs(x - oldPos.x), 2) + std::pow(std::abs(y - oldPos.y), 2)) > limit) {
		move_Player_Failed(player, oldPos);
		return;
	}
	//Broadcast the player new position
	broadcast_Player_Moved(player);
}

void Running_Player(Player &player) {
	player.running = true;
	player.speed = 160;
	broadcast_Running_Player(player);
}

void Walking_Player(Player &player) {
	player.running = false;
	player.speed = 100;
	broadcast_Walking_Player(player);
}

void item_Grabbing_Failed(Player &player, Item &item) {
	sf::Packet packet;
	packet << Item_Grabbing_Failed;
	packet << item.id;
	packet << static_cast<sf::Int64>(item.collisionRect.getPosition().x);
	packet << static_cast<sf::Int64>(item.collisionRect.getPosition().y);
	player.socket->send(packet);
}

void item_Inserting_Failed(Player &player, Item &item) {
	sf::Packet packet;
	packet << Item_inInventory_Failed;
	packet << item.id;
	player.socket->send(packet);
}

void item_Auto_Inserting_Failed(Player &player, Item &item) {
	sf::Packet packet;
	packet << Item_Auto_Insert;
	packet << item.id;
	packet << static_cast<sf::Int64>(item.collisionRect.getPosition().x);
	packet << static_cast<sf::Int64>(item.collisionRect.getPosition().y);
	player.socket->send(packet);
}

void item_Take_Out_Failed(Player &player, Item &item) {
	sf::Packet packet;
	packet << Item_Take_Off;
	packet << item.id;
	packet << static_cast<unsigned char>(item.inventoryPosition.x);
	packet << static_cast<unsigned char>(item.inventoryPosition.y);
	player.socket->send(packet);
}

void item_Releasing_Failed(Player &player, Item &item) {
	sf::Packet packet;
	packet << Item_Releasing_Failed;
	packet << item.id;
	player.socket->send(packet);
}

void item_Repositioning_Failed(Player &player, Item &item) {
	sf::Packet packet;
	packet << Item_Repositioning_Failed;
	packet << item.id;
	packet << static_cast<unsigned char>(item.inventoryPosition.x);
	packet << static_cast<unsigned char>(item.inventoryPosition.y);
	player.socket->send(packet);
}

//Player grabbed "new" item
void item_Grabbed(Player &player, sf::Packet &packet) {
	//Read received data
	unsigned int item_id = 0;
	packet >> item_id;

	//Search which item was grabbed
	unsigned int i = 0;
	for (; i < items.size(); ++i)
		if (items[i].id == item_id)
			break;

	//If the item was not found, idk, do nothing
	if (i >= items.size()) {
		return;
	}

	//Item is free or is it in the inventory?
	if (items[i].owner != -1 && items[i].owner != player.id) {
		item_Grabbing_Failed(player, items[i]);
		return;
	}

	//Item pick up possible?
	if (!items[i].inInventory && !items[i].grabbed) {
		//If distance between the player and the item is larger than 200
		if (std::sqrt(std::pow(std::abs(player.collisionRect.getPosition().x - items[i].collisionRect.getPosition().x), 2) + std::pow(std::abs(player.collisionRect.getPosition().y - items[i].collisionRect.getPosition().y), 2)) > 200) {
			item_Grabbing_Failed(player, items[i]);
			return;
		}
	}

	//Set item grabbed
	items[i].grabbed = true;
	items[i].owner = player.id;
	//Broadcast item grabbed
	broadcast_Item_Grabbed(player, items[i]);
}

void item_Insert(Player &player, sf::Packet &packet) {
	//Read received data
	unsigned int item_id = 0;
	unsigned char InsertingCellX = 0, InsertingCellY = 0;
	packet >> item_id >> InsertingCellX >> InsertingCellY;

	//Search which item inserted
	unsigned int i = 0;
	for (; i < items.size(); ++i)
		if (items[i].id == item_id)
			break;

	//If the item was not found, idk, do nothing
	if (i >= items.size()) {
		return;
	}

	//Item grabbed?
	if (!items[i].grabbed) {
		item_Inserting_Failed(player, items[i]);
		return;
	}

	//Beginning of the inserting process
	unsigned char itemSizeX = items[i].inventorySize.x, itemSizeY = items[i].inventorySize.y;

	//The entire item fits in the inventory?
	if (InsertingCellX + itemSizeX > inventoryX || InsertingCellY + itemSizeY > inventoryY) {
		item_Inserting_Failed(player, items[i]);
		return;
	}

	//At the position the space is free?
	for (unsigned char y = InsertingCellY; y < InsertingCellY + itemSizeY; ++y)
		for (unsigned char x = InsertingCellX; x < InsertingCellX + itemSizeX; ++x)
			if (!player.inventoryFreeSpace[x][y]) {
				item_Inserting_Failed(player, items[i]);
				return;
			}

	//Captured inventory space set to false
	for (unsigned char y = InsertingCellY; y < InsertingCellY + itemSizeY; ++y)
		for (unsigned char x = InsertingCellX; x < InsertingCellX + itemSizeX; ++x)
			player.inventoryFreeSpace[x][y] = false;

	//Insert item into the player inventory
	items[i].owner = player.id;
	items[i].inventoryPosition.x = InsertingCellX;
	items[i].inventoryPosition.y = InsertingCellY;
	items[i].inInventory = true;
	items[i].grabbed = false;
}

void item_AutoInsert(Player &player, sf::Packet &packet) {
	//Read received data
	unsigned int item_id = 0;
	unsigned char InsertingCellX = 0, InsertingCellY = 0;
	packet >> item_id >> InsertingCellX >> InsertingCellY;

	//Search which item inserted
	unsigned int i = 0;
	for (; i < items.size(); ++i)
		if (items[i].id == item_id)
			break;

	//If the item was not found, idk, do nothing
	if (i >= items.size()) {
		return;
	}

	//If distance between the player and the item position is larger than 200
	if (std::sqrt(std::pow(std::abs(player.collisionRect.getPosition().x - items[i].collisionRect.getPosition().x), 2) + std::pow(std::abs(player.collisionRect.getPosition().y - items[i].collisionRect.getPosition().y), 2)) > 200) {
		item_Auto_Inserting_Failed(player, items[i]);
		return;
	}

	//Beginning of the inserting process
	unsigned char itemSizeX = items[i].inventorySize.x, itemSizeY = items[i].inventorySize.y;

	//The entire item fits in the inventory?
	if (InsertingCellX + itemSizeX > inventoryX || InsertingCellY + itemSizeY > inventoryY) {
		item_Inserting_Failed(player, items[i]);
		return;
	}

	//At the position the space is free?
	for (unsigned char y = InsertingCellY; y < InsertingCellY + itemSizeY; ++y)
		for (unsigned char x = InsertingCellX; x < InsertingCellX + itemSizeX; ++x)
			if (!player.inventoryFreeSpace[x][y]) {
				item_Inserting_Failed(player, items[i]);
				return;
			}

	//Captured inventory space set to false
	for (unsigned char y = InsertingCellY; y < InsertingCellY + itemSizeY; ++y)
		for (unsigned char x = InsertingCellX; x < InsertingCellX + itemSizeX; ++x)
			player.inventoryFreeSpace[x][y] = false;

	//Insert item into the player inventory
	items[i].owner = player.id;
	items[i].inventoryPosition.x = InsertingCellX;
	items[i].inventoryPosition.y = InsertingCellY;
	items[i].inInventory = true;

	//Broadcast item grabbed
	broadcast_Item_Grabbed(player, items[i]);
}

void item_Take_Out(Player &player, sf::Packet &packet) {
	//Read received data
	unsigned int item_id = 0;
	sf::Int64 x = 0, y = 0;
	packet >> item_id >> x >> y;

	//Search which item was taked out
	unsigned int i = 0;
	for (; i < items.size(); ++i)
		if (items[i].id == item_id)
			break;

	//If the item was not found, idk, do nothing
	if (i >= items.size()) {
		return;
	}

	//Player owns this item?
	if (items[i].owner != player.id) {
		item_Take_Out_Failed(player, items[i]);
		return;
	}

	//Item grabbed?
	if (!items[i].grabbed) {
		item_Take_Out_Failed(player, items[i]);
		return;
	}

	//If distance between the player and the releasing position is larger than 200
	if (std::sqrt(std::pow(std::abs(player.collisionRect.getPosition().x - x), 2) + std::pow(std::abs(player.collisionRect.getPosition().y - y), 2)) > 200) {
		item_Releasing_Failed(player, items[i]);
		return;
	}

	//Release item
	items[i].grabbed = false;
	items[i].inInventory = false;
	items[i].equipped = false;
	items[i].owner = -1;
	items[i].collisionRect.setPosition(x, y);

	//Free up player inventory
	for (unsigned char y = items[i].inventoryPosition.y; y < items[i].inventoryPosition.y + items[i].inventorySize.y; ++y)
		for (unsigned char x = items[i].inventoryPosition.x; x < items[i].inventoryPosition.x + items[i].inventorySize.x; ++x)
			player.inventoryFreeSpace[x][y] = true;

	//Broadcast item released
	broadcast_Item_Released(player, items[i]);
}

void item_Repositioned(Player &player, sf::Packet &packet) {
	//Read received data
	unsigned int item_id = 0;
	unsigned char InsertingCellX = 0, InsertingCellY = 0;
	packet >> item_id >> InsertingCellX >> InsertingCellY;

	//Search which item was repositioned
	unsigned int i = 0;
	for (; i < items.size(); ++i)
		if (items[i].id == item_id)
			break;

	//If the item was not found, idk, do nothing
	if (i >= items.size()) {
		return;
	}

	//The player owns this item?
	if (items[i].owner != player.id) {
		item_Releasing_Failed(player, items[i]);
		return;
	}

	//Item is inside the inventory?
	if (!items[i].inInventory) {
		item_Releasing_Failed(player, items[i]);
		return;
	}

	//Item is grabbed?
	if (!items[i].grabbed) {
		item_Releasing_Failed(player, items[i]);
		return;
	}

	//Beginning of the positioning process
	unsigned char itemSizeX = items[i].inventorySize.x, itemSizeY = items[i].inventorySize.y;

	//The entire item fits in the inventory?
	if (InsertingCellX + itemSizeX > inventoryX || InsertingCellY + itemSizeY > inventoryY) {
		item_Inserting_Failed(player, items[i]);
		return;
	}

	//Free up last position space
	for (unsigned char y = items[i].inventoryPosition.y; y < items[i].inventoryPosition.y + itemSizeY; ++y)
		for (unsigned char x = items[i].inventoryPosition.x; x < items[i].inventoryPosition.x + itemSizeX; ++x)
			player.inventoryFreeSpace[x][y] = true;

	//At the position the space is free?
	for (unsigned char y = InsertingCellY; y < InsertingCellY + itemSizeY; ++y)
		for (unsigned char x = InsertingCellX; x < InsertingCellX + itemSizeX; ++x)
			if (!player.inventoryFreeSpace[x][y]) {
				item_Inserting_Failed(player, items[i]);
				return;
			}

	//Captured inventory space set to false
	for (unsigned char y = InsertingCellY; y < InsertingCellY + itemSizeY; ++y)
		for (unsigned char x = InsertingCellX; x < InsertingCellX + itemSizeX; ++x)
			player.inventoryFreeSpace[x][y] = false;

	//Set new item position
	items[i].owner = player.id;
	items[i].inventoryPosition.x = InsertingCellX;
	items[i].inventoryPosition.y = InsertingCellY;
	items[i].grabbed = false;
}

void item_Released(Player &player, sf::Packet &packet) {
	//Read received data
	unsigned int item_id = 0;
	sf::Int64 x = 0, y = 0;
	packet >> item_id >> x >> y;

	//Search which item was released
	unsigned int i = 0;
	for (; i < items.size(); ++i)
		if (items[i].id == item_id)
			break;

	//If the item was not found, idk, do nothing
	if (i >= items.size()) {
		return;
	}

	//The player owns this item?
	if (items[i].owner != player.id) {
		item_Releasing_Failed(player, items[i]);
		return;
	}

	//Item is grabbed?
	if (!items[i].grabbed) {
		item_Releasing_Failed(player, items[i]);
		return;
	}

	//If distance between the player and the releasing position is larger than 200
	if (std::sqrt(std::pow(std::abs(player.collisionRect.getPosition().x - x), 2) + std::pow(std::abs(player.collisionRect.getPosition().y - y), 2)) > 200) {
		item_Releasing_Failed(player, items[i]);
		return;
	}

	//Set item released
	items[i].grabbed = false;
	items[i].inInventory = false;
	items[i].equipped = false;
	items[i].owner = -1;
	items[i].collisionRect.setPosition(x, y);

	//Broadcast item released
	broadcast_Item_Released(player, items[i]);
	return;
}

void sendPlayer(Player &player) {
	sf::Packet packet;
	packet << Send_Player;
	packet << player.id;
	packet << player.username;
	packet << player.dead;
	packet << player.running;
	packet << static_cast<sf::Int64>(player.speed);
	packet << static_cast<sf::Int64>(player.attackSpeed * 100);
	packet << static_cast<sf::Int64>(player.maxHealth);
	packet << static_cast<sf::Int64>(player.health);
	packet << static_cast<sf::Int64>(player.maxMana);
	packet << static_cast<sf::Int64>(player.mana);
	packet << static_cast<sf::Int64>(player.collisionRect.getPosition().x);
	packet << static_cast<sf::Int64>(player.collisionRect.getPosition().y);
	packet << player.direction;
	player.socket->send(packet);
}

void sendWalls(Player &player) {
	sf::Packet packet;
	unsigned int stepper = 0;

	packet << Send_Walls;
	for (Wall &w : walls) {
		packet << w.id;
		packet << static_cast<sf::Int64>(playerBehindWall[stepper].x);
		packet << static_cast<sf::Int64>(playerBehindWall[stepper].y);
		packet << w.direction;
		++stepper;
	}
	player.socket->send(packet);
}

void sendOnlinePlayersTo(Player &player) {
	sf::Packet packet;
	packet << Send_Online_Players;
	for (Player &p : players) {
		if (player.id != p.id) {
			packet << p.id;
			packet << p.username;
			packet << p.dead;
			packet << p.running;
			packet << static_cast<sf::Int64>(p.speed);
			packet << static_cast<sf::Int64>(p.attackSpeed * 100);
			packet << static_cast<sf::Int64>(p.maxHealth);
			packet << static_cast<sf::Int64>(p.health);
			packet << static_cast<sf::Int64>(p.maxMana);
			packet << static_cast<sf::Int64>(p.mana);
			packet << static_cast<sf::Int64>(p.collisionRect.getPosition().x);
			packet << static_cast<sf::Int64>(p.collisionRect.getPosition().y);
			packet << p.direction;
		}
	}
	player.socket->send(packet);
}

void sendItems(Player &player) {
	sf::Packet packet;
	packet << Send_Items;
	for (Item &item : items) {
		packet << item.id;
		packet << item.owner;
		packet << item.type;
		packet << item.visible;
		packet << item.equipped;
		packet << item.grabbed;
		packet << item.inInventory;
		packet << static_cast<unsigned char>(item.inventoryPosition.x);
		packet << static_cast<unsigned char>(item.inventoryPosition.y);
		packet << static_cast<sf::Int64>(item.collisionRect.getPosition().x);
		packet << static_cast<sf::Int64>(item.collisionRect.getPosition().y);
	}
	player.socket->send(packet);
}

void send_Daemons(Player &player) {
	sf::Packet packet;
	packet << Send_Daemons;
	for (Daemon &d : daemons) {
		packet << d.id;
		packet << d.dead;
		packet << d.running;
		packet << static_cast<sf::Int64>(d.speed);
		packet << static_cast<sf::Int64>(d.attackSpeed * 100);
		packet << static_cast<sf::Int64>(d.maxHealth);
		packet << static_cast<sf::Int64>(d.health);
		packet << static_cast<sf::Int64>(d.maxMana);
		packet << static_cast<sf::Int64>(d.mana);
		packet << static_cast<sf::Int64>(d.collisionRect.getPosition().x);
		packet << static_cast<sf::Int64>(d.collisionRect.getPosition().y);
		packet << d.direction;
	}
	player.socket->send(packet);
}

void send_Safezones(Player &player) {
	sf::Packet packet;
	packet << Send_Safezones;
	for (Safezone &s : safezones) {
		packet << s.id;
		packet << static_cast<sf::Int64>(s.rc.getPosition().x);
		packet << static_cast<sf::Int64>(s.rc.getPosition().y);
		packet << static_cast<sf::Int64>(s.rc.getGlobalBounds().width);
		packet << static_cast<sf::Int64>(s.rc.getGlobalBounds().height);
		packet << s.active;
	}
	player.socket->send(packet);
}

void send_Regenerators(Player &player) {
	sf::Packet packet;
	packet << Send_Regenerators;
	for (Regenerator &r : regenerators) {
		packet << r.id;
		packet << static_cast<sf::Int64>(r.rc.getPosition().x);
		packet << static_cast<sf::Int64>(r.rc.getPosition().y);
		packet << static_cast<sf::Int64>(r.rc.getGlobalBounds().width);
		packet << static_cast<sf::Int64>(r.rc.getGlobalBounds().height);
		packet << r.active;
		packet << r.type;
	}
	player.socket->send(packet);
}

void respawn_Player(Player &player) {
	player.dead = false;
	player.health = player.maxHealth;
	player.mana = player.maxMana;
	player.collisionRect.setPosition(player.respawn);

	sf::Packet packet;
	packet << Send_Respawn;
	packet << player.id;
	packet << player.health;
	packet << player.mana;
	packet << static_cast<sf::Int64>(player.collisionRect.getPosition().x);
	packet << static_cast<sf::Int64>(player.collisionRect.getPosition().y);

	for (Player &p : players)
		p.socket->send(packet);
}

void login(Player &player, char *username, std::string password) {
	sf::Packet packet;

	//This user already logged in?
	for (Player &p : players) {
		if (p.logged_in && p.username == username) {
			std::cout << username << " already logged in" << std::endl;
			packet << Already_logged_in;

			if (selector.isReady(*player.socket))
				player.socket->send(packet);

			return;
		}
	}

	//Database contains this username?
	for (unsigned int i = 0; i < users.size(); ++i) {
		if (username == users[i].username && password == users[i].password) {
			//			Player logged in
			//Set player datas
			player.logged_in = true;
			strncpy_s(player.username, username, 20);
			player.id = users[i].id;

			//Send login successful message
			packet << Login;
			player.socket->send(packet);
			std::cout << std::endl << CurrentTime() << player.socket->getRemoteAddress() << " logged in" << std::endl << "Me: ";

			//Broadcast new player to the other players
			broadcast_New_Player(player);

			//Add targets to new player
			Player_Target_List_Add(player);

			//Add new player to the mob attack list
			Enemies_Target_List_Add(player);
			return;
		}
	}
	//Login was failed
	packet << Login_failed;
	player.socket->send(packet);
	std::cout << std::endl << CurrentTime() << player.socket->getRemoteAddress() << " login failed" << std::endl << "Me: ";
}