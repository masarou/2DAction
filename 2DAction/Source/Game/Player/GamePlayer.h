/* ====================================================================== */
/**
 * @brief  �v���C���[�N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef SYSTEM_GAME_PLAYER
#define SYSTEM_GAME_PLAYER

#include "System/Task/SystemTaskUnit.h"
#include "System/Input/SystemInputWatcher.h"
#include "AttackGun/GamePlayerAttackGun.h"
#include "Game/Game2DBase.h"
#include "Game/Item/ItemObject.h"

class GamePlayer : public TaskUnit, InputWatcher
{
public:

	static GamePlayer *CreatePlayer();
	~GamePlayer(void);

	//virtual bool Init() override;
	virtual void Update() override;
	virtual void CollisionUpdate() override;
	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

	virtual void PadEventUp() override;
	virtual void PadEventDown() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;
	virtual void PadEventDecide() override;
	virtual void PadEventCancel() override;
	
	// ���擾
	const TEX_DRAW_INFO &GetDrawInfo();
	const uint32_t	&GetCurrentLife(){ return m_playerLife; }

	//! �^�X�N�ɃC�x���g�ǉ�
	void AddEvent( const Common::CMN_EVENT &cmnEvent ) override;

protected:

	void EventUpdate( const Common::CMN_EVENT &eventId ) override;

private:

	GamePlayer(void);

	// ��ꂩ��R�[�������Common::CMN_EVENT�ɑΉ������֐�
	void EventDamage();
	void PlayerGetItem( const ItemObject::ITEM_KIND &itemKind );

	uint32_t			m_playerLife;	// ���[�U�[���C�t
	uint32_t			m_invisibleTime;// ���炩�̗��R�œG�̍U�����󂯂Ȃ�����
	AttackGun			*m_attackGun;	// �}�V���K���N���X

	// �`��
	Game2DBase			*m_player2D;	// ���[�U�[�̉摜���Ǘ�
	TEX_DRAW_INFO		m_playerInfo;	// �`��֌W���

	Game2DBase			*m_life2D;		// ���C�t�`��N���X
	TEX_DRAW_INFO		m_lifeInfo;		// ���C�t�`����

	Game2DBase			*m_lifeFrame2D;	// ���C�t�O�g�N���X
	TEX_DRAW_INFO		m_lifeFrameInfo;// ���C�t�O�g���
};

#endif