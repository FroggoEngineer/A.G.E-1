#include <SFML/Graphics.hpp>
#include <thread>
#include "Matrix.h"
#include <iostream>
#include "MemoryModule.h"
#include "RenderModule.h"
#include "PlayerModule.h"
#include "PhysicsModule.h"
#include "LevelModule.h"

using namespace std;

int main()
{

	/*
	Asynchronous Game Engine (AGE)
	Each module will run on it's own thread, decoupled from the main thread.
	The main thread only initializes all the modules and their threads, for one exception, the menu.
	Hoping to achieve great performance and modularity using this system.
	New tools for measuring the performance must be created due to FPS not being a good indicator anymore.
	Frames Per Second - Measure GPU performance
	Thread performance - Measures performance of each Thread for CPU performance
	Memory performance - Measures how fast it can load assets from disc to ram

	Main Thread + 7threads (not counting NetServer & NetClient)

	RenderModule  (Handles all updates regarding rendering but Draw Calls are left to the Main thread)
	NetServer     (The server when multiplayer is used)
	NetClient     (The client when multiplayer is used)
	PlayerModule  (Handles everything on the player. Input, items etc)
	PhysicsModule (Handles all movement and physics related things)
	MemoryModule  (A shared memory module where all shared Data is stored between modules)
	AudioModule   (Handles audio playing)
	LevelModule   (Loads the map, npcs, events etc)
	AIModule      (Handles the update of npcs using Data from MemoryModule)
	

	Optimizations will be done, one already implemented is 2D Matrix as a 1D Matrix.
	Other optimizations will be usage of std::move and std::swap for efficient and quick memory handling.

	Initialization queue:
	1. MemoryModule
	2. NetServer (If mp)
	3. NetClient (If mp)
	4. LevelModule
	5. PlayerModule
	6. AIModule
	7. AudioModule
	8. PhysicsModule
	9. RenderModule

	When the game starts, start all threads.

	*/
	
	//Module Initialization
	MemoryModule sharedMemory;
	LevelModule level(&sharedMemory);
	PlayerModule player(&sharedMemory);
	PhysicsModule physics(&sharedMemory);
	RenderModule renderer(&sharedMemory);
	
	//Setting up window
	sharedMemory.setWindowSize(1280, 720);
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	sf::RenderWindow window(sf::VideoMode(1280,720), "A.G.E Project");
	window.setVerticalSyncEnabled(false);

	//Create first player input
	sharedMemory.createPlayers(1);

	//Start the modules on their own threads
	player.start();
	physics.start();	//Move to when game starts
	renderer.start();	//Move to when game starts

	
	//Setting up things for rendering, should be moved to RenderModule
	std::vector<sf::Sprite> players;
	std::vector<sf::Sprite> npcs;
	std::vector<sf::Sprite> objects;

	sf::Sprite a;
	players.push_back(a);

	//Temporary solution for the Map, needs to be heavily optimized.
	//---------------------------------------------
	sf::Texture mapTexture{ level.getMapTex() };
	sf::Sprite mapSprite{ mapTexture };
	mapSprite.setPosition(0, 0);
	//---------------------------------------------
	
	sf::Texture background;
	if (!background.loadFromFile("Data/background.jpg")) {

	}

	//Move to Render module?
	sf::Sprite backSprite{ background };
	backSprite.setPosition(0, 0);


	
	

	while (window.isOpen())
	{
		//Timer for checking render update performance in milliseconds
		auto renderPerf = std::chrono::high_resolution_clock::now();

		
		//Check if window is to be closed, execute closing sequence
		//-------------------------------------------------------
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sharedMemory.getShutdown())
			window.close();
		//-------------------------------------------------------


		//Move to Render Module
		for (int i{ 0 }; i < players.size(); ++i) {
			players[i].setPosition(sharedMemory.getX(i), sharedMemory.getY(i));
			players[i].setTexture(sharedMemory.getPlayerTex(sharedMemory.getCurrentAnimation(i)));
		}
		
		
		
		
		

		window.clear();

		window.draw(backSprite); //Background
		window.draw(mapSprite);  //Map

		//Vector for npcs
		for (int i{ 0 }; i < npcs.size(); ++i) {
			window.draw(npcs[i]);
		}

		//Vector for players
		for (int i{ 0 }; i < players.size(); ++i) {
			window.draw(players[i]);
		}

		//Vector for objects
		for (int i{ 0 }; i < objects.size(); ++i) {
			window.draw(objects[i]);
		}

		


		std::chrono::duration<double, std::milli> elapsedTime = std::chrono::high_resolution_clock::now() - renderPerf;

		sharedMemory.setRenderPerf(elapsedTime.count());

		window.display();
		
		
	}

	

	
	return 0;
}