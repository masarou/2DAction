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

class GamePlayer : public TaskUnit, InputWatcher
{
public:

	GamePlayer(void);
	~GamePlayer(void);

	//virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

	virtual void PadEventUp() override;
	virtual void PadEventDown() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;
	virtual void PadEventDecide() override;

	bool IsNext(){return m_nextFlag;}

protected:

	void EventUpdate( const Common::CMN_EVENT &eventId ) override;

private:

	// ��ꂩ��R�[�������Common::CMN_EVENT�ɑΉ������֐�
	void EventDamage();
	void PlayerGetItem( const uint32_t &itemId );



	bool				m_nextFlag;
	uint32_t			m_invisibleTime;//	���炩�̗��R�œG�̍U�����󂯂Ȃ�����
	Game2DBase			*m_player2D;	// ���[�U�[�̉摜���Ǘ�
	AttackGun			*m_attackGun;	// �}�V���K���N���X
	TEX_DRAW_INFO		m_playerInfo;	// �`��֌W���

};

#endif