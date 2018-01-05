#pragma once
#ifndef RENDERMODULE_H
#define RENDERMODULE_H

#include <SFML/Graphics.hpp>
#include <thread>
#include "MemoryModule.h"


class Animation {

public:
	Animation() = default;
	~Animation() = default;
	Animation(int texStart, int texEnd, int speed);
	void update(int milisecs);
	int getFrame() const { return animationList[step]; };
private:
	int* animationList{ nullptr };
	int speed{ 0 };
	int counter{ 0 };
	int step{ 0 };
	int length{ 0 };
};

class RenderModule {

public:
	RenderModule() = default;
	RenderModule(MemoryModule* mem) { sharedMemory = mem; };
	~RenderModule() = default;
	
	void start();
private:
	std::thread t;
	void update();
	bool run{ false };
	void loadPlayerAnimations();
	void playerAnimation();
	MemoryModule* sharedMemory{ nullptr };
	int adder{ 1 };

	Animation* playerWalk;
	Animation* playerIdle;
	Animation* playerAttack;
	Animation* mainPlayerAnimations{ nullptr };

};


#endif
