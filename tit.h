#pragma once

class TITLE{

public:

	void Out() {
		DrawGraph(0, -240, Pic.Tit, TRUE);
		DrawFormatStringToHandle(250, 180, Col.Bla, Fon.c[50], "���e�l�R");
		DrawFormatStringToHandle(250, 550, Col.Bla, Fon.c[30], "Enter�L�[�������Ă�������");
		if (Key[KEY_INPUT_NUMPADENTER] == 1) {
			Sce = MEN::MEN_01_Select_Sta;
		}
	}
	
};

TITLE Tit;