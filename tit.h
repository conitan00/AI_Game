#pragma once

class TITLE{

public:

	void Out() {
		DrawGraph(0, -240, Pic.Tit, TRUE);
		DrawFormatStringToHandle(250, 180, Col.Blu, Fon.c[100], "爆弾ネコ");
		DrawFormatStringToHandle(250, 430, Col.Whi, Fon.c[30], "Enterキーを押してください");
		if (Key[KEY_INPUT_NUMPADENTER] == 1) {
			Sce = MEN::MEN_01_Select_Sta;
		}
	}
	
};

TITLE Tit;