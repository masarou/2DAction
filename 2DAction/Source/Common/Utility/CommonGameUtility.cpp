/* ====================================================================== */
/**
 * @brief  ゲーム全体utilityクラス
 *
 * @note
 *
 */
/* ====================================================================== */
#include <random>
#include "System/picojson.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/Sound/SystemSoundManager.h"
#include "CommonGameUtility.h"
#include "Math/MathUtility.h"
#include "Game/Enemy/AI/EnemyAIBase.h"
#include "Game/Enemy/AI/EnemyAISearch.h"
#include "Game/Enemy/AI/EnemyAITackle.h"
#include "Game/Enemy/AI/EnemyAIShoot.h"
#include "Game/Enemy/AI/EnemyAIDashTackle.h"
#include "Game/Enemy/AI/EnemyAIRanger.h"
#include "Game/Enemy/AI/SlimeKing/AISlimeKingSearching.h"
#include "Game/Enemy/AI/SlimeKing/AISlimeKingTackle.h"
#include "Game/Enemy/AI/EnemyAIWizard.h"
#include "Game/Enemy/AI/EnemyAIDragon.h"
#include "Game/Enemy/AI/LastBoss/EnemyAILastBoss.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"
#include "Game/GameRecorder.h"

namespace Utility
{
	
/* ================================================ */
/**
 * @brief	ゲームがストップする瞬間周りのフラグ
 */
/* ================================================ */
static bool s_gamePauseFlg		= false;
static bool s_gameStopFlg		= false;
static bool s_showBlackFilter	= false;

// ポーズ画面に入った時
void StartGamePause()
{
	s_gamePauseFlg = true;
	SoundManager::GetInstance()->PlaySE("PauseStart");
}

void EndGamePause()
{
	s_gamePauseFlg = false;
	SoundManager::GetInstance()->PlaySE("PauseEnd");
}

bool IsGamePause()
{
	return s_gamePauseFlg;
}

// チュートリアル、演出等でゲームが止まった時
void StartGameStop( bool withFileter )
{
	s_gameStopFlg = true;
	s_showBlackFilter = withFileter;
}

void EndGameStop()
{
	s_gameStopFlg = false;
	s_showBlackFilter = false;
}

bool IsGameStop()
{
	return s_gameStopFlg;
}

bool IsShowPauseFilter()
{
	return s_showBlackFilter;
}

/* ================================================ */
/**
 * @brief	ゲームが一周して終わるとき、タイトルに戻るときなどにプレイ記録リセット
 */
/* ================================================ */
void GameInitALL()
{
	// スコア等初期化
	GameRecorder::GetInstance()->InitRecord();
	// プレイヤーオフセットリセット
	GameAccesser::GetInstance()->InitAll();
}

/* ================================================ */
/**
 * @brief	プレイヤーオフセットを足す
 */
/* ================================================ */
void AddPlayerOffsetPos( math::Vector2 &pos )
{
	//!プレイヤー情報取得
	pos += GameAccesser::GetInstance()->GetPlayerOffSet();
}

/* ================================================ */
/**
 * @brief	プレイヤーオフセット取得
 */
/* ================================================ */
const math::Vector2 &GetPlayerOffsetPos()
{
	//!プレイヤー情報取得
	return GameAccesser::GetInstance()->GetPlayerOffSet();
}

/* ================================================ */
/**
 * @brief	二つの所属空間が当たり判定が必要かどうか判定
 */
/* ================================================ */
const bool IsSameBelongArea( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB )
{
	bool retVal = false;

	// どちらかの空間Lvが0=画面全体なら当たり判定は必ず必要
	// 同じ空間Lv,番号なら当たり判定を行う
	if( (texA.m_belongLv == 0 || texB.m_belongLv == 0)
		|| (texA.m_belongLv == texB.m_belongLv && texA.m_belongIndex == texB.m_belongIndex) ){
		retVal = true;
	}
	else{
		// 空間Lvが同じなのにIndexが違うなら判定なし
		if( texA.m_belongLv == texB.m_belongLv ){
			retVal = false;
		}

		uint32_t smallLv	= 0;
		uint32_t smallIndex	= 0;
		uint32_t bigLv		= 0;
		uint32_t bigIndex	= 0;

		if( texA.m_belongLv < texB.m_belongLv ){
			smallLv		= texA.m_belongLv;
			smallIndex	= texA.m_belongIndex;
			bigLv		= texB.m_belongLv;
			bigIndex	= texB.m_belongIndex;
		}
		else{
			smallLv		= texB.m_belongLv;
			smallIndex	= texB.m_belongIndex;
			bigLv		= texA.m_belongLv;
			bigIndex	= texA.m_belongIndex;
		}
		uint32_t diffLv = bigLv - smallLv;
		uint32_t convIndex = bigIndex >> diffLv*2;
		if( convIndex == smallIndex ){
			retVal = true;
		}
	}
	return retVal;
}

/* ================================================ */
/**
 * @brief	描画位置が重なっているかどうか(当たり判定)
 */
/* ================================================ */
const bool IsInRangeTexture( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB )
{
	if( !IsSameBelongArea( texA, texB ) ){
		return false;
	}

	const TEX_INIT_INFO &texInfoA = TextureResourceManager::GetInstance()->GetLoadTextureInfo( texA.m_fileName.c_str() );
	const TEX_INIT_INFO &texInfoB = TextureResourceManager::GetInstance()->GetLoadTextureInfo( texB.m_fileName.c_str() );

	math::Vector2 calcPosA = texA.m_posOrigin;
	math::Vector2 calcPosB = texB.m_posOrigin;

	// offsetを使用しない = Window上の特定位置に常にいる場合
	if( !texA.m_usePlayerOffset ){
		calcPosA = ConvertWindowPosToGamePos( calcPosA );
	}
	if( !texB.m_usePlayerOffset ){
		calcPosB = ConvertWindowPosToGamePos( calcPosB );
	}

	// 以下、判定
	if( texInfoA.m_collisionArray.size() == 0 && texInfoB.m_collisionArray.size() == 0 )
	{
		// collisionCircleが両方とも設定されていない場合(単に画像サイズから求める)
		math::Vector2 diff = calcPosA - calcPosB;

		float inRangeX = math::Absf(texInfoA.m_sizeWidth/2.0f) + math::Absf(texInfoB.m_sizeWidth/2.0f);
		float inRangeY = math::Absf(texInfoA.m_sizeHeight/2.0f) + math::Absf(texInfoB.m_sizeHeight/2.0f);

		if( math::Absf( diff.x ) < inRangeX && math::Absf( diff.y ) < inRangeY ){
			return true;
		}
	}
	else if( texInfoA.m_collisionArray.size() != 0 && texInfoB.m_collisionArray.size() != 0 )
	{
		// collisionCircleが両方とも設定されている場合
		for( uint32_t i = 0; i < texInfoA.m_collisionArray.size() ; ++i ){
			for( uint32_t j = 0; j < texInfoB.m_collisionArray.size() ; ++j ){
				CollisionCircle circleA = texInfoA.m_collisionArray.at( i );
				CollisionCircle circleB = texInfoB.m_collisionArray.at( j );
				math::Vector2 centerA = circleA.m_relativePos + ( calcPosA - math::Vector2( texInfoA.m_sizeWidth / 2.0f, texInfoA.m_sizeHeight / 2.0f ) );
				math::Vector2 centerB = circleB.m_relativePos + ( calcPosB - math::Vector2( texInfoB.m_sizeWidth / 2.0f, texInfoB.m_sizeHeight / 2.0f ) );
				float range = circleA.m_radius + circleB.m_radius;
				if( math::GetDistance( centerA, centerB ) < range * range ){
					return true;
				}
#ifdef _DEBUG
				Utility::DrawDebugCircle( centerA, circleA.m_radius );
				Utility::DrawDebugCircle( centerB, circleB.m_radius );
#endif
			}
		}
	}
	else
	{
		// collisionCircleが片方のみ設定されている場合
		math::Vector2 squarePos[2];
		if( texInfoA.m_collisionArray.size() == 0 ){
			squarePos[0].x = calcPosA.x - ( texInfoA.m_sizeWidth/2.0f );	// 左上
			squarePos[0].y = calcPosA.y - ( texInfoA.m_sizeHeight/2.0f );	// 左上
			
			squarePos[1].x = calcPosA.x + ( texInfoA.m_sizeWidth/2.0f );	// 右下
			squarePos[1].y = calcPosA.y + ( texInfoA.m_sizeHeight/2.0f );	// 右下

			for( uint32_t i = 0; i < texInfoB.m_collisionArray.size() ; ++i ){
				CollisionCircle checkCircle;
				checkCircle.m_relativePos = texInfoB.m_collisionArray.at(i).m_relativePos + ( calcPosB - math::Vector2( texInfoB.m_sizeWidth / 2.0f, texInfoB.m_sizeHeight / 2.0f ) );
				checkCircle.m_radius = texInfoB.m_collisionArray.at(i).m_radius;

				if( CheckCollisionCircleSquare( checkCircle, squarePos[0], squarePos[1] ) ){
					return true;
				}
			}
		}
		else{
			squarePos[0].x = calcPosB.x - ( texInfoB.m_sizeWidth/2.0f );	// 左上
			squarePos[0].y = calcPosB.y - ( texInfoB.m_sizeHeight/2.0f );	// 左上

			squarePos[1].x = calcPosB.x + ( texInfoB.m_sizeWidth/2.0f );	// 左上
			squarePos[1].y = calcPosB.y + ( texInfoB.m_sizeHeight/2.0f );	// 左上

			for( uint32_t i = 0; i < texInfoA.m_collisionArray.size() ; ++i ){
				CollisionCircle checkCircle;
				checkCircle.m_relativePos = texInfoA.m_collisionArray.at(i).m_relativePos + ( calcPosA - math::Vector2( texInfoA.m_sizeWidth / 2.0f, texInfoA.m_sizeHeight / 2.0f ) );
				checkCircle.m_radius = texInfoA.m_collisionArray.at(i).m_radius;

				if( CheckCollisionCircleSquare( checkCircle, squarePos[0], squarePos[1] ) ){
					return true;
				}
			}
		}
	}



	return false;
}

/* ================================================ */
/**
 * @brief	指定の円と四角形の当たり判定
 */
/* ================================================ */
const bool CheckCollisionCircleSquare( const CollisionCircle &circle, const math::Vector2 &squarePosUpperLeft, const math::Vector2 &squarePosUnderRight )
{
	math::Vector2 squarePos[4];

	squarePos[0] = squarePosUpperLeft;	// 左上
	squarePos[1].x = squarePosUnderRight.x;	// 右上
	squarePos[1].y = squarePosUpperLeft.y;	// 右上
	squarePos[2].x = squarePosUpperLeft.x;	// 左下
	squarePos[2].y = squarePosUnderRight.y;	// 左下
	squarePos[3] = squarePosUnderRight;	// 右下

	// 縦でのチェック
	if( 
		( squarePos[0].x < circle.m_relativePos.x && circle.m_relativePos.x < squarePos[3].x ) &&
		( squarePos[0].y - circle.m_radius < circle.m_relativePos.y && circle.m_relativePos.y < squarePos[3].y + circle.m_radius )
	)
	{
		return true;
	}
	// 横でのチェック
	if(
		( squarePos[0].x - circle.m_radius < circle.m_relativePos.x && circle.m_relativePos.x < squarePos[3].x + circle.m_radius ) &&
		( squarePos[0].y < circle.m_relativePos.y && circle.m_relativePos.y < squarePos[3].y )
	)
	{
		return true;
	}
	// 斜めチェック
	for( uint32_t j = 0; j < NUMBEROF( squarePos ); ++j ){
		if( math::GetDistance( circle.m_relativePos, squarePos[j] ) < circle.m_radius * circle.m_radius ){
			return true;
		}
	}

	return false;
}

/* ================================================ */
/**
 * @brief	第一引数から第二引数までのベクトルを求める(大きさは1)
 */
/* ================================================ */
math::Vector2 GetDirAToB( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB )
{
	math::Vector2 vec = texB.m_posOrigin - texB.m_posOrigin;
	vec.Normalize();
	return vec;
}

/* ================================================ */
/**
 * @brief	マップ上での所属空間を求める
 */
/* ================================================ */
const void GetBelongAreaInMap( TEX_DRAW_INFO &tex )
{
	uint32_t retVal = 0;
	const TEX_INIT_INFO &texInfo	= TextureResourceManager::GetInstance()->GetLoadTextureInfo( tex.m_fileName.c_str() );
	const GameRegister *pRegister	= GameRegister::GetInstance();
	if( !pRegister ){ return; }
	const GameMap *pMap				= pRegister->GetGameMap();
	if( !pMap ){ return; }

	// 画像の左上と右下の位置を求める
	math::Vector2 offsetPos = tex.m_posOrigin;

	// オフセットを使用していない
	// 常に画面上に固定で表示されている描画物の当たり判定の場合
	// プレイヤーの初期位置を考慮して値を変更してやる必要がある
	if( !tex.m_usePlayerOffset ){
		offsetPos = ConvertWindowPosToGamePos( WINDOW_CENTER );
	}

	math::Vector2 upperLeft		= math::Vector2( offsetPos.x - (texInfo.m_sizeWidth/2.0f), offsetPos.y - (texInfo.m_sizeHeight/2.0f) );
	math::Vector2 underRight	= math::Vector2( offsetPos.x + (texInfo.m_sizeWidth/2.0f), offsetPos.y + (texInfo.m_sizeHeight/2.0f) );

	const uint32_t upper = pMap->GetBelongArea( upperLeft );
	const uint32_t under = pMap->GetBelongArea( underRight );

	uint32_t areaNum = upper^under;
	uint32_t belongLv = 0;		// rootがLv0, 大きくになるにつれて親、子、孫となる
	uint32_t belongIndex = 0;

	uint32_t lastShift = 0;
	for( int32_t i = 0 ; i <= HIT_AREA_SPLIT_MAX; ++i ){
		if( (areaNum >> (2*i)) == 0 ){
			lastShift = i; 
			break;
		}
	}
	tex.m_belongLv = HIT_AREA_SPLIT_MAX - lastShift;
	tex.m_belongIndex = under >> 2*lastShift;
	//DEBUG_PRINT( " m_belongLv = %d, m_belongIndex = %d\n", tex.m_belongLv, tex.m_belongIndex );
}

/* ================================================ */
/**
 * @brief	マップ上での高さを取得
 */
/* ================================================ */
const int32_t GetMapHeight( const uint32_t &posX, const uint32_t &posY )
{
	return GetMapHeight( math::Vector2( static_cast<float>(posX), static_cast<float>(posY) ) );
}

const int32_t GetMapHeight( const math::Vector2 &pos )
{
	int32_t retVal = 0;
	const GameMap *pMap = GameRegister::GetInstance()->GetGameMap();
	if( pMap ){
		retVal = pMap->GetTileHeight( pos );
	}
	return retVal;
}

/* ================================================ */
/**
 * @brief	マップ上で適当な数値を返す
 */
/* ================================================ */
math::Vector2 GetMapRandamPos( bool allowInWindow, int32_t mapHeight )
{
	const GameMap *pMap = GameRegister::GetInstance()->GetGameMap();
	math::Vector2 retVec = DEFAULT_VECTOR2;
	for(;;){
		math::Vector2 pos = math::Vector2( GetRandamValueFloat( pMap->GetMapWidth(), 0 ), GetRandamValueFloat( pMap->GetMapHeight(), 0 ));

		// マップの高さが指定よりも高かったらやり直し
		if( mapHeight != GetMapHeight(pos) ){
			continue;
		}
		if( !allowInWindow ){
			// 画面外かどうかチェック
			if( IsPositionInWindowArea( static_cast<uint32_t>( pos.x ), static_cast<uint32_t>( pos.y ) ) ){
				continue;
			}
		}
		else{
			// プレイヤーにかぶらないかどうかだけチェック
			if( IsPositionInPlayerPos( pos.x, pos.y ) ){
				continue;
			}
		}

		// 位置決定
		retVec = pos;
		break;
	}
	return retVec;
}

/* ================================================ */
/**
 * @brief	画面上に表示しているオブジェクトかどうか
 */
/* ================================================ */
bool IsPositionInWindowArea( const TEX_DRAW_INFO &texInfo )
{
	if( !texInfo.m_usePlayerOffset ){
		return true;
	}
	return IsPositionInWindowArea( static_cast<uint32_t>( texInfo.m_posOrigin.x ), static_cast<uint32_t>( texInfo.m_posOrigin.y ) );
}

bool IsPositionInWindowArea( const uint32_t &xx, const uint32_t &yy )
{
	bool retVal = false;

	//!プレイヤー情報取得
	float offsetx = 0.0f;
	float offsety = 0.0f;
	GameAccesser::GetInstance()->GetPlayerOffSet(offsetx, offsety);

	//!現在描画したい端(画面外でも一マス分では描画)
	int32_t WidthLower	= -1 * 50;
	int32_t WidthUpper	= WINDOW_WIDTH + 50;
	int32_t HeightLower = -1 * 50;
	int32_t HeightUpper = WINDOW_HEIGHT + 50;

	//!描画しようとしている位置
	int32_t posY = static_cast<uint32_t>( yy - offsety );
	int32_t posX = static_cast<uint32_t>( xx - offsetx );

	if(posX < WidthUpper && posX > WidthLower
	&& posY < HeightUpper && posY > HeightLower){
		retVal = true;
	}
	return retVal;
}

/* ================================================ */
/**
 * @brief	プレイヤーに重なるかどうか
	true	重なっている
	false	重なっていない
 */
/* ================================================ */
bool IsPositionInPlayerPos( const float &xx, const float &yy )
{
	math::Vector2 otherPos = math::Vector2( xx, yy );
	if( math::IsInRange( GetPlayerPos(), otherPos, 250.0f ) ){
		return true;
	}
	return false;
}

/* ================================================ */
/**
 * @brief	指定のAIクラスの生成
 */
/* ================================================ */
EnemyAIBase *CreateEnemyAI( Common::ENEMY_AI nextAI )
{
	EnemyAIBase *pRetAI = NULL;
	switch( nextAI ){
	case Common::AI_SEARCHING:		// プレイヤーを探している
		pRetAI = EnemyAISearch::Create();
		break;
	case Common::AI_MOVE_PLAYER:	// プレイヤーに近づく(体当たり攻撃)
		pRetAI = EnemyAITackle::Create();
		break;
	case Common::AI_DASH_TACKLE:	// ダッシュ体当たり
		pRetAI = EnemyAIDashTackle::Create();
		break;
	case Common::AI_SHOOTING:		// 遠距離攻撃(遠距離攻撃)
		pRetAI = EnemyAIShoot::Create();
		break;
	case Common::AI_ATTACK_NEAR:	// 遠距離攻撃(遠距離攻撃)
		pRetAI = AIRanger::Create();
		break;
	case Common::AI_SEARCHING_SLIME_KING:	// スライムキングプレイヤー探索
		pRetAI = AISlimeKingSearching::Create();
		break;
	case Common::AI_MOVE_PLAYER_SLIME_KING:	// スライムキングプレイヤーへ移動
		pRetAI = AISlimeKingTackle::Create();
		break;
	case Common::AI_ATTACK_WIZARD:
		pRetAI = AIWizard::Create();
		break;
	case Common::AI_ATTACK_DRAGON:
		pRetAI = AIDragon::Create();
		break;
	case Common::AI_ATTACK_LAST_BOSS:
		pRetAI = EnemyAILastBoss::Create();
		break;
	}
	return pRetAI;
}

/* ================================================ */
/**
 * @brief	プレイヤーの位置情報を取得
 */
/* ================================================ */
math::Vector2 GetPlayerPos()
{
	math::Vector2 plPos;
	const GamePlayer *pPlayer = GameRegister::GetInstance()->GetPlayer();

	plPos = pPlayer->GetDrawInfo().m_posOrigin;
	plPos += GetPlayerOffsetPos();

	return plPos;
}

/* ================================================ */
/**
 * @brief	Window上の特定の位置に常にいる実体の位置をゲーム上の位置に変換
 */
/* ================================================ */
math::Vector2 ConvertWindowPosToGamePos( const math::Vector2 &windowPos )
{
	if( windowPos.x < 0.0f || windowPos.x > WINDOW_WIDTH
		|| windowPos.y < 0.0f || windowPos.y > WINDOW_HEIGHT){
			DEBUG_ASSERT( 0, "数値がおかしい" );
			return math::Vector2();
	}

	math::Vector2 retPos = windowPos;

	//!プレイヤーの位置分ずらす
	float offsetPlx = 0.0f;
	float offsetPly = 0.0f;
	GameAccesser::GetInstance()->GetPlayerOffSet(offsetPlx, offsetPly);
	retPos.x +=  offsetPlx;
	retPos.y +=  offsetPly;
			 
	return retPos;
}

/* ================================================ */
/**
 * @brief	向く方向を上下左右から選択
 */
/* ================================================ */
InputWatcher::BUTTON_KIND GetDirection( const float dirX, const float dirY )
{
	InputWatcher::BUTTON_KIND kind = InputWatcher::BUTTON_INVALID;
	if( math::Absf(dirY) >= math::Absf(dirX) ){
		if( dirY > 0 ){
			kind = InputWatcher::BUTTON_DOWN;
		}
		else if( dirY < 0 ){
			kind = InputWatcher::BUTTON_UP;
		}
	}
	else{
		if( dirX > 0 ){
			kind = InputWatcher::BUTTON_RIGHT;
		}
		else if( dirX < 0 ){
			kind = InputWatcher::BUTTON_LEFT;
		}
	}
	return kind;
}

/* ================================================ */
/**
 * @brief	指定の範囲内のランダムな数字を返す
 */
/* ================================================ */
int32_t GetRandamValue( const int32_t &max, const int32_t &min)
{
	return min + (int)( rand()*( (max-1)-min+1.0 )/( 1.0+RAND_MAX ) );
}

float GetRandamValueFloat( const int32_t &max, const int32_t &min)
{
	return static_cast<float>(GetRandamValue( max, min ));
}

/* ================================================ */
/**
 * @brief	セーブ、ロード関数
 */
/* ================================================ */
bool GetSaveData( Common::SAVE_DATA &saveData )
{
	// ファイルがない場合もあるので一度開いて作成しておく
	FILE *fpCheck = NULL;
	fopen_s( &fpCheck, "playLog.dat", "r" );
	if( !fpCheck ){
		// ファイルがないので一度開いて作成しておく
		fopen_s( &fpCheck, "playLog.dat", "a" );
		if( fpCheck ){
			fclose( fpCheck );
		}
		// デフォルトの値を詰めておく
		Common::SAVE_DATA scoreLog = {
			true,
			true,
			false,
			false,
			0,
			{ 1000, 500, 300, 100, 0},
			{ 0 }
		};
		FILE *fpWriteDef = NULL;
		fopen_s( &fpWriteDef, "playLog.dat", "wb" );
		if( fpWriteDef == false ){
			return false;
		}
		fwrite( &scoreLog, sizeof(scoreLog), 1, fpWriteDef );
		fclose( fpWriteDef );
	}
	else{
		// ファイルは存在している
		fclose( fpCheck );
	}

	// プレイログ読み込み
	FILE *fpRead = NULL;
	fopen_s( &fpRead, "playLog.dat", "rb" );
	if( fpRead == NULL ){
		return 0;
	}
	fread( &saveData, sizeof(saveData), 1, fpRead );
	fclose( fpRead );

	return true;
}

bool OverWriteSaveData( Common::SAVE_DATA &saveData )
{
	// プレイログ書き出し
	FILE *fpWrite = NULL;
	fopen_s( &fpWrite, "playLog.dat", "wb" );
	if( fpWrite == NULL ){
		return false;
	}
	fwrite( &saveData, sizeof(saveData), 1, fpWrite );
	fclose( fpWrite );
	return true;
}

/* ================================================ */
/**
 * @brief	ステータスレベルから実際にセットする値へ変換(0~9の間)
 */
/* ================================================ */
static const uint32_t STATE_TABLE_MAX = Common::STATUS_LEVEL_MAX+1;
uint32_t ConvertLevelToBaseState( Common::PLAYER_BASE_STATE stateKind, uint32_t level )
{
	if( level >= STATE_TABLE_MAX){
		DEBUG_ASSERT( 0, "指定レベルが想定外");
		return 0;
	}

	// ライフのレベルテーブル
	static uint32_t s_lifeStateTable[STATE_TABLE_MAX] = {
		0,		// lv1
		40,		// lv2
		80,		// lv3
		110,	// lv4
		150,	// lv5
		290,	// lv6
		220,	// lv7
		240,	// lv8
		270,	// lv9
		300,	// lv10
	};

	// 斬撃のレベルテーブル
	static uint32_t s_damageSlashingTable[STATE_TABLE_MAX] = {
		0,		// lv1
		30,		// lv2
		60,		// lv3
		90,		// lv4
		120,	// lv5
		140,	// lv6
		160,	// lv7
		180,	// lv8
		210,	// lv9
		240,	// lv10
	};

	// マシンガンのレベルテーブル
	static uint32_t s_damageBulletTable[STATE_TABLE_MAX] = {
		0,		// lv1
		20,		// lv2
		35,		// lv3
		50,		// lv4
		65,		// lv5
		80,		// lv6
		95,		// lv7
		110,	// lv8
		120,	// lv9
		130,	// lv10
	};

	// マシンガンSPDのレベルテーブル
	static uint32_t s_speedBulletTable[STATE_TABLE_MAX] = {
		0,		// lv1
		3,		// lv2
		5,		// lv3
		8,		// lv4
		10,		// lv5
		12,		// lv6
		15,		// lv7
		17,		// lv8
		20,		// lv9
		20,		// lv10
	};

	uint32_t retVal = 0;
	switch( stateKind ){

	case Common::BASE_STATE_LIFE:
		retVal = s_lifeStateTable[level];
		break;
	case Common::BASE_STATE_MOVE_SPEED:
		retVal = static_cast<uint32_t>(level*0.5f);
		break;
	case Common::BASE_STATE_DEFFENCE:
		retVal = level*5;
		break;
	case Common::BASE_STATE_BLADE_LEVEL:
		retVal = s_damageSlashingTable[level];
		break;
	case Common::BASE_STATE_BULLET_SPD:
		retVal = s_speedBulletTable[level];
		break;
	case Common::BASE_STATE_BULLET_DMG:
		retVal = s_damageBulletTable[level];
		break;
	case Common::BASE_STATE_CONTINUE:
		DEBUG_ASSERT( 0, "該当項目が返す値はありません");
		break;
	}
	return retVal;
}

/* ================================================ */
/**
 * @brief	画面のjsonからパーツ情報を取得("partsInfo")
 */
/* ================================================ */
void GetPartsInfoFromJson( const std::string &jsonStr, std::map< std::string, Common::PARTS_INFO > &vParts )
{
	std::string readJson = JSON_GAME2D_PATH;
	readJson += jsonStr;
	idxfstream ifs(readJson.c_str());

	picojson::value root;
	picojson::parse( root, ifs);

	// 各種パーツ情報を取得
	picojson::value null;
	picojson::value partsData = root.get("partsInfo");
	if( partsData == null ){
		// パーツ情報なし
		return;
	}
	for( uint32_t i = 0;;++i){
		if( partsData.get(i) == null ){
			break;
		}
		std::string name = "";
		Common::PARTS_INFO info;
		info.Init();

		// 名前と位置情報取得
		name			= partsData.get(i).get("partsName").get<std::string>();
		info.m_pos.x	= static_cast<float>( partsData.get(i).get("x").get<double>() );
		info.m_pos.y	= static_cast<float>( partsData.get(i).get("y").get<double>() );

		// パーツ情報が書かれたjsonファイル名
		if( partsData.get(i).get("loadJson") != null ){
			info.m_jsonStr	= partsData.get(i).get("loadJson").get<std::string>();
		}
		else{
			info.m_jsonStr = "";
		}

		// 特殊な機能を持ったパーツの場合そのタイプをセットしておく
		if( partsData.get(i).get("partsType") != null ){
			if( partsData.get(i).get("partsType").get<std::string>() == "PARTS_SINGLE_DRAW" ){
				info.m_type	= Common::PARTS_SINGLE_DRAW;
			}
			else if( partsData.get(i).get("partsType").get<std::string>() == "PARTS_NUM_COUNTER" ){
				info.m_type	= Common::PARTS_NUM_COUNTER;
			}
		}

		vParts.insert( std::map< std::string, Common::PARTS_INFO >::value_type( name, info ));
	}
}

/* ================================================ */
/**
 * @brief	画像サイズを考慮して指定位置に移動できるか
 */
/* ================================================ */
bool IsMovable( const std::string &resourceJson, const math::Vector2 &pos )
{
	const TEX_INIT_INFO &playerTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( resourceJson.c_str() );
	math::Vector2 up = pos;
	up.y += playerTexInfo.m_sizeHeight/2.0f;
	math::Vector2 down = pos;
	down.y -= playerTexInfo.m_sizeHeight/2.0f;
	math::Vector2 left = pos;
	left.x -= playerTexInfo.m_sizeWidth/2.0f;
	math::Vector2 right = pos;
	right.x += playerTexInfo.m_sizeWidth/2.0f;

	if( Utility::GetMapHeight( up ) == 0
		&& Utility::GetMapHeight( down ) == 0
		&& Utility::GetMapHeight( left ) == 0
		&& Utility::GetMapHeight( right ) == 0){
		return true;
	}
	return false;
}

/* ================================================ */
/**
 * @brief	対象の当たり判定物からイベントタイプを取得
 */
/* ================================================ */
Common::EVENT_MESSAGE GetEventMessageFromObjType( const Common::TYPE_OBJECT &objType )
{
	for( uint32_t i = 0 ; i < NUMBEROF(Common::s_collisionInfo) ; ++i ){
		if( Common::s_collisionInfo[i].m_type == objType ){
			return Common::s_collisionInfo[i].m_eventMessage;
		}
	}
	return Common::EVENT_MESSAGE_MAX;
}

/* ================================================ */
/**
 * @brief	イベントタイプからもととなる当たり判定物のTYPEを取得
 */
/* ================================================ */
Common::TYPE_OBJECT GetObjTypeFromEventMessage( const Common::EVENT_MESSAGE &eventMessage )
{
	for( uint32_t i = 0 ; i < NUMBEROF(Common::s_collisionInfo) ; ++i ){
		if( Common::s_collisionInfo[i].m_eventMessage == eventMessage ){
			return Common::s_collisionInfo[i].m_type;
		}
	}
	return Common::TYPE_MAX;
}

/* ================================================ */
/**
 * @brief	デバッグ用文字列描画
 */
/* ================================================ */
void DrawStringOnWindow( const std::string &str, const math::Vector2 &pos, uint32_t color )
{
	Draw2DManager::GetInstance()->PushDrawString( str, pos, color );
}

void DrawDebugCircle( const math::Vector2 &pos, const float &radius )
{
#ifdef _DEBUG
	Draw2DManager::DrawCircleInfo drawInfo = { radius, pos };
	Draw2DManager::GetInstance()->m_drawCircle.push_back( drawInfo );
#endif
}

void DrawDebugBox( const math::Vector2 &upperLeft, const float &sideDistance )
{
#ifdef _DEBUG
	Draw2DManager::DrawBoxInfo drawInfo = { upperLeft, sideDistance };
	Draw2DManager::GetInstance()->m_drawBox.push_back( drawInfo );
#endif
}


}