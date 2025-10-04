# include "TurnSelect.hpp"

TurnSelect::TurnSelect(const InitData& init)
	: IScene{ init }
{

}

void TurnSelect::update()
{
	firstTransition.update(firstButton.mouseOver());
	secondTransition.update(secondButton.mouseOver());
	randomTransition.update(randomButton.mouseOver());
	backTransition.update(backButton.mouseOver());

	if (firstButton.mouseOver() || secondButton.mouseOver() || randomButton.mouseOver() || backButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	if (firstButton.leftClicked())
	{
		// 先攻
		getData().firstPlayer = 0;
		changeScene(State::Game, 0.3s);
	}
	else if (secondButton.leftClicked())
	{
		// 後攻
		getData().firstPlayer = 1;
		changeScene(State::Game, 0.3s);
	}
	else if (randomButton.leftClicked())
	{
		// ランキングシーンへ
		getData().firstPlayer = 2;
		changeScene(State::Game, 0.3s);
	}
	else if (backButton.leftClicked())
	{
		// 終了
		getData().player = 0;
		changeScene(State::Title, 0.3s);
	}
}

void TurnSelect::draw() const
{
	Scene::SetBackground(Palette::White);

	firstButton.draw(ColorF{ 0.7, firstTransition.value() }).drawFrame(2, Palette::Black);
	secondButton.draw(ColorF{ 0.7, secondTransition.value() }).drawFrame(2, Palette::Black);
	randomButton.draw(ColorF{ 0.7, randomTransition.value() }).drawFrame(2, Palette::Black);
	backButton.draw(ColorF{ 0.7, backTransition.value() }).drawFrame(2, Palette::Black);

	FontAsset(U"LargeFont")(U"ラムゼーの定理ゲーム").drawAt(Vec2{ Scene::Center().x, 200 }, Palette::Black);
	FontAsset(U"MediumFont")(U"先攻").drawAt(firstButton.center(), ColorF{ 0.25 });
	FontAsset(U"MediumFont")(U"後攻").drawAt(secondButton.center(), ColorF{ 0.25 });
	FontAsset(U"MediumFont")(U"ランダム").drawAt(randomButton.center(), ColorF{ 0.25 });
	FontAsset(U"MediumFont")(U"BACK").drawAt(backButton.center(), ColorF{ 0.25 });

	FontAsset(U"MediumFont")(U"先攻・後攻を選択してください").drawAt(selectTextPosition, Palette::Black);
}
