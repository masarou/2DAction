/* ====================================================================== */
/**
 * @brief  �v���C���[�N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __SYSTEM_GAME_PLAYER__
#define __SYSTEM_GAME_PLAYER__

#include "System/Input/SystemInputWatcher.h"
#include "System/Collision/SystemCollisionUnit.h"
#include "Game/Game2DBase.h"
#include "Game/Item/ItemObject.h"
#include "Game/Effect/GameEffect.h"
#include "GamePlayerStatusMenu.h"

class AttackGun;
class AttackBlade;

class GamePlayer : public TaskUnit, public Collision2DUnit, public InputWatcher
{
public:

	static GamePlayer *CreatePlayer();
	~GamePlayer(void);

	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

	virtual void PadEventUp() override;
	virtual void PadEventDown() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;
	virtual void PadEventDecide() override;
	virtual void PadEventR1() override;
	virtual void PadEventL1() override;
	
	// ���擾
	const TEX_DRAW_INFO &GetDrawInfo()  const;
	const uint32_t	&GetCurrentLife() const{ return m_playerLife; }
	const uint32_t	&GetPlayerLifeMax() const;

	//! �^�X�N�ɃC�x���g�ǉ�
	void AddEvent( const Common::CMN_EVENT &cmnEvent );

protected:

	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_PLAYER; }
	void EventUpdate( Common::CMN_EVENT &eventId ) override;

private:

	// �v���C�����Ȃ肤��ُ���
	enum PLAYER_ABNORMAL_STATE{
		ABNORMAL_STATE_NONE			= 0,	// �ʏ���
		ABNORMAL_STATE_POISON		= 1<<0,	// ��
		ABNORMAL_STATE_MOVE_LOCK	= 1<<1,	// �����Ȃ�
		ABNORMAL_STATE_FIRE			= 1<<2,	// ���_���[�W

		ABNORMAL_STATE_MAX,
	};

	GamePlayer(void);

	// �_�b�V������̏����܂Ƃߊ֐�
	void ActionPlayerDash();

	// ���݂̃v���C���[�̏󋵂���Đ�����A�j���^�O�擾
	std::string GetAnimTag();

	// �x�[�X�ƂȂ�X�e�[�^�X�{�擾�A�C�e�����𔽉f������
	void SetupInitPlayerState();

	// �X�e�[�^�X���x��������ۂɃZ�b�g����l�֕ϊ�
	uint32_t ConvertLevelToBaseState( Common::PLAYER_BASE_STATE stateKind, uint32_t level );

	// �ړ�����
	void UpdateMove( math::Vector2 &moveVec, bool isForce = false );

	// �����̃x�N�^�[���ړ����ł��邩�ǂ����`�F�b�N
	bool CanMoveThisPos( const math::Vector2 &nextFlameAddValue ) const;

	// ��ꂩ��R�[�������Common::CMN_EVENT�ɑΉ������֐�
	void EventDamage( Common::CMN_EVENT &eventId );
	void PlayerGetItem( const Common::ITEM_KIND &itemKind, bool isCountUp = true );

	// ���ۂɃ_���[�W�������s��
	void ReflectDamage( const uint32_t &damageValue );

	// �v���C���[���Y���̃X�e�[�^�X��Ԃ��ǂ���
	bool IsPlayerState( const PLAYER_ABNORMAL_STATE &checkState ) const;
	void SetPlayerState( const PLAYER_ABNORMAL_STATE &checkState, const bool &flag );

	// �v���C���[�̃X�e�[�^�XLv
	uint32_t				m_lifeLv;			// HPLevel
	uint32_t				m_speedLv;			// SPDLevel
	uint32_t				m_defenceLv;		// DEFLevel

	uint32_t				m_playerLife;		// ���[�U�[���C�t
	uint32_t				m_playerLifeMax;	// ���[�U�[���C�t�ő�l
	uint32_t				m_playerState;		//�v���C���[�ُ��ԃX�e�[�^�X
	uint32_t				m_speedMove;		// �s�����x�ɔ{�����������u�Ԃ̃X�s�[�h
	uint32_t				m_speedMoveBase;	// ��{�s�����x
	float					m_deffenceLate;		// �h���
	float					m_speedMultiply;	// �s�����x�̔{��
	int32_t					m_poisonTime;		// �ŏ�Ԏ���
	int32_t					m_fireTime;			// ����Ԏ���
	uint32_t				m_invisibleTime;	// ���炩�̗��R�œG�̍U�����󂯂Ȃ�����
	uint32_t				m_invalidCtrlTime;	// ���炩�̗��R�ő�����󂯕t���Ȃ�����
	Common::EX_FORCE_MOVE	m_forceMoveInfo;	// ���̃N���X����󂯂�Ռ�

	AttackGun			*m_attackGun;	// �}�V���K���N���X
	AttackBlade			*m_attackBlade;	// �ߐڍU��(��)�N���X

	PlayerStatusMenu	*m_pStatusMenu;	// �X�e�[�^�X���j���[
	GameEffectLoop		*m_pMyStateIcon;// �ُ�X�e�[�^�X��\���A�C�R��
};

#endif