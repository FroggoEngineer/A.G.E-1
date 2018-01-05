#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H


class Character {

public:
	Character() = default;
	~Character() = default;

	int getX() const { return posX; };
	int getY() const { return posY; };
	int getDirection() const { return playerDirection; };

	bool getMove() const { return playerMove; };
	bool getJump() const { return playerJump; };
	bool getIdle() const { return playerIdle; };
	bool getFalling() const { return playerFalling; };
	int getId() const { return id; };

	void setX(int x) { posX = x; };
	void setY(int y) { posY = y; };

	void setMove(bool&& mov) { playerMove = mov; };
	void setJump(bool&& jump) { playerJump = jump; };
	void setIdle(bool&& idle) { playerIdle = idle; };
	void setFalling(bool&& fall) { playerFalling = fall; };


private:

	int posX{ 0 };
	int posY{ 0 };
	int id{ 0 };
	int playerDirection{ 0 };
	bool playerMove{ false };
	bool playerJump{ false };
	bool playerIdle{ true };
	bool playerFalling{ false };


};

#endif // !CHARACTER_H
