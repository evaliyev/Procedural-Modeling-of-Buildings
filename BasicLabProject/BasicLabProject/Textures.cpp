#include "Textures.h"
#include <stdio.h>
#include <errno.h>
 
Textures::Textures() {
	for (int i = 0; i < files.size(); i++)
		this->textures[files[i]] = loadTexture(("textures/"+files[i]+".bmp").c_str(), 1024, 1024);
}

GLuint Textures::loadTexture(const char * filename, int width, int height){
	GLuint texture;
	unsigned char * data;
	FILE * file; errno_t err;
	if ((err = fopen_s(&file, filename, "r")) != 0) {
		throw "Couldn't load image";
	}
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	//int size = fseek(file,);
	fread(data, width * height * 3, 1, file);
	fclose(file);
	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];
		data[index] = R;
		data[index + 2] = B;
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	return texture;
}

GLuint Textures::getTexture(std::string texture) {
	return this->textures[texture];
}

void Textures::activateTexture(std::string texture) {
	glBindTexture(GL_TEXTURE_2D, getTexture(texture));
}
