#pragma once

class SELECT_CHA {

public:

	int size = 90; //表示するキャラのサイズ
	int le = 85; // キャラ一覧の左端
	int up = 260; // キャラ一覧の上端
	int CursorX, CursorY; // カーソル位置
	int width = 8; // キャラ一覧の列数
	int height = 4; // キャラ一覧の行数
	int plaID = 0; // キャラID

	float flipTime = FLIP_SECOND;
	bool flip = true;

	void Out() {

		flipTime--;

		if (flipTime == 0)flip = false;

		if (flip) {

			DrawFormatStringToHandle(270, 30, Col.Whi, Fon.c[50], "Select a mode.");
			DrawBox(237, 208, 633, 291, Col.Whi, FALSE);
			DrawFormatStringToHandle(350, 230, Col.Whi, Fon.c[38], "VS CPU");

			DrawBox(POS_MAX_X * (flipTime / FLIP_SECOND), 0, POS_MAX_X * (flipTime / FLIP_SECOND) + POS_MAX_X, POS_MAX_Y, Col.Bla, TRUE);
			DrawFormatStringToHandle(270 + POS_MAX_X * (flipTime / FLIP_SECOND), 50, Col.Whi, Fon.c[50], "Select a cat.");
			string name = to_string(plaID + 1) + "P";
			DrawFormatStringToHandle(400 + POS_MAX_X * (flipTime / FLIP_SECOND), 150, Col.Whi, Fon.c[35], name.c_str());
			int i = 0;
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 8; x++)
				{
					DrawBox(le + x * size + POS_MAX_X * (flipTime / FLIP_SECOND), up + y * size, le + x * size + size + POS_MAX_X * (flipTime / FLIP_SECOND), up + y * size + size, Col.Whi, FALSE);

					DrawRotaGraph(le + x * size + size / 2 + POS_MAX_X * (flipTime / FLIP_SECOND), up + y * size + size / 2, size / CHA_Y, 0.0, Pic.Cat[i][1], TRUE);
					i++;
				}
			}

		}

		else {
			DrawFormatStringToHandle(270, 50, Col.Whi, Fon.c[50], "Select a cat.");
			string name = to_string(plaID + 1) + "P";
			DrawFormatStringToHandle(400, 150, Col.Whi, Fon.c[35], name.c_str());

			int Button, ClickX, ClickY, LogType;
			if (GetMouseInputLog2(&Button, &ClickX, &ClickY, &LogType, TRUE) == 0) {
				if ((Button & MOUSE_INPUT_LEFT) != 0) {
					if (LogType == MOUSE_INPUT_LOG_DOWN && le <= ClickX && ClickX <= le + size * width && up <= ClickY && ClickY <= up + size * height) {
						int x = int((ClickX - le) / size);
						int y = int((ClickY - up) / size);
						Act.SetPic(plaID, y * width + x);
						plaID += 1;
						if (plaID >= Act.numCha)
						{
							plaID = 0;
							Act.init();
							Sce = MEN::MEN_03_Action;
						}
					}
				}
			}

			GetMousePoint(&CursorX, &CursorY);
			if (CursorX >= le) CursorX = int((CursorX - le) / size);
			else CursorX = -1;
			if (CursorY >= up) CursorY = int((CursorY - up) / size);
			else CursorY = -1;

			int i = 0;
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 8; x++)
				{
					if (x == CursorX && y == CursorY) DrawBox(le + x * size, up + y * size, le + x * size + size, up + y * size + size, Col.Whi, TRUE);
					else DrawBox(le + x * size, up + y * size, le + x * size + size, up + y * size + size, Col.Whi, FALSE);

					DrawRotaGraph(le + x * size + size / 2, up + y * size + size / 2, size / CHA_Y, 0.0, Pic.Cat[i][1], TRUE);
					i++;
				}
			}
		}
		
	}
};

SELECT_CHA Select_Cha;