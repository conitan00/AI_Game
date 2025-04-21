#pragma once
#include <iostream>
#include <string>
using namespace std;
class {

public:

	static const int numCat = 32;
	
	int Tit;
	int Ren1;
	int Ren2;
	int Ren3;
	int Bom[60];
	int Bl0;
	int Bl1;
	int Bl2;
	int Bl3;
	int Pla1;
	int Ene1;
	int Ene2;
	int Ene3;
	int Cat[numCat][12];
	int Tuti[5];
	int Midori[88];
	int Rock[5];
	int Efe[30];
	int Fire1;
	int Fire2;
	int Kutu;
	int Cur[18];

	void Read() {
		LoadDivGraph("C:/bomberman/picture/bomb.png", 60, 6, 10, 48, 48, Bom);
		LoadDivGraph("C:/bomberman/picture/cur.png", 18, 1, 18, 900, 675, Cur);
		Ren1 = LoadGraph("C:/bomberman/picture/renga1.png");
		Ren2 = LoadGraph("C:/bomberman/picture/renga2.png");
		Ren3 = LoadGraph("C:/bomberman/picture/renga3.png");
		Bl0 = LoadGraph("C:/bomberman/picture/blo0.png");
		Bl1 = LoadGraph("C:/bomberman/picture/blo1.png");
		Bl2 = LoadGraph("C:/hobby/picture/blo3.png");
		Pla1 = LoadGraph("C:/bomberman/picture/pla1.png");
		Ene1 = LoadGraph("C:/bomberman/picture/ene1.png");
		Ene2 = LoadGraph("C:/bomberman/picture/ene2.png");
		Ene3 = LoadGraph("C:/bomberman/picture/ene3.png");
		Tit = LoadGraph("C:/bomberman/picture/tit1.png");
		Fire1 = LoadGraph("C:/bomberman/picture/fire_red.png");
		Fire2 = LoadGraph("C:/bomberman/picture/fire_blue.png");
		Kutu = LoadGraph("C:/bomberman/picture/kutu.png");
		LoadDivGraph("C:/bomberman/picture/tuti.png", 5, 1, 5, 60, 60, Tuti);
		for (int i = 1; i <= numCat; i++)
		{
			string fineName = "C:/bomberman/picture/cat/cat (" + to_string(i)+ ").png";
			LoadDivGraph(fineName.c_str(), 12, 3, 4, 48, 48, Cat[i - 1]);
		}
		LoadDivGraph("C:/bomberman/picture/midori.png", 88, 8, 11, 60, 60, Midori);
		LoadDivGraph("C:/bomberman/picture/rock.png", 5, 1, 5, 60, 60, Rock);
		LoadDivGraph("C:/bomberman/picture/efe1.png", 30, 5, 6, 200, 200, Efe);
	}

}Pic;