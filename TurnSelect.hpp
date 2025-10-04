#pragma once
#include "Common.hpp"

class TurnSelect : public App::Scene
{
public:

	TurnSelect(const InitData& init);

	void update() override;

	void draw() const override;

private:

	SecondsF change = 0.2s;

	Vec2 selectTextPosition{ Scene::Center().movedBy(0, -50) };

	Rect firstButton{ Arg::center = Scene::Center().movedBy(-250, 50), 400, 60 };
	Transition firstTransition{ change, change };

	Rect secondButton{ Arg::center = Scene::Center().movedBy(250, 50), 400, 60 };
	Transition secondTransition{ change, change };

	Rect randomButton{ Arg::center = Scene::Center().movedBy(0, 150), 400, 60 };
	Transition randomTransition{ change, change };

	Rect backButton{ Arg::center = Scene::Center().movedBy(0, 250), 400, 60 };
	Transition backTransition{ change, change };
};
