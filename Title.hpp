#pragma once
#include "Common.hpp"

struct ModeChangeEffect : IEffect
{
	SecondsF coverTime = 0.2s;
	//SecondsF circleTimeFast = 0.2s;
	SecondsF circleTimeSlow = 1.5s;
	SecondsF textTime = 1.0s;

	Color ringColor;
	String displayText;

	explicit ModeChangeEffect(const Color col, const String s)
		: ringColor{col}, displayText{s}
	{}

	Transition coverTrans		{ coverTime,			coverTime };
	//Transition circleTransFast	{ circleTimeFast * 2.0,	0.0s };
	Transition circleTransSlow	{ circleTimeSlow,		0.0s };
	Transition textTrans		{ textTime,				0.0s };

	bool update(double t) override
	{
		if (t <= coverTime.count()) {
			coverTrans.update(true);
		}
		else if (t <= (coverTime + circleTimeSlow).count()) {
			//circleTransFast.update(true);
			circleTransSlow.update(true);
		}
		else {
			coverTrans.update(false);
		}
		
		// カバー用背景
		Rect{ 0,0,Scene::Size()}.draw(ColorF(0.0, 0.8 * coverTrans.value()));

		// 円
		if (coverTime.count() <= t && t <= (coverTime + circleTimeSlow).count()) {
			Circle{ Scene::Center(), (circleTransSlow.value() * 100 + 300) }.drawFrame(20.0 - (18.0 * circleTransSlow.value()), ringColor);
			FontAsset(U"MediumFont")(displayText).drawAt(Scene::Center(), Palette::White);
		}

		return (t < (coverTime * 2.0 + circleTimeSlow).count());
	}
};

class Title : public App::Scene
{
public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;

private:

	//ボタンの暗転の秒数
	SecondsF change = 0.2s;

	//1P MODE
	Rect onePlayerButton{ Arg::center = Scene::Center().movedBy(-250, 0), 400, 60};
	Transition onePlayerTransition{ change, change };

	//2P MODE
	Rect twoPlayerButton{ Arg::center = Scene::Center().movedBy(250, 0), 400, 60 };
	Transition twoPlayerTransition{ change, change };

	//HOW TO PLAY
	Rect howToPlayButton{ Arg::center = Scene::Center().movedBy(0, 100), 400, 60 };
	Transition howToPlayTransition{ change, change };

	//EXIT
	Rect exitButton{ Arg::center = Scene::Center().movedBy(0, 200), 400, 60 };
	Transition exitTransition{ change, change };

	//LICENSE
	Point licenseButtonSize{ 150, 40 };
	Rect licenseButton{ Scene::Width() - licenseButtonSize.x - 10, Scene::Height() - licenseButtonSize.y - 10, licenseButtonSize.x, licenseButtonSize.y};
	Transition licenseTransition{ change, change };

	//EFFECT
	Effect effect;
};
