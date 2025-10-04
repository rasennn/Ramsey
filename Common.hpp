#pragma once
#include <Siv3D.hpp>

enum class State {
	LoadGraph,
	Title,
	TurnSelect,
	HowToPlay,
	Game,
};


struct GameData {
	//プレイ人数 1 or 2
	int32 player = 0;

	//先攻のプレイヤー(1P MODEのみ)
	// 0 or 1 or 2(random)
	int32 firstPlayer = 0;

	//相手は必勝法を使ってくる
	bool optimal = false;

	//必勝法
	// state : 0,1,2, ...
	//{0,1,2, ... } -> {0,1,2, ... }
	std::array<Array<int32>, 3731613> edge;
	//{0, 1, 2, ... } -> grundy
	std::array<int32, 3731613> grundy;
	//{0,1,2, ... } -> 状態を表す15桁
	std::array<int32, 3731613> conv_state;
	//状態を表す15桁　-> {0,1,2, ... }
	std::map<int32, int32> inv_state;
};


using App = SceneManager<State, GameData>;
