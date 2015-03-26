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
#include "Game/GameRecorder.h"
#include "System/Sound/SystemSoundManager.h"

AttackGun *EnemyBase::s_pAttackGun = NULL;

EnemyBase::EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind )
: TaskUnit("Enemy")
, Collision2DUnit( jsonName.c_str() )
, m_uniqueIdOfEnemyAll( uniqueId )
, m_enemyKind( kind )
, m_HP(10)
, m_eye(math::Vector2( 1.0f, 0.0f ))
, m_walkHeight( 0 )
, m_stunTime( 0 )
, m_pEnemyAI( NULL )
, m_nextAI( Common::AI_NONE )
, m_prevAI( Common::AI_NONE )
{
	m_actionInfoAI.Init();

	m_drawTexture.m_texInfo.m_fileName = jsonName;
	m_drawTexture.m_pTex2D->SetDrawInfo( m_drawTexture.m_texInfo );

	m_textureLife.Init();
	m_textureLife.m_pTex2D = NEW Game2DBase( "enemyGauge.json" );
	m_textureLife.m_texInfo.m_fileName = "enemyGauge.json";
	m_textureLife.m_pTex2D->SetDrawInfo( m_textureLife.m_texInfo );
}

EnemyBase::~EnemyBase()
{
}

bool EnemyBase::Init()
{
	m_HP		= GetEnemyDefaultHP();

	// ���O��AI���Ȃ��̂œ���AI�ɂ��Ă���
	m_prevAI	= Common::AI_SEARCHING;
	m_nextAI	= Common::AI_SEARCHING;

	if( !m_pEnemyAI ){
		m_pEnemyAI = Utility::CreateEnemyAI( m_nextAI );
	}

	if( !s_pAttackGun ){
		// �U���e�쐬�B���L+�����_���Ŕ��˂Ȃ̂Ŕ��˂̃C���^�[�o���͂Ȃ�
		s_pAttackGun = AttackGun::CreateGun( Common::OWNER_ENEMY );
		AttackGun::GunState &gunStatus = s_pAttackGun->UpdateGunState();
		gunStatus.m_shootInterval = 0;
	}

	return InitMain();
}

bool EnemyBase::DieMain()
{
	if( !DieMainCustom() ){
		return false;
	}

	if( GameRegister::GetInstance()->GetManagerEnemy() ){
		GameRegister::GetInstance()->UpdateManagerEnemy()->RemoveEnemy( this );
	}
	
	m_textureLife.DeleteAndInit();
	SAFE_DELETE( m_pEnemyAI );

	// �|���ꂽ�G���J�E���g���Ă���
	GameRegister::GetInstance()->UpdateManagerGame()->AddDestroyCount();

	return true;
};

/* ================================================ */
/**
 * @brief	�e�t�F�[�Y�̍X�V�֐�
 */
/* ================================================ */
void EnemyBase::Update()
{
	if( m_stunTime == 0 ){
		if( m_pEnemyAI && m_nextAI != Common::AI_MAX )
		{
			// AI�ύX
			m_prevAI = m_pEnemyAI->GetAIKind();
			SAFE_DELETE( m_pEnemyAI );
			m_pEnemyAI = Utility::CreateEnemyAI( m_nextAI );
			m_pEnemyAI->SetThingingEnemy(this);
			m_nextAI = Common::AI_MAX;
		}

		if( m_pEnemyAI ){
			m_pEnemyAI->Exec( m_drawTexture.m_texInfo, m_actionInfoAI );
		}

		// AI�ɂ���čX�V���ꂽ�l�𔽉f
		m_drawTexture.m_pTex2D->SetDrawInfo( m_drawTexture.m_texInfo );

		// AI�ɂ���Đݒ肳�ꂽ�s����ݒ�
		RefrectAIAction();
	}
	else{
		// �X�^����ԂȂ�f�N�������g
		--m_stunTime;
	}

	// HP�`�揀��
	m_textureLife.m_texInfo.m_posOrigin.x = m_drawTexture.m_texInfo.m_posOrigin.x;
	m_textureLife.m_texInfo.m_posOrigin.y = m_drawTexture.m_texInfo.m_posOrigin.y + 30.0f;
	m_textureLife.m_texInfo.m_scale.x = ( m_HP/static_cast<float>(GetEnemyDefaultHP()) )*10.0f;
	m_textureLife.m_pTex2D->SetDrawInfo( m_textureLife.m_texInfo );
}

void EnemyBase::DrawUpdate()
{
	m_drawTexture.m_pTex2D->DrawUpdate2D();
	m_textureLife.m_pTex2D->DrawUpdate2D();
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

	case Common::EVENT_HIT_BULLET_PLAYER:	// Player�̒e�ɓ�������
		HitPlayreBullet( eventId.m_eventValue );
		break;

	case Common::EVENT_HIT_BLADE_PLAYER:	// Player�̎a���ɓ�������
		HitPlayreSlashing( eventId.m_eventValue );
		break;

	case Common::EVENT_SHOOT_BULLET:
		if( s_pAttackGun ){
			math::Vector2 direction = Utility::GetPlayerPos() - GetDrawInfo().m_posOrigin;
			direction.Normalize();
			s_pAttackGun->ShootBullet( GetDrawInfo().m_posOrigin, direction );
		}
		break;
	}
}

/* ================================================ */
/**
 * @brief	�`����擾
 */
/* ================================================ */
const TEX_DRAW_INFO &EnemyBase::GetDrawInfo() const
{
	return m_drawTexture.m_texInfo;
}

/* ================================================ */
/**
 * @brief	�C�x���g�ɑΉ������֐��Q
 */
/* ================================================ */

// �v���C���[�̒e�ɓ�������
void EnemyBase::HitPlayreBullet( const uint32_t &damageValue )
{
	UpdateEnemyDamage( damageValue );
}
// �v���C���[�̎a���ɓ�������
void EnemyBase::HitPlayreSlashing( const uint32_t &damageValue )
{
	m_stunTime = 10;
	UpdateEnemyDamage( damageValue );
}

/* ================================================ */
/**
 * @brief	AI���ʂ𔽉f
 */
/* ================================================ */
void EnemyBase::RefrectAIAction()
{
	// �i�[���ꂽ�C�x���g���b�Z�[�W���Z�b�g
	for( uint32_t i = 0; i < m_actionInfoAI.m_pushEventArray.size(); ++i){
		Common::CMN_EVENT eventInfo;
		eventInfo.Init();
		eventInfo.m_event = m_actionInfoAI.m_pushEventArray.at(i).m_EventMessage;
		SystemMessageManager::GetInstance()->PushMessage( GetUniqueId(), eventInfo );
	}
	m_actionInfoAI.m_pushEventArray.clear();

	//switch( m_actionInfoAI.m_AItype ){
	//default:

	//	break;
	//case AI_SHOOT_BULLET:
	//	// m_actionInfoAI.m_AIInfo����������o���Ă��낢��s��
	//	break;
	//}
}

/* ================================================ */
/**
 * @brief	�_���[�W���ʏ���
 */
/* ================================================ */
void EnemyBase::UpdateEnemyDamage( const uint32_t &damageValue )
{
	if( m_HP <= damageValue ){
		m_HP = 0;
	}
	else{
		m_HP -= damageValue;
	}

	//�_���[�W�G�t�F�N�g�쐬
	GameEffectDamage::GetInstance()->CreateEffectDamage( damageValue
		, static_cast<uint32_t>(m_drawTexture.m_texInfo.m_posOrigin.x)
		, static_cast<uint32_t>(m_drawTexture.m_texInfo.m_posOrigin.y));

	if( m_HP <= 0 ){
		// �X�R�A�ǉ�
		Common::ENEMY_KIND kind = GetKind();
		switch( kind ){
		case Common::ENEMY_KIND_AAA:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_AAA_DEATH );
			break;
		case Common::ENEMY_KIND_BBB:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_BBB_DEATH );
			break;
		case Common::ENEMY_KIND_CCC:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_CCC_DEATH );
			break;
		case Common::ENEMY_KIND_BOSS:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_BOSS_DEATH );
			break;
		}

		// ���j�G�t�F�N�g���o��
		GameEffect *effect = NEW GameEffect( GameEffect::EFFECT_BOMB, static_cast<uint32_t>(m_drawTexture.m_texInfo.m_posOrigin.x), static_cast<uint32_t>(m_drawTexture.m_texInfo.m_posOrigin.y) );

		// ����SE�炷
		SoundManager::GetInstance()->PlaySE("Death");

		// ���S
		TaskStartDie();
	}

	// �A��Hit�����Z
	GameRecorder::GetInstance()->IncHitCounter();
}
