#pragma once

#include "texture.h"

class Data{

public:
	Data(void);
	~Data(void);

	int  getID(int img);
	void getSize(int img, int &w, int &h);
	bool LoadImage(int img, char *filename, PixelFormat format);

private:
	Texture texture[];
};