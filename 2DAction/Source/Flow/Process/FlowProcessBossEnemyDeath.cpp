/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�{�X�N���X���S�����o�N���X
 */
/* ====================================================================== */

#include "FlowProcessBossEnemyDeath.h"
#include "System/Sound/SystemSoundManager.h"

#include "System/SystemFPSManager.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Game/Effect/GameEffect.h"

#define EFFECT_TIME 200

ProcessBossEnemyDeath *ProcessBossEnemyDeath::Create( const math::Vector2 &centerPos )
{
	return NEW ProcessBossEnemyDeath( centerPos );
}

ProcessBossEnemyDeath::ProcessBossEnemyDeath( const math::Vector2 &centerPos )
: m_centerPos( centerPos )
, m_effectCounter( 0 )
{
}


ProcessBossEnemyDeath::~ProcessBossEnemyDeath(void)
{
}

bool ProcessBossEnemyDeath::CanDie()
{
	return true;
}

bool ProcessBossEnemyDeath::Init()
{

	return true;
}


void ProcessBossEnemyDeath::Update()
{
	if( Utility::GetRandamValue( EFFECT_TIME, 0 ) < static_cast<int>( m_effectCounter/2 ) )
	{
		math::Vector2 pos = m_centerPos;
		pos.x += Utility::GetRandamValue( 150, -150 );
		pos.y += Utility::GetRandamValue( 150, -150 );
		GameEffect::CreateEffect( GameEffect::EFFECT_BOMB, pos );

		// ���ʉ�
		SoundManager::GetInstance()->PlaySE("Death");
	}
	
	++m_effectCounter;

	if( m_effectCounter < EFFECT_TIME ){
		return;
	}

	// �G�t�F�N�g�I��
	SetStateNext();
}
