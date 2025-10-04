# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene{ init }
{
	//頂点の設置
	//頂点の色の初期化
	for (int i = 0; i < vertexSize; i++) {
		double angle = ((double)i / (double)vertexSize * Math::TwoPi) + Math::HalfPi;
		vertexPositions << Vec2{ graphCenter.x + (graphRadius * Math::Cos(angle)), graphCenter.y - (graphRadius * Math::Sin(angle))};
		vertexColor << 0;
	}

	//辺の設置
	for (int i = 0; i < vertexSize; i++) {
		edges << Array<Quad>{};

		for (int j = 0; j < i + 1; j++) {
			edges[i] << Quad{ {-100,-100},{-100,-100},{-100,-100},{-100,-100} };
		}
		for (int j = i + 1; j < vertexSize; j++) {
			int32 id = Min(j - i, 6 - (j - i)) - 1;
			edges[i] << RectF{ vertexPositions[i].movedBy(-edgeWidth / 2.0, 0), edgeWidth, edgeLength[id] }.rotatedAt(vertexPositions[i], (3 - i - j) * 30_deg);
		}
	}

	//辺の色の初期化
	for (int i = 0; i < vertexSize; i++) {
		edgeColor << Array<int32>{};

		for (int j = 0; j < i + 1; j++) {
			edgeColor[i] << -1;
		}
		for (int j = i + 1; j < vertexSize; j++) {
			edgeColor[i] << 0;
			restEdges << PII{ i,j };
		}
	}
	
}

void Game::update()
{
	//リトライ＆タイトル
	if (isWinnerDisplayFinished) {
		isAnythingMouseOver = retryButton.mouseOver() || titleButton.mouseOver();
		retryTransition.update(retryButton.mouseOver());
		titleTransition.update(titleButton.mouseOver());

		if (retryButton.leftClicked()) {
			changeScene(State::Game, 0.3s);
		}
		if (titleButton.leftClicked()) {
			changeScene(State::Title, 0.3s);
		}
	}

	//頂点の色の更新

	for (int i = 0; i < vertexSize; i++) {
		for (int j = i + 1; j < vertexSize; j++) {
			for (int k = j + 1; k < vertexSize; k++) {
				if (edgeColor[i][j] == 4 && edgeColor[j][k] == 4 && edgeColor[i][k] == 4) {
					vertexColor[i] = 1;
					vertexColor[j] = 1;
					vertexColor[k] = 1;
					isFinished = true;
				}
				if (edgeColor[i][j] == 5 && edgeColor[j][k] == 5 && edgeColor[i][k] == 5) {
					vertexColor[i] = 2;
					vertexColor[j] = 2;
					vertexColor[k] = 2;
					isFinished = true;
				}
			}
		}
	}

	//ゲーム終了時の処理
	if (isFinished) {
		if (winnerDisplayTime > 0.0) {
			winnerDisplayTransition.update(true);
			instruction = U"";
		}
		else { //winnerDisplayTime <= 0.0
			winnerDisplayTransition.update(false);
			instruction = U"";
			isWinnerDisplayFinished = true;
		}

		if (winnerDisplayTransition.value() == 1.0) {
			winnerDisplayTime -= Scene::DeltaTime();
			if (winnerDisplayCover.leftClicked()) {
				winnerDisplayTime = -1.0;
			}
		}

		CannotSelect();
		return;
	}

	//一人プレイ用
	if (getData().player == 1 && turnPlayer != 0 && not isFinished) {
		CannotSelect();

		if (not isComSelecting) {
			if (getData().optimal) {
				OptimalSelectEdge();
			}
			else {
				RandomSelectEdge();
			}
			isComSelecting = true;
		}
		else if (comThinkingTime > 0.0) {
			comThinkingTime -= Scene::DeltaTime();
		}
		else {
			if (getData().optimal) {
				OptimalPaintEdge(OptimalSelectColor);
			}
			else {
				RandomPaintEdge();
			}
			isComSelecting = false;
		}
		
		return;
	}

	
	isAnythingMouseOver = false;

	//操作説明
	if (selectedEdge == PII{ 0,0 }) {
		instruction = U"辺を選択してください";
	}
	else if (!isFinished) {
		instruction = U"色を選択してください";
	}

	//辺の色の更新
	//マウスオーバーによる色の更新
	for (int i = 0; i < vertexSize; i++) {
		for (int j = i + 1; j < vertexSize; j++) {
			if (edgeColor[i][j] >= 4) { continue; }

			if (edges[i][j].leftClicked() && selectedEdge != PII{i,j}) {
				selectedEdge = PII{ i,j };
			}
			else if (edges[i][j].leftClicked() && selectedEdge == PII{ i,j }) {
				selectedEdge = PII{ 0,0 };
			}
			else if (edges[i][j].mouseOver() && selectedEdge == PII{ i,j }) {
				edgeColor[i][j] = 3;
				isAnythingMouseOver = true;
			}
			else if (edges[i][j].mouseOver() && selectedEdge != PII{ i,j }) {
				edgeColor[i][j] = 1;
				isAnythingMouseOver = true;
			}
			else {
				edgeColor[i][j] = 0;
			}
		}
	}

	if (not edges[selectedEdge.first][selectedEdge.second].mouseOver()) {
		edgeColor[selectedEdge.first][selectedEdge.second] = 2;
	}

	//赤ボタンと青ボタンの色
	redTransition.update(redButton.mouseOver());
	blueTransition.update(blueButton.mouseOver());
	if (not isWinnerDisplayFinished) {
		isAnythingMouseOver = isAnythingMouseOver || redButton.mouseOver() || blueButton.mouseOver();
	}
	/*
		押したときに暗くしたいなあ…
	*/

	//ボタンを押して辺の色を更新
	if (redButton.leftClicked()) {
		if (selectedEdge != PII{ 0,0 }) {
			state = getData().inv_state[getData().conv_state[state] + pow3[EtoS[selectedEdge.first][selectedEdge.second]]];
			edgeColor[selectedEdge.first][selectedEdge.second] = 4;
			selectedEdge = PII{ 0,0 };
			turnPlayer = (turnPlayer + 1) % 2;
		}
	}
	if (blueButton.leftClicked()) {
		if (selectedEdge != PII{ 0,0 }) {
			state = getData().inv_state[getData().conv_state[state] + pow3[EtoS[selectedEdge.first][selectedEdge.second]] * 2];
			edgeColor[selectedEdge.first][selectedEdge.second] = 5;
			selectedEdge = PII{ 0,0 };
			turnPlayer = (turnPlayer + 1) % 2;
		}
	}
	
	
	
	if (isAnythingMouseOver) {
		Cursor::RequestStyle(CursorStyle::Hand);
	}
	else {
		Cursor::RequestStyle(CursorStyle::Arrow);
	}

	//デバッグ用
	
	ClearPrint();
	/*
	
	for (int32 x : getData().edge[state]) {
		state_s.clear();
		for (int32 i = 0; i < 15; i++) {
			state_s += Format(ternary(getData().conv_state[x], i));
		}
		Print << U"next : " << state_s;
	}

	Print << U"Grundy : " << getData().grundy[state];

	state_s.clear();
	for (int32 i = 0; i < 15; i++) {
		state_s += Format(ternary(getData().conv_state[state], i));
	}
	Print << state_s;

	Print << U"OptimalColor : " << OptimalSelectColor;
	*/
}

void Game::draw() const
{
	//辺の色を更新
	for (int i = 0; i < vertexSize; i++) {
		for (int j = i + 1; j < vertexSize; j++) {
			edges[i][j].draw(edgePalette[edgeColor[i][j]]);
		}
	}

	

	//頂点の色を更新
	for (int i = 0; i < vertexSize; i++) {
		Circle(vertexPositions[i], vertexRadius + vertexFrame).draw(Palette::Black);
		Circle(vertexPositions[i], vertexRadius).movedBy(0, vertexFrame * (-0.6)).draw(vertexPalette[vertexColor[i]]);

		//頂点が色付きなら
		if (vertexColor[i] > 0) {
			FontAsset(U"LargeFont")(i + 1).drawAt(vertexPositions[i], Palette::White);
		}
		//頂点が白いなら
		else {
			FontAsset(U"LargeFont")(i + 1).drawAt(vertexPositions[i], Palette::Black);
		}
		
	}

	if (not isWinnerDisplayFinished) {
		//ボタンの描画
		redButton.draw(ColorF{ 1.0, 0, 0, 1.0 - redTransition.value() * 0.5 }).drawFrame(2, Palette::Black);
		blueButton.draw(ColorF{ 0, 0, 1.0, 1.0 - blueTransition.value() * 0.5 }).drawFrame(2, Palette::Black);

		//ボタンの文字の描画
		FontAsset(U"LargeFont")(U"RED").drawAt(redButton.center(), Palette::White);
		FontAsset(U"LargeFont")(U"BLUE").drawAt(blueButton.center(), Palette::White);
	}
	else {
		//ボタンの描画
		retryButton.draw(ColorF{ 0.7, retryTransition.value() }).drawFrame(2, Palette::Black);
		titleButton.draw(ColorF{ 0.7, titleTransition.value() }).drawFrame(2, Palette::Black);

		//ボタンの文字の描画
		FontAsset(U"MediumFont")(U"RETRY").drawAt(retryButton.center(), Palette::Black);
		FontAsset(U"MediumFont")(U"TITLE").drawAt(titleButton.center(), Palette::Black);
	}
	

	//ターンプレイヤーと説明文の描画
	if (not isWinnerDisplayFinished) {
		FontAsset(U"MediumFont")(U"NOW").drawAt(turnTextCenter.movedBy(-160, -80), Palette::Black);
	}
	else {
		FontAsset(U"MediumFont")(U"WINNER").drawAt(turnTextCenter.movedBy(-160, -80), Palette::Black);
	}
	
	FontAsset(U"LargeFont")(participants[getData().player][turnPlayer]).drawAt(turnTextCenter, Palette::Black);

	//操作説明
	if (getData().player == 2 || turnPlayer == 0 || isFinished) {
		FontAsset(U"MediumFont")(instruction).drawAt(turnTextCenter.movedBy(0, 120), Palette::Black);
	}

	//ゲーム終了時の表示
	if (isFinished && winnerDisplayTransition.value() > 0) {
		winnerDisplayCover.draw(ColorF{ 0.0, (0.8 * winnerDisplayTransition.value()) });
		FontAsset(U"MediumFont")(winner).drawAt(Scene::Center().movedBy(0, -100), ColorF{1.0, winnerDisplayTransition.value()});
		FontAsset(U"LLFont")(participants[getData().player][turnPlayer]).drawAt(Scene::Center(), ColorF{1.0, winnerDisplayTransition.value()});
	}
}

