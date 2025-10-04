# include "Title.hpp"

Title::Title(const InitData& init)
	: IScene{ init }
{

}

void Title::update()
{
	//ボタンの暗転の管理
	onePlayerTransition.update(onePlayerButton.mouseOver());
	twoPlayerTransition.update(twoPlayerButton.mouseOver());
	howToPlayTransition.update(howToPlayButton.mouseOver());
	exitTransition.update(exitButton.mouseOver());
	licenseTransition.update(licenseButton.mouseOver());

	//カーソルの形
	if (onePlayerButton.mouseOver() || twoPlayerButton.mouseOver() || howToPlayButton.mouseOver() || exitButton.mouseOver() || licenseButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	if (onePlayerButton.leftClicked())
	{
		// 先攻後攻選択シーンへ
		getData().player = 1;
		changeScene(State::TurnSelect, 0.3s);
	}
	else if (twoPlayerButton.leftClicked())
	{
		// 2Pゲームシーンへ
		getData().player = 2;
		getData().firstPlayer = 0;
		changeScene(State::Game, 0.3s);
	}
	else if (howToPlayButton.leftClicked())
	{
		// 遊び方説明シーンへ
		changeScene(State::HowToPlay, 0.3s);
	}
	else if (exitButton.leftClicked())
	{
		// 終了
		System::Exit();
	}

	else if (licenseButton.leftClicked())
	{
		// ライセンス表示
		LicenseManager::ShowInBrowser();
	}

	if (KeyO.pressed() && KeyP.pressed() && KeyT.pressed() && not getData().optimal) {
		getData().optimal = true;
		effect.add<ModeChangeEffect>(Palette::Orangered, U"O p t i m i z a t i o n   a c t i v a t e d .");
	}
	else if (KeyR.pressed() && getData().optimal) {
		getData().optimal = false;
		effect.add<ModeChangeEffect>(Palette::Skyblue, U"R a n d o m   m o d e");
	}

	
}

void Title::draw() const
{
	//背景色
	Scene::SetBackground(Palette::White);

	//ボタン描画
	onePlayerButton.draw(ColorF{ 0.7, onePlayerTransition.value() }).drawFrame(2, Palette::Black);
	twoPlayerButton.draw(ColorF{ 0.7, twoPlayerTransition.value() }).drawFrame(2, Palette::Black);
	howToPlayButton.draw(ColorF{ 0.7, howToPlayTransition.value() }).drawFrame(2, Palette::Black);
	exitButton.draw(ColorF{ 0.7, exitTransition.value() }).drawFrame(2, Palette::Black);
	licenseButton.draw(ColorF{ 0.7, licenseTransition.value() }).drawFrame(2, Palette::Black);

	//文字描画
	FontAsset(U"LargeFont")(U"ラムゼーの定理ゲーム").drawAt(Vec2{ Scene::Center().x, 200 }, Palette::Black);
	FontAsset(U"MediumFont")(U"1P PLAY").drawAt(onePlayerButton.center(), ColorF{ 0.25 });
	FontAsset(U"MediumFont")(U"2P PLAY").drawAt(twoPlayerButton.center(), ColorF{ 0.25 });
	FontAsset(U"MediumFont")(U"HOW TO PLAY").drawAt(howToPlayButton.center(), ColorF{ 0.25 });
	FontAsset(U"MediumFont")(U"EXIT").drawAt(exitButton.center(), ColorF{ 0.25 });
	FontAsset(U"SmallFont")(U"LICENSE").drawAt(licenseButton.center(), ColorF{ 0.25 });

	//バージョン表示
	FontAsset(U"SmallFont")(U"Ver.2.0").draw(Vec2{ 10,10 }, Palette::Black);

	effect.update();
}
