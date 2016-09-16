#pragma once
#include <GL/glut.h>
#include <iostream>
#include <map>
#include<vector>

class Textures {
	std::vector<std::string> files = { "blue_tiles","grunge","metal_door","old_windows", "night_office","concrete", "blocks","bricks", "wood",
		"grass", "grey_wall","glass","windows","door","night_windows" };
	std::map<std::string, GLuint> textures;
	GLuint loadTexture(const char * filename, int width, int height);
	
public:
	Textures();
	GLuint getTexture(std::string texture);
	void activateTexture(std::string texture);
};