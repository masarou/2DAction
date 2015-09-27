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

	static AIWizard *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_WIZARD; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����

private:

	enum ACTION_TYPE{
		ACTION_NORMAL,
		ACTION_CONTINUE_BULLET,
		ACTION_WAY_BULLET,
		ACTION_SET_CRYSTAL_PLAYER,

		ACTION_MAX,
	};

	void ChangeActionType( const ACTION_TYPE &nextAction );
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
			m_waitCounter = 60;
		}
		
		uint32_t next = Utility::GetRandamValue( ACTION_MAX, 0 );
		ChangeActionType( static_cast<ACTION_TYPE>(next) );

		return true;
	}

	AIWizard(void);
	~AIWizard(void);

	ACTION_TYPE		m_currAction;
	ACTION_TYPE		m_nextAction;
	uint32_t		m_waitCounter;	// �A���œ����s�������Ȃ����߂̃J�E���^
	coroutine		m_coro;			// �R���[�`��
};


#endif