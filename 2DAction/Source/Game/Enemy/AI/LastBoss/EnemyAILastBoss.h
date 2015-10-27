/* ====================================================================== */
/**
 * @brief  ���X�{�XAI
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_LAST_BOSS__
#define __ENEMY_AI_LAST_BOSS__

#include "Game/GameRegister.h"
#include "Game/Enemy/AI/EnemyAIBase.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Sound/SystemSoundManager.h"

class LastBossRight;
class LastBossLeft;

class EnemyAILastBoss : public EnemyAIBase
{
public:

	static EnemyAILastBoss *Create();

	// AI�̎�ނ�h����ŃZ�b�g���Ă���
	virtual const Common::ENEMY_AI GetAIKind() const{ return Common::AI_ATTACK_LAST_BOSS; }

protected:
	
	virtual bool InitAI() override;								// AI������
	virtual void ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo ) override;	// �h����ł�AI����
	virtual void EnemyIsDead();

private:

	void ExecHandsUpdate( TEX_DRAW_INFO &enemyInfo );

	EnemyAILastBoss(void);
	virtual ~EnemyAILastBoss(void);

	// ���삷�闼��N���X
	LastBossRight	*m_pRightHand;
	LastBossLeft	*m_pLeftHand;

	// �ӂ�ӂ핂���̂ŁA�`��Ƃ͕ʂɊ�ƂȂ�ʒu������
	math::Vector2 m_moveBasicPos;

};


/* ====================================================================== */
/**
 * @brief  ���X�{�X�E�荶��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */
class LastBossHand : public TaskUnit, public Collision2DUnit
{
public:

	enum ACTION_KIND{
		ACTION_NONE,	// �������Ȃ�(��ʒu��)
		ACTION_FIST,	// �p���`
		ACTION_GUARD,	// �{�̂��K�[�h
		ACTION_SUMMON,	// �����X�^�[����(���j�[�N�L����)
		ACTION_SUMMONS,	// �����X�^�[����(�G���L����)
		ACTION_MAX,
	};

	void SetBasicPos( math::Vector2 basicPos );

	// �w�肳��Ă���A�N�V�������s(�I��������true)
	bool ExecAction();

	void SetAnotherHand( LastBossHand *pHand );

	// ���݂̎��s�A�N�V�������擾
	const ACTION_KIND &GetCurrentActionKind() const{ return m_currAction; }

	// ���S���Ƀ{�XAI����Ă΂��
	void StartDie(){ TaskStartDie(); }

protected:

	LastBossHand( const std::string &readFileName, const math::Vector2 &enemyPos );
	virtual ~LastBossHand(void);

	virtual bool Init() override;
	virtual void Update() override;			// �ړ����̓������l�̍X�V
	virtual void DrawUpdate() override;		// �`��X�V
	virtual bool DieMain() override{return true;}

	// �s������Ƃ��̉�]�ʂ��擾
	virtual float GetRotateDefault() = 0;
	virtual float GetRotActionFist() = 0;
	virtual float GetRotActionSummon() = 0;

	// �e��A�N�V�������ł̉E��A����p�ʒu�����I�t�Z�b�g
	virtual math::Vector2 GetSummonEffectOffset() = 0;
	
	// ���j�[�N�����X�^�[�𐶐�������(����ŋ���)
	static uint32_t	m_createUniqueEnemyCount;

private:

	// �p���`�A�N�V����
	virtual bool ExecFist( TEX_DRAW_INFO &drawInfo )
	{
		math::Vector2 currentOffset = drawInfo.m_posOrigin - m_basicPos;

		reenter( m_coro ){

			// �r�̐U��グ
			for(;;){
				if( MoveToTargetPos( 
					m_basicPos + math::Vector2( -100.0f, -150.0f )
					, 15.0f
					, 0.1f )
					){
					m_waitCounter = 30;
					break;
				}
				yield return false;
			}
			
			// �U�肨�낵������߂�
			m_moveTargetPos = Utility::GetPlayerPos();
			m_waitCounter = 10;
			yield return false;

			// �ړI�n�܂ňړ�
			for(;;){
				if( MoveToTargetPos( m_moveTargetPos, 20.0f, 0.4f ) ){
					m_waitCounter = 30;
					break;
				}
				yield return false;
			}

			// ���̈ʒu�ɖ߂�
			for(;;){
				if( MoveToTargetPos( m_basicPos, 10.0f, 0.3f ) ){
					m_waitCounter = 120;
					break;
				}
				yield return false;
			}

			drawInfo.m_rot = GetRotateDefault();
			m_moveTargetPos = math::Vector2();
		}
		
		return true;
	}

	
	// �����X�^�[����(���j�[�N)
	virtual bool ExecSummonMonster( TEX_DRAW_INFO &drawInfo )
	{
		reenter( m_coro ){
			// �����_���Ȉʒu�Ɉړ�
			for(;;){
				m_moveTargetPos = Utility::GetMapRandamPos( /*bool allowInWindow =*/true );
				// ���΂̎肪����ʒu�ɍs���ƕs���R�Ȃ̂ō�����ʒu���Ɍ���
				if( ( GetTypeObject() == Common::TYPE_LAST_BOSS_LEFT
					&& m_moveTargetPos.x < drawInfo.m_posOrigin.x )
					|| ( GetTypeObject() == Common::TYPE_LAST_BOSS_RIGHT
					&& m_moveTargetPos.x > drawInfo.m_posOrigin.x ) )
				{
					// �ړ��ł���ꏊ���`�F�b�N
					if( Utility::GetMapHeight( m_moveTargetPos + GetSummonEffectOffset() ) == 0){
						break;
					}
				}
			}
			for(;;){
				if( MoveToTargetPos( m_moveTargetPos, 5.0f, 0.3f ) ){
					m_waitCounter = 30;
					RotateToTargetAngle( GetRotActionSummon(), /*bool isForceSet=*/true );
					break;
				}
				else if( drawInfo.m_rot != GetRotActionSummon() ){
					// �ړ������R�Ȋp�x�ɉ�]������
					RotateToTargetAngle( GetRotActionSummon() );
				}
				yield return false;
			}

			// �����G�t�F�N�g
			GameEffect::CreateEffect( GameEffect::EFFECT_SUMMON, drawInfo.m_posOrigin + GetSummonEffectOffset() );
			m_waitCounter = 20;
			yield return false;

			// �����X�^�[����
			GameRegister::GetInstance()->UpdateManagerGame()->CreateEnemy( DecideCreateMonster(), 0, true, drawInfo.m_posOrigin + GetSummonEffectOffset() );
			SoundManager::GetInstance()->PlaySE("Summon");

			// ���̈ʒu�ɖ߂�
			for(;;){
				if( MoveToTargetPos( m_basicPos, 10.0f, 0.3f ) ){
					m_waitCounter = 30;
					RotateToTargetAngle( GetRotateDefault(), /*bool isForceSet=*/true );
					break;
				}
				else if( drawInfo.m_rot != GetRotateDefault() ){
					// �ړ������R�Ȋp�x�ɉ�]������
					RotateToTargetAngle( GetRotateDefault() );
				}
				yield return false;
			}
		}
		m_waitCounter = 120;
		return true;
	}

	
	// �G�������X�^�[����
	bool ExecSummonLightMonsters( TEX_DRAW_INFO &drawInfo )
	{
		reenter( m_coro ){

			// ��萔�����X�^�[�𐶐�
			for(;;){

				// �����_���Ȉʒu�Ɉړ�
				for(;;){
					m_moveTargetPos = Utility::GetMapRandamPos( /*bool allowInWindow =*/true );
					// ���΂̎肪����ʒu�ɍs���ƕs���R�Ȃ̂ō�����ʒu���Ɍ���
					if( ( GetTypeObject() == Common::TYPE_LAST_BOSS_LEFT
						&& m_moveTargetPos.x < drawInfo.m_posOrigin.x )
						|| ( GetTypeObject() == Common::TYPE_LAST_BOSS_RIGHT
						&& m_moveTargetPos.x > drawInfo.m_posOrigin.x ) )
					{
						// �ړ��ł���ꏊ���`�F�b�N
						if( Utility::GetMapHeight( m_moveTargetPos + GetSummonEffectOffset() ) == 0){
							break;
						}
					}
				}
				for(;;){
					if( MoveToTargetPos( m_moveTargetPos, 5.0f, 0.3f ) ){
						m_waitCounter = 30;
						RotateToTargetAngle( GetRotActionSummon(), /*bool isForceSet=*/true );
						break;
					}
					else if( drawInfo.m_rot != GetRotActionSummon() ){
						// �ړ������R�Ȋp�x�ɉ�]������
						RotateToTargetAngle( GetRotActionSummon() );
					}
					yield return false;
				}

				// �����G�t�F�N�g
				GameEffect::CreateEffect( GameEffect::EFFECT_SUMMON, drawInfo.m_posOrigin + GetSummonEffectOffset() );
				m_waitCounter = 20;
				yield return false;

				// �����X�^�[����
				GameRegister::GetInstance()->UpdateManagerGame()->CreateEnemy( DecideCreateLightMonster(), 0, true, drawInfo.m_posOrigin + GetSummonEffectOffset() );
				SoundManager::GetInstance()->PlaySE("Summon");

				// �����X�^�[�������s���ǂ���
				if( !IsCreateLightMonster() ){
					// �����X�^�[�����I��
					// ���̈ʒu�ɖ߂�
					for(;;){
						if( MoveToTargetPos( m_basicPos, 10.0f, 0.3f ) ){
							m_waitCounter = 30;
							RotateToTargetAngle( GetRotateDefault(), /*bool isForceSet=*/true );
							break;
						}
						else if( drawInfo.m_rot != GetRotateDefault() ){
							// �ړ������R�Ȋp�x�ɉ�]������
							RotateToTargetAngle( GetRotateDefault() );
						}
						yield return false;
					}
					break;
				}
				else{
					// �������������X�^�[����
				}
			}
		}
		m_waitCounter = 120;
		return true;
	}


	// ���̃A�N�V���������߂�
	ACTION_KIND GetNextActionKind();

	// �����X�^�[���쐬���Ă��悢���ǂ����`�F�b�N
	uint32_t CountUniqueMonster() const;
	bool IsCreateUniqueMonster() const;
	bool IsCreateLightMonster() const;

	// ���ɐ������郂���X�^�[������
	Common::ENEMY_KIND DecideCreateMonster() const;
	Common::ENEMY_KIND DecideCreateLightMonster() const;

	// �w���Ɉړ� true�œ��B
	bool MoveToTargetPos( const math::Vector2 &targetPos, const float &maxSpeed, const float &rateSpeed );

	// �w��̉�]�p�x�܂ň��ʂŉ�]
	bool RotateToTargetAngle( const math::Angle &targetDegree, const bool &forceSet = false );

	// ��_�ƂȂ�ʒu���
	math::Vector2	m_basicPos;

	// �ړ����Ă��鎞�̖ڕW�n�_
	math::Vector2	m_moveTargetPos;

	// ���̍s���Ɉڂ��܂ł̎���
	uint32_t		m_waitCounter;

	// �ێ����Ă�����s�A�N�V�����̎��
	ACTION_KIND		m_currAction;
	ACTION_KIND		m_nextAction;

	// �����Е��̘r�̃|�C���^
	LastBossHand	*m_pAnotherHand;
	
	// �R���[�`��
	coroutine		m_coro;

};

class LastBossRight : public LastBossHand
{
public:
	
	static LastBossRight *Create( const std::string &readFileName, const math::Vector2 &enemyPos );

protected:
	
	LastBossRight( const std::string &readFileName, const math::Vector2 &enemyPos );

	// �N���X�̓G�^�C�v
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_LAST_BOSS_LEFT; }

	// �s������Ƃ��̉�]�ʂ��擾
	virtual float GetRotateDefault() override{ return 20.0f; }
	virtual float GetRotActionFist() override{ return 0.0f; }
	virtual float GetRotActionSummon() override{ return 90.0f; }
	
	virtual math::Vector2 GetSummonEffectOffset() override{ return math::Vector2( -60.0f, 70.0f ); }

private:

	virtual ~LastBossRight(void);

};

class LastBossLeft : public LastBossHand
{
public:

	static LastBossLeft *Create( const std::string &readFileName, const math::Vector2 &enemyPos );

protected:
	
	LastBossLeft( const std::string &readFileName, const math::Vector2 &enemyPos );

	// �N���X�̓G�^�C�v
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_LAST_BOSS_RIGHT; }
		
	// �s������Ƃ��̉�]�ʂ��擾
	virtual float GetRotateDefault() override{ return -20.0f; }
	virtual float GetRotActionFist() override{ return 0.0f; }
	virtual float GetRotActionSummon() override{ return -90.0f; }

	virtual math::Vector2 GetSummonEffectOffset() override{ return math::Vector2( 60.0f, 70.0f ); }

private:

	virtual ~LastBossLeft(void);
};

#endif