#pragma once
#include <SFML/Graphics.hpp>

#define SQRT_2 std::sqrt(2.f);

#define inventoryX 10
#define inventoryY 10

//				ITEM TYPES
//WEAPONS					(	0 - 12999		)
//Axes						(	0 - 999			)
//Bows						(	1000 - 1999		)
//Crossbows					(	2000 - 2999		)
//Daggers					(	3000 - 3999		)
#define Dagger				((unsigned short ) 3000)
//Javelins					(	4000 - 4999		)
//Maces						(	5000 - 5999		)
//Polearms					(	6000 - 6999		)
//Scepters					(	7000 - 7999		)
//Spears					(	8000 - 8999		)
//Staves					(	9000 - 9999		)
//Swords					(	10000 - 10999	)
#define Short_Sword			((unsigned short ) 9000)
//Throwing Weapons			(	11000 - 11999	)
//Wands						(	12000 - 12999	)

//ARMORS					(	13000 - 20999	)
//Amulets					(	13000 - 13999	)
#define Amulet_1			((unsigned short ) 12000)
#define Amulet_2			((unsigned short ) 12001)
#define Amulet_3			((unsigned short ) 12002)
//Armours					(	14000 - 14999	)
#define Quilted_Armor		((unsigned short ) 14000)
//Belts						(	15000 - 15999	)
#define Sash				((unsigned short ) 15000)
//Boots						(	16000 - 16999	)
#define Boots				((unsigned short ) 16000)
//Circlets 					(	17000 - 17999	)
#define Circlet				((unsigned short ) 17000)
//Gloves 					(	18000 - 18999	)
#define Leather_Gloves		((unsigned short ) 18000)
//Helms 					(	19000 - 19999	)
#define Cap					((unsigned short ) 19000)
//Rings 					(	20000 - 20999	)
#define Ring_1				((unsigned short ) 20000)
#define Ring_2				((unsigned short ) 20001)
#define Ring_3				((unsigned short ) 20002)
#define Ring_4				((unsigned short ) 20003)
#define Ring_5				((unsigned short ) 20004)

unsigned const short possible_items_size = 16;
unsigned short possible_items[possible_items_size] = {
	Dagger,
	Short_Sword,
	Amulet_1,
	Amulet_2,
	Amulet_3,
	Quilted_Armor,
	Sash,
	Boots,
	Circlet,
	Leather_Gloves,
	Cap,
	Ring_1,
	Ring_2,
	Ring_3,
	Ring_4,
	Ring_5
};

//ID auto incrementers
unsigned int ClientID = 0;

unsigned int WallID = 1;
unsigned int SafezoneID = 1;
unsigned int RegeneratorID = 1;
unsigned int ItemID = 1;
unsigned int DaemonID = 1;

struct Solid {
	unsigned int id;
	unsigned short type;
	sf::RectangleShape &collisionRect;

	Solid(unsigned int id, unsigned short type, sf::RectangleShape &_collisionRect)
		: id(id), type(type), collisionRect(_collisionRect)
	{}
};
std::vector<Solid *> solids;

struct Target {
	void *targetRef;
	sf::RectangleShape *collisionRect;
	bool *dead;
	bool *targetable;

	//1	-	player
	//2	-	daemon
	unsigned int type;

	Target(void *targetRef, sf::RectangleShape &collisionRect, bool &dead, bool &targetable, unsigned int type)
		: targetRef(targetRef), collisionRect(&collisionRect), dead(&dead), targetable(&targetable), type(type)
	{}

	Target() :targetRef(nullptr), collisionRect(nullptr), dead(nullptr), targetable(nullptr), type(0)
	{}

	bool operator == (const Target &t) {
		return t.targetRef == targetRef;
	}
};

struct Attack_Normal {
	sf::RectangleShape collisionRect;
	bool active;
	bool ready;
	float dmg;
	float speed;
	float currentTime;
	bool check;
	float width;
	float height;

	std::vector<Target> targets;

	Attack_Normal(float x, float y, float width, float height, float dmg, float speed)
		: active(false), ready(true), dmg(dmg), speed(speed), currentTime(0), check(true), width(width), height(height)
	{
		collisionRect.setPosition(x, y);
		collisionRect.setSize(sf::Vector2f(width, height));
		collisionRect.setOrigin(collisionRect.getLocalBounds().width / 2, collisionRect.getLocalBounds().height / 2);
	}

	void addTarget(Target &t) {
		targets.push_back(t);
	}

	void removeTarget(Target &t) {
		targets.erase(std::remove(targets.begin(), targets.end(), t), targets.end());
	}

	void setTargets(std::vector<Target> _targets) {
		targets = _targets;
	}

	Target update(float x, float y, sf::Uint8 direction, float deltaTime) {
		//Set new Position and angle:
		//Right
		if (direction == 0) {
			//Set size
			collisionRect.setSize(sf::Vector2f(width, height));
			//Set Origin
			collisionRect.setOrigin(collisionRect.getLocalBounds().width / 2, collisionRect.getLocalBounds().height / 2);
			//Set new position
			collisionRect.setPosition(x + collisionRect.getLocalBounds().width * 0.75f, y);
			//Set new angle
			collisionRect.setRotation(180);
		}
		//Left
		else if (direction == 7) {
			//Set size
			collisionRect.setSize(sf::Vector2f(width, height));
			//Set Origin
			collisionRect.setOrigin(collisionRect.getLocalBounds().width / 2, collisionRect.getLocalBounds().height / 2);
			//Set new position
			collisionRect.setPosition(x - collisionRect.getLocalBounds().width * 0.75f, y);
			//Set new angle
			collisionRect.setRotation(0);
		}
		//Up
		else if (direction == 1) {
			//Set size
			collisionRect.setSize(sf::Vector2f(width, height));
			//Set Origin
			collisionRect.setOrigin(collisionRect.getLocalBounds().width / 2, collisionRect.getLocalBounds().height / 2);
			//Set new position
			collisionRect.setPosition(x, y - collisionRect.getLocalBounds().height * 0.75f);
			//Set new angle
			collisionRect.setRotation(90);
		}
		//Down
		else if (direction == 4) {
			//Set size
			collisionRect.setSize(sf::Vector2f(width, height));
			//Set Origin
			collisionRect.setOrigin(collisionRect.getLocalBounds().width / 2, collisionRect.getLocalBounds().height / 2);
			//Set new position
			collisionRect.setPosition(x, y + collisionRect.getLocalBounds().height * 0.75f);
			//Set new angle
			collisionRect.setRotation(270);
		}
		//Up Right
		else if (direction == 2) {
			//Set size
			collisionRect.setSize(sf::Vector2f(width / std::sqrt(2.f), height / std::sqrt(2.f)));
			//Set Origin
			collisionRect.setOrigin(collisionRect.getLocalBounds().width / 2 / std::sqrt(2.f), collisionRect.getLocalBounds().height / 2 / std::sqrt(2.f));
			//Set new position
			collisionRect.setPosition(x + collisionRect.getLocalBounds().width * 0.75f, y - collisionRect.getLocalBounds().height * 0.75f);
			//Set new angle
			collisionRect.setRotation(135);
		}
		//Up Left
		else if (direction == 3) {
			//Set size
			collisionRect.setSize(sf::Vector2f(width / std::sqrt(2.f), height / std::sqrt(2.f)));
			//Set Origin
			collisionRect.setOrigin(collisionRect.getLocalBounds().width / 2 / std::sqrt(2.f), collisionRect.getLocalBounds().height / 2 / std::sqrt(2.f));
			//Set new position
			collisionRect.setPosition(x - collisionRect.getLocalBounds().width * 0.75f, y - collisionRect.getLocalBounds().height * 0.75f);
			//Set new angle
			collisionRect.setRotation(45);
		}
		//Down Right
		else if (direction == 5) {
			//Set size
			collisionRect.setSize(sf::Vector2f(width / std::sqrt(2.f), height / std::sqrt(2.f)));
			//Set Origin
			collisionRect.setOrigin(collisionRect.getLocalBounds().width / 2 / std::sqrt(2.f), collisionRect.getLocalBounds().height / 2 / std::sqrt(2.f));
			//Set new position
			collisionRect.setPosition(x + collisionRect.getLocalBounds().width * 0.75f, y + collisionRect.getLocalBounds().height * 0.75f);
			//Set new angle
			collisionRect.setRotation(225);
		}
		//Down Left
		else if (direction == 6) {
			//Set size
			collisionRect.setSize(sf::Vector2f(width / std::sqrt(2.f), height / std::sqrt(2.f)));
			//Set Origin
			collisionRect.setOrigin(collisionRect.getLocalBounds().width / 2 / std::sqrt(2.f), collisionRect.getLocalBounds().height / 2 / std::sqrt(2.f));
			//Set new position
			collisionRect.setPosition(x - collisionRect.getLocalBounds().width * 0.75f, y + collisionRect.getLocalBounds().height * 0.75f);
			//Set new angle
			collisionRect.setRotation(315);
		}

		currentTime += deltaTime;

		if (currentTime >= speed)
			ready = true;

		if (currentTime < speed)
			return Target();

		if (!active)
			return Target();

		if (!check)
			return Target();

		check = false;

		//Check collisions
		for (Target t : targets) {
			if (collisionRect.getGlobalBounds().intersects(t.collisionRect->getGlobalBounds())) {
				//Hit happend
				return t;
			}
		}
		return Target();
	}
};

struct Item {
	unsigned int id;
	//Player id
	int owner;
	sf::Vector2i inventorySize;
	sf::Vector2i inventoryPosition;
	sf::RectangleShape collisionRect;
	char *name;

	unsigned short type;

	bool inInventory;
	bool grabbed;
	bool equipped;

	bool visible;

	Item(unsigned int _id, unsigned short _type, float x, float y)
		: id(_id), inventoryPosition({ 0, 0 }), type(_type), inInventory(false), grabbed(false), equipped(false), visible(true), owner(-1)
	{
		if (type == Dagger) {
			inventorySize = { 1, 2 };
			name = "Dagger";
		}
		else if (type == Short_Sword) {
			inventorySize = { 1, 3 };
			name = "Short Sword";
		}
		else if (type == Amulet_1) {
			inventorySize = { 1, 1 };
			name = "Amulet_1";
		}
		else if (type == Amulet_2) {
			inventorySize = { 1, 1 };
			name = "Amulet_2";
		}
		else if (type == Amulet_3) {
			inventorySize = { 1, 1 };
			name = "Amulet_3";
		}
		else if (type == Quilted_Armor) {
			inventorySize = { 2, 3 };
			name = "Quilted_Armor";
		}
		else if (type == Sash) {
			inventorySize = { 2, 1 };
			name = "Sash";
		}
		else if (type == Boots) {
			inventorySize = { 2, 2 };
			name = "Boots";
		}
		else if (type == Circlet) {
			inventorySize = { 2, 2 };
			name = "Circlet";
		}
		else if (type == Leather_Gloves) {
			inventorySize = { 2, 2 };
			name = "Leather_Gloves";
		}
		else if (type == Cap) {
			inventorySize = { 2, 2 };
			name = "Cap";
		}
		else if (type == Ring_1) {
			inventorySize = { 1, 1 };
			name = "Ring_1";
		}
		else if (type == Ring_2) {
			inventorySize = { 1, 1 };
			name = "Ring_2";
		}
		else if (type == Ring_3) {
			inventorySize = { 1, 1 };
			name = "Ring_3";
		}
		else if (type == Ring_4) {
			inventorySize = { 1, 1 };
			name = "Ring_4";
		}
		else if (type == Ring_5) {
			inventorySize = { 1, 1 };
			name = "Ring_5";
		}

		collisionRect.setSize(sf::Vector2f(inventorySize.x * 24, inventorySize.y * 24));
		collisionRect.setPosition(x, y);
	}
};

struct Player {
	unsigned int id;
	bool logged_in;
	sf::TcpSocket *socket;
	float time;

	char username[20];
	float maxHealth;
	float health;
	float maxMana;
	float mana;

	bool dead;
	bool running;
	float speed;
	float dmg;
	float attackSpeed;

	bool targetable;

	sf::Vector2f respawn;

	//0	-	RIGHT
	//1	-	UP
	//2	-	UP RIGHT
	//3	-	UP LEFT
	//4	-	DOWN
	//5	-	DOWN RIGHT
	//6	-	DOWN LEFT
	//7	-	LEFT
	sf::Uint8 direction;
	sf::RectangleShape collisionRect;

	bool inventoryFreeSpace[10][10];
	std::vector<Item *>Inventory;

	Attack_Normal normalAttack;

	Player(unsigned int id, float x, float y)
	:id(id),time(0),username(""),dead(false),running(false),maxHealth(10),health(10),maxMana(10),mana(10),speed(100),dmg(1),attackSpeed(0.84f),direction(4),logged_in(false),normalAttack(x, y, 22, 20, dmg, attackSpeed),respawn(0,0),targetable(false)
	{
		for (unsigned char y = 0; y < inventoryY; ++y)
			for (unsigned char x = 0; x < inventoryY; ++x)
				inventoryFreeSpace[x][y] = true;

		collisionRect.setPosition(x, y);
		collisionRect.setSize(sf::Vector2f{ 26, 44 });
		collisionRect.setOrigin(13, 22);
		socket = new sf::TcpSocket;
	}

	void attack() {
		if (normalAttack.ready) {
			normalAttack.active = true;
			normalAttack.ready = false;
			normalAttack.check = true;
			normalAttack.currentTime = 0;
		}
	}

	Target updateAttack(float deltaTime) {
		return normalAttack.update(collisionRect.getPosition().x, collisionRect.getPosition().y, direction, deltaTime);
	}
};
std::vector<Player *> players;
std::vector<sf::Vector2f> playerBeforeWall;
std::vector<sf::Vector2f> playerBehindWall;

struct Wall {
	unsigned int id;
	sf::RectangleShape collisionRect;
	//0	-	RIGHT
	//1	-	UP
	//2	-	UP RIGHT
	//3	-	UP LEFT
	//4	-	DOWN
	//5	-	DOWN RIGHT
	//6	-	DOWN LEFT
	//7	-	LEFT
	sf::Uint8 direction;

	Wall(unsigned int id, float x, float y, sf::Uint8 direction)
		: id(id), direction(direction)
	{
		collisionRect.setPosition(x, y);
		if (direction == 1 || direction == 4) {
			collisionRect.setSize(sf::Vector2f{ 62.f, 40.f });
			collisionRect.setOrigin(31.f, 30.f);
		}
		else if (direction == 0 || direction == 7) {
			collisionRect.setSize(sf::Vector2f{ 20.f, 64.f });
			collisionRect.setOrigin(22.f, 40.f);
		}
		else {
			collisionRect.setSize(sf::Vector2f{ 30.f, 20.f });
			collisionRect.setOrigin(12.f, 20.f);
			collisionRect.rotate(-35);
		}
	}
};
std::vector<Wall *> walls;

struct Safezone {
	unsigned int id;
	float active;
	sf::RectangleShape rc;

	Safezone(unsigned int id, float x, float y, float w, float h, bool active)
		: id(id), active(active)
	{
		rc.setPosition(x, y);
		rc.setSize(sf::Vector2f(w, h));
		rc.setOrigin(x / 2, y / 2);
	}
};
std::vector<Safezone *> safezones;

struct Regenerator {
	unsigned int id;
	float active;
	sf::RectangleShape rc;
	sf::Uint8 type;
	float time;

	Regenerator(unsigned int id, float x, float y, float w, float h, bool active, sf::Uint8 type)
		: id(id), active(active), type(type), time(0)
	{
		rc.setPosition(x, y);
		rc.setSize(sf::Vector2f(w, h));
		rc.setOrigin(x / 2, y / 2);
	}
};
std::vector<Regenerator *> regenerators;

bool collisionHappend(sf::RectangleShape &rc) {
	unsigned int j = 0;
	for (Solid *s : solids) {
		//If s is a wall
		if (s->type == 0) {
			//Put the collision rectangle up or down
			if (rc.getPosition().y < s->collisionRect.getPosition().y + 30)
				s->collisionRect.setPosition(playerBeforeWall[j]);
			if (rc.getPosition().y > s->collisionRect.getPosition().y - 30)
				s->collisionRect.setPosition(playerBehindWall[j]);
			++j;
		}

		//Checking for collision
		if (s->collisionRect.getGlobalBounds().intersects(rc.getGlobalBounds()))
			return true;
	}
	return false;
}

struct Daemon {
	unsigned int id;
	sf::RectangleShape collisionRect;
	sf::RectangleShape listenerRect;
	float maxHealth;
	float health;
	float maxMana;
	float mana;

	bool dead;
	float dead_Time;
	bool running;
	float speed;
	float dmg;
	float attackSpeed;
	float targetUnseen_Time;
	bool doBeenHit;
	float beenHitTime;
	bool targetable;

	std::vector<sf::Vector2f> path;
	Target target;

	//0	-	RIGHT
	//1	-	UP
	//2	-	UP RIGHT
	//3	-	UP LEFT
	//4	-	DOWN
	//5	-	DOWN RIGHT
	//6	-	DOWN LEFT
	//7	-	LEFT
	sf::Uint8 direction;

	Attack_Normal normalAttack;

	bool dropped;

	Daemon(unsigned int id, float x, float y,sf::Uint8 direction, float maxHealth, float health, float maxMana, float mana, bool running, float speed, float dmg, float attackSpeed)
	: id(id),maxHealth(maxHealth),health(health),maxMana(maxMana),mana(mana),dead(false),dead_Time(0),running(running),speed(speed),dmg(dmg),attackSpeed(attackSpeed),direction(direction),normalAttack(x, y, 22, 20, dmg, attackSpeed),dropped(false),targetUnseen_Time(0), doBeenHit(false), beenHitTime(0), targetable(true)
	{
		collisionRect.setPosition(x, y);
		collisionRect.setSize(sf::Vector2f{ 26, 44 });
		collisionRect.setOrigin(13, 22);

		listenerRect.setPosition(x, y);
		listenerRect.setSize(sf::Vector2f{ 200, 44 });
		listenerRect.setOrigin(200, 22);

		std::vector<Target> targets;
		targets.reserve(sizeof(Target) * players.size());
		for (Player *p : players) {
			targets.emplace_back(Target(p, p->collisionRect, p->dead, p->targetable, 1));
		}
		normalAttack.setTargets(targets);
	}

	bool attack() {
		if (normalAttack.ready && !doBeenHit) {
			normalAttack.active = true;
			normalAttack.ready = false;
			normalAttack.check = true;
			normalAttack.currentTime = 0;
			return true;
		}
		return false;
	}

	bool move(float deltaTime) {
		if (path.size() < 1)
			return false;

		long meX = collisionRect.getPosition().x;
		long meY = collisionRect.getPosition().y;
		sf::Vector2f goal = path.back();

		sf::Vector2f movement(0.f, 0.f);
		//Up Right
		if (goal.x > meX && goal.y < meY) {
			movement.x += speed * deltaTime;
			movement.y -= speed * deltaTime;
			movement /= SQRT_2;
			direction = 2;
		}
		//Up Left
		else if (goal.x < meX && goal.y < meY) {
			movement.x -= speed * deltaTime;
			movement.y -= speed * deltaTime;
			movement /= SQRT_2;
			direction = 3;
		}
		//Down Right
		else if (goal.x > meX && goal.y > meY) {
			movement.x += speed * deltaTime;
			movement.y += speed * deltaTime;
			movement /= SQRT_2;
			direction = 5;
		}
		//Down Left
		else if (goal.x < meX && goal.y > meY) {
			movement.x -= speed * deltaTime;
			movement.y += speed * deltaTime;
			movement /= SQRT_2;
			direction = 6;
		}
		//Right
		else if (goal.x > meX) {
			movement.x += speed * deltaTime;
			direction = 0;
		}
		//Left
		else if (goal.x < meX) {
			movement.x -= speed * deltaTime;
			direction = 7;
		}
		//Up
		else if (goal.y < meY) {
			movement.y -= speed * deltaTime;
			direction = 1;
		}
		//Down
		else if (goal.y > meY) {
			movement.y += speed * deltaTime;
			direction = 4;
		}

		collisionRect.move(movement);

		//If we are close enough to the goal, then delete it
		if (std::abs(collisionRect.getPosition().x - goal.x) < 10 && std::abs(collisionRect.getPosition().y - goal.y) < 10)
			path.erase(path.begin() + path.size() - 1);

		if (movement.x == 0 && movement.y == 0)
			return false;
		return true;
	}

	bool move(sf::Vector2f pos, float deltaTime) {
		long posX = pos.x;
		long posY = pos.y;
		long meX = collisionRect.getPosition().x;
		long meY = collisionRect.getPosition().y;

		if (posX == meX && posY == meY)
			return false;

		sf::Vector2f movement(0.f, 0.f);
		//Up Right
		if (posX > meX && posY < meY) {
			movement.x += speed * deltaTime;
			movement.y -= speed * deltaTime;
			movement /= SQRT_2;
			direction = 2;
		}
		//Up Left
		else if (posX < meX && posY < meY) {
			movement.x -= speed * deltaTime;
			movement.y -= speed * deltaTime;
			movement /= SQRT_2;
			direction = 3;
		}
		//Down Right
		else if (posX > meX && posY > meY) {
			movement.x += speed * deltaTime;
			movement.y += speed * deltaTime;
			movement /= SQRT_2;
			direction = 5;
		}
		//Down Left
		else if (posX < meX && posY > meY) {
			movement.x -= speed * deltaTime;
			movement.y += speed * deltaTime;
			movement /= SQRT_2;
			direction = 6;
		}
		//Right
		else if (posX > meX) {
			movement.x += speed * deltaTime;
			direction = 0;
		}
		//Left
		else if (posX < meX) {
			movement.x -= speed * deltaTime;
			direction = 7;
		}
		//Up
		else if (posY < meY) {
			movement.y -= speed * deltaTime;
			direction = 1;
		}
		//Down
		else if (posY > meY) {
			movement.y += speed * deltaTime;
			direction = 4;
		}
		sf::Vector2f oldPos = collisionRect.getPosition();
		collisionRect.move(movement);

		unsigned int stepper = 0;
		//Check for Collisions
		for (Solid *s : solids) {
			//Put the collision rectangle up or down
			if (s->type == 0) {
				if (collisionRect.getPosition().y < s->collisionRect.getPosition().y + 30)
					s->collisionRect.setPosition(playerBeforeWall[stepper]);
				if (collisionRect.getPosition().y > s->collisionRect.getPosition().y - 30)
					s->collisionRect.setPosition(playerBehindWall[stepper]);
			}
			++stepper;

			//Checking for collision
			if (collisionRect.getGlobalBounds().intersects(s->collisionRect.getGlobalBounds())) {
				collisionRect.setPosition(oldPos);
				return false;

			}
		}
		return true;
	}

	Target* listen() {
		float angle, direction_angle;
		bool pastZero = false;
		std::vector<Target *> possibleTargets;

		for (Target &t : normalAttack.targets) {
			if (*t.dead || !*t.targetable)
				continue;

			//Get the angle between the this and the target
			angle = atan2(collisionRect.getPosition().y - t.collisionRect->getPosition().y, collisionRect.getPosition().x - t.collisionRect->getPosition().x);
			angle = angle * 180 / 3.1415926f;
			if (angle < 0)
				angle += 360;

			//Get the looking angle
			//Right
			if (direction == 0) {
				direction_angle = 180;
				pastZero = angle > 85 && angle < 275;
			}
			//Left
			else if (direction == 7) {
				direction_angle = 0;
				pastZero = angle > 265 || angle < 95;
			}
			//Up
			else if (direction == 1) {
				direction_angle = 90;
				pastZero = angle > 355 || angle < 185;
			}
			//Down
			else if (direction == 4) {
				direction_angle = 270;
				pastZero = angle > 175 || angle < 5;
			}
			//Up Right
			else if (direction == 2) {
				direction_angle = 135;
				pastZero = angle > 40 && angle < 230;
			}
			//Up Left
			else if (direction == 3) {
				direction_angle = 45;
				pastZero = angle > 310 || angle < 140;
			}
			//Down Right
			else if (direction == 5) {
				direction_angle = 225;
				pastZero = angle > 130 && angle < 320;
			}
			//Down Left
			else if (direction == 6) {
				direction_angle = 315;
				pastZero = angle > 220 || angle < 50;
			}

			bool obstacle = false;

			//Set listener rectangle
			listenerRect.setPosition(collisionRect.getPosition().x, collisionRect.getPosition().y);
			listenerRect.setRotation(angle);

			//If the target is behind this
			if (!pastZero) {
				listenerRect.setSize(sf::Vector2f{ 120, 44 });
				listenerRect.setOrigin(120, 22);
			}

			//The target is close enough?
			if (listenerRect.getGlobalBounds().intersects(t.collisionRect->getGlobalBounds())) {
				//There is a obstacle in the listener Rect?
				for (Wall *w : walls) {
					float distance_obstacle = std::sqrt(std::pow(std::abs(collisionRect.getPosition().x - w->collisionRect.getPosition().x), 2) + std::pow(std::abs(collisionRect.getPosition().y - w->collisionRect.getPosition().y), 2));
					float distance_target = std::sqrt(std::pow(std::abs(collisionRect.getPosition().x - t.collisionRect->getPosition().x), 2) + std::pow(std::abs(collisionRect.getPosition().y - t.collisionRect->getPosition().y), 2));
					//The obstacle is closer?
					if (distance_obstacle < distance_target) {
						//The obstacle is at the right angle?
						if (listenerRect.getGlobalBounds().intersects(w->collisionRect.getGlobalBounds())) {
							obstacle = true;
							break;
						}
					}
				}
				if (!obstacle) {
					//Target found
					listenerRect.setSize(sf::Vector2f{ 320, 44 });
					listenerRect.setOrigin(320, 22);
					possibleTargets.push_back(&t);
				}
			}
		}
		if (possibleTargets.size() == 1)
			return possibleTargets[0];
		else if (possibleTargets.size() > 1) {
			//Which target is the closest?
			unsigned int min = 0;
			float xd,yd,xdM,ydM;
			for (unsigned int i = 1; i < possibleTargets.size(); ++i) {
				xd = collisionRect.getPosition().x - possibleTargets[i]->collisionRect->getPosition().x;
				yd = collisionRect.getPosition().y - possibleTargets[i]->collisionRect->getPosition().y;
				xdM = collisionRect.getPosition().x - possibleTargets[min]->collisionRect->getPosition().x;
				ydM = collisionRect.getPosition().y - possibleTargets[min]->collisionRect->getPosition().y;
				if (std::abs(xd * xd + yd * yd) < std::abs(xdM * xdM + ydM * ydM))
					min = i;
			}
			return possibleTargets[min];
		}
		listenerRect.setSize(sf::Vector2f{200, 44});
		listenerRect.setOrigin(200, 22);
		return nullptr;
	}

	Target updateAttack(float deltaTime) {
		return normalAttack.update(collisionRect.getPosition().x, collisionRect.getPosition().y, direction, deltaTime);
	}
};

std::vector<Item *> items;
std::vector<Daemon *> daemons;

void CreateMap(void) {
	//Down
	walls.push_back(new Wall(WallID++, -300, 200, 4));//63 40 || 20 64
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, -247, 200, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, -184, 200, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, -121, 200, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, -58, 200, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, 5, 200, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, 68, 200, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, 131, 200, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());

	//Left
	walls.push_back(new Wall(WallID++, -308, 156, 7));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, -308, 96, 7));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, -308, 36, 7));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, -308, -24, 7));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, -308, -84, 7));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());

	//Right
	walls.push_back(new Wall(WallID++, 164, 156, 7));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, 164, 100, 7));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, 164, -84, 7));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());

	//Up
	walls.push_back(new Wall(WallID++, -300, -120, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, -247, -120, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, -184, -120, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, -121, -120, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, -58, -120, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, 5, -120, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, 68, -120, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, 131, -120, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());

	//Test
	//UP
	walls.push_back(new Wall(WallID++, 280, -60, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, 343, -60, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, 406, -60, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());

	//Right
	walls.push_back(new Wall(WallID++, 520, 0, 7));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());

	//Down
	walls.push_back(new Wall(WallID++, 280, 60, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, 343, 60, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());
	walls.push_back(new Wall(WallID++, 406, 60, 4));
	playerBeforeWall.push_back(sf::Vector2f(walls.back()->collisionRect.getPosition().x, walls.back()->collisionRect.getPosition().y + 50));
	playerBehindWall.push_back(walls.back()->collisionRect.getPosition());

	daemons.push_back(new Daemon(DaemonID++, 400, 500, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 300, 500, 0, 100, 100, 10, 10, false, 100, 5, 0.4f));
	daemons.push_back(new Daemon(DaemonID++, 700, 600, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, 550, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, 500, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, 450, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, 400, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, 350, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, 300, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, 250, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, 200, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, 150, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, 100, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700,  50, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700,	  0, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, -50, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, -100, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, -150, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));
	daemons.push_back(new Daemon(DaemonID++, 700, -200, 0, 3, 3, 3, 3, false, 100, 1, 0.84f));

	items.push_back(new Item(ItemID++, Short_Sword, -100, 50));
	items.push_back(new Item(ItemID++, Amulet_1, 60, 100));
	items.push_back(new Item(ItemID++, Quilted_Armor, -50, 0));

	safezones.push_back(new Safezone(SafezoneID++, -630, -130, 476, 320, true));

	regenerators.push_back(new Regenerator(RegeneratorID++, -100, -44, 100, 100, true, 0));

	//Add solids
	//Safezones
	solids.reserve(sizeof(Solid) * safezones.size());
	for (Safezone *s : safezones)
		solids.emplace_back(new Solid(s->id, 1, s->rc));
	//Walls
	solids.reserve(sizeof(Solid) * walls.size());
	for (Wall *w : walls)
		solids.emplace_back(new Solid(w->id, 0, w->collisionRect));
}

void Player_Target_List_Add(Player *player) {
	//Player can hit:
	//Daemons
	for (Daemon *d : daemons)
		player->normalAttack.addTarget(Target(d, d->collisionRect, d->dead, d->targetable, 2));
}

void Enemies_Target_List_Add(Player *player) {
	//Daemon can hit:
	//Players
	for (Daemon *d : daemons)
		d->normalAttack.addTarget(Target(player, player->collisionRect, player->dead, player->targetable, 1));
}

void Enemies_Target_List_Remove(Player *player) {
	for (Daemon *d : daemons)
		for (Target t : d->normalAttack.targets)
			if (t.type == 1 && static_cast<Player *>(t.targetRef) == player) {
				d->normalAttack.removeTarget(t);
				return;
			}
}

void DeleteGame(sf::Thread *thread_Receive, sf::Thread *thread_gameLoop) {
	if (thread_Receive)
	{
		thread_Receive->terminate();
		delete thread_Receive;
	}

	if (thread_gameLoop)
	{
		thread_gameLoop->terminate();
		delete thread_gameLoop;
	}

	unsigned int i = 0;

	for (i = 0; i < players.size(); ++i)
		delete players[i];

	for (i = 0; i < walls.size(); ++i)
		delete walls[i];

	for (i = 0; i < safezones.size(); ++i)
		delete safezones[i];

	for (i = 0; i < regenerators.size(); ++i)
		delete regenerators[i];

	for (i = 0; i < items.size(); ++i)
		delete items[i];

	for (i = 0; i < daemons.size(); ++i)
		delete daemons[i];

	for (i = 0; i < solids.size(); ++i)
		delete solids[i];
}