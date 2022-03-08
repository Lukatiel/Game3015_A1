#pragma once
namespace Category
{
	enum Type
	{
		//Each category has one bit to set to 1 except none
		None = 0,
		Scene = 1 << 0,
		PlayerAircraft = 1 << 1,
		AlliedAirecraft = 1 << 2,
		EnemyAircraft = 1 << 3,
	};
}