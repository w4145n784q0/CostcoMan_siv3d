#include"globals.h"
#include"gameSequence.h"
#include "stages.h"

double CDTimer = 6.0;//カウントダウンタイマー
bool TimerOn = false;
bool isMoving = false;//moveフラグ
double speed = 0.3;
bool allclear = false;
Map crrMap;//コピー用のグローバル変数


void UpdateTitle(GAME_STATE& _state)
{
	if (!AudioAsset(U"TITLE_SND").isPlaying())
	{
		AudioAsset(U"TITLE_SND").setVolume(0.5);
		AudioAsset(U"TITLE_SND").setLoop(true);
		AudioAsset(U"TITLE_SND").play();
	}
	if (KeySpace.down())
	{
		InitCrrentMap(crrMap,crrStageNumber);//ゲーム初期化　crrStageNumber＝0
		_state = GAME_STATE::PLAY;
		AudioAsset(U"TITLE_SND").stop();
	}
		
}
void DrawTitle()
{
	Scene::SetBackground(Palette::White);
	TextureAsset(U"Title").drawAt(Scene::Center());
	FontAsset(U"font")(U"Push KeySpace!").drawAt(400,500);
	FontAsset(U"font")(U"TITLE_SCENE").drawAt(Scene::Center());
	
}
void UpdatePlay(GAME_STATE& _state)
{
	if (!AudioAsset(U"STAGE_SND").isPlaying())
	{
		AudioAsset(U"STAGE_SND").setVolume(0.5);
		AudioAsset(U"STAGE_SND").setLoop(true);
		AudioAsset(U"STAGE_SND").play();
	}

	if (KeyEnter.down())//エンターキー押したら面リセット
	{
		InitCrrentMap(crrMap, crrStageNumber);
	}

	if (CheckClear(crrMap))// (CheckClear(crrMap))
	{
		if (isMoving == true)
		{
			UpdateCharPosSmoothMove(crrMap);
		}
		AudioAsset(U"STAGE_SND").stop();
		if(!AudioAsset(U"CLEAR_SND").isPlaying())
		{
			AudioAsset(U"CLEAR_SND").setVolume(0.5);
			AudioAsset(U"CLEAR_SND").setLoop(false);
			AudioAsset(U"CLEAR_SND").play();
			AudioAsset(U"CLEAR_SND").stop();
			TimerOn = true;
		}
		
		if (CDTimer < 3)//タイマーが3秒切っていたら
		{
			_state = GAME_STATE::CLEAR;
		}
	}
	else//クリア状態ではないなら新しい入力を取る
	{
	//	Print << isMoving;
	
		if (isMoving == false)
		{
			direction nextDir = GetDirection();
			MoveObject(nextDir, crrMap);
		}
			
		UpdateCharPosSmoothMove(crrMap);
	}

	if (TimerOn == true)
	{
		CDTimer = CDTimer - Scene::DeltaTime();
	}
	/*if (CheckClear(maps[crrStageNumber]) == true)
	{
		_state = GAME_STATE::CLEAR;
	}*/
		
}
void DrawPlay()
{
	Scene::SetBackground(Palette::Black);
	//FontAsset(U"font")(U"PLAY_SCENE").drawAt(Scene::Center());
	//DrawStage(crrMap);
	DrawStageSmoothMove(crrMap);
}
void UpdateClear(GAME_STATE& _state)
{
	if(TimerOn == true)
		CDTimer = CDTimer - Scene::DeltaTime();

	if (CDTimer < 0)//タイマーが0秒切っていたら(3秒経過したら)d
	{
		crrStageNumber = crrStageNumber + 1;
		InitCrrentMap(crrMap, crrStageNumber);
		if (crrStageNumber >= MAX_STAGE_NUM)
		{
			/*crrStageNumber = MAX_STAGE_NUM - 1;
			_state = GAME_STATE::PLAY;*/
			allclear = true;
			_state = GAME_STATE::TITLE;
		}
		_state = GAME_STATE::PLAY;
		TimerOn = false;//タイマーのリセット
		CDTimer = 6;
	}
	
}
void DrawClear()
{
	Scene::SetBackground(Palette::Black);
	FontAsset(U"font")(U"Try Next Stage!").drawAt(Scene::Center());
	//Scene::SetBackground(Palette::Black);
}
void DrawStage(Map& _map)
{
	Scene::SetBackground(Palette::Darkgray);
	//FLOOR,WALL,LUGG,GOAL,HUMAN,HUMAN_ON_GOAL,LUGG_ON_GOALの順に画像をtextureの配列でAssetからコピー
	Texture ObjImg[7]{
		TextureAsset(U"FLOOR"),
		TextureAsset(U"WALL"),
		TextureAsset(U"LUGG"),
		TextureAsset(U"GOAL"),
		TextureAsset(U"PLAYER"),
		TextureAsset(U"PLAYER"),//キャラが重なっているときはとりあえず一番上にいるキャラを描く
		TextureAsset(U"LUGG"),
	};
	/*for (int i = 0; i < 7; i++)
	{
		ObjImg[i].resized(CHR_SIZE).draw(i * CHR_SIZE, 0);
	}*/
	for (int j = 0; j < _map.stage_height; j++)
	{
		for (int i = 0; i < _map.stage_width; i++)
		{
			OBJNAME objnum = GetObjectNum(Point{ j,i }, _map);//Point{ j,i }の位置のオブジェクト番号が入る
			//if (objnum != 7)//7は何もないのでスルー
			//{
			//	ObjImg[FLOOR].draw(j * CHR_SIZE, i * CHR_SIZE);//ステージになっている座標は一度床を描いてからオブジェクト設置
			//	ObjImg[GetObjectNum({i,j},_map)].draw(j * CHR_SIZE, i * CHR_SIZE);
			//}
			switch (objnum)
			{
			case FLOOR:
				ObjImg[FLOOR].draw(j * CHR_SIZE, i * CHR_SIZE);
				break;

			case WALL:
				ObjImg[FLOOR].draw(j * CHR_SIZE, i * CHR_SIZE);
				ObjImg[objnum].draw(j * CHR_SIZE, i * CHR_SIZE);
				break;

			case LUGG:
				ObjImg[FLOOR].draw(j * CHR_SIZE, i * CHR_SIZE);
				ObjImg[objnum].draw(j * CHR_SIZE, i * CHR_SIZE);
				break;

			case GOAL:
				ObjImg[FLOOR].draw(j * CHR_SIZE, i * CHR_SIZE);
				ObjImg[objnum].draw(j * CHR_SIZE, i * CHR_SIZE);
				break;

			case HUMAN:
				ObjImg[FLOOR].draw(j * CHR_SIZE, i * CHR_SIZE);
				ObjImg[objnum].draw(j * CHR_SIZE, i * CHR_SIZE);
				break;

			case HUMAN_ON_GOAL:
				if (KeyShift.pressed())
				{
					ObjImg[FLOOR].draw(j * CHR_SIZE, i * CHR_SIZE);
					ObjImg[GOAL].draw(j * CHR_SIZE, i * CHR_SIZE);
					ObjImg[objnum].draw(j * CHR_SIZE, i * CHR_SIZE, ColorF{ 1.0,0.5 });
				}
				else
				{
					ObjImg[FLOOR].draw(j * CHR_SIZE, i * CHR_SIZE);
					ObjImg[GOAL].draw(j * CHR_SIZE, i * CHR_SIZE);
					ObjImg[objnum].draw(j * CHR_SIZE, i * CHR_SIZE);
				}
				break;

			case LUGG_ON_GOAL:
				if(KeyShift.pressed())
				{
					ObjImg[FLOOR].draw(j * CHR_SIZE, i * CHR_SIZE);
					ObjImg[GOAL].draw(j * CHR_SIZE, i * CHR_SIZE);
					ObjImg[objnum].draw(j * CHR_SIZE, i * CHR_SIZE, ColorF{ 1.0,0.5 });
				}
				else
				{
					ObjImg[FLOOR].draw(j * CHR_SIZE, i * CHR_SIZE);
					ObjImg[GOAL].draw(j * CHR_SIZE, i * CHR_SIZE);
					ObjImg[objnum].draw(j * CHR_SIZE, i * CHR_SIZE);
				}
				break;

			default:
				break;
			}
		}
	}
}

OBJNAME GetObjectNum(Point _pos, Map & _map)
{
	return (OBJNAME(_map.Dat[_pos.y][_pos.x]));
}
void SetObjectMap(OBJNAME _obj, Point _pos, Map& _map)
{
	if ((_pos.x >= 0) && (_pos.x <= _map.stage_width) && (_pos.y >= 0) && (_pos.x <= _map.stage_height))
	{
		_map.Dat[_pos.y][_pos.x] = _obj;
	}
}
Point GetPlayerPos(Map& _map)
{
	for (int j = 0; j < _map.stage_height; j++)
	{
		for (int i = 0; i < _map.stage_width; i++)
		{
			if (_map.Dat[j][i] == OBJNAME::HUMAN || _map.Dat[j][i] == OBJNAME::HUMAN_ON_GOAL)
				return { i,j };
		}
	}
	return{ -1,-1 };
}
void MoveObject(direction _dir, Map& _map)
{
	Point dirVector[5] =
	{
		{0,-1},{-1,0},{0,+1},{+1,0},{0,0}
	};//この関数でしか使わないのでローカル関数にする

	Point pp = GetPlayerPos(_map);//プレイヤーの位置を取得(現在地は人か人on the Goalしかない)
	Point nextPos = { pp.x + dirVector[_dir].x, pp.y + dirVector[_dir].y };//移動先の予測位置
	Point nextnextPos = { nextPos.x + dirVector[_dir].x, nextPos.y + dirVector[_dir].y };
	OBJNAME crr = GetObjectNum(pp, _map);//ppのオブジェクト名を取得(判別)
	OBJNAME next = GetObjectNum(nextPos, _map);/*OBJNAME(_map.Dat[nextPos.x][nextPos.y]);*///nextPosのオブジェクト名を取得(判別)
	OBJNAME nextnext = GetObjectNum(nextnextPos, _map);

	switch (next)//隣のオブジェクトが
	{
	case FLOOR://隣が床なら
		SetObjectMap(OBJNAME::HUMAN, nextPos, _map);//隣を人にする
		SetMoveObject(_dir, nextPos, _map);//プレイヤーが動くならFromデータをセット
		isMoving = true;
		if (crr == OBJNAME::HUMAN_ON_GOAL)//現在値に自分が人 on the ゴールだったら
			SetObjectMap(OBJNAME::GOAL, pp, _map);//元の位置にゴールを置く
		else
			SetObjectMap(OBJNAME::FLOOR, pp, _map);//元の位置に床を設置
		break;
	case WALL://隣が壁なら
		break;//何もしない
	case GOAL://隣がゴールなら
		SetObjectMap(OBJNAME::HUMAN_ON_GOAL, nextPos, _map);//移動先を人 on the ゴールにする
		SetMoveObject(_dir, nextPos, _map);//プレイヤーが動くならFromデータをセット
		isMoving = true;
		if (crr == OBJNAME::HUMAN_ON_GOAL)//現在値が人 on the ゴールなら（2つ続いている状態）
			SetObjectMap(OBJNAME::GOAL, pp, _map);//現在値をゴールに
		else
			SetObjectMap(OBJNAME::FLOOR, pp, _map);//現在値を床にする
		break;

	case LUGG:
		switch (nextnext)
		{
		case FLOOR:
			SetObjectMap(OBJNAME::LUGG, nextnextPos, _map);
			SetMoveObject(_dir, nextnextPos, _map);//荷物分のFromデータをセット
			SetObjectMap(OBJNAME::HUMAN, nextPos, _map);
			SetMoveObject(_dir, nextPos, _map);//プレイヤー分のFromデータをセット
			isMoving = true;
			if (crr == OBJNAME::HUMAN_ON_GOAL)//元の位置が人 on the ゴールなら
				SetObjectMap(OBJNAME::GOAL, pp, _map);//そこを空のゴールにする
			else
				SetObjectMap(OBJNAME::FLOOR, pp, _map);

			break;

		case GOAL:
			SetObjectMap(OBJNAME::LUGG_ON_GOAL, nextnextPos, _map);
			SetMoveObject(_dir, nextnextPos, _map);//荷物分のFromデータをセット
			SetObjectMap(OBJNAME::HUMAN, nextPos, _map);
			SetMoveObject(_dir, nextPos, _map);//プレイヤー分のFromデータをセット
			isMoving = true;
			if (crr == OBJNAME::HUMAN_ON_GOAL)//元の位置が人 on the ゴールなら
				SetObjectMap(OBJNAME::GOAL, pp, _map);//そこを空のゴールにする
			else
				SetObjectMap(OBJNAME::FLOOR, pp, _map);
			break;
		case WALL:
			break;
		default:
			break;//次の位置が荷物や壁なら何もしない
		}
		break;
	case LUGG_ON_GOAL:
		switch (nextnext)
		{
		case FLOOR:
			SetObjectMap(OBJNAME::LUGG, nextnextPos, _map);
			SetMoveObject(_dir, nextnextPos, _map);//荷物分のFromデータをセット
			SetObjectMap(OBJNAME::HUMAN_ON_GOAL, nextPos, _map);
			SetMoveObject(_dir, nextPos, _map);//プレイヤー分のFromデータをセット
			isMoving = true;
			if (crr == OBJNAME::HUMAN_ON_GOAL)//元の位置が人 on the ゴールなら
				SetObjectMap(OBJNAME::GOAL, pp, _map);//そこを空のゴールにする
			else
				SetObjectMap(OBJNAME::FLOOR, pp, _map);
			break;

		case GOAL:
			SetObjectMap(OBJNAME::LUGG_ON_GOAL, nextnextPos, _map);
			SetMoveObject(_dir, nextnextPos, _map);//荷物分のFromデータをセット
			SetObjectMap(OBJNAME::HUMAN_ON_GOAL, nextPos, _map);
			SetMoveObject(_dir, nextPos, _map);//プレイヤー分のFromデータをセット
			isMoving = true;
			if (crr == OBJNAME::HUMAN_ON_GOAL)//元の位置が人 on the ゴールなら
				SetObjectMap(OBJNAME::GOAL, pp, _map);//そこを空のゴールにする
			else
				SetObjectMap(OBJNAME::FLOOR, pp, _map);
			break;
		default:
			break;
		}
	default:
		break;
	}
}
direction GetDirection()
{
	if (KeyW.down() || KeyUp.down())
	{
		return UP;
	}
	else if (KeyA.down() || KeyLeft.down())
	{
		return LEFT;
	}
	else if (KeyS.down() || KeyDown.down())
	{
		return DOWN;
	}
	else if (KeyD.down() || KeyRight.down())
	{
		return RIGHT;
	}
	else
		return NONE;
}
bool CheckClear(Map& _map)
{
	for (int j = 0; j < _map.stage_height; j++)
	{
		for (int i = 0; i < _map.stage_width; i++)
		{
			if (_map.Dat[j][i] == OBJNAME::LUGG)
				return false;
		}
	}
	return true;
}
void InitCrrentMap(Map& _map, int _stageNum)
{
	//crrMap = maps[crrStageNumber];
	_map.stage_height = maps[_stageNum].stage_height;
	_map.stage_width = maps[_stageNum].stage_height;

	for (int j = 0; j < MAX_STAGE_HEIGHT; j++)
	{
		for (int i = 0; i < MAX_STAGE_WIDTH; i++)
		{
			_map.Dat[j][i] = maps[_stageNum].Dat[j][i];
			_map.From[j][i] = direction::NONE;
		}
	}
	_map.moveRatio = 0;
}

void DrawStaticObject(Map& _map)
{
	Texture ObjImg[7]{
		TextureAsset(U"FLOOR"),
		TextureAsset(U"WALL"),
		TextureAsset(U"LUGG"),
		TextureAsset(U"GOAL"),
		TextureAsset(U"PLAYER"),
		TextureAsset(U"PLAYER"),
		TextureAsset(U"LUGG"),
	};
	for (int j = 0; j < _map.stage_height; j++)
	{
		for (int i = 0; i < _map.stage_width; i++)
		{
			OBJNAME objnum = GetObjectNum(Point{i,j}, _map);
			switch (objnum)
			{
			case FLOOR:
			case HUMAN:
			case LUGG:
				ObjImg[FLOOR].draw(i * CHR_SIZE, j * CHR_SIZE);
				break;
			case WALL:
				ObjImg[FLOOR].draw(i * CHR_SIZE,j * CHR_SIZE);
				ObjImg[WALL].draw(i * CHR_SIZE, j * CHR_SIZE);
				break;
			case GOAL:
			case HUMAN_ON_GOAL:
			case LUGG_ON_GOAL:
					ObjImg[FLOOR].draw(i * CHR_SIZE, j * CHR_SIZE);
					ObjImg[GOAL].draw(i * CHR_SIZE, j * CHR_SIZE);
					break;
			default:
				continue;
				break;
			}
		}
	}
}

void DrawMovableObject(Map& _map)
{
	Texture ObjImg[7]{
		TextureAsset(U"FLOOR"),
		TextureAsset(U"WALL"),
		TextureAsset(U"LUGG"),
		TextureAsset(U"GOAL"),
		TextureAsset(U"PLAYER"),
		TextureAsset(U"PLAYER"),
		TextureAsset(U"LUGG"),
	};
	ColorF col{ 1.0,1.0 };
	if (KeyShift.pressed())
	{
		col = ColorF{ 1.0,0.5 };
	}
	Vec2 dirVector[5] = //Vec2型にして方向のベクトルを実数対応にする
	{
		{0,-1},{-1,0},{0,+1},{+1,0},{0,0}
	};
	for (int j = 0; j < _map.stage_height; j++) {
		for (int i = 0; i < _map.stage_width; i++) {
			OBJNAME objnum = GetObjectNum(Point{ i,j }, _map);
			Vec2 pos = Vec2{ i,j } - dirVector[_map.From[j][i]];//移動前の配列上の座標を求めて pos=移動前の座標
			pos = pos * CHR_SIZE;//キャラクターサイズをかけて表示座標に変換
			Vec2 interpolationVector = dirVector[_map.From[j][i]] * _map.moveRatio;
			//Print << _map.moveRatio;
			switch (objnum)
			{
			case HUMAN_ON_GOAL:
			case LUGG_ON_GOAL:
				pos =  pos + interpolationVector * (double)CHR_SIZE;//(移動率＋移動前の座標)してから、キャラクターサイズをかけて表示座標に変換
				//Print << interpolationVector;
				ObjImg[objnum].draw(pos, col);
				break;
			case HUMAN:
			case LUGG:
				pos = pos + interpolationVector * (double)CHR_SIZE;
				//Print << interpolationVector;
				ObjImg[objnum].draw(pos);//移動前の座標に表示
				break;
			default:
				continue;
				break;
			}
		}
	}
}


void DrawStageSmoothMove(Map& _map)
{
	Scene::SetBackground(Palette::Darkgray);
	DrawStaticObject(_map);//静的なオブジェクトの描画
	DrawMovableObject(_map);//動的なオブジェクトの描画
}

void UpdateCharPosSmoothMove(Map& _map)
{
	if (isMoving)
	{
		//Print << _map.moveRatio;//画面に移動レートを表示
		_map.moveRatio = Clamp(_map.moveRatio + Scene::DeltaTime() / speed, 0.0,1.0);//Clamp(制約をかける値、最小値、最大値)で値を範囲内に収められる
		if (abs(_map.moveRatio - 1.0) < 1e-10)//1.0との誤差を計算し、10の-10乗より小さかったらほぼ等しいということにする
		{
			_map.moveRatio = 0;//移動が終わったので初期化
			isMoving = false;//移動が終わったので初期化
			ResetFromDat(_map);
		}
	}
}



void ResetFromDat(Map& _map)
{
	_map.moveRatio = 0.0;
	for (int j = 0; j < _map.stage_height; j++)
	{
		for (int i = 0; i < _map.stage_width; i++)
		{
			_map.From[j][i] = NONE;//_map.Fromを初期化
		}
	}
}

void SetMoveObject(direction _dir, Point _pos, Map& _map)
{
	if (_dir == NONE)
	{
		return;//入力方向が方向なしならそのままreturn
	}
	Point dirVector[5] =
	{
		{0,-1},{-1,0},{0,+1},{+1,0},{0,0}
	};
	direction fromDir[5] =
	{
		UP,LEFT,DOWN,RIGHT,NONE
	};
	if (isMoving == false)
	{
		_map.moveRatio = 0.0;
		_map.From[_pos.y][_pos.x] = fromDir[_dir];
	}
}
