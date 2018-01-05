#include "MemoryModule.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

MemoryModule::MemoryModule()
{
	
	posX = new int[4];
	posY = new int[4];
	playerDirection = new int[4];
	playerMove = new bool[4];
	playerJump = new bool[4];
	playerIdle = new bool[4];
	playerFalling = new bool[4];
	charAnim = new int[4];
	playerDodge = new bool[4];
	for (int i{ 0 }; i < 4; i++) {
		posX[i] = 10;
		posY[i] = 10;
		playerDirection[i] = 1;
		playerMove[i] = false;
		playerJump[i] = false;
		playerIdle[i] = true;
		playerFalling = false;
		charAnim[i] = 0;
		playerDodge[i] = false;
	}
	map.fillWith(0);
	loadPlayer();
	loadBlocks();
	loadMap();
}

sf::Texture& MemoryModule::getPlayerTex(int frame) const
{
	return player[frame];
}

void MemoryModule::createPlayers(int number)
{
	//Remove array version
	
	Character* tmp = new Character[number]; //Create new players for input
	for (int i{ 0 }; i < number; ++i) {
		Character a;
		tmp[i] = a;
	}

	std::swap(players, tmp); //Swap pointers
	if(tmp != nullptr)
		delete[] tmp; //Delete old players
	numOfPlayers = number;
	

	//Create new vector with specified amount of players
	std::vector<Character> tmpVec;
	for (int i{ 0 }; i < number; ++i) {
		Character a;
		tmpVec.push_back(a);
	}
	//Move the new vector into the old one
	playersV = std::move(tmpVec);
	

}

void MemoryModule::removePlayer(int id)
{
	//Find specified ID and remove.
	for (int i{ 0 }; i < playersV.size(); ++i) {
		if (playersV[i].getId() == id) {
			std::swap(playersV[i], playersV[playersV.size() - 1]);
			playersV.pop_back();
		}
	}
}

void MemoryModule::setAnimationPlayer(int character, int anim)
{
	charAnim[character] = anim;
}

void MemoryModule::setPlayerPos(int character, int x, int y)
{
	//To be removed after character implementation
	std::swap(posX[character], x);
	std::swap(posY[character], y);
	
	if (character < numOfPlayers) {
		players[character].setX(x);
		players[character].setY(y);
	}
	
}

void MemoryModule::loadMap()
{
	ifstream input;
	input.open("Data/Maps/map1.txt");

	int width;
	int height;
	if (!input) {
		cerr << "Error loading file info.txt" << endl;
	}
	else {
		input >> width;
		input >> height;
		map = Matrix{ width,height };

		for (int i{ 0 }; i < height; i++)
			for (int j{ 0 }; j < width; j++) {
				input >> map.at(j, i);
			}
	}
}

void MemoryModule::loadBlocks()
{
	/*
	1. Read from file the amount of blocks
	2. Create the array with that size, use try-catch for error handling
	3. Load in blocks from 0 to n-1
	*/

	ifstream input;
	input.open("Data/Blocks/Info");	//Add error control

	//Lock access to memory before assigning to avoid error from other threads trying to assign memory.
	concurrent.lock();
	blocks = new sf::Texture[18];	//Size should be taken from file
	concurrent.unlock();

	//Set pathway
	string s{ "Data/Blocks/" };

	//Loop and add all blocks to the array
	for (int i = 1; i < 7; i++) {

		s.append(to_string(i));
		s.append(".png");

		if (!blocks[i-1].loadFromFile(s))
			cout << "Couldn't load block: " << i << endl;
		else
			cout << "Loaded block: " << i << endl;

		s = "Data/Blocks/";
	}

}

void MemoryModule::loadPlayer()
{

	string s{ "Data/Player/" };

	ifstream input;
	s.append("players.txt");
	input.open(s);

	string playerName{ "" };

	input >> playerName;

	int textureAmount{ 0 };

	input >> textureAmount;

	player = new sf::Texture[textureAmount];

	

	for (int i{ 0 }; i < textureAmount; i++) {
		s = "Data/Player/";
		s.append(to_string(i));
		s.append(".png");
		cout << s << endl;

		if (!player[i].loadFromFile(s))
			cout << "Couldn't load playerTex: " << i << endl;
		else
			cout << "Loaded playerTex: " << i << endl;
		player[i].setSmooth(true);

	}

}
