# include <Siv3D.hpp> // OpenSiv3D v0.6.10
#include"globals.h"
#include"gameSequence.h"

GAME_STATE gState = GAME_STATE::TITLE;

void Main()
{
	FontAsset::Register(U"font", FontMethod::MSDF, 30, Typeface::Regular);//自由に拡大縮小できるフォント

	TextureAsset::Register(U"PLAYER", U"human.png");
	TextureAsset::Register(U"FLOOR", U"floor.png");
	TextureAsset::Register(U"LUGG", U"LUGG.png");
	TextureAsset::Register(U"WALL", U"brick.png");
	TextureAsset::Register(U"GOAL", U"GOAL.png");
	TextureAsset::Register(U"Title", U"CostcoMan3.png");

	AudioAsset::Register(U"TITLE_SND", U"BGM_title.mp3");
	AudioAsset::Register(U"STAGE_SND", U"BGM_Play.mp3");
	AudioAsset::Register(U"CLEAR_SND", U"BGM_Clear.wav");

	while (System::Update())
	{
		switch (gState)
		{
		case GAME_STATE::TITLE:
			UpdateTitle(gState);
			DrawTitle();
			break;
		case GAME_STATE::PLAY:
			UpdatePlay(gState);
			DrawPlay();
			break;
		case GAME_STATE::CLEAR:
			UpdateClear(gState);
			DrawClear();
			break;
		default:
			break;
		}
	}
}
