#include "PhysicsModule.h"
#include <iostream>
#include <chrono>

using namespace std;

/*
	TODO:
	1. Adjust speed, velocity and forces parameters to work with delta time
*/

PhysicsModule::PhysicsModule(MemoryModule* mem)
{
	sharedMemory = mem;
	//Four players, should be remade to handle X amount
	for (int i{ 0 }; i < 4; i++) {
		relX[i] = 0;
		relY[i] = 0;
		velX[i] = 0;
		velY[i] = 0;
		forceVecX[i] = 0;
		forceVecY[i] = 0;
		instantForceX[i] = 0;
		instantForceY[i] = 0;
		moveSpeed[i] = 0;
	}

	mapMatrix = &sharedMemory->getMapMatrix();
	
}

/*
	Instant Force
		-Jump
		-Dodge
		-Knockback
		-Collision
	Continuous Force
		-Gravity
		-Air Resistance (moving objects)
	Velocity
		-Speed of players
	Position
		-Position of players

	Sum of C.Forces + I.Forces -> Velocity

*/

void PhysicsModule::update()
{
	while (run) {

		//Start time
		auto startTime = std::chrono::high_resolution_clock::now();

		//Exert Forces
		exertForces();
		//Player Collision
		playerCollision();
		//Move players using the velocity vectors
		movePlayers(); 
		sharedMemory->setJump(0, false);

		//End time
		auto endTime = std::chrono::high_resolution_clock::now();
		//Delta time
		std::chrono::duration<double, std::milli> elapsedTime = endTime - startTime;
		elapsedTimeTest = elapsedTime.count();

	}
}

void PhysicsModule::updateStats() {
	while (true) {
		system("cls");
		cout << "----------Physics----------" << endl;
		cout << "MoveSpeed: " << moveSpeed[0] << endl;
		cout << "Velocity X: " << velX[0] << endl;
		cout << "Velocity Y: " << velY[0] << endl;
		cout << "Force X: " << forceVecX[0] << endl;
		cout << "Force Y: " << forceVecY[0] << endl;
		cout << "Elapsed time: " << elapsedTimeTest << endl;
		cout << endl;
		cout << "----------Rendering----------" << endl;
		cout << "Update: " << sharedMemory->getRenderPerf() << "ms" << endl;

		std::this_thread::sleep_for(20ms);

	}
}

void PhysicsModule::start()
{
	for (int i{ 0 }; i < sharedMemory->getAmountPlayers(); i++) {
		relX[i] = sharedMemory->getX(i);
		relY[i] = sharedMemory->getY(i);
	}
	run = true;
	t = std::thread(&PhysicsModule::update, this);
	stats = std::thread(&PhysicsModule::updateStats, this);
}

void PhysicsModule::movePlayers()
{
	/*
	50 = half a meter
	300units per second
	
	*/
	//double delta{ 0.0001 };

	double delta{ elapsedTimeTest };
	double speed{ 100 };
	

	//move the players in X-axis
	for (int i{ 0 }; i < sharedMemory->getAmountPlayers(); i++) {
		moveSpeed[i] = 0;
		if (sharedMemory->getMove(i))
			moveSpeed[i] = speed*delta*sharedMemory->getDirection(i);


		relX[i] += moveSpeed[i]+(velX[i]*delta);
		relY[i] += velY[i]*delta;


	}

	
	for (int i{ 0 }; i < sharedMemory->getAmountPlayers(); i++)
		sharedMemory->setPlayerPos(i, (int)round(relX[i]), (int)round(relY[i]));

	
	
	
}



void PhysicsModule::exertForces()
{
	//Make sure the forces are zero to not have a build up effect
	for (int i{ 0 }; i < sharedMemory->getAmountPlayers(); i++) {
		forceVecX[i] = 0;
		forceVecY[i] = 0;
	}

	//Jump force, ForceVecY
	for (int i{ 0 }; i < sharedMemory->getAmountPlayers(); i++) {
		int x = sharedMemory->getX(i);
		int y = sharedMemory->getY(i);
		if (sharedMemory->getJump(i) && (mapMatrix->at(x / 50, (y + 90) / 50) != 0 || mapMatrix->at((x + 90) / 50, (y + 90) / 50) != 0)) {
			forceVecY[i] += -80;
			instantForceY[i] += -80;
		}
	}
	//Gravity ForceVecY
	for (int i{ 0 }; i < sharedMemory->getAmountPlayers(); i++) {
		forceVecY[i] += gravity;
	}
	//Air resistance ForceVecX
	for (int i{ 0 }; i < sharedMemory->getAmountPlayers(); i++) {
		if (forceVecX[i] > 0)
			forceVecX[i] -= airResistance;
		else if (forceVecX[i] < 0)
			forceVecX[i] += airResistance;
	}
	//Wind force
	for (int i{ 0 }; i < sharedMemory->getAmountPlayers(); i++) {
		forceVecX[i] += windForce;
	}
	//Knockback ForceVecX + ForceVecY
	
	/* 
	Dodge - WIP, not good
	for (int i{ 0 }; i < 4; i++) {
		int x = sharedMemory->getX(i);
		int y = sharedMemory->getY(i);
		if (sharedMemory->getDodge(i) && (mapMatrix->at(x / 50, (y + 90) / 50) != 0 || mapMatrix->at((x + 90) / 50, (y + 90) / 50) != 0) ) {
			forceVecY[i] += -40;
			instantForceY[i] += -30;
			forceVecX[i] += -50*sharedMemory->getDirection(i);
			instantForceX[i] += -50*sharedMemory->getDirection(i);
			sharedMemory->setDodge(i, false);
			sharedMemory->setJump(i, false);
		}
	}
	*/

	//Forces affect speed
	for (int i{ 0 }; i < sharedMemory->getAmountPlayers(); i++) {
		velX[i] += forceVecX[i];
		velY[i] += forceVecY[i];
		forceVecY[i] -= instantForceY[i];
		instantForceY[i] = 0;
		if (velX[i] > 10)
			velX[i] = 10;
		else if (velX[i] < -10)
			velX[i] = -10;
	}
	
	
}

void PhysicsModule::playerCollision()
{
	int height = 90;
	int width = 90;

	
	for (int i{ 0 }; i < sharedMemory->getAmountPlayers(); i++) {
		int x = sharedMemory->getX(i);
		int y = sharedMemory->getY(i);

		//Ground
		if ((mapMatrix->at(x / 50, (y + height) / 50) != 0 || mapMatrix->at((x + width) / 50, (y + height) / 50) != 0) && !sharedMemory->getJump(i)) {
			if (forceVecY[i] > 0) {
				velY[i] = 0;
				//forceVecY[i] = 0;
			}
		}

		//Above
		if ((mapMatrix->at(x / 50, (y - 2) / 50) != 0 || mapMatrix->at((x + width) / 50, (y -2) / 50) != 0)) {
			forceVecY[i] = 1;
			velY[i] = 0;
			sharedMemory->setPlayerPos(i, x, y + 2);
			relY[i] += 2;
		}

		if ((mapMatrix->at((x-2) / 50, (y) / 50) != 0 || mapMatrix->at((x-2) / 50, (y+height-5) / 50) != 0) && !sharedMemory->getJump(i)) {
			//forceVecX[i] = 0.1;
		}

		if ((mapMatrix->at((x +width+ 2) / 50, (y) / 50) != 0 || mapMatrix->at((x + width + 2) / 50, (y + height - 5) / 50) != 0) && !sharedMemory->getJump(i)) {
			//forceVecX[i] = -0.1;
		}

		if (x < 0) {
			sharedMemory->setPlayerPos(i, 0, y);
			relX[i] = 0;
		}
		if (x+width > sharedMemory->getWindowX()) {
			sharedMemory->setPlayerPos(i, sharedMemory->getWindowX()-width, y);
			relX[i] = sharedMemory->getWindowX() - width;
		}
		if (y < 0) {
			sharedMemory->setPlayerPos(i, x, 0);
			relY[i] = 0;
		}

	}

	for (int i{ 0 }; i < sharedMemory->getAmountPlayers(); i++) {
		int x = sharedMemory->getX(i);
		int y = sharedMemory->getY(i);

		if ((mapMatrix->at(x / 50, (y + height+2) / 50) == 0 || mapMatrix->at((x + width) / 50, (y + height+2) / 50) == 0)) {
			sharedMemory->setJump(i, false);
		}
	}
	

}
