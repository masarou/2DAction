
#include "System/Draw2D/SystemDraw2DResource.h"
#include "CommonGameUtility.h"
#include "Math/MathUtility.h"
#include "Game/Enemy/EnemyAIBase.h"

void AddPlayerOffsetPos( math::Vector2 &pos )
{
	//!プレイヤー情報取得
	pos += GameAccesser::GetInstance()->GetPlayerOffSet();
}
const math::Vector2 &GetPlayerOffsetPos()
{
	//!プレイヤー情報取得
	return GameAccesser::GetInstance()->GetPlayerOffSet();
}

const bool IsInRangeTexture( const TEX_DRAW_INFO &texA, const TEX_DRAW_INFO &texB )
{
	const TEX_INIT_INFO &texInfoA = TextureResourceManager::GetInstance()->GetLoadTextureInfo( texA.m_fileName.c_str() );
	const TEX_INIT_INFO &texInfoB = TextureResourceManager::GetInstance()->GetLoadTextureInfo( texB.m_fileName.c_str() );

	math::Vector2 diff = texA.m_pos - texB.m_pos;

	float inRangeX = math::Absf(texInfoA.m_sizeWidth/2.0f) + math::Absf(texInfoB.m_sizeWidth/2.0f);
	float inRangeY = math::Absf(texInfoA.m_sizeHeight/2.0f) + math::Absf(texInfoB.m_sizeHeight/2.0f);

	if( math::Absf( diff.x ) < inRangeX && math::Absf( diff.y ) < inRangeY ){
		return true;
	}

	return false;
}

EnemyAIBase *ChangeEnemyAI( Common::ENEMY_AI nextAI )
{
	EnemyAIBase *pRetAI = NULL;
	//switch( nextAI ){
	//case Common::AI_SEARCHING:	// プレイヤーを探している
	//	pRetAI = NEW EnemyAIBase();
	//	break;
	//case Common::AI_MOVE_PLAYER:	// プレイヤーに近づく(体当たり攻撃)
	//	pRetAI = NEW EnemyAIBase();
	//	break;
	//case Common::AI_SHOOTING:	// 遠距離攻撃(遠距離攻撃)
	//	pRetAI = NEW EnemyAIBase();
	//	break;
	//}
	return pRetAI;
}
