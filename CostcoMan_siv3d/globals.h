#pragma once
const int MAX_STAGE_WIDTH = 10;
const int MAX_STAGE_HEIGHT = 10;
const int CHR_SIZE = 64;

enum GAME_STATE
{
	TITLE,PLAY,CLEAR
};

enum direction
{
	UP, LEFT, DOWN, RIGHT, NONE
};
struct Map
{
	int stage_width;//ステージの最大幅
	int stage_height;//ステージの最大高さ
	int Dat[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];
	//ここから移動レート
	double moveRatio;
	direction From[MAX_STAGE_HEIGHT][MAX_STAGE_WIDTH];//そのオブジェクトはどこから来たか
};
// 0　通路
// 1　壁
// 2　荷物
// 3　目的地(何も乗ってない状態)
// 4　人
// 5　人 on the 目的地
// 6　荷物 on the　目的地
//オブジェクト名を便利に使いたいためのenum
enum OBJNAME
{
	FLOOR,WALL,LUGG,GOAL,HUMAN,HUMAN_ON_GOAL,LUGG_ON_GOAL
};
// const string OBJS[] = { " ","#","$",".","@","+","*"," " };グラフィック使うので不要
