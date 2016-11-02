#pragma once

#include "fmod.h"
#include "fmod_errors.h"


class Sound
{
public:
	Sound();
	~Sound();

private:
	
	FMOD::System* fmodSystem;
};

Sound::Sound()
{
}

Sound::~Sound()
{
}