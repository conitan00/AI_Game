#pragma once

class STAGE {

public:

	struct {
		int Type[STAGE_MAX_X][STAGE_MAX_Y];
		int Type_Exp[POS_MAX_X][POS_MAX_Y];
		int Item[STAGE_MAX_X][STAGE_MAX_Y] = {0};
	}Blo;

	//Read
	void Read() {
		ifstream file("C:/bomberman/stage.txt");
		string line;
		int y = 0;
		while (getline(file, line)) {
			for (int x = 0; x < line.size(); x++) {
				Blo.Type[x][y] = line[x];
				for (int xe = 0; xe < CELL; xe++) {
					for (int ye = 0; ye < CELL; ye++) {
						Blo.Type_Exp[x * CELL + xe][y * CELL + ye] = line[x];
					}
				}
			}
			y++;
		}
		cout << Blo.Item[2][2];
	}

	//Out
	void Out() {
		for (int x = 0; x < STAGE_MAX_X; x++) {
			for (int y = 0; y < STAGE_MAX_Y; y++) {
				switch (Blo.Type[x][y])
				{
				case '0':
					//DrawGraph(CELL * x, CELL * y, Pic.Midori[0], FALSE);
					//DrawGraph(CELL * x, CELL * y, Pic.Tuti[4], FALSE);
					DrawGraph(CELL * x, CELL * y, Pic.Ren2, FALSE);
					break;
				case '1':
					//DrawGraph(CELL * x, CELL * y, Pic.Bl1, FALSE);
					//DrawGraph(CELL * x, CELL * y, Pic.Midori[0], FALSE);
					//DrawGraph(CELL * x, CELL * y, Pic.Tuti[2], TRUE);
					//DrawGraph(CELL * x, CELL * y, Pic.Rock[0], TRUE);
					DrawGraph(CELL * x, CELL * y, Pic.Ren2, FALSE);
					DrawGraph(CELL * x, CELL * y, Pic.Ren3, TRUE);
					break;
				}
			}
		}
	}
};

STAGE Sta;