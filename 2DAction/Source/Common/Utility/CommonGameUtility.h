/* ====================================================================== */
/**
 * @brief  ゲーム全体utilityクラス
 *
 * @note
 *
 */
/* ====================================================================== */
#ifndef __CMN_GAME_UTILITY__
#define __CMN_GAME_UTILITY__

#include "Common/CommonDefine.h"
#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"
#include "Math/MathUtility.h"
#include "System/SystemDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"
#include "System/Input/SystemInputWatcher.h"

class EnemyAIBase;

namespace Utility
{

// ゲームが一周して終わるとき、タイトルに戻るときなどにプレイ記録リセット
void GameInitALL();

void AddPlayerOffsetPos( math::Vector2 &pos );

const math::Vector2 &GetPlayerOffsetPos();

// 二つの所属空間が当たり判定が必要かどうか判定
const bool IsSameBelongArea( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// 描画位置が重なっているかどうか(当たり判定)
const bool IsInRangeTexture( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// マップ上での所属空間を求める
const void GetBelongAreaInMap( TEX_DRAW_INFO &tex );

// マップ上での高さを取得
const uint32_t GetMapHeight( const uint32_t &posX, const uint32_t &posY );
const uint32_t GetMapHeight( const math::Vector2 &pos );

// マップ上で適当な数値を返す
math::Vector2 GetMapRandamPos( bool allowInWindow = true, uint32_t mapHeight = 0 );

// 画面上に表示しているオブジェクトかどうか
bool IsPositionInWindowArea( const TEX_DRAW_INFO &texInfo );
bool IsPositionInWindowArea( const int32_t &xx, const int32_t &yy );

// 指定のAIクラスの生成
EnemyAIBase *CreateEnemyAI( Common::ENEMY_AI nextAI );

// プレイヤーの位置情報を取得
math::Vector2 GetPlayerPos();

// Window上の特定の位置に常にいる実体の位置をゲーム上の位置に変換
math::Vector2 ConvertWindowPosToGamePos( const math::Vector2 &windowPos );

// 向く方向を上下左右から選択
InputWatcher::BUTTON_KIND GetDirection( const float dirX, const float dirY );

// 指定の範囲内のランダムな数字を返す
int32_t GetRandamValue( const int32_t &max, const int32_t &min);
float GetRandamValueFloat( const int32_t &max, const int32_t &min);

// セーブデータの情報を取得
bool GetSaveRanking( Common::SAVE_SCORE &saveData );

// 画面のjsonからパーツ情報を取得("partsInfo")
void GetPartsInfoFromJson( const std::string &jsonStr, std::map< std::string, Common::PARTS_INFO > &vParts );

#ifdef _DEBUG
void DrawDebugCircle( math::Vector2 drawPos );
#endif

} // Utility

#endif