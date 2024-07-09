#pragma once


void UpdateTitle(GAME_STATE& _state);
void DrawTitle();
void UpdatePlay(GAME_STATE& _state);
void DrawPlay();
void UpdateClear(GAME_STATE& _state);
void DrawClear();

//mainからは呼ばず、gamesequenceの方だけで呼ぶ
void DrawStage(Map& _map);
//コンソール版から引数変更
direction GetDirection();

OBJNAME GetObjectNum(Point _pos, Map& _map);
void SetObjectMap(OBJNAME _obj, Point _pos, Map& _map);
Point GetPlayerPos(Map& _map);
void MoveObject(direction _dir, Map& _map);
bool CheckClear(Map& _map);
void InitCrrentMap(Map& _map,int _stageNum);
void DrawStaticObject(Map& _map);
void DrawStageSmoothMove(Map& _map);
void UpdateCharPosSmoothMove(Map& _map);
void DrawMovableObject(Map& _map);
void ResetFromDat(Map& _map);
void SetMoveObject(direction _dir, Point _pos, Map& _map);
