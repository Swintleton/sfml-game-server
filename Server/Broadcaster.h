#pragma once
#include "Communication_Code_List.h"

void broadcast_Player_Moved(Player &player) {
	sf::Packet packet;
	packet << Online_Player_Moved;
	packet << player.id;
	packet << static_cast<sf::Int64>(player.collisionRect.getPosition().x);
	packet << static_cast<sf::Int64>(player.collisionRect.getPosition().y);
	packet << player.direction;

	for (Player &p : players)
		if (p.id != player.id)
			p.socket->send(packet);
}

void broadcast_Daemon_Moved(Daemon &daemon) {
	sf::Packet packet;
	packet << Daemon_Moved;
	packet << daemon.id;
	packet << static_cast<sf::Int64>(daemon.collisionRect.getPosition().x);
	packet << static_cast<sf::Int64>(daemon.collisionRect.getPosition().y);
	packet << daemon.direction;

	for (Player &p : players)
		p.socket->send(packet);
}

void broadcast_Delete_Online_Player(Player &player) {
	sf::Packet packet;
	packet << Online_Player_Deleted;
	packet << player.id;

	for (Player &p : players)
		if (p.id != player.id)
			p.socket->send(packet);
}

void broadcast_Item_Grabbed(Player &player, Item &item) {
	sf::Packet packet;
	packet << Item_Grabbed;
	packet << item.id;
	packet << item.owner;
	for (Player &p : players)
		if (p.id != player.id)
			p.socket->send(packet);
}

void broadcast_Item_Released(Player &player, Item &item) {
	sf::Packet packet;
	packet << Item_Released;
	packet << item.id;
	packet << static_cast<sf::Int64>(item.collisionRect.getPosition().x);
	packet << static_cast<sf::Int64>(item.collisionRect.getPosition().y);
	for (Player &p : players)
		if (p.id != player.id)
			p.socket->send(packet);
}

void broadcast_New_Player(Player &player) {
	sf::Packet packet;
	packet << Send_Online_Player;
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

	for (Player &p : players)
		if (p.id != player.id)
			p.socket->send(packet);
}

void broadcast_Attack_Player(Player &player) {
	sf::Packet packet;
	packet << Attack_Player;
	packet << player.id;

	for (Player &p : players)
		if (p.id != player.id)
			p.socket->send(packet);
}

void broadcast_Running_Player(Player &player) {
	sf::Packet packet;
	packet << Running;
	packet << player.id;

	for (Player &p : players)
		if (p.id != player.id)
			p.socket->send(packet);
}

void broadcast_Walking_Player(Player &player) {
	sf::Packet packet;
	packet << Walking;
	packet << player.id;

	for (Player &p : players)
		if (p.id != player.id)
			p.socket->send(packet);
}

void broadcast_Attack_Daemon(Daemon &daemon) {
	sf::Packet packet;
	packet << Attack_Daemon;
	packet << daemon.id;

	for (Player &p : players)
		p.socket->send(packet);
}

//Broadcast dmg happened to everyone
void broadcast_Damage_Taken_Player(Player &player, float dmg) {
	sf::Packet packet;
	packet << Damage_Taken_Player;
	packet << player.id;
	packet << static_cast<sf::Int64>(dmg);

	for (Player &p : players)
		p.socket->send(packet);
}

void broadcast_Damage_Taken_Daemon(Daemon &daemon, float dmg) {
	sf::Packet packet;
	packet << Damage_Taken_Daemon;
	packet << daemon.id;
	packet << static_cast<sf::Int64>(dmg);

	for (Player &p : players)
		p.socket->send(packet);
}

void broadcast_Healed_Player(Player &player, float heal) {
	sf::Packet packet;
	packet << Healed_Player;
	packet << player.id;
	packet << static_cast<sf::Int64>(heal);

	for (Player &p : players)
		p.socket->send(packet);
}

void broadcast_Healed_Daemon(Daemon &daemon, float heal) {

}

void boradcast_Die_Player(Player &player) {
	sf::Packet packet;
	packet << Die_Player;
	packet << player.id;

	for (Player &p : players)
		p.socket->send(packet);
}

void broadcast_Die_Daemon(Daemon &daemon) {
	sf::Packet packet;
	packet << Die_Daemon;
	packet << daemon.id;

	for (Player &p : players)
		p.socket->send(packet);
}

void broadcast_Daemon_Deleted(Daemon &daemon) {
	sf::Packet packet;
	packet << Daemon_Deleted;
	packet << daemon.id;

	for (Player &p : players)
		p.socket->send(packet);
}

void broadcast_Message(Player &player, std::string &msg) {
	sf::Packet packet;
	std::string colon = ": ";
	packet << Chat_Message;
	packet << player.username + colon + msg;
	for (Player &p : players)
		if (p.id != player.id)
			p.socket->send(packet);
}

void broadcast_New_Item(Item &item) {
	sf::Packet packet;
	packet << Send_Item;
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

	for (Player &p : players)
		p.socket->send(packet);
}