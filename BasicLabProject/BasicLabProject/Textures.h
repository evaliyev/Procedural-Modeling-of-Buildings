#pragma once
#include <GL/glut.h>
#include <iostream>
#include <map>
#include<vector>

class Textures {
	std::vector<std::string> files = {"concrete", "blocks","bricks", "wood", "grass", "grey_wall","glass","windows","door" };
	std::map<std::string, GLuint> textures;
	GLuint loadTexture(const char * filename, int width, int height);
	
public:
	Textures();
	GLuint getTexture(std::string texture);
	void activateTexture(std::string texture);
};