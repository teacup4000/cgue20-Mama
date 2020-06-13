#include "stdafx.h"
#include "Game.h"

void Game::Win()
{
	
}

void Game::Lose()
{
	Stop();
}

void Game::GetDamage()
{
	m_Life -= 60;
	if (m_Life <= 0)
	{
		Lose();
	}

}

void Game::GetLife()
{
	m_Life += 30;
}

void Game::CheckStatus(uint32_t status)
{
	switch (status)
	{
	case 1:
		Win();
		break;
	case 2:
		Lose();
		break;
	}
}

void Game::Stop()
{
	m_Running = false;
}