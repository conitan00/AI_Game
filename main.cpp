#include "DxLib.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <windows.h>
#include <random>
#include <deque>
#include <memory>
#include <algorithm>
//#include <opencv2/opencv.hpp>
#include <thread>
#include <Windows.h>
#include <numeric>//

using namespace std;




// ok bokujou

// saikyou


using Vector1D = vector<long double>;
using Vector2D = vector<Vector1D>;
using Vector3D = vector<Vector2D>;
using Vector4D = vector<Vector3D>;

#define WIN_MAX_X 900
#define WIN_MAX_Y 660

#define POS_MAX_X WIN_MAX_X
#define POS_MAX_Y WIN_MAX_Y
#define CELL 60
#define CHA_X 30
#define CHA_Y 48
#define CHA_X2 CHA_X / 2
#define CHA_Y2_UP 14
#define CHA_Y2_DO 21
#define STAGE_MAX_X WIN_MAX_X / CELL
#define STAGE_MAX_Y WIN_MAX_Y / CELL
#define MAR 10
#define FLIP_SECOND 20
#define MODEL_USE true

enum MEN
{
    MEN_00_Title,
    MEN_01_Select_Sta,
    MEN_02_Select_Cha,
    MEN_03_Action,
    MEN_04_AI,
};
int Sce = MEN::MEN_00_Title;

#include "activation.h"
#include "loss.h"
#include "FullyConnectedLayer.h"
#include "ConvolutionLayer.h"
#include "FlattenLayer.h"
#include "ReplayBuffer.h"
#include "DQN.h"
#include "CNN.h"
#include "DQNAgent.h"
#include "DQNEnv.h"

#include "sub.h"
#include "pic.h"
#include "tit.h"
#include "sta.h"
#include "act.h"
#include "act_AI.h"
#include "select_sta.h"
#include "select_cha.h"

void inputThread(std::atomic<bool>& clicked) {
    std::string input;
    while (true) {
        getline(cin, input);
        if (input == "c") {
            clicked = true;
        }
    }
}

void processingThread(std::atomic<bool>& clicked, bool* out) {
    while (true) {
        if (clicked) {

            *out = true;

            clicked = false;
        }
    }
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    SetWaitVSyncFlag(FALSE);  // FPS�����𖳌����I

    ChangeWindowMode(TRUE); // �E�B���h�E���[�h�ɐݒ�
    SetAlwaysRunFlag(TRUE);        // ��A�N�e�B�u�ł����삷��悤�ɐݒ�
    SetOutApplicationLogValidFlag(TRUE);
    SetWindowPosition(0, 0); //�E�B���h�E�̈ʒu�i����j
    if (DxLib_Init() == -1) {
        MessageBox(NULL, "DxLib_Init���s", "�G���[", MB_OK);
        return -1;
    }

    SetMouseDispFlag(TRUE); // �J�[�\���\��

    SetWindowText("ConioGame"); //�E�B���h�E�̖��O
    SetGraphMode(WIN_MAX_X, WIN_MAX_Y, 32); //�E�B���h�E�̃T�C�Y, �F(bit)
    SetBackgroundColor(0, 0, 0); //�w�i�̐F
    SetDrawScreen(DX_SCREEN_BACK);

    // open console
    AllocConsole();
    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);

    //Read
    Col.Read();
    Fon.Read();
    Pic.Read();
    Sta.Read();

    Act_AI.init();

    while (ScreenFlip() == 0 &&
        ClearDrawScreen() == 0 &&
        ProcessMessage() == 0 &&
        GetKey() == 0 &&
        Key[KEY_INPUT_ESCAPE] == 0)
    {
        switch (Sce)
        {
        case MEN::MEN_00_Title:
            Tit.Out();
            break;
        case MEN::MEN_01_Select_Sta:
            Select_Sta.Out();
            break;
        case MEN::MEN_02_Select_Cha:
            Select_Cha.Out();
            break;
        case MEN::MEN_03_Action:
            Act.Out();
            break;
        case MEN::MEN_04_AI:
            Act_AI.Out();
            break;
        }
    }

    WaitKey();      // �L�[���͑҂�

    DxLib_End();    // DX���C�u�����I������
    return 0;
}