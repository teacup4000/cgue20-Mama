#pragma once
#include <cstdint>

enum class GameStatus {

	DEFAULT = 0,
	WIN = 1,
	LOSE = 2
};

class Game
{
public:
	bool m_Running;

	void Win();
	void Lose();
	void GetDamage();
	void GetLife();
	void CheckStatus(uint32_t status);
	void Stop();

private:
	GameStatus m_Status;
	uint32_t m_Life;

};