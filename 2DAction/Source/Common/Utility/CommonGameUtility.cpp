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
#include "Game/Enemy/AI/Boss/AIBossNearAttack.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"
#include "Game/GameRecorder.h"

namespace Utility
{
	
/* ================================================ */
/**
 * @brief	ゲームが一周して終わるとき、タイトルに戻るときなどにプレイ記録リセット
 */
/* ================================================ */
static bool s_gamePauseFlg = false;
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

	math::Vector2 diff = calcPosA - calcPosB;

	float inRangeX = math::Absf(texInfoA.m_sizeWidth/2.0f) + math::Absf(texInfoB.m_sizeWidth/2.0f);
	float inRangeY = math::Absf(texInfoA.m_sizeHeight/2.0f) + math::Absf(texInfoB.m_sizeHeight/2.0f);

	if( math::Absf( diff.x ) < inRangeX && math::Absf( diff.y ) < inRangeY ){
		return true;
	}

	return false;
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
		offsetPos = ConvertWindowPosToGamePos( math::Vector2( WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f) );
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
const uint32_t GetMapHeight( const uint32_t &posX, const uint32_t &posY )
{
	return GetMapHeight( math::Vector2( static_cast<float>(posX), static_cast<float>(posY) ) );
}

const uint32_t GetMapHeight( const math::Vector2 &pos )
{
	uint32_t retVal = 0;
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
math::Vector2 GetMapRandamPos( bool allowInWindow, uint32_t mapHeight )
{
	const GameMap *pMap = GameRegister::GetInstance()->GetGameMap();
	math::Vector2 retVec = DEFAULT_VECTOR2;
	for(;;){
		math::Vector2 pos = math::Vector2( GetRandamValueFloat( pMap->GetMapWidth(), 0 ), GetRandamValueFloat( pMap->GetMapHeight(), 0 ));

		// マップの高さが指定よりも高かったらやり直し
		if( mapHeight < GetMapHeight(pos) ){
			continue;
		}
		if( !allowInWindow ){
			// 画面外かどうかチェック
			if( IsPositionInWindowArea( static_cast<uint32_t>(pos.x), static_cast<uint32_t>(pos.y) ) ){
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
	return IsPositionInWindowArea( static_cast<int32_t>(texInfo.m_posOrigin.x), static_cast<int32_t>(texInfo.m_posOrigin.y) );
}

bool IsPositionInWindowArea( const int32_t &xx, const int32_t &yy )
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
	int32_t posY = yy - static_cast<int32_t>(offsety);
	int32_t posX = xx - static_cast<int32_t>(offsetx);

	if(posX < WidthUpper && posX > WidthLower
	&& posY < HeightUpper && posY > HeightLower){
		retVal = true;
	}
	return retVal;
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
	case Common::AI_SEARCHING:	// プレイヤーを探している
		pRetAI = EnemyAISearch::Create();
		break;
	case Common::AI_MOVE_PLAYER:	// プレイヤーに近づく(体当たり攻撃)
		pRetAI = EnemyAITackle::Create();
		break;
	case Common::AI_SHOOTING:	// 遠距離攻撃(遠距離攻撃)
		pRetAI = EnemyAIShoot::Create();
		break;
	case Common::AI_ATTACK_NEAR:	// 遠距離攻撃(遠距離攻撃)
		pRetAI = AIBossNearAttack::Create();
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
	return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}

float GetRandamValueFloat( const int32_t &max, const int32_t &min)
{
	return static_cast<float>(GetRandamValue( max, min ));
}

/* ================================================ */
/**
 * @brief	セーブデータの情報を取得
 */
/* ================================================ */
bool GetSaveRanking( Common::SAVE_SCORE &saveData )
{
	// ファイルがない場合もあるので一度開いて作成しておく
	FILE *fpCheck = fopen( "playLog.dat", "r" );
	if( !fpCheck ){
		// ファイルがない場合もあるので一度開いて作成しておく
		FILE *fpCreate = fopen( "playLog.dat", "a" );
		fclose( fpCreate );

		// デフォルトの値を詰めておく
		Common::SAVE_SCORE scoreLog = {
			{ 1000, 500, 300, 100, 0},
			{ 1000, 500, 300, 100, 0},
		};
		FILE *fpWriteDef = fopen( "playLog.dat", "wb" );
		if( fpWriteDef == NULL ){
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
	FILE *fpRead = fopen( "playLog.dat", "rb" );
	if( fpRead == NULL ){
		return 0;
	}
	fread( &saveData, sizeof(saveData), 1, fpRead );
	fclose( fpRead );

	return true;
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
	std::ifstream ifs(readJson.c_str());

	picojson::value root;
	picojson::parse( root, ifs);

	// 各種パーツ情報を取得
	picojson::value partsData = root.get("partsInfo");
	for( uint32_t i = 0;;++i){
		picojson::value null;
		if( partsData.get(i) == null ){
			break;
		}
		std::string name = "";
		Common::PARTS_INFO info;
		info.Init();

		name			= partsData.get(i).get("partsName").get<std::string>();
		info.m_pos.x	= static_cast<float>( partsData.get(i).get("x").get<double>() );
		info.m_pos.y	= static_cast<float>( partsData.get(i).get("y").get<double>() );

		vParts.insert( std::make_pair< std::string, Common::PARTS_INFO >( name, info ));
	}
}

#ifdef _DEBUG
void DrawDebugCircle( math::Vector2 drawPos )
{
	Draw2DManager::GetInstance()->m_drawCircle.push_back( drawPos );
}
#endif

}