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

#include "Bullet.h"
#include "Common/Utility/CommonGameUtility.h"

Bullet::Bullet( uint32_t uniqueNum, math::Vector2 &pos, math::Vector2 &vec )
	: m_uniqueNumber( uniqueNum )
	, m_liveTime( 0 )
	, m_drawBullet( NULL )
	, m_bulletVec( vec )
	, m_speed( 12.0f )
{
	m_drawBullet = NEW Game2DBase("player.json");

	// �`����Z�b�g
	TEX_DRAW_INFO info;
	m_bulletInfo.Init();

	//!�����ʒu�Z�b�g
	m_bulletInfo.m_pos = pos;
	m_drawBullet->SetDrawInfo(m_bulletInfo);
}

Bullet::~Bullet(void)
{
	SAFE_DELETE(m_drawBullet);
}

void Bullet::Update()
{
	m_bulletInfo.m_pos += m_bulletVec * m_speed;
	m_bulletInfo.m_offset = gameUtility::GetPlayerOffsetPos();
	m_drawBullet->SetDrawInfo(m_bulletInfo);

	++m_liveTime;
}

void Bullet::Draw()
{
	// �e�`��
	m_drawBullet->DrawUpdate2D();
}

const TEX_DRAW_INFO &Bullet::GetDrawInfo()
{
	return m_drawBullet->GetDrawInfo();
}