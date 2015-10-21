/* ====================================================================== */
/**
 * @brief  �GWizard�N���XAI
 *
 * @note
 *		�f�t�H���gAI
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_DRAGON__
#define __ENEMY_AI_DRAGON__

#include "EnemyAIBase.h"
#include "Game/Enemy/EnemyWizard.h"
#include "System/Sound/SystemSoundManager.h"

class DragonFireBomb;

class AIDragon : public EnemyAIBase
{
public:

	// ���̊e�s���Ɉڂ�܂ł̑҂�����
	static const uint32_t ACTION_INTERVAL_SHORT = 30;
	static const uint32_t ACTION_INTERVAL = 60;
	static const uint32_t ACTION_INTERVAL_LONG = 100;

	static AIDragon *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_DRAGON; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����
	virtual void EnemyRecievedEvent( const Common::CMN_EVENT &eventInfo ) override;			// �G�N���X�����̃N���X�����������C�x���g

private:

	enum ACTION_TYPE{
		ACTION_MOVE_AWAY,		// ������悤�Ȉړ�
		ACTION_FIRE_BOMB,		// ���̒e����
		ACTION_LARGE_EXPLOSION,	// �唚��

		ACTION_MAX,
	};

	ACTION_TYPE GetRandamNextAction();
	void ChangeActionType( const ACTION_TYPE &nextAction );
	void SetInterruptAction( const ACTION_TYPE &interruptAction );

	// ���肩�瓦���鎞�̖ړI�n�Z�b�g
	bool SetRunAwayPosFromPlayer( TEX_DRAW_INFO &enemyInfo );

	// �ݒ肵���ړI�n�܂ňړ�(true���A���Ă���ΏI��)
	bool DashMove( TEX_DRAW_INFO &enemyInfo, const math::Vector2 &targetPos );

	// �e�s���ɑΉ������֐�
	// �R���[�`�����g�p���Ă���֐��̐錾�A��`�͓����t�@�C���ɏ����Ȃ��ƃG���[���o��͗l
	bool ActionShootFireBall( TEX_DRAW_INFO &enemyInfo, const bool &onlyAction = false );
	bool ActionFireWall( TEX_DRAW_INFO &enemyInfo ){

		static uint32_t fireCounter = 0;

		reenter( m_coro ){
			// ����O�G�t�F�N�g
			GameEffect::CreateEffect( GameEffect::EFFECT_PRE_FIRE_WALL, enemyInfo.m_posOrigin );
			m_waitCounter = ACTION_INTERVAL_LONG;
			yield return false;
			
			// ����G�t�F�N�g
			GameEffect::CreateEffect( GameEffect::EFFECT_FIRE_WALL, WINDOW_CENTER );
			SoundManager::GetInstance()->PlaySE("PreExplosion");

			// ������(���x������)
			for( fireCounter = 0 ; fireCounter < GetEnemyLevel() ; ++fireCounter ){
				GameEffectWithCollision::CreateEffect( Common::OWNER_ENEMY, GameEffectWithCollision::EFFECT_FIRE, Utility::GetMapRandamPos( /*bool allowInWindow=*/true ) );
				m_waitCounter = 5;
				yield return false;
			}
		}

		fireCounter = 0;

		// ���̍s���Z�b�g
		if( math::IsInRange( Utility::GetPlayerPos(), enemyInfo.m_posOrigin, 400.0f ) )
		{
			// �߂��Ƀv���C���[������Ȃ瓦����
			ChangeActionType( ACTION_MOVE_AWAY );
		}
		else{
			ChangeActionType( GetRandamNextAction() );
		}
		
		m_waitCounter = ACTION_INTERVAL_LONG;
		return true;
	}

	bool ActionAwayFromPlayer( TEX_DRAW_INFO &enemyInfo ){

		reenter( m_coro ){

			// �ړ���Z�b�g
			SetRunAwayPosFromPlayer( enemyInfo );
			yield return false;

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
		ChangeActionType( GetRandamNextAction() );
		m_waitCounter = ACTION_INTERVAL_LONG;
		return true;
	}



	AIDragon(void);
	~AIDragon(void);

	ACTION_TYPE		m_currAction;
	ACTION_TYPE		m_nextAction;	// ���ɃZ�b�g���ꂽ�s��
	math::Vector2	m_movingPos;	// �ړ�����Ƃ��̖ړI�n
	uint32_t		m_waitCounter;	// �A���œ����s�������Ȃ����߂̃J�E���^

	coroutine		m_coro;			// �R���[�`��
};


class DragonFireBomb : public TaskUnit, public Collision2DUnit
{
public:

	static DragonFireBomb *Create( const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage );

protected:


	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;

	// �ق��̃N���X����̃C�x���g����
	virtual void EventUpdate( Common::CMN_EVENT &eventId ) override;

	virtual const Common::TYPE_OBJECT GetTypeObject() const;

private:

	DragonFireBomb( const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage );
	~DragonFireBomb(void);
	
	uint32_t	m_liveTime;		// �������Ă���̎���
	uint32_t	m_liveTimeMax;	// ������̍ő吶������

	math::Vector2	m_vec;
	uint32_t		m_damage;

};

#endif