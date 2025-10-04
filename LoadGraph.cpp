#include "LoadGraph.hpp"

LoadGraph::LoadGraph(const InitData& init)
	: IScene{ init }
{
	// オープンに失敗
	if (not edgeReader) { throw Error{ U"Failed to open text/grundy/ramsey_edge.txt" }; }
	if (not grundyReader) { throw Error{ U"Failed to open text/grundy/ramsey_grundy.txt" }; }
	if (not stateReader) { throw Error{ U"Failed to open text/grundy/ramsey_state.txt" }; }

	PreReadData();
}

void LoadGraph::update()
{
	progressPercent = progress * 100 / (OptimalNode * 3 + OptimalEdge);

	if (progress < OptimalNode) {
		progress += ReadState((int32)progress, loadBlock);
	}
	else if (progress < OptimalNode * 2) {
		progress += ReadGrundy((int32)progress - OptimalNode, loadBlock);
	}
	else if (progress < OptimalNode * 3) {
		progress += ReadInvState((int32)progress - OptimalNode * 2, loadBlock);
	}
	else if (progress < OptimalNode * 3 + OptimalEdge) {
		progress += ReadEdge(0, loadBlock);
	}
	else {
		changeScene(State::Title);
	}
}

void LoadGraph::draw() const
{
	//背景色
	Scene::SetBackground(Palette::White);

	FontAsset(U"MediumFont")(U"LOADING...   " + Format(progressPercent) + U"%").drawAt(Scene::Center(), ColorF{0.25});
}
