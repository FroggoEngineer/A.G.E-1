#include "RenderModule.h"
#include <SFML/Graphics.hpp>

void RenderModule::update()
{
	while (run) {
		sf::Clock timer{};
		playerAnimation();
		sf::sleep(sf::milliseconds(1));
		mainPlayerAnimations->update(timer.getElapsedTime().asMilliseconds());
		sharedMemory->setAnimationPlayer(0, mainPlayerAnimations->getFrame());
	}
}

void RenderModule::loadPlayerAnimations()
{
	playerIdle = new Animation(0, 1, 300);
	playerWalk = new Animation(2, 3, 250);
	playerAttack = new Animation(4, 9, 250);
}

void RenderModule::playerAnimation()
{
	if (sharedMemory->getIdle(0)) {
		mainPlayerAnimations = playerIdle;
	}
	else if (!sharedMemory->getIdle(0)) {
		mainPlayerAnimations = playerWalk;
	}
	else if (false) {
		mainPlayerAnimations = playerAttack;
	}
	
}

void RenderModule::start()
{
	loadPlayerAnimations();
	run = true;
	t = std::thread(&RenderModule::update, this);

}

Animation::Animation(int texStart, int texEnd, int speed)
{
	length = texEnd - texStart;
	animationList = new int[length];

	for (int i{ 0 }; i < length; i++)
		animationList[i] = i + texStart;

	this->speed = speed;
}

void Animation::update(int milisecs)
{
	counter += milisecs;
	if (counter >= speed) {
		step++;
		counter = 0;
	}

	if (step >= length)
		step = 0;
}
