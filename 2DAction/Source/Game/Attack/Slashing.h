/* ====================================================================== */
/**
 * @brief  �a���A�j��
 *
 * @note
 *		���Đ������E ����Unit�Ƃ̓����蔻�肪����̂ő����̂��̂��p��
 *		
 */
/* ====================================================================== */

#ifndef __BLADE__
#define __BLADE__

#include "Common/CommonDefine.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"

class Slashing : public TaskUnit, public Collision2DUnit
{

public:

	enum TYPE_SLASHING{
		TYPE_1ST,
		TYPE_2ND,
		TYPE_3RD,

		TYPE_MAX,
	};

	static const uint32_t SLASHING_ANIM_END = 0;

	Slashing( const Common::OWNER_TYPE ownerType, const TYPE_SLASHING &type, const math::Vector2 &pos, const math::Vector2 &vec, uint32_t damage, bool deleteBullet = false );
	~Slashing(void);

	// ���擾
	const TYPE_SLASHING &GetTypeSlashing() const{ return m_slashingType; }
	const uint32_t &GetBladeDamage() const{ return m_bladeDamage; }
	const TEX_DRAW_INFO &GetDrawInfo() const;
	const uint32_t &GetLiveTime() const{ return m_liveTime; }
	const bool &IsDeleteBullet() const{ return m_IsDeleteBullet; }

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

	// �����蔻�肪�L���ƂȂ�t���[���擾
	void GetHitFrame();

	const std::string GetJsonFileStr();
	
	Common::OWNER_TYPE		m_ownerType;
	TYPE_SLASHING			m_slashingType;
	uint32_t				m_liveTime;			// ��������Ă���̎���

	bool					m_IsDeleteBullet;	// �G�̒e�����������邩
	uint32_t				m_bladeDamage;		// �a���З�
	math::Vector2			m_bladePos;			// �a���ʒu
	math::Vector2			m_bladeVec;			// �a������
	std::vector<uint32_t>	m_hitFrame;			// �����蔻�肪�L���ƂȂ�t���[��

};

#endif
