#pragma once

class TITLE{

public:

	void Out() {
		DrawGraph(0, -240, Pic.Tit, TRUE);
		DrawFormatStringToHandle(250, 180, Col.Blu, Fon.c[100], "���e�l�R");
		DrawFormatStringToHandle(250, 430, Col.Whi, Fon.c[30], "Enter�L�[�������Ă�������");
		if (Key[KEY_INPUT_NUMPADENTER] == 1) {
			Sce = MEN::MEN_01_Select_Sta;
		}
	}
	
};

TITLE Tit;