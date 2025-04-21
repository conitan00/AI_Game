#pragma once

// Color
class {

public:

	int Whi;
	int Bla;
	int Gre;
	int Red;
	int Blu;

	void Read() {
		Whi = GetColor(255, 255, 255);
		Bla = GetColor(0, 0, 0);
		Gre = GetColor(0, 255, 0);
		Red = GetColor(255, 0, 0);
		Blu = GetColor(0, 0, 255);
	}

}Col;

// Chara Font

class {

public:

	int c[100 + 1];

	void Read() {
		for (int i = 0; i < 100 + 1; i++) {
			c[i] = CreateFontToHandle("MS ƒSƒVƒbƒN", i, 6, DX_FONTTYPE_NORMAL);
		}
	}

}Fon;

// Key
int Key[256];

int GetKey() {
	char allkey[256];
	GetHitKeyStateAll(allkey);
	for (int i = 0; i < 256; i++) {
		if (allkey[i] == 1) {
			Key[i]++;
		}
		else if (allkey[i] == 0) {
			Key[i] = 0;
		}
	}
	return 0;
}