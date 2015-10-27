/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
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
	// ���j�G�t�F�N�g���o��
	GameEffectWithCollision::CreateEffect( Common::OWNER_ENEMY, GameEffectWithCollision::EFFECT_POISON, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );

	// ����SE�炷
	SoundManager::GetInstance()->PlaySE("Death");

	// ���m���ŉ�ō܂𗎂Ƃ�
	if( Utility::GetRandamValue( 6, 0 ) == 0 ){
		GameManager *pGameMan = GameRegister::GetInstance()->UpdateManagerGame();
		if( pGameMan ){
			pGameMan->CreateItem( Common::ITEM_KIND_ANTIDOTE, GetDrawInfo().m_posOrigin );
		}
	}

	// ���S
	TaskStartDie();
}


static const uint32_t DEFAULT_HP = 50;
static const float DEFAULT_SPEED = 1.0f;
static const uint32_t DEFAULT_DAMAGE_HIT = 10;

 const uint32_t EnemyAnotherSlime::GetEnemyDefaultHP() const
{
	// Lv�ɂ���čő僉�C�t�ύX
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
		// �}�V���K���U���͌����ɂ���( 0.9~4.5 )
		eventId.m_eventValue *= 0.9f - levelReduce;
		break;
	case Common::EVENT_HIT_BLADE_PLAYER:
		// �a���̓_���[�W�𑝂₷
		eventId.m_eventValue *= 1.2f - levelReduce;
		break;
	}
}