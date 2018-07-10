#pragma once
#include <thread>
#include <chrono>
#include "Dropper.h"

void gameLoop() {
	sf::Clock clock;
	float deltaTime = 0;
	float time = 0;

	unsigned int stepper_daemon;

	Target* listened;
	Dropper drop_Daemon(2);

	float heal;

	while (!quit) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
		deltaTime = clock.restart().asSeconds();

		time += deltaTime;

		//Handle players
		for (Player *p : players) {
			//Player is targetable?
			p->targetable = true;
			for (Safezone *s : safezones) {
				if (p->collisionRect.getGlobalBounds().intersects(s->rc.getGlobalBounds())) {
					p->targetable = false;
					break;
				}
			}

			Target t = p->updateAttack(deltaTime);
			if (t.type == 2) {
				Daemon *damaged_Daemon = static_cast<Daemon *>(t.targetRef);
				if (damaged_Daemon != nullptr && !damaged_Daemon->dead) {
					//Simple attack
					damaged_Daemon->health -= p->dmg;

					damaged_Daemon->doBeenHit = true;

					//Broadcast Damage Happend
					broadcast_Damage_Taken_Daemon(damaged_Daemon, p->dmg);

					//Daemon died?
					if (damaged_Daemon->health <= 0) {
						damaged_Daemon->dead = true;
						broadcast_Die_Daemon(damaged_Daemon);
					}
				}
			}
		}

		//Daemon AI
		stepper_daemon = 0;
		for (Daemon *d : daemons) {
			//If daemon live
			if (!d->dead) {
				//Been hit
				if (d->doBeenHit) {
					d->normalAttack.active = false;
					d->beenHitTime += deltaTime;
					if (d->beenHitTime > 1) {
						d->beenHitTime = 0;
						d->doBeenHit = false;
					}
				}

				//Update attack
				Target t = d->updateAttack(deltaTime);
				//The target is a player?
				if (t.targetRef != nullptr && t.type == 1) {
					Player *damaged_Player = static_cast<Player *>(t.targetRef);
					if (damaged_Player != nullptr && !damaged_Player->dead) {
						//Simple attack
						damaged_Player->health -= d->dmg;

						//Broadcast Damage Happend
						broadcast_Damage_Taken_Player(damaged_Player, d->dmg);

						//Player died?
						if (damaged_Player->health <= 0) {
							damaged_Player->dead = true;
							boradcast_Die_Player(damaged_Player);
							//Lost target
							d->target = Target();
							d->path = std::vector<sf::Vector2f>();
						}
					}
				}

				//Attack when a player is inside the range
				for(Player *p : players)
					if (!p->dead && d->normalAttack.collisionRect.getGlobalBounds().intersects(p->collisionRect.getGlobalBounds())) {
						//attack
						if (d->attack()) {
							broadcast_Attack_Daemon(d);
							break;	//Daemon hit 1 player at the time
						}
					}

				if (players.size() > 0) {
					//If have no target yet
					if (d->target.targetRef == nullptr) {
						//Listen
						listened = d->listen();
						if (listened != nullptr) {
							d->target = *listened;
							d->targetUnseen_Time = 0;
						}
					}
					else {
						//If the path is empty or the path end is far from to the target
						if (d->path.size() < 1 || std::abs(d->path[0].x - d->target.collisionRect->getPosition().x) > 12 && std::abs(d->path[0].y - d->target.collisionRect->getPosition().y) > 18)
							//Set new path
							d->path = findPath(d->collisionRect.getPosition(), d->target.collisionRect->getPosition());

						//Move
						if (d->normalAttack.ready && !d->doBeenHit && d->move(deltaTime))
							broadcast_Daemon_Moved(d);

						//The target is trackable?
						listened = d->listen();
						if (listened != nullptr && listened->targetRef != nullptr) {
							//Daemon sees the target
							d->targetUnseen_Time = 0;
							if (listened->targetRef != d->target.targetRef)
								d->target = *listened;
						}
						//Target unseen
						else
							d->targetUnseen_Time += deltaTime;
						if (d->targetUnseen_Time > 8) {
							//Lost target
							d->target = Target();
							d->path = std::vector<sf::Vector2f>();
						}
					}
				}
			}
			//Daemon dies
			else {
				d->dead_Time += deltaTime;
				if (!d->dropped && d->dead_Time > 2) {
					d->dropped = true;
					drop_Daemon.drop(d->collisionRect.getPosition().x, d->collisionRect.getPosition().y);
				}
				if (d->dead_Time > 5) {
					//Delete daemon
					broadcast_Daemon_Deleted(d);
					std::cout << d->id << ". daemon died, remaining daemons: " << daemons.size()-1 << std::endl;
					daemons.erase(daemons.begin() + stepper_daemon);
				}
			}
			++stepper_daemon;
		}

		//Regenerators AI
		for (Regenerator *r : regenerators) {
			for (Player *p : players) {
				if (r->rc.getGlobalBounds().intersects(p->collisionRect.getGlobalBounds())) {
					r->time += deltaTime;
					if (r->type == 0 && r->time > 1) {
						if (p->health < p->maxHealth) {
							heal = p->maxHealth / 10;
							r->time = 0;
							p->health += heal;
							broadcast_Healed_Player(p, heal);
						}
					}
				}
			}
		}

	}
}