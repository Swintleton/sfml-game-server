#pragma once
#include <math.h>
#include <SFML\Graphics.hpp>

struct Node {
	int x;
	int y;
	//distance from start point
	int G;
	//distance from end point
	int F;
	Node *parent;

	Node(int x, int y, int G, int F, Node *parent = nullptr)
		: x(x), y(y), G(G), F(F), parent(parent)
	{}

	//Estimate the remaining distance to the goal
	const int &estimate(const int &PosX, const int &PosY) const
	{
		static int xd, yd, d;
		xd = PosX - x;
		yd = PosY - y;

		//Euclidian Distance
		d = static_cast<int>(sqrt(xd * xd + yd * yd));

		//Manhattan distance
		//d=abs(xd)+abs(yd);

		//Chebyshev distance
		//d=max(abs(xd), abs(yd));

		return(d);
	}

	void updateF(const int &x, const int &y)
	{
		F = G + estimate(x, y) * 10;
	}
};

namespace path {
	int move = 26;//26
	//0	-	UP
	//1	-	DOWN
	//2	-	LEFT
	//3	-	RIGHT
	//4	-	UP LEFT
	//5	-	UP RIGHT
	//6	-	DOWN LEFT
	//7	-	DOWN RIGHT
	const int xDirections[] = {  0,  0, -move,  move, -move,  move, -move, move };
	const int yDirections[] = { -move,  move,  0,  0, -move, -move,  move, move };

	std::vector<sf::Vector2f> path;
	std::vector<Node> openList;
	std::vector<Node> closedList;
	unsigned int openListIndex;
	static Node* selectedNode;
	static Node* neighborNode;
	unsigned int i, j;
	bool free;

	sf::RectangleShape collisionRect;	//How big you are
}

std::vector<sf::Vector2f> findPath(sf::Vector2f start, sf::Vector2f end)
{
	path::path.clear();
	path::openList.clear();
	path::closedList.clear();
	path::j = 0;

	path::collisionRect.setSize(sf::Vector2f(26, 44));//26 44
	path::collisionRect.setOrigin(13, 22);

	//Path is possible?
	path::collisionRect.setPosition(end.x, end.y);
	if (collisionHappend(path::collisionRect))
		return path::path;

	//Create the start node and push into the openList
	path::selectedNode = new Node(start.x, start.y, 0, 0);
	path::selectedNode->updateF(end.x, end.y);
	path::openList.push_back(*path::selectedNode);
	delete path::selectedNode;

	while (!path::openList.empty())
	{
		//Select the node with the lowest F
		path::openListIndex = 0;
		for (path::i = 1; path::i < path::openList.size(); ++path::i)
			if (path::openList[path::i].F < path::openList[path::openListIndex].F)
				path::openListIndex = path::i;
		path::selectedNode = new Node(path::openList[path::openListIndex].x, path::openList[path::openListIndex].y, path::openList[path::openListIndex].G, path::openList[path::openListIndex].F, path::openList[path::openListIndex].parent);

		//Delete selectedNode from openList
		path::openList.erase(path::openList.begin() + path::openListIndex);
		//Add to selectedNode closedList
		path::closedList.push_back(*path::selectedNode);

		//If it's stuck somehow...
		if (path::closedList.size() > 50) {
			delete path::selectedNode;
			return path::path;
		}

		//selectedNode is the endNode?
		if (std::abs(end.x - path::selectedNode->x) < 21 && abs(end.y - path::selectedNode->y) < 31)
		{
			//Generate the path from finish to start
			Node* parent = &path::closedList.back();
			path::path.push_back(sf::Vector2f(parent->x, parent->y));
			while (parent->parent != nullptr) {
				path::path.push_back(sf::Vector2f(parent->x, parent->y));
				parent = parent->parent;
			}

			delete path::selectedNode;
			return path::path;
		}

		//Generate nodes in all possible directions (from the selectedNode)
		for (path::i = 0; path::i < 8; ++path::i)
		{
			//Neighbor on the closedList?
			path::free = true;
			for (path::j = 0; path::j < path::closedList.size(); ++path::j)
				if (path::closedList[path::j].x == path::selectedNode->x + path::xDirections[path::i] && path::closedList[path::j].y == path::selectedNode->y + path::yDirections[path::i]) {
					path::free = false;
					break;
				}
			if (!path::free) continue;

			//Block check
			path::collisionRect.setPosition(path::selectedNode->x + path::xDirections[path::i], path::selectedNode->y + path::yDirections[path::i]);
			if (collisionHappend(path::collisionRect)) continue;

			//Generate neighbor
			path::neighborNode = new Node(path::selectedNode->x + path::xDirections[path::i], path::selectedNode->y + path::yDirections[path::i], path::i < 4 ? 10 : 14, path::selectedNode->F, path::selectedNode);
			path::neighborNode->updateF(end.x, end.y);

			//It is on the openList?
			path::free = false;
			for (path::j = 0; path::j < path::openList.size(); ++path::j) {
				if (path::openList[path::j].x == path::neighborNode->x && path::openList[path::j].y == path::neighborNode->y) {
					path::free = true;
					//New F is lower, than the old?
					if (path::openList[path::j].F > path::neighborNode->F) {
						path::openList[path::j].F = path::neighborNode->F;
						path::openList[path::j].parent = path::selectedNode;
					}
					break;
				}
			}
				
			//If it is not in the open list then add into that
			if(!path::free)
				path::openList.push_back(*path::neighborNode);
			delete path::neighborNode;
		}
	}
	delete path::selectedNode;
	return path::path;
}