/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyBase.h"
#include "EnemyManager.h"

#include "Common/CommonDefine.h"
#include "Common/Utility/CommonGameUtility.h"
#include "Game/GameRegister.h"
#include "Game/Effect/GameEffect.h"
#include "Game/GameScoreRecorder.h"
#include "System/Sound/SystemSoundManager.h"

EnemyBase::EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind )
: m_uniqueIdOfEnemyAll( uniqueId )
, m_enemyKind( kind )
, m_HP(1)
, m_eye(math::Vector2())
, m_pEnemyAI( NULL )
, m_pEnemy2D( NULL ) 
, m_nextAI( Common::AI_MAX )
{
	m_pEnemy2D = NEW Game2DBase( jsonName.c_str() );

	//!�������Z�b�g
	m_enemyInfo.Init();
	m_enemyInfo.m_fileName = jsonName;
}

EnemyBase::~EnemyBase()
{
	SAFE_DELETE(m_pEnemy2D);
}

void EnemyBase::ChangeAIState( const Common::ENEMY_AI &nextAI )
{


}

void EnemyBase::UpdateEnemy()
{
	if( m_pEnemyAI && m_nextAI != Common::AI_MAX )
	{
		SAFE_DELETE( m_pEnemyAI );
		m_pEnemyAI = ChangeEnemyAI( m_nextAI );
		m_nextAI = Common::AI_MAX;
	}

	if( m_pEnemyAI ){
		m_pEnemyAI->Exec( m_enemyInfo );
	}

	// AI�ɂ���čX�V���ꂽ�l�𔽉f
	m_enemyInfo.m_pos += 0.5f;
	m_pEnemy2D->SetDrawInfo( m_enemyInfo );
}

void EnemyBase::DrawEnemy()
{
	// AI������Ȃ�X�V
	//if( m_pEnemyAI ){
		m_pEnemy2D->DrawUpdate2D();
	//}
}

/* ================================================ */
/**
 * @brief	���N���X����̃C�x���g����
 */
/* ================================================ */
void EnemyBase::EventUpdate( const Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	default:

		break;

	case Common::EVENT_HIT_BULLET:	// Player�̒e�ɓ�������
		HitPlayreBullet();
		break;

	}
}

/* ================================================ */
/**
 * @brief	�`����擾
 */
/* ================================================ */
const TEX_DRAW_INFO &EnemyBase::GetDrawInfo()
{
	return m_enemyInfo;
}

/* ================================================ */
/**
 * @brief	�C�x���g�ɑΉ������֐��Q
 */
/* ================================================ */

// �v���C���[�̒e�ɓ�������
void EnemyBase::HitPlayreBullet()
{
	--m_HP;

	if( m_HP <= 0 ){
		// �X�R�A�ǉ�
		ScoreRecorder::GetInstance()->AddScore(123);

		// ���j�G�t�F�N�g���o��
		GameEffect *effect = NEW GameEffect( GameEffect::EFFECT_BOMB, static_cast<uint32_t>(m_enemyInfo.m_pos.x), static_cast<uint32_t>(m_enemyInfo.m_pos.y) );

		// ����SE�炷
		SoundManager::GetInstance()->PlaySE("Bomb");

		// manager�ɊǗ�����O���悤�ɓ`����
		GameRegister::GetInstance()->GetManagerEnemy()->DeleteEnemy( GetUniqueNumber() );
	}
}