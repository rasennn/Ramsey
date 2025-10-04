#pragma once
#include "Common.hpp"

class PII {
public:
	int32 first;
	int32 second;

	PII(int32 first_, int32 second_) {
		first = first_;
		second = second_;
	}

	bool operator==(PII a) {
		return (first == a.first) && (second == a.second);
	}
};

class Game : public App::Scene
{
public:

	Game(const InitData& init);

	void update() override;

	void draw() const override;

private:

	void CannotSelect() {
		redTransition.update(false);
		blueTransition.update(false);
		//isAnythingMouseOver = false;
	}

	//0:塗れない 1:赤 2:青 3:赤と青
	int32 WhetherCanPaint(PII x) {
		if (edgeColor[x.first][x.second] >= 4) { return 0; }

		int32 res = 3;
		for (int i = 0; i < vertexSize; i++) {
			if (i == x.first || i == x.second) { continue; }
			if (edgeColor[Min(i, x.first)][Max(i, x.first)] == 4 && edgeColor[Min(i, x.second)][Max(i, x.second)] == 4) {
				res -= 1;
				break;
			}
		}
		for (int i = 0; i < vertexSize; i++) {
			if (i == x.first || i == x.second) { continue; }
			if (edgeColor[Min(i, x.first)][Max(i, x.first)] == 5 && edgeColor[Min(i, x.second)][Max(i, x.second)] == 5) {
				res -= 2;
				break;
			}
		}

		return res;
	}

	void LoseSelect() {
		for (int i = 0; i < vertexSize; i++) {
			for (int j = i + 1; j < vertexSize; j++) {
				if (edgeColor[i][j] == 0) {
					selectedEdge = PII{ i,j };
					edgeColor[selectedEdge.first][selectedEdge.second] = 2;
					return;
				}
			}
		}
	}

	void RandomSelectEdge() {
		if (restEdges.empty()) {
			LoseSelect();
			return;
		}

		comThinkingTime = 1.0;

		for (int i = 0; i < restEdges.size(); i++) {
			if (WhetherCanPaint(restEdges[i]) == 0) {
				restEdges.erase(restEdges.begin() + i);
				i--;
			}
		}

		if (restEdges.empty()) {
			LoseSelect();
			return;
		}
		int32 comSelectID = (int32)Random(restEdges.size() - 1);
		PII comSelect = restEdges[comSelectID];

		selectedEdge = comSelect;
		edgeColor[selectedEdge.first][selectedEdge.second] = 2;

		return;		
	}

	void RandomPaintEdge() {
		switch (WhetherCanPaint(selectedEdge))
		{
		case 0:
			edgeColor[selectedEdge.first][selectedEdge.second] = Random(4, 5);
			break;
		case 1:
			edgeColor[selectedEdge.first][selectedEdge.second] = 4;
			break;
		case 2:
			edgeColor[selectedEdge.first][selectedEdge.second] = 5;
			break;
		case 3:
			edgeColor[selectedEdge.first][selectedEdge.second] = Random(4, 5);
			break;
		default:
			break;
		}
		state = getData().inv_state[getData().conv_state[state] + pow3[EtoS[selectedEdge.first][selectedEdge.second]] * (edgeColor[selectedEdge.first][selectedEdge.second] - 3)];
		selectedEdge = PII{ 0,0 };
		turnPlayer = (turnPlayer + 1) % 2;

		return;
	}

	void OptimalSelectEdge() {
		Array<int32> candidate;
		for (int32 x : getData().edge[state]) {
			if (getData().grundy[x] == 0) {
				candidate << x;
			}
		}

		comThinkingTime = 1.0;

		if (candidate.empty()) {
			RandomSelectEdge();
			OptimalSelectColor = -1;
			return;
		}

		//comSelectID : 15桁
		int32 comSelectID = getData().conv_state[Sample(candidate)] - getData().conv_state[state];
		PII comSelect = PII{ 0,0 };
		for (int32 i = 0; i < 15; i++) {
			if (ternary(comSelectID, i) != 0) {
				OptimalSelectColor = ternary(comSelectID, i) + 3;
				comSelect = PII{ StoE[i] / 10 - 1, StoE[i] % 10 - 1 };
				break;
			}
		}
		
		selectedEdge = comSelect;
		edgeColor[selectedEdge.first][selectedEdge.second] = 2;

		return;
	}
	void OptimalPaintEdge(int32 selectedColor) {
		switch (selectedColor)
		{
		case 4:
			edgeColor[selectedEdge.first][selectedEdge.second] = 4;
			break;
		case 5:
			edgeColor[selectedEdge.first][selectedEdge.second] = 5;
			break;
		case -1:
			RandomPaintEdge();
			return;
			break;
		default:
			break;
		}
		state = getData().inv_state[getData().conv_state[state] + pow3[EtoS[selectedEdge.first][selectedEdge.second]] * (selectedColor - 3)];
		selectedEdge = PII{ 0,0 };
		turnPlayer = (turnPlayer + 1) % 2;

		return;
	}
	int32 ternary(int32 x, int32 d) { //下からd(0-indexed)桁目
		return (x / pow3[d]) % 3;
	}


	Vec2 graphCenter = { Scene::Center().x / 2.0 + 30 , Scene::Center().y };
	const double graphRadius = 250;

	Array<Vec2> vertexPositions;
	const int32 vertexSize = 6;
	const double vertexRadius = 60;
	const double vertexFrame = 5;

	Array<int32> vertexColor;
	const Array<Color> vertexPalette = { Palette::White, Palette::Red, Palette::Blue };

	Array<Array<Quad>> edges;
	const double edgeWidth = 20;
	Array<double> edgeLength = { graphRadius, graphRadius * Math::Sqrt3, graphRadius * 2.0 };

	//0:未選択 1:マウスオーバー 2:選択済み 3:選択済みマウスオーバー 4:赤 5:青
	Array<Array<int32>> edgeColor;
	const Array<Color> edgePalette = { Palette::Black, Palette::Gray, Palette::Orange, Palette::Darkorange, Palette::Red, Palette::Blue };
	PII selectedEdge = PII{ 0,0 };

	Array<PII> restEdges;
	double comThinkingTime = 1.0;
	bool isComSelecting = false;


	Point turnTextCenter = Scene::Center().movedBy(Scene::Center().x / 2, -200);

	String instruction;

	
	double winnerDisplayTime = 3.0;
	Rect winnerDisplayCover{ 0, 0, Scene::Size() };
	Transition winnerDisplayTransition{ 0.5s, 0.5s };
	String winner = U"WINNER";

	SecondsF change = 0.2s;

	Rect redButton{ Arg::center = Scene::Center().movedBy(Scene::Center().x / 2, 50), 350, 120 };
	Transition redTransition{ change, change };

	Rect blueButton{ Arg::center = Scene::Center().movedBy(Scene::Center().x / 2, 220), 350, 120 };
	Transition blueTransition{ change, change };

	bool isFinished = false;
	bool isWinnerDisplayFinished = false;
	bool isAnythingMouseOver = false;
	int32 turnPlayer = (getData().firstPlayer == 2) ? Random(1) : getData().firstPlayer;

	Array<Array<String>> participants = { {}, {U"PLAYER", U"COM"}, {U"PLAYER 1", U"PLAYER 2"} };

	Rect retryButton{ Arg::center = Scene::Center().movedBy(Scene::Center().x / 2, 50), 400, 60 };
	Transition retryTransition{ change, change };

	Rect titleButton{ Arg::center = Scene::Center().movedBy(Scene::Center().x / 2, 150), 400, 60 };
	Transition titleTransition{ change, change };

	int32 state = 0;
	int32 OptimalSelectColor;
	const int32 StoE[15] = { 12,13,14,15,16,23,24,25,26,34,35,36,45,46,56 };
	const int32 EtoS[6][6] = { {-1,0,1,2,3,4},{0,-1,5,6,7,8},{1,5,-1,9,10,11},{2,6,9,-1,12,13},{3,7,10,12,-1,14},{4,8,11,13,14,-1} };
	const int32 pow3[16] = { 1,3,9,27,81,243,729,2187,6561,19683,59049,177147,531441,1594323,4782969,14348907 };
	String state_s;
};
