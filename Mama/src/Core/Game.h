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
	bool m_Running = true;

	void Win();
	void Lose();
	void GetDamage();
	void GetLife();
	void CheckStatus(uint32_t status);
	void Stop();
	GameStatus getStatus() { return m_Status; }

private:
	GameStatus m_Status = GameStatus::DEFAULT;
	int32_t m_Life = 100;
};