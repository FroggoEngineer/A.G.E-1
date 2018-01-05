#include "LevelModule.h"
#include <iostream>

using namespace std;
void LevelModule::createTex()
{
	map = &sharedMemory->getMapMatrix();
	int width = map->sizeX();
	int height = map->sizeY();
	cout << width << "  :  " << height << endl;
	if (!mapTex.create( width*50, height*50))
		std::cerr << "Failed to load off_screen texture" << std::endl;

	sf::Sprite renderSprite;

	for (int i{ 0 }; i < height; i++) {
		for (int j{ 0 }; j < width; j++) {
			cout << map->at(j, i) << " ";
			if (map->at(j, i) > 0) {
				renderSprite.setPosition(j * 50, i * 50);
				renderSprite.setTexture(sharedMemory->getBlock(map->at(j, i)-1));
				mapTex.draw(renderSprite);
			}
		}
		cout << endl;
	}
	mapTex.display();
	cout << "Displaying mapTex" << endl;
}
