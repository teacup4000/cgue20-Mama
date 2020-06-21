#include "stdafx.h"
#include "Game.h"

void Game::Win()
{
	m_Status = GameStatus::WIN;
}

void Game::Lose()
{
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

void Game::TrampleDamage()
{
	m_Life -= 90;
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