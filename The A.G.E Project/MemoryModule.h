#ifndef MEMORYMODULE_H
#define MEMORYMODULE_H


#include <SFML/Graphics.hpp>
//#include "Matrix.h"
#include <string>
#include <fstream>
#include <iostream>
#include "Matrix.h"
#include "Character.h"

/*
	Optimizations:
	1. Reference "&" used with "const" get-methods to remove copying
	2. in set-methods, std::swap is used to remove race between set and get
		2.1: With get-methods, you get either the old value or the new value. Where this is used, the one that gets the values is the one that will also set the new values.
			 Render will request values, but only once which doesn't cause a problem run-time.
	3. 


*/

class MemoryModule {
public:
	MemoryModule();			//Let the compiler handle default constructor
	//MemoryModule(const std::string&);
	~MemoryModule() = default;			//noexcept flag improves performance on destructor
	//Matrix& getMap() { return map; }; 

	//Access functions Window
	//----------------------------
	sf::Texture& getPlayerTex(int) const;
	void setWindowSize(int x, int y) { windowX = x; windowY = y; };
	int getWindowX() { return windowX; };
	int getWindowY() { return windowY; };
	void setShutdown(bool&& shut) { shutdown = shut; }
	bool getShutdown() const { return shutdown; }
	//----------------------------
	
	//Player Module
	//----------------
	void createPlayers(int number);
	void addPlayer();
	void removePlayer(int);

	void setDirection(int character, int dir) { playerDirection[character] = dir; };
	void setJump(int character, bool jump) { playerJump[character] = jump; };
	void setMove(int character, bool move) { playerMove[character] = move; playerIdle[character] = !move; };
	void setSound(int);
	void setDodge(int character, bool dodge) { playerDodge[character] = dodge; };

	
	//Reference & to skip unnecessary copying when transferring to physics.
	bool& getDodge(int character) const { return playerDodge[character]; };
	int getX(int character) const { return players[character].getX(); }; //Player
	int getY(int character) const { return players[character].getY(); }; //Player
	int& getDirection(int character) const { return playerDirection[character]; };
	bool& getMove(int character) const { return playerMove[character]; };
	bool& getJump(int character) const { return playerJump[character]; };
	int getAmountPlayers() const { return numOfPlayers; };
	//----------------

	//Render Module
	//----------------
	void setAnimationPlayer(int, int); //Player, frame
	bool getIdle(int character) const { return playerIdle[character]; };
	void setIdle(int character, bool idle) { playerIdle[character] = idle; };
	bool getFalling(int character) const { return playerFalling[character]; };
	int& getCurrentAnimation(int character) const { return charAnim[character]; };
	
	//----------------

	//Physics Module
	//----------------
	void setPlayerPos(int, int, int); //Player, X, Y
	double getRenderPerf() { return renderPerf; };
	void setRenderPerf(double&& perf) { renderPerf = perf; };
	//----------------

	//Audio Module
	//----------------
	
	//----------------


	//Level Module
	//----------------
	Matrix& getMapMatrix() { return map; };
	sf::Texture& getBlock(int i) { return blocks[i]; };
	
	//----------------

private:
	//Load functions
	//------------------
	void loadMap();
	void loadBlocks(); //-> Modify so that blocks is a class instead of just texture, allows for greater freedom of creation
	void loadPlayer(); //-> Loads info for the PlayerModule
	void loadMonster();
	//void loadWeapons(); -> load items, create item class
	void loadSounds();

	//------------------
	sf::Mutex concurrent{};
	sf::Texture* blocks{ nullptr };
	sf::Texture* player{ nullptr };

	//Player variables, MOVE TO PLAYER CLASS!
	//------------------
	Character* players{ nullptr };
	std::vector<Character> playersV;
	int numOfPlayers{ 0 };
	int* posX{ nullptr }; //Set return values for position
	int* posY{ nullptr }; //Set return values for position
	int* playerDirection{ nullptr }; 
	bool* playerMove{ nullptr };
	bool* playerJump{ nullptr };
	bool* playerIdle{ nullptr };
	bool* playerFalling{ nullptr };
	int* charAnim{ nullptr };
	bool* playerDodge{ nullptr };
	//------------------

	//Maps
	//------------------
	Matrix map;
	//------------------

	//Temporary render performance for physics stats
	double renderPerf{ 0 };

	//Window size
	int windowX{ 0 };
	int windowY{ 0 };
	bool shutdown{ false };
};

#endif
