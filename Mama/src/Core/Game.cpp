#include "stdafx.h"
#include "Game.h"

void Game::Win()
{
	OutputDebugString("WON!\n");
	m_Status = GameStatus::WIN;
}

void Game::Lose()
{
	OutputDebugString("LOST\n");
	m_Status = GameStatus::LOSE;
	Stop();
}

void Game::ReceiveDamage()
{
	m_Life -= 60;
	if (m_Life <= 0)
	{
		m_Life = 0;
		Lose();
	}

}

void Game::GainLife()
{
	m_Life += 30;
	if (m_Life > 100) {
		m_Life = 100;
	}
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

void Game::volumeIncrease() {
	if (volume < 1) {
		volume += 0.05f;
	}
}

void Game::volumeDecrease() {
	if (volume > 0) {
		volume -= 0.05f;
	}
}