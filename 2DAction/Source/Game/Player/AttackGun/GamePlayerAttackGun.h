/* ====================================================================== */
/**
 * @brief  �v���C���[�N���X
 *
 * @note
 *		Bullet�N���X���x�N�^�Ŏ���
 *		���ۂɓI�Ƃ̂�����Bullet�N���X���s�����̃N���X��Bullet�N���X�Ƒ��N���X�Ƃ̒���
 *		CmnGameAccesser��ʂ��Ă��̃N���X���Ǘ����Ă���Bullet�N���X�̈ʒu�����擾�����肷��
 *		
 */
/* ====================================================================== */

#ifndef SYSTEM_ATTACK_GUN
#define SYSTEM_ATTACK_GUN

#include "System/Task/SystemTaskUnit.h"
#include "Bullet.h"

class AttackGun : public TaskUnit
{

public:

	AttackGun(void);
	~AttackGun(void);
	
	virtual void Update() override;
	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

	// �e�̔���
	void ShootBullet( math::Vector2 pos = math::Vector2(), math::Vector2 vec = math::Vector2( 0.0f, 1.0f ) );
	// �e�̍폜(��ʊO�ɏo���A�G�ɓ����������X)
	void DeleteBullet( uint32_t uniqueNumber );

private:

	uint32_t				m_intervalTime; // ���˂̊Ԋu
	std::vector<Bullet*>	m_magazine;		// �e�̊Ǘ��x�N�^

};

#endif

