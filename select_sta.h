#pragma once

class SELECT_STA {

public:

	float flipTime = FLIP_SECOND;
	bool flip = true;

	void Out() {

		flipTime--;

		if (flipTime == 0)flip = false;

		if (flip) {
			DrawGraph(0, -240, Pic.Tit, TRUE);
			DrawFormatStringToHandle(250, 180, Col.Blu, Fon.c[100], "爆弾ネコ");
			DrawFormatStringToHandle(250, 430, Col.Whi, Fon.c[30], "Enterキーを押してください");

			DrawBox(POS_MAX_X * (flipTime / FLIP_SECOND), 0, POS_MAX_X * (flipTime / FLIP_SECOND) + POS_MAX_X, POS_MAX_Y, Col.Bla, TRUE);
			DrawFormatStringToHandle(270 + POS_MAX_X * (flipTime / FLIP_SECOND), 30, Col.Whi, Fon.c[50], "Select a mode.");
			DrawBox(237 + POS_MAX_X * (flipTime / FLIP_SECOND), 208, 633 + POS_MAX_X * (flipTime / FLIP_SECOND), 291, Col.Whi, FALSE);
			DrawFormatStringToHandle(350 + POS_MAX_X * (flipTime / FLIP_SECOND), 230, Col.Whi, Fon.c[38], "VS CPU");
		}

		else {
			DrawFormatStringToHandle(220, 80, Col.Whi, Fon.c[38], "モードを選んでください");
		

			DrawBox(237, 188, 633, 271, Col.Whi, FALSE);
			DrawFormatStringToHandle(350, 210, Col.Whi, Fon.c[38], "対人モード");

			DrawBox(237, 318, 633, 401, Col.Whi, FALSE);
			DrawFormatStringToHandle(330, 340, Col.Whi, Fon.c[38], "AI学習モード");

			DrawBox(237, 448, 633, 531, Col.Whi, FALSE);
			DrawFormatStringToHandle(330, 470, Col.Whi, Fon.c[38], "AI対戦モード");
		}

		int Button, ClickX, ClickY, LogType;
		if (GetMouseInputLog2(&Button, &ClickX, &ClickY, &LogType, TRUE) == 0) {
			if ((Button & MOUSE_INPUT_LEFT) != 0) {
				if (LogType == MOUSE_INPUT_LOG_DOWN && 237 <= ClickX && ClickX <= 633 && 188 <= ClickY && ClickY <= 271) {
					Act.numPla = 1;
					Act.numEne = 2;
					Act.numCha = Act.numPla + Act.numEne;
					Sce = MEN::MEN_02_Select_Cha;
				}
				if (LogType == MOUSE_INPUT_LOG_DOWN && 237 <= ClickX && ClickX <= 633 && 318 <= ClickY && ClickY <= 401) {
					model = false;
					Act_AI.init();
					Sce = MEN::MEN_04_AI;
				}
				if (LogType == MOUSE_INPUT_LOG_DOWN && 237 <= ClickX && ClickX <= 633 && 448 <= ClickY && ClickY <= 531) {
					model = true;
					Act_AI.init();
					Sce = MEN::MEN_04_AI;
				}
			}
		}
	}

	int numPla = -1;
	int numEne = -1;

private:

};

SELECT_STA Select_Sta;