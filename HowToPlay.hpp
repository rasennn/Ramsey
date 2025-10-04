#pragma once
#include "Common.hpp"

class HowToPlay : public App::Scene
{
public:

	HowToPlay(const InitData& init);

	void update() override;

	void draw() const override;

private:
	//現在のページ数(0-indexed)
	int32 currentPage = 0;
	//ページの総数
	const int32 maxPage = 7;

	//説明の画像
	Array<Texture> instImages;
	Array<String> imagePaths;

	//説明のテキスト
	Array<String> instTexts;
	Array<String> textPaths;

	//説明画像の中心
	const Point instImageCenter{ Scene::Center().x, 300 };

	//ページ数表示の中心
	const Vec2 pageNumberCenter = Scene::Center().movedBy(0, 270);
	//ページ移動の矢印
	Triangle toPrevPage{ pageNumberCenter.x - 150, pageNumberCenter.y, 60, 30_deg };
	Triangle toNextPage{ pageNumberCenter.x + 150, pageNumberCenter.y, 60, -30_deg };
	//ページ移動の矢印(色管理)
	Color toPrevPageColor = Palette::Darkgreen;
	Color toNextPageColor = Palette::Darkgreen;

	//タイトルボタン
	RectF titleButton{ pageNumberCenter.x + 300, pageNumberCenter.y - 30 , 300, 60 };
	Transition titleTransition{ 0.2s, 0.2s };
};
