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

#ifndef ATTACK_BULLET
#define ATTACK_BULLET

#include "Game/Game2DBase.h"

// �Œ�l
static const uint32_t BULLET_LIVE_TIME	= 150;

class Bullet
{

public:
	Bullet( const uint32_t &uniqueNum, const math::Vector2 &pos, const math::Vector2 &vec, float speed );
	~Bullet(void);

	// ���Z�b�g
	void	SetBulletVec( math::Vector2 &vec ){ m_bulletVec = vec; }
	void	SetBulletSpeed( float &spd ){ m_speed = spd; }

	void	Update();
	void	Draw();

	// ���擾
	const TEX_DRAW_INFO &GetDrawInfo();
	uint32_t GetUniqueNumber(){ return m_uniqueNumber; }
	uint32_t GetLiveTime(){ return m_liveTime; }

private:
	
	uint32_t		m_uniqueNumber;	// �ق��̒e�Ƌ�ʂ��邽�߂Ƀ��j�[�N�ԍ�
	uint32_t		m_liveTime;		// ��������Ă���̎���
	Texture2D		m_textureBullet;// �v���C���[�摜

	math::Vector2	m_bulletVec;	// ���˕���
	float			m_speed;		// ���˃X�s�[�h

};

#endif
