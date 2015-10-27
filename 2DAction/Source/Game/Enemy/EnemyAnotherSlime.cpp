/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyAnotherSlime.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"
#include "System/Sound/SystemSoundManager.h"

EnemyAnotherSlime *EnemyAnotherSlime::Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	std::string fileStr = "EnemySlimeAnother.json";
	return NEW EnemyAnotherSlime( fileStr, uniqueID, enemyLevel, enemyPos );
}

EnemyAnotherSlime::EnemyAnotherSlime( const std::string &fileName, const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
	: EnemyBase( fileName, uniqueID, Common::ENEMY_KIND_SLIME_ANOTHER, enemyLevel, enemyPos )
{
}

EnemyAnotherSlime::~EnemyAnotherSlime(void)
{
}

bool EnemyAnotherSlime::InitMain()
{

	return true;
}

void EnemyAnotherSlime::EnemyDeath()
{
	// 爆破エフェクトを出す
	GameEffectWithCollision::CreateEffect( Common::OWNER_ENEMY, GameEffectWithCollision::EFFECT_POISON, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );

	// 爆発SE鳴らす
	SoundManager::GetInstance()->PlaySE("Death");

	// 一定確率で解毒剤を落とす
	if( Utility::GetRandamValue( 6, 0 ) == 0 ){
		GameManager *pGameMan = GameRegister::GetInstance()->UpdateManagerGame();
		if( pGameMan ){
			pGameMan->CreateItem( Common::ITEM_KIND_ANTIDOTE, GetDrawInfo().m_posOrigin );
		}
	}

	// 死亡
	TaskStartDie();
}


static const uint32_t DEFAULT_HP = 50;
static const float DEFAULT_SPEED = 1.0f;
static const uint32_t DEFAULT_DAMAGE_HIT = 10;

 const uint32_t EnemyAnotherSlime::GetEnemyDefaultHP() const
{
	// Lvによって最大ライフ変更
	return DEFAULT_HP + ( 150 * GetEnemyLevel() );
}

 const float EnemyAnotherSlime::GetEnemyDefaultSPD() const
{
	return DEFAULT_SPEED + ( static_cast<float>( GetEnemyLevel() ) / 3.0f );
}

const uint32_t EnemyAnotherSlime::GetPlayerHitDamage() const
{
	return DEFAULT_DAMAGE_HIT + ( 5 * GetEnemyLevel() );
}

void EnemyAnotherSlime::ReduceDamage( Common::CMN_EVENT &eventId )
{
	float levelReduce = static_cast<float>( GetEnemyLevel()*0.05f );
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:
		// マシンガン攻撃は効きにくい( 0.9~4.5 )
		eventId.m_eventValue *= 0.9f - levelReduce;
		break;
	case Common::EVENT_HIT_BLADE_PLAYER:
		// 斬撃はダメージを増やす
		eventId.m_eventValue *= 1.2f - levelReduce;
		break;
	}
}