#ifndef PHYSICSMODULE_H
#define PHYSICSMODULE_H
#include "MemoryModule.h"
#include "Matrix.h"
#include <thread>
class PhysicsModule {

public:
	PhysicsModule() = default;
	PhysicsModule(MemoryModule* mem);
	~PhysicsModule() = default;
	void update();
	void start();
private:
	void movePlayers();
	void moveNPCs();
	void moveItems();
	void exertForces();
	void playerCollision();
	void updateStats();
	sf::Time deltaTime{ sf::milliseconds(0) };
	double elapsedTimeTest{ 0 };
	//Player variables
	//------------------
	double* relX = new double[4]; //Calculation values
	double* relY = new double[4]; //Calculation values
	double* velX = new double[4];
	double* velY = new double[4];
	double* forceVecX = new double[4];
	double* forceVecY = new double[4];
	double* instantForceX = new double[4];
	double* instantForceY = new double[4];
	double* moveSpeed = new double[4];
	//------------------

	MemoryModule* sharedMemory;
	bool run{ false };
	std::thread t;
	std::thread stats;
	Matrix* mapMatrix;

	double gravity{ 0.001 };
	double airResistance{ 0.01 };
	double windForce{ 0 };
	

	double speedTimeTest{ 0 };

};



#endif
