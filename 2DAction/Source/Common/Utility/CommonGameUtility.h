
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

// 描画位置が重なっているかどうか(当たり判定)
const bool IsInRangeTexture( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB );

// 指定のAIクラスの生成
EnemyAIBase *ChangeEnemyAI( Common::ENEMY_AI nextAI );

// 指定の範囲内のランダムな数字を返す
int32_t GetRandamValue( const int32_t &max, const int32_t &min);

#endif