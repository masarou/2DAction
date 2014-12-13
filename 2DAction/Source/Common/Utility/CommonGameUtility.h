
#ifndef CMN_GAME_UTILITY
#define CMN_GAME_UTILITY

#include "Common/CommonDefine.h"
#include "Math/MathCommon.h"
#include "Math/MathVector.h"
#include "Math/MathAngle.h"
#include "Math/MathUtility.h"
#include "System/SystemDefine.h"
#include "System/Draw2D/SystemDraw2DDefine.h"


class EnemyAIBase;

void AddPlayerOffsetPos( math::Vector2 &pos );

const math::Vector2 &GetPlayerOffsetPos();

// 二つの所属空間が当たり判定が必要かどうか判定
const bool IsSameBelongArea( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// 描画位置が重なっているかどうか(当たり判定)
const bool IsInRangeTexture( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// マップ上での所属空間を求める
const void GetBelongAreaInMap( TEX_DRAW_INFO &tex );

// 画面上に表示しているオブジェクトかどうか
bool IsPositionInWindowArea( const TEX_DRAW_INFO &texInfo );
bool IsPositionInWindowArea( const int32_t &xx, const int32_t &yy );

// 指定のAIクラスの生成
EnemyAIBase *ChangeEnemyAI( Common::ENEMY_AI nextAI );

// プレイヤーの位置情報を取得
math::Vector2 GetPlayerPos();

// 指定の範囲内のランダムな数字を返す
int32_t GetRandamValue( const int32_t &max, const int32_t &min);

// セーブデータの情報を取得
bool GetSaveRanking( Common::SAVE_SCORE &saveData );

#endif