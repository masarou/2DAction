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

#include "Common/CommonDefine.h"
#include "System/SystemDefine.h"
#include "Game/Game2DBase.h"
#include "Game/Enemy/EnemyAIBase.h"
#include "Game/Enemy/EnemyManager.h"

class EnemyBase
{

	friend EnemyManager;

public:

	enum ENEMY_STATE{
		ENEMY_INIT,
		ENEMY_ACTION,

		ENEMY_MAX
	};

	virtual ~EnemyBase(void);

	void EventUpdate( const Common::CMN_EVENT &eventId );	// manager����̃C�x���g����

	// ���擾�֐�
	const ENEMY_STATE &GetState() const{ return m_enemyState; }
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t &GetEnemyHitPoint() const{ return m_HP; }
	const math::Vector2 &GetEnemyEyeSight() const{ return m_eye; }
	const uint32_t &GetUniqueNumber() const{ return m_uniqueIdOfEnemyAll; }
	void ChangeAIState( const Common::ENEMY_AI &nextAI );

protected:

	EnemyBase( const std::string &jsonName, const uint32_t &uniqueId, const Common::ENEMY_KIND &kind );

	virtual bool Init();								// ������
	virtual void UpdateEnemy();							// �ʒu��AI�ɂ�鐔�l����̍X�V
	virtual void DrawEnemy();							// �`��X�V
	virtual void HitPlayreBullet( uint32_t damageValue );// �e�������������̏���

	// �h����ŃZ�b�g����֐�
	virtual uint32_t GetEnemyDefaultHP(){return 10;}	// �G�N���X�̃f�t�H���gHP�擾

	EnemyAIBase			*m_pEnemyAI;					// �v�l�N���X

private:

	ENEMY_STATE			m_enemyState;					// ���̃N���X�̏��
	uint32_t			m_uniqueIdOfEnemyAll;			// �G�S�̂̒��ł̎���ID
	Common::ENEMY_KIND	m_enemyKind;					// �G�̎��
	uint32_t			m_HP;							// �G�̗�
	math::Vector2		m_eye;							// �G�̎���
	Game2DBase			*m_pEnemy2D;					// �G�̉摜���Ǘ�
	TEX_DRAW_INFO		m_enemyInfo;					// �`��֌W���

	Texture2D			m_textureEnemy;					// �G�摜
	Texture2D			m_textureLife;					// �G���C�t�摜

	Common::ENEMY_AI	m_nextAI;						// ���Ɏv�l����AI�X�e�[�g
};

#endif