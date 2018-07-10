#pragma once

struct Dropper {
	unsigned char chance;
	unsigned char multi_Chance;
	unsigned char time;
	unsigned char max;
	long x, y;

	unsigned char rnd;
	unsigned char stepper;

	unsigned int index;

	Dropper(unsigned int type) : time(1)
	{
		if (type == 2)
		{
			chance = 50;
			max = 4;
			multi_Chance = 30;
		}
	}

	void drop(float target_x, float target_y)
	{
		//Generate random number
		rnd = std::rand() % 100;
		//There is chance for drop?
		if (rnd < chance) {
			//How many drop?
			time = 1;
			rnd = std::rand() % 100;
			while (rnd < multi_Chance && time < max) {
				//Multi drop
				rnd = std::rand() % 100;
				++time;
			}
			//Drop item(s)
			stepper = 0;
			while (stepper < time) {
				index = std::rand() % possible_items_size;
				x = target_x + (std::rand() % 121) - 60;
				y = target_y + (std::rand() % 121) - 60;

				items.push_back(new Item(ItemID++, possible_items[index], x, y));
				broadcast_New_Item(items.back());

				++stepper;
			}
		}
	}
};