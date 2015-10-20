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
#include "System/Draw2D/SystemDraw2DResource.h"

EnemyBase::EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
: TaskUnit("Enemy")
, Collision2DUnit( jsonName.c_str() )
, m_uniqueIdOfEnemyAll( uniqueId )
, m_enemyKind( kind )
, m_enemyLv( enemyLevel )
, m_HP(10)
, m_speed( 0 )
, m_eye(math::Vector2( 1.0f, 0.0f ))
, m_walkHeight( 0 )
, m_stunTime( 40 )
, m_pEnemyAI( NULL )
, m_nextAI( Common::AI_NONE )
, m_prevAI( Common::AI_NONE )
{
	if( m_enemyLv > 10 ){
		DEBUG_ASSERT( 0, "�G�̃��x�����z��O");
		m_enemyLv = 10;
	}

	m_actionInfoAI.Init();
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_fileName = jsonName;

	m_textureLife.Init();
	m_textureLife.m_pTex2D = Game2DBase::Create( "EnemyGauge.json" );
	m_textureLife.m_pTex2D->UpdateDrawInfo().m_fileName = "EnemyGauge.json";

	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = enemyPos;
}

EnemyBase::~EnemyBase()
{
}

bool EnemyBase::Init()
{
	m_HP		= GetEnemyDefaultHP();
	m_speed		= GetEnemyDefaultSPD();

	// ���O��AI���Ȃ��̂œ���AI�ɂ��Ă���
	m_prevAI	= GetEnemyDefaultAI();
	m_nextAI	= GetEnemyDefaultAI();

	if( !m_pEnemyAI ){
		m_pEnemyAI = Utility::CreateEnemyAI( m_nextAI );
		m_pEnemyAI->SetThingingEnemy(this);
	}

	// �����ʒu�Z�b�g
	if( m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin == DEFAULT_VECTOR2 ){
		for(;;){
			math::Vector2 candidatePos = Utility::GetMapRandamPos( /*allowInWindow=*/false );
			// �}�b�v��̓����鍂���Ȃ琶��
			if( Utility::GetMapHeight( candidatePos ) <= GetWalkHeight() ){
				m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = candidatePos;
				DEBUG_PRINT( "�G���� x = %f, y = %f\n", candidatePos.x, candidatePos.y );
				break;
			}
		}
	}

	return InitMain();
}

bool EnemyBase::DieMain()
{
	if( !DieMainCustom() ){
		return false;
	}

	// AI�ɂ����S�ʒm
	if( m_pEnemyAI ){
		m_pEnemyAI->EnemyIsDead();
	}

	if( GameRegister::GetInstance()->GetManagerEnemy() ){
		GameRegister::GetInstance()->UpdateManagerEnemy()->RemoveEnemy( this );
	}
	
	m_textureLife.DeleteAndInit();
	SAFE_DELETE( m_pEnemyAI );

	if( GetEnemyHitPoint() == 0 ){
		// �|���ꂽ�G���J�E���g���Ă���
		GameRegister::GetInstance()->UpdateManagerGame()->AddDestroyCount();
	}

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
			m_pEnemyAI->Exec( m_drawTexture.m_pTex2D->UpdateDrawInfo(), m_actionInfoAI );
		}

		// AI�ɂ���Đݒ肳�ꂽ�s����ݒ�
		RefrectAIAction();
	}
	else{
		// �X�^����ԂȂ�f�N�������g
		--m_stunTime;
	}

	// HP�`�揀��
	Game2DBase *pTex2D = m_drawTexture.m_pTex2D;
	if( pTex2D ){
		TEX_DRAW_INFO drawInfo;
		const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( pTex2D->GetDrawInfo().m_fileName.c_str() );
		drawInfo.m_posOrigin.x = pTex2D->GetDrawInfo().m_posOrigin.x - 5.0f;
		drawInfo.m_posOrigin.y = pTex2D->GetDrawInfo().m_posOrigin.y + (texInfo.m_sizeHeight / 2) + 10.0f;
		drawInfo.m_scale.x = ( m_HP/static_cast<float>(GetEnemyDefaultHP()) )*10.0f;
		m_textureLife.m_pTex2D->SetDrawInfo( drawInfo );
	}

	// HP���Ȃ��Ȃ玀�S
	if( m_HP <= 0 ){
		TaskStartDie();
	}
	else{
		// �h����X�V����
		UpdateCustom();
	}
}

void EnemyBase::DrawUpdate()
{
	m_drawTexture.m_pTex2D->DrawUpdate2D();
	m_textureLife.m_pTex2D->DrawUpdate2D();
	
	// �h����`�揈��
	DrawUpdateCustom();
}

/* ================================================ */
/**
 * @brief	���N���X����̃C�x���g����
 */
/* ================================================ */
void EnemyBase::EventUpdate( Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	default:

		break;

	case Common::EVENT_HIT_BULLET_PLAYER:	// Player�̒e�ɓ�������
		ReduceDamage( eventId );
		HitPlayreBullet( eventId.m_eventValue );
		break;

	case Common::EVENT_HIT_BLADE_PLAYER:	// Player�̎a���ɓ�������
		ReduceDamage( eventId );
		HitPlayreSlashing( eventId.m_eventValue );
		break;
	}

	if( m_pEnemyAI )
	{
		// AI�ɂ��C�x���g�����������Ƃ�`����
		m_pEnemyAI->EnemyRecievedEvent( eventId );
	}
}

/* ================================================ */
/**
 * @brief	�`����擾
 */
/* ================================================ */
const TEX_DRAW_INFO &EnemyBase::GetDrawInfo() const
{
	if( !m_drawTexture.m_pTex2D ){
		DEBUG_ASSERT( 0, "�K�v�ȃN���X���쐬����Ă��Ȃ�");
	}
	return m_drawTexture.m_pTex2D->GetDrawInfo();
}

/* ================================================ */
/**
 * @brief	���x���擾
 */
/* ================================================ */
const uint32_t &EnemyBase::GetEnemyLevel() const
{
	return m_enemyLv;
}

/* ================================================ */
/**
 * @brief	���݂�AI�擾
 */
/* ================================================ */
const Common::ENEMY_AI EnemyBase::GetCurrentAIKind() const
{
	Common::ENEMY_AI ai = Common::AI_MAX;
	if( m_pEnemyAI ){
		ai = m_pEnemyAI->GetAIKind();
	}
	return ai;
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
	SetStunTime( 10 );
	GameEffect::CreateEffect( GameEffect::EFFECT_SLASHING_HIT, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );
	UpdateEnemyDamage( damageValue );
}

/* ================================================ */
/**
 * @brief	HP���Ȃ��Ȃ�|���ꂽ�Ƃ��ɌĂ΂��
 */
/* ================================================ */
void EnemyBase::EnemyDeath()
{
	// ���j�G�t�F�N�g���o��
	GameEffect::CreateEffect( GameEffect::EFFECT_BOMB, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );

	// ����SE�炷
	SoundManager::GetInstance()->PlaySE("Death");

	// ���S
	TaskStartDie();
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

	uint32_t totalDamage = damageValue;
	float rate = static_cast<float>(Utility::GetRandamValue( 120, 80 )) / 100.0f;
	totalDamage *= rate;

	//�_���[�W�G�t�F�N�g�쐬
	GameEffectDamage::GetInstance()->CreateEffectDamage( totalDamage
		, static_cast<uint32_t>(m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin.x)
		, static_cast<uint32_t>(m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin.y));

	if( m_HP <= 0 ){
		// �X�R�A�ǉ�
		Common::ENEMY_KIND kind = GetKind();
		switch( kind ){
		case Common::ENEMY_KIND_SLIME:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_SLIME_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_SLIME_ANOTHER:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_SLIME_ANOTHER_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_AHRIMAN:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_AHRIMAN_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_COW:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_COW_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_RANGER:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_BOSS_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_SLIME_KING:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_SLIME_KING_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_WIZARD:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_WIZARD_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_DRAGON:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_DRAGON_DEATH, GetEnemyLevel() );
			break;
		case Common::ENEMY_KIND_LAST_BOSS:
			GameRecorder::GetInstance()->ScoreEvent( GameRecorder::ENEMY_LAST_BOSS_DEATH, GetEnemyLevel() );
			break;
		}

		if( GetStatus() != TASK_PRE_DIE ){
			// ���S
			EnemyDeath();
		}
	}

	if( damageValue != 0 ){
		// �A��Hit�����Z
		GameRecorder::GetInstance()->IncHitCounter();
	}
}
