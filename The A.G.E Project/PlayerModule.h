#ifndef PLAYERMODULE_H
#define PLAYERMODULE_H

#include <thread>
#include "MemoryModule.h"

class PlayerModule {

public:
	PlayerModule() = default;
	PlayerModule(MemoryModule*);
	//create player from file:
	//PlayerModule(Stream& playerfile);
	~PlayerModule() = default;
	//Threaded updater
	void start();
	
	
	int getX() const { return posX; };
	int getY() const { return posY; };
	
	int getDirection() const { return directionHori; };
private:
	//Parallell thread and function
	std::thread t;
	void update();
	bool run{ false };

	//Function that continously checks for input
	void inputs();	

	//To be removed
	int directionHori{ 0 };
	bool jump;
	
	
	int posX{ 10 };
	int posY{ 100 };

	MemoryModule* sharedMemory{ nullptr };

	sf::Clock timer{};
};

#endif
