#include "PlayerModule.h"
#include <SFML/Window.hpp>
#include <thread>
#include <iostream>

using namespace std;

PlayerModule::PlayerModule(MemoryModule* mem)
{
	sharedMemory = mem;
}

void PlayerModule::start()
{
	run = true;
	t = std::thread(&PlayerModule::update, this);
}

void PlayerModule::update()
{
	while (run) {

		
		inputs();



	}

}

void PlayerModule::inputs()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		sharedMemory->setShutdown(true);


	//Test for gamepad
	/*
		A: 0
		B: 1
		X: 2
		Y: 3
		Left: 4
		Right: 5
		Left start: 6
		Right start: 7
		Left joystick: 8
		Right joystick: 9
	
	system("cls");
	cout << "V Axis: " << sf::Joystick::hasAxis(0, sf::Joystick::V) << endl;
	cout << "R Axis: " << sf::Joystick::hasAxis(0, sf::Joystick::R) << endl;
	cout << "X Axis: " << sf::Joystick::getAxisPosition(0, sf::Joystick::X) << endl;
	cout << "Y Axis: " << sf::Joystick::getAxisPosition(0, sf::Joystick::Y) << endl;
	cout << "PovX Axis: " << sf::Joystick::getAxisPosition(0, sf::Joystick::Z) << endl;
	cout << "PovY Axis: " << sf::Joystick::getAxisPosition(0, sf::Joystick::V) << endl;
	cout << "Amount of buttons: " << sf::Joystick::getButtonCount(0) << endl;
	for (int i{ 0 }; i < 10; ++i) {
		if (sf::Joystick::isButtonPressed(0, i)) {
			cout << i << ": " << "I am pressed now!" << endl;
		}
	}
	*/

	//Player 1
	sharedMemory->setMove(0, false);
	//sharedMemory->setJump(0, false);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) <= -50)
	{
		sharedMemory->setDirection(0, -1);
		sharedMemory->setMove(0, true);
		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) >= 50)
	{
		sharedMemory->setDirection(0, 1);
		sharedMemory->setMove(0, true);
		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Joystick::isButtonPressed(0,9))
	{
		sharedMemory->setJump(0, true);
		
	}


}
