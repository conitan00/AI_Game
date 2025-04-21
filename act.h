#pragma once

class ACTION {

public:
	void Out() {
		cnt++;
		Cal();
		cha();
	};
	void init(); // 初期化
	int numPla = -1; // プレイヤー数
	int numEne = -1; // 敵キャラ数
	int numCha = -1;
	void SetPic(int chaID, int picID);
	void Death(int i);
	void End();

private:

	int numDeath = 0;

	int cnt = 0;

	static const int num_bom = 10;

	int Keys[2][4] = { { KEY_INPUT_S, KEY_INPUT_A, KEY_INPUT_D, KEY_INPUT_W  },
		               { KEY_INPUT_J, KEY_INPUT_H, KEY_INPUT_K, KEY_INPUT_U }}; // 各プレイヤーの移動キー
	int bonKeys[2] = { KEY_INPUT_Q, KEY_INPUT_I }; // 各プレイヤーの爆弾設置キー
	int Dirs[4][2] = { {0, 1}, {-1, 0}, {1, 0}, {0, -1} }; // 移動方向（下左右上）
	// キャラクターの初期座標（4キャラ分）
	int initPosCha[4][2] = { {CELL / 2, CELL / 2}, {POS_MAX_X - CELL / 2, POS_MAX_Y - CELL / 2}, {POS_MAX_X - CELL / 2, 0}, {0, POS_MAX_Y - CELL / 2} };

	// 爆弾
	struct BOM {
		bool exist = false;
		float time = 13.0;
		int X, Y, X_cell, Y_cell;
	};

	// キャラクター（プレイヤー＆敵キャラ）
	struct CHA {
		int X, Y, X_cell, Y_cell;
		int Dir = 0;
		int numBom = 1;
		int Mov = 2;
		int fireLength = 1;
		int picID;
		bool death = false;
		int death_cnt = 8;
		int totalMove = 0;
		string PorE;
		BOM boms[num_bom];
	};

	// キャラクターの動的配列
	vector<CHA> Cha;

	// アイテム
	struct ITEM {
		int X_cell, Y_cell;
	};

	// 終了演出
	bool end = false;
	int endCnt = 100;
	int endPlaID;

	

	void Cal();
	void cha();
	void Item();
	// 移動距離
	const int Mov = CELL / 20;

};

ACTION Act;

void ACTION::Cal() {

	// プレイヤー
	for (int i = 0; i < ACTION::numCha; i++) {
		if (Cha[i].death) continue;

		// キャラクターが何セル目に位置するか記録
		Cha[i].X_cell = Cha[i].X / CELL;
		Cha[i].Y_cell = Cha[i].Y / CELL;

		// プレイヤーの移動＆衝突判定
		if (Cha[i].PorE == "Pla") {
			for (int dir = 0; dir < 4; dir++) {
				if (Key[Keys[i][dir]] != 0) {
					Cha[i].Dir = dir;
					Cha[i].X += Cha[i].Mov * Dirs[dir][0];
					Cha[i].Y += Cha[i].Mov * Dirs[dir][1];
					if (0 <= Cha[i].X - CHA_X2 && Cha[i].X + CHA_X2 <= POS_MAX_X &&
						0 <= Cha[i].Y - CHA_Y2_UP && Cha[i].Y + CHA_Y2_DO <= POS_MAX_Y) {
					}
					else {
						Cha[i].X -= Cha[i].Mov * Dirs[dir][0];
						Cha[i].Y -= Cha[i].Mov * Dirs[dir][1];
						break;
					}

					if (Sta.Blo.Type_Exp[Cha[i].X - CHA_X2][Cha[i].Y + CHA_Y2_DO] == '1' ||
						Sta.Blo.Type_Exp[Cha[i].X + CHA_X2][Cha[i].Y + CHA_Y2_DO] == '1' ||
						Sta.Blo.Type_Exp[Cha[i].X - CHA_X2][Cha[i].Y - CHA_Y2_UP] == '1' ||
						Sta.Blo.Type_Exp[Cha[i].X + CHA_X2][Cha[i].Y - CHA_Y2_UP] == '1') {
						Cha[i].X -= Cha[i].Mov * Dirs[dir][0];
						Cha[i].Y -= Cha[i].Mov * Dirs[dir][1];
					}

					break;
				}
			}
		}
		// 敵キャラの移動
		else if (Cha[i].PorE == "Ene") {
			if (Cha[i].totalMove == 0) {
				while (true) {
					random_device rd;
					mt19937 gen(rd());
					uniform_int_distribution<int> dist(0, 3);
					int tempDir = dist(gen);
					if (0 < Cha[i].X + Dirs[tempDir][0] * CELL && Cha[i].X + Dirs[tempDir][0] * CELL < POS_MAX_X &&
						0 < Cha[i].Y + Dirs[tempDir][1] * CELL && Cha[i].Y + Dirs[tempDir][1] * CELL < POS_MAX_Y) {
						Cha[i].Dir = tempDir;
						break;
					}
				}
			}
			Cha[i].X += Dirs[Cha[i].Dir][0] * Mov;
			Cha[i].Y += Dirs[Cha[i].Dir][1] * Mov;
			Cha[i].totalMove += Mov;
			if (Cha[i].totalMove >= 2 * CELL) Cha[i].totalMove = 0;	
		}

		// プレイヤーの爆弾設置
		if (Cha[i].PorE == "Pla") {
			if (Key[bonKeys[i]] == 1) {
				for (int j = 0; j < Cha[i].numBom; j++) {
					if (Cha[i].boms[j].exist == false) {
						Cha[i].boms[j] = { true, 13.0, CELL * Cha[i].X_cell + CELL / 2, CELL * Cha[i].Y_cell + CELL / 2, Cha[i].X_cell , Cha[i].Y_cell };
						break;
					}
				}
			}
		}

		// 敵キャラの爆弾設置
		/*else if (Cha[i].PorE == "Ene") {
			for (int j = 0; j < num_bom; j++) {
				if (Cha[i].boms[j].exist == false) {
					Cha[i].boms[j] = { true, 13.0, CELL * Cha[i].X_cell + CELL / 2, CELL * Cha[i].Y_cell + CELL / 2 , Cha[i].X_cell , Cha[i].Y_cell };
					break;
				}
			}
		}*/
	}
}

void ACTION::Item() {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dist(1, 100000);
	uniform_int_distribution<int> ran(1, 4);
	for (int x = 0; x < STAGE_MAX_X; x++)
	{
		for (int y = 0; y < STAGE_MAX_Y; y++)
		{
			if (Sta.Blo.Type[x][y] == '0' && Sta.Blo.Item[x][y] == 0)
			{
				if (dist(gen) >= 100000) Sta.Blo.Item[x][y] = ran(gen);
			}
		}
	}

	for (int x = 0; x < STAGE_MAX_X; x++)
	{
		for (int y = 0; y < STAGE_MAX_Y; y++)
		{
			if (Sta.Blo.Item[x][y] != 0 && Sta.Blo.Type[x][y] != 1)
			{
				for (int i = 0; i < ACTION::numCha; i++)
				{
					if (Cha[i].death) continue;
					if (x == Cha[i].X_cell && y == Cha[i].Y_cell)
					{
						switch (Sta.Blo.Item[x][y])
						{
						case 1:
							if (Cha[i].numBom < num_bom) Cha[i].numBom++;
							break;
						case 2:
							Cha[i].fireLength += 1;
							break;
						case 3:

							break;
						case 4:
							Cha[i].Mov += 1;
							break;
						}
						Sta.Blo.Item[x][y] = 0;
					}
				}
				DrawRotaGraph(CELL * x + CELL / 2, CELL * y + CELL / 2, 0.35, 0, Pic.Efe[15], TRUE);
				switch (Sta.Blo.Item[x][y])
				{
				case 1:
					DrawRotaGraph(CELL * x + CELL / 2, CELL * y + CELL / 2, 0.70, 0, Pic.Bom[0], TRUE);
					break;
				case 2:
					DrawRotaGraph(CELL * x + CELL / 2, CELL * y + CELL / 2, 0.75, 0, Pic.Fire1, TRUE);
					break;
				case 3:
					DrawRotaGraph(CELL * x + CELL / 2, CELL * y + CELL / 2, 0.75, 0, Pic.Fire2, TRUE);
					break;
				case 4:
					DrawRotaGraph(CELL * x + CELL / 2, CELL * y + CELL / 2, 0.85, 0, Pic.Kutu, TRUE);
					break;
				}

			}
		}
	}

}

void ACTION::cha() {

	Sta.Out();

	Item();
	
	for (int i = 0; i < ACTION::numCha; i++) {
		if (Cha[i].death) continue;
		for (int b = 0; b < num_bom; b++) {
			if (Cha[i].boms[b].exist) {
				if (3.0 < Cha[i].boms[b].time && Cha[i].boms[b].time <= 13.0) {
					if      (Cha[i].boms[b].time >= 11.0) DrawRotaGraph(Cha[i].boms[b].X, Cha[i].boms[b].Y, 1.0, 0, Pic.Bom[1], TRUE);
					else if (Cha[i].boms[b].time >=  9.0) DrawRotaGraph(Cha[i].boms[b].X, Cha[i].boms[b].Y, 1.0, 0, Pic.Bom[2], TRUE);
					else if (Cha[i].boms[b].time >=  7.0) DrawRotaGraph(Cha[i].boms[b].X, Cha[i].boms[b].Y, 1.0, 0, Pic.Bom[1], TRUE);
					else if (Cha[i].boms[b].time >=  5.0) DrawRotaGraph(Cha[i].boms[b].X, Cha[i].boms[b].Y, 1.0, 0, Pic.Bom[3], TRUE);
					else if (Cha[i].boms[b].time >=  3.0) DrawRotaGraph(Cha[i].boms[b].X, Cha[i].boms[b].Y, 1.0, 0, Pic.Bom[4], TRUE);
				}
				else {
					for (int fireLength = 0; fireLength <= Cha[i].fireLength; fireLength++)
					{
						for (int fireDir = 0; fireDir < 4; fireDir++)
						{
							if (Sta.Blo.Type[Cha[i].boms[b].X_cell + Dirs[fireDir][0]][Cha[i].boms[b].Y_cell + Dirs[fireDir][1]] == '0')
							{
								if      (Cha[i].boms[b].time >= 2.5) DrawRotaGraph(Cha[i].boms[b].X + fireLength * Dirs[fireDir][0] * CELL, Cha[i].boms[b].Y + fireLength * Dirs[fireDir][1] * CELL, 1.0, 0, Pic.Bom[30], TRUE);
								else if (Cha[i].boms[b].time >= 2.0) DrawRotaGraph(Cha[i].boms[b].X + fireLength * Dirs[fireDir][0] * CELL, Cha[i].boms[b].Y + fireLength * Dirs[fireDir][1] * CELL, 1.0, 0, Pic.Bom[31], TRUE);
								else if (Cha[i].boms[b].time >= 1.5) DrawRotaGraph(Cha[i].boms[b].X + fireLength * Dirs[fireDir][0] * CELL, Cha[i].boms[b].Y + fireLength * Dirs[fireDir][1] * CELL, 1.0, 0, Pic.Bom[32], TRUE);
								else if (Cha[i].boms[b].time >= 1.0) DrawRotaGraph(Cha[i].boms[b].X + fireLength * Dirs[fireDir][0] * CELL, Cha[i].boms[b].Y + fireLength * Dirs[fireDir][1] * CELL, 1.0, 0, Pic.Bom[33], TRUE);
								else if (Cha[i].boms[b].time >= 0.5) DrawRotaGraph(Cha[i].boms[b].X + fireLength * Dirs[fireDir][0] * CELL, Cha[i].boms[b].Y + fireLength * Dirs[fireDir][1] * CELL, 1.0, 0, Pic.Bom[34], TRUE);
								else if (Cha[i].boms[b].time >= 0.0) DrawRotaGraph(Cha[i].boms[b].X + fireLength * Dirs[fireDir][0] * CELL, Cha[i].boms[b].Y + fireLength * Dirs[fireDir][1] * CELL, 1.0, 0, Pic.Bom[35], TRUE);

								for (int k = 0; k < ACTION::numCha; k++) {
									if (Cha[i].death) continue;
									if (Cha[i].boms[b].X_cell + fireLength * Dirs[fireDir][0] == Cha[k].X_cell && Cha[i].boms[b].Y_cell + fireLength * Dirs[fireDir][1] == Cha[k].Y_cell)
									{
										if (end == false) {
											if (!Cha[k].death) {
												Cha[k].death = true;
												numDeath++;
											}
											if (numDeath == numCha - 1) {
												end = true;
											}

											break;
										}
										else {
											break;
										}
									}
								}
							}
						}
					}
				}
				Cha[i].boms[b].time -= 0.1;
				if (Cha[i].boms[b].time < 0) {
					Cha[i].boms[b].exist = false;
				}
			}
		}
	}

	for (int i = 0; i < ACTION::numCha; i++) {
		int p = 1;
		for (int k = 0; k < 4; k++) {
			if (Key[Keys[i][k]] != 0) p = (cnt / 12) % 3;
		}
		if (Cha[i].PorE == "Ene") p = (cnt / 12) % 3;
		if (!Cha[i].death) {
			DrawRotaGraph(Cha[i].X, Cha[i].Y, 1.0, 0, Pic.Cat[Cha[i].picID][Cha[i].Dir * 3 + p], TRUE);
		}
	}

	for (int i = 0; i < ACTION::numCha; i++) {
		if (Cha[i].death) Death(i);
	}

	if (end) End();
	
}


// 初期化
void ACTION::init() {

	numCha = numPla + numEne;

	for (int i = 0; i < numPla + numEne; i++)
	{
		if (i < numPla) Cha[i].PorE = "Pla";
		else Cha[i].PorE = "Ene";
		Cha[i].X = initPosCha[i][0];
		Cha[i].Y = initPosCha[i][1];
		Cha[i].Dir = 0;
		Cha[i].numBom = 1;
		Cha[i].Mov = 2;
		Cha[i].fireLength = 1;

		for (int b = 0; b < num_bom; b++)
		{
			Cha[i].boms[b].exist = false;
		}
	}

    
	// アイテム初期化
	for (int x = 0; x < STAGE_MAX_X; x++)
	{
		for (int y = 0; y < STAGE_MAX_Y; y++)
		{
			Sta.Blo.Item[x][y] = 0;
		}
	}
}

void ACTION::SetPic(int chaID, int picID) {

	Act.Cha.push_back(CHA());
	Cha[chaID].picID = picID;

}

void ACTION::Death(int i) {

	Cha[i].death_cnt--;
	if (Cha[i].death_cnt >= 0) {
		DrawFormatStringToHandle(Cha[i].X, Cha[i].Y, Col.Blu, Fon.c[30], "Lose");
	}

}

void ACTION::End() {

	endCnt--;

	for (int i = 0; i < numCha; i++) {
		if (!Cha[i].death) DrawFormatStringToHandle(Cha[i].X, Cha[i].Y, Col.Red, Fon.c[30], "Win");
	}
	
	if (endCnt <= 0) {
		Sce = MEN::MEN_01_Select_Sta;
		init();
		end = false;
	}

}