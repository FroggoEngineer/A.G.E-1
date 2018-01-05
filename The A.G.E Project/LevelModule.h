#ifndef LEVELMODULE_H
#define LEVELMODULE_H
#include "Matrix.h"
#include <SFML/Graphics.hpp>
#include "MemoryModule.h"

class LevelModule {

public:
	LevelModule() = default;
	LevelModule(MemoryModule* mem) { sharedMemory = mem; createTex(); };
	sf::Texture getMapTex() { return mapTex.getTexture(); };
private:
	void createTex();
	Matrix* map{ nullptr };
	sf::RenderTexture mapTex;
	MemoryModule* sharedMemory{ nullptr };
};

#endif
