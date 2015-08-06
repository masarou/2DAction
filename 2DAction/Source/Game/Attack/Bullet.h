/* ====================================================================== */
/**
 * @brief  GamePlayerAttackGun���甭�˂���鋅�N���X
 *
 * @note
 *		GamePlayerAttackGun��e�Ƃ��Ĕ��˂��ꂽ���̃N���X
 *		�ꔭ�̋ʂ̏��������A�X�s�[�h������A�ʒu��������
 *		
 */
/* ====================================================================== */

#ifndef __ATTACK_BULLET__
#define __ATTACK_BULLET__

#include "Common/CommonDefine.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"

// �Œ�l
static const uint32_t BULLET_LIVE_TIME_PLAYER	= 40;
static const uint32_t BULLET_LIVE_TIME_ENEMY	= 120;

class Bullet : public TaskUnit, public Collision2DUnit
{

public:

	Bullet( const Common::OWNER_TYPE ownerType, const math::Vector2 &pos, const math::Vector2 &vec, const uint32_t &damage, const uint32_t &speed );
	~Bullet(void);

	// ���Z�b�g
	void	SetBulletVec( math::Vector2 &vec ){ m_bulletVec = vec; }
	void	SetBulletSpeed( uint32_t &spd ){ m_speed = spd; }

	// ���擾
	const uint32_t &GetBulletDamage() const{ return m_bulletDamage; }
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t &GetLiveTime() const{ return m_liveTime; }

protected:

	virtual bool Init() override;
	virtual bool DieMain() override;
	virtual void Update() override;			// �ړ����̓������l�̍X�V
	virtual void DrawUpdate() override;		// �`��X�V

	// �ق��̃N���X����̃C�x���g����
	virtual void EventUpdate( Common::CMN_EVENT &eventId ) override;

	// ���̃N���X�̎�ރZ�b�g
	virtual const Common::TYPE_OBJECT GetTypeObject() const override;

private:
	
	Common::OWNER_TYPE	m_ownerType;
	uint32_t			m_liveTime;		// ��������Ă���̎���
	uint32_t			m_liveTimeMax;	// ��������Ă��玀�ʂ܂ł̎���

	uint32_t			m_bulletDamage;	// �e�̈З�
	math::Vector2		m_bulletVec;	// ���˕���
	uint32_t			m_speed;		// ���˃X�s�[�h

};

#endif
