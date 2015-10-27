/* ====================================================================== */
/**
 * @brief  RangerAI�N���X
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#ifndef __AI_ENEMY_RANGER__
#define __AI_ENEMY_RANGER__

#include "Game/Enemy/AI/EnemyAIBase.h"
#include "Game/Enemy/EnemyRanger.h"
#include "Common/Utility/CommonGameUtility.h"

class AIRanger : public EnemyAIBase
{
public:

	// ���̊e�s���Ɉڂ�܂ł̑҂�����
	static const uint32_t ACTION_INTERVAL_SHORT = 40;
	static const uint32_t ACTION_INTERVAL = 60;

	static AIRanger *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_NEAR; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����

private:

	enum ACTION_TYPE{
		ACTION_SPREAD_BULLET,
		ACTION_SLASHING,
		ACTION_MOVE,

		ACTION_MAX,
	};

	void ChangeActionType( const ACTION_TYPE &nextType );

	// ���̍s�����擾
	ACTION_TYPE GetNextAction();
	
	// �R���[�`���ŌĂё����邱�ƂŖړI�n�܂ł̈ړ����s�� true�ŖړI�n�ɓ��B
	bool DashMove( TEX_DRAW_INFO &enemyInfo, const math::Vector2 &targetPos );

	// �V���b�g�K���I�ȍU��
	bool ExecSpreadBullet( TEX_DRAW_INFO &enemyInfo );

	// �ړ��I�ȍU��
	bool ExecMoving( TEX_DRAW_INFO &enemyInfo )
	{
		reenter( m_coro ){

			// �����_���Ȉʒu��ړI�n�ɂ���
			m_movingPos = Utility::GetMapRandamPos( /*bool allowInWindow=*/true );

			// �ړ�
			for(;;){
				if( DashMove( enemyInfo, m_movingPos ) ){
					break;
				}
				if( GetNowCount()%5 == 0 ){
					ShootBullet( enemyInfo.m_posOrigin, math::Vector2(), 45, 12 );
				}
				yield return false;
			}
		}
		// �ړI�n������
		m_movingPos = math::Vector2();

		// ���̍s���Z�b�g
		ChangeActionType( GetNextAction() );
		m_actionIntervalTime = Utility::GetRandamValue( ACTION_INTERVAL, ACTION_INTERVAL_SHORT );
		return true;
	}

	// �v���C���[���狗������鎞�̖ړI�n�ݒ�
	bool SetRunAwayPosFromPlayer( TEX_DRAW_INFO &enemyInfo );

	// �a���U��
	bool ExecSlashing( TEX_DRAW_INFO &enemyInfo )
	{
		uint32_t currTime = GetNowCount();
		const EnemyRanger *const pBoss = static_cast<const EnemyRanger*const>( GetEnemeyMine() );

		reenter( m_coro ){

			// ���݂̃v���C���[�ʒu��ړI�n��
			m_movingPos = Utility::GetPlayerPos();

			// �v���C���[�ɋ}�ڋ�
			for(;;){
				if( DashMove( enemyInfo, m_movingPos ) ){
					break;
				}
				yield return false;
			}

			// �a���U��
			Slashing( Slashing::TYPE_3RD, pBoss->GetSlashingDamage() );
			yield return false;

			// ���������ڕW�ƂȂ�ʒu�ݒ�
			SetRunAwayPosFromPlayer( enemyInfo );

			// ���������
			for(;;){
				if( DashMove( enemyInfo, m_movingPos ) ){
					break;
				}
				yield return false;
			}
		}
		// �ړI�n������
		m_movingPos = math::Vector2();
		
		// ���̍s���Z�b�g
		ChangeActionType( GetNextAction() );
		m_actionIntervalTime = Utility::GetRandamValue( ACTION_INTERVAL, ACTION_INTERVAL_SHORT );
		return true;
	}

	AIRanger(void);
	~AIRanger(void);

	coroutine		m_coro;					// �R���[�`��
	uint32_t		m_actionIntervalTime;	// �A�N�V�������N�����Ă��玟�̃A�N�V�����܂ł̗]�Ɏ���

	ACTION_TYPE		m_currActionType;		// ���݂̃A�N�V����
	ACTION_TYPE		m_nextActionType;		// ���ɐݒ肳��Ă���A�N�V����

	// ��Ɏa���Ŏg�p���郁���o
	uint32_t		m_currActionTime;		// ���݂̃A�N�V�����^�C�v�ɂȂ��Ă���̎���

	// �a�����̈ړ����Ɏg�p����ړI�n
	math::Vector2	m_movingPos;
};

#endif