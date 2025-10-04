# include "Common.hpp"
# include "LoadGraph.hpp"
# include "Title.hpp"
# include "HowToPlay.hpp"
# include "Game.hpp"
# include "TurnSelect.hpp"

void Main()
{
	//フォントアセット
	FontAsset::Register(U"LLFont", 120, Resource(U"Fonts/yumindb.ttf"));
	FontAsset::Register(U"LargeFont", 90, Resource(U"Fonts/yumindb.ttf"));
	FontAsset::Register(U"MediumFont", 40, Resource(U"Fonts/yumin.ttf"));
	FontAsset::Register(U"SmallFont", 30, Resource(U"Fonts/yumin.ttf"));

	//シーン登録
	App manager;
	manager.add<LoadGraph>(State::LoadGraph);
	manager.add<Title>(State::Title);
	manager.add<TurnSelect>(State::TurnSelect);
	manager.add<HowToPlay>(State::HowToPlay);
	manager.add<Game>(State::Game);

	//フェードイン・アウト時の色
	manager.setFadeColor(Palette::White);

	//ウィンドウ管理
	Window::Resize(1280, 720);
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetFullscreen(true);
	Window::SetTitle(U"ラムゼーの定理ゲーム");

	//manager.init(State::Title);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}


