#include <stdio.h>
#include <irrKlang.h>
#include <iostream>

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

#if defined(WIN32)
#include <conio.h>
#else
#include "common/conio.h"
#endif

class IrrKlang
{
public:
	IrrKlang() = default;
	~IrrKlang() { m_Engine->drop(); }

	void createSound()
	{
		m_Engine = irrklang::createIrrKlangDevice();
		
		if (!m_Engine)
		{
			printf("Could not start engine \n");
		}
	}

	void play(const char* filepath, bool repeat)
	{
		m_Engine->play2D(filepath, repeat);
	}	

	void stop()
	{
		m_Engine->stopAllSounds();
	}

	void setVolume(float volume)
	{
		m_Engine->setSoundVolume(volume);
	}

private:
	irrklang::ISoundEngine* m_Engine;

};