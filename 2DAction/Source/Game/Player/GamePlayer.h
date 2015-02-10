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
#include "AttackGun/GamePlayerAttackGun.h"
#include "Game/Game2DBase.h"
#include "Game/Item/ItemObject.h"
#include "GamePlayerStatusMenu.h"

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
	virtual void PadEventCancel() override;
	virtual void PadEventR1() override;
	virtual void PadEventL1() override;
	
	// ���擾
	const TEX_DRAW_INFO &GetDrawInfo()  const;
	const uint32_t	&GetCurrentLife() const{ return m_playerLife; }

	//! �^�X�N�ɃC�x���g�ǉ�
	void AddEvent( const Common::CMN_EVENT &cmnEvent );

protected:

	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_PLAYER; }
	void EventUpdate( const Common::CMN_EVENT &eventId ) override;

private:

	GamePlayer(void);

	// ���݂̃v���C���[�̏󋵂���Đ�����A�j���^�O�擾
	std::string GetAnimTag();

	// �����̃x�N�^�[���ړ����ł��邩�ǂ����`�F�b�N
	bool CanMoveThisPos( const math::Vector2 &nextFlameAddValue );

	// ��ꂩ��R�[�������Common::CMN_EVENT�ɑΉ������֐�
	void EventDamage( uint32_t damageValue );
	void PlayerGetItem( const ItemObject::ITEM_KIND &itemKind, bool isCountUp = true );

	uint32_t			m_playerLife;	// ���[�U�[���C�t
	uint32_t			m_speedMove;	// �s�����x
	uint32_t			m_speedMoveBase;// ��{�s�����x
	float				m_speedMultiply;// �s�����x�̔{��
	uint32_t			m_invisibleTime;// ���炩�̗��R�œG�̍U�����󂯂Ȃ�����
	AttackGun			*m_attackGun;	// �}�V���K���N���X

	PlayerStatusMenu	*m_pStatusMenu;	// �X�e�[�^�X���j���[
};

#endif