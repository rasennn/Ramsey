# include "HowToPlay.hpp"

HowToPlay::HowToPlay(const InitData& init)
	: IScene{ init }
{
	String baseImagePath = U"image/howtoplay/instruction";
	String baseTextPath = U"text/howtoplay/instruction";
	for (int i = 0; i < maxPage; i++) {
		//画像ファイル読み込み
		imagePaths << baseImagePath + Format(i + 1) + U".jpg";
		instImages << Texture{ Resource(imagePaths[i]) };

		//テキストファイル読み込み
		textPaths << baseTextPath + Format(i + 1) + U".txt";
		TextReader reader{ Resource(textPaths[i]) };

		// オープンに失敗
		if (not reader)
		{
			// 例外を投げて終了
			throw Error{ U"Failed to open " + baseTextPath + Format(i + 1) + U".txt" };
		}

		instTexts << reader.readAll();
	}

}

void HowToPlay::update()
{
	//ページ移動の矢印(色変化)
	if (toPrevPage.leftPressed())		{ toPrevPageColor = Palette::Darkgreen; }
	else if (toPrevPage.mouseOver())	{ toPrevPageColor = Palette::Lightgreen; }
	else								{ toPrevPageColor = Palette::Green; }

	if (toNextPage.leftPressed())		{ toNextPageColor = Palette::Darkgreen; }
	else if (toNextPage.mouseOver())	{ toNextPageColor = Palette::Lightgreen; }
	else								{ toNextPageColor = Palette::Green; }

	//カーソルの形
	if ((currentPage>0 && toPrevPage.mouseOver()) || (currentPage<maxPage-1 && toNextPage.mouseOver()) || titleButton.mouseOver()) {
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	//ページ移動
	if (toPrevPage.leftClicked()) {
		currentPage = Max(0, currentPage - 1);
	}
	if (toNextPage.leftClicked()) {
		currentPage = Min(maxPage - 1, currentPage + 1);
	}

	//タイトルボタンの処理
	if (titleButton.mouseOver()) {
		Cursor::RequestStyle(CursorStyle::Hand);
	}
	titleTransition.update(titleButton.mouseOver());

	if (titleButton.leftClicked()) {
		changeScene(State::Title, 0.3s);
	}
	
}

void HowToPlay::draw() const
{
	//画像の表示
	instImages[currentPage].drawAt(instImageCenter);

	//ページ移動の矢印の表示
	if (currentPage != 0) {
		toPrevPage.draw(toPrevPageColor);
	}
	if (currentPage != maxPage - 1) {
		toNextPage.draw(toNextPageColor);
	}

	//説明テキスト
	FontAsset(U"SmallFont")(instTexts[currentPage]).draw(730, 70, Palette::Black);

	//ページ数
	FontAsset(U"MediumFont")(Format(currentPage + 1) + U" / " + Format(maxPage)).drawAt(pageNumberCenter, Palette::Black);

	//タイトルボタン
	titleButton.draw(ColorF{ 0.7 , titleTransition.value() }).drawFrame(2, Palette::Black);
	FontAsset(U"MediumFont")(U"TITLE").drawAt(titleButton.center(), Palette::Black);
	
}
