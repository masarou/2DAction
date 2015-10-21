/* ====================================================================== */
/**
 * @brief  �GWizard�N���XAI
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_WIZARD__
#define __ENEMY_AI_WIZARD__

#include "EnemyAIBase.h"
#include "Game/Enemy/EnemyWizard.h"

class AIWizard : public EnemyAIBase
{
public:

	// ���̊e�s���Ɉڂ�܂ł̑҂�����
	static const uint32_t ACTION_INTERVAL_SHORT = 60;
	static const uint32_t ACTION_INTERVAL = 90;

	static AIWizard *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_WIZARD; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����
	virtual void EnemyRecievedEvent( const Common::CMN_EVENT &eventInfo ) override;			// �G�N���X�����̃N���X�����������C�x���g

private:

	enum ACTION_TYPE{
		ACTION_NORMAL,				// �P���̒e������
		ACTION_CONTINUE_BULLET,		// �����̒e��A�����Č���
		ACTION_WAY_BULLET,			// ����Ɉ�萔��e����x�ɕ���
		ACTION_SET_CRYSTAL_PLAYER,	// ����v���C���[�ɃN���X�^�������

		ACTION_WORP,				// ���[�v�ړ�

		ACTION_MAX,
	};

	ACTION_TYPE GetRandamNextAction();
	void ChangeActionType( const ACTION_TYPE &nextAction );
	void SetInterruptAction( const ACTION_TYPE &interruptAction );
	bool ActionNormal();
	bool ActionSetCrystalPlayer();
	bool ActionWayBullet();

	// �R���[�`�����g�p���Ă���֐��̐錾�A��`�͓����t�@�C���ɏ����Ȃ��ƃG���[���o��͗l
	bool ActionContinueBullet(){
		const EnemyWizard *const pWizard = static_cast<const EnemyWizard*const>( GetEnemeyMine() );
		reenter( m_coro ){
			for( uint32_t i = 0; i < pWizard->GetCrystalIndex() ; ++i ){
				ShootBullet( pWizard->GetCrystalPos( i ), math::Vector2(), pWizard->GetBulletDamage() );
			}
			m_waitCounter = 5;
			yield return false;
			
			for( uint32_t i = 0; i < pWizard->GetCrystalIndex() ; ++i ){
				ShootBullet( pWizard->GetCrystalPos( i ), math::Vector2(), pWizard->GetBulletDamage() );
			}
			m_waitCounter = 5;
			yield return false;
			
			for( uint32_t i = 0; i < pWizard->GetCrystalIndex() ; ++i ){
				ShootBullet( pWizard->GetCrystalPos( i ), math::Vector2(), pWizard->GetBulletDamage() );
			}
			m_waitCounter = 5;
			yield return false;
			
			for( uint32_t i = 0; i < pWizard->GetCrystalIndex() ; ++i ){
				ShootBullet( pWizard->GetCrystalPos( i ), math::Vector2(), pWizard->GetBulletDamage() );
			}
			m_waitCounter = 5;
			yield return false;
			
			for( uint32_t i = 0; i < pWizard->GetCrystalIndex() ; ++i ){
				ShootBullet( pWizard->GetCrystalPos( i ), math::Vector2(), pWizard->GetBulletDamage() );
			}
			m_waitCounter = ACTION_INTERVAL_SHORT;
		}
		
		ChangeActionType( GetRandamNextAction() );

		return true;
	}

	bool ActionEnemyWorp( TEX_DRAW_INFO &enemyInfo ){
		reenter( m_coro ){
			
			m_waitCounter = Utility::GetRandamValue( 20, 10 );
			yield return false;

			// �����_���Ȉʒu���Z�b�g
			enemyInfo.m_posOrigin = Utility::GetMapRandamPos( /*bool allowInWindow=*/true );

			// ��莞�Ԍ�ړ���őҋ@
			m_waitCounter = 20;
			GameEffect::CreateEffect( GameEffect::EFFECT_WORP, enemyInfo.m_posOrigin );
			yield return false;

			// ���̃A�N�V�����Z�b�g
			ChangeActionType( GetRandamNextAction() );
		}
		return true;
	}

	AIWizard(void);
	~AIWizard(void);

	ACTION_TYPE		m_currAction;
	ACTION_TYPE		m_nextAction;		// ���ɃZ�b�g���ꂽ�s��
	math::Vector2	m_nextWorpPos;	// ���[�v���̈ړ���
	uint32_t		m_waitCounter;	// �A���œ����s�������Ȃ����߂̃J�E���^
	coroutine		m_coro;			// �R���[�`��
};


#endif