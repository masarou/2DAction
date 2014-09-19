/* ====================================================================== */
/**
 * @brief  �G�Ǘ��N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef ENEMY_BASE
#define ENEMY_BASE

#include "System/SystemDefine.h"
#include "Game/Game2DBase.h"

class EnemyBase : public TaskUnit
{
private:


protected:

	EnemyBase( std::string jsonName, uint32_t uniqueId, Common::ENEMY_KIND kind );
	virtual ~EnemyBase(void);

	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

	// ���N���X����̃C�x���g����
	void EventUpdate( const Common::CMN_EVENT &eventId ) override;
	void HitPlayreBullet();

public:

	uint32_t			m_uniqueIdOfEnemyAll;	// �G�S�̂̒��ł̎���ID
	Common::ENEMY_KIND	m_enemyKind;			// �G�̎��

	uint32_t			m_HP;					// �G�̗�
	math::Vector2		m_eye;					// �G�̎���
	Game2DBase			*m_enemy2D;				// �G�̉摜���Ǘ�
	TEX_DRAW_INFO		m_enemyInfo;			// �`��֌W���
};

#endif