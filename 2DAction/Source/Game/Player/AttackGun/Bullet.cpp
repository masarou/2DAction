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
#include "Game/Enemy/EnemyManager.h"
#include "Game/GameRegister.h"

Bullet::Bullet( const uint32_t &uniqueNum, const math::Vector2 &pos, const math::Vector2 &vec, float speed )
	: m_uniqueNumber( uniqueNum )
	, m_liveTime( 0 )
	, m_bulletVec( vec )
	, m_speed( speed )
{
	m_textureBullet.Init();
	m_textureBullet.m_pTex2D = NEW Game2DBase("bullet.json");

	//!�����ʒu�Z�b�g
	m_textureBullet.m_texInfo.Init();
	m_textureBullet.m_texInfo.m_fileName = "bullet.json";
	m_textureBullet.m_texInfo.m_pos = pos;
}

Bullet::~Bullet(void)
{
	m_textureBullet.DeleteAndInit();
}

void Bullet::Update()
{
	m_textureBullet.m_texInfo.m_pos += m_bulletVec * m_speed;
	m_textureBullet.m_pTex2D->SetDrawInfo(m_textureBullet.m_texInfo);

	// �G�ɓ����������`�F�b�N
	bool isHit = GameRegister::GetInstance()->GetManagerEnemy()->CheckCollisionToBullet( this );
	if( isHit ){
		m_liveTime = BULLET_LIVE_TIME;
	}
	else{
		++m_liveTime;
	}
}

void Bullet::Draw()
{
	// �e�`��
	m_textureBullet.m_pTex2D->DrawUpdate2D();
}

const TEX_DRAW_INFO &Bullet::GetDrawInfo()
{
	if( m_textureBullet.m_pTex2D == NULL ){
		DEBUG_ASSERT( 0, "�e�̕`��N���X��NULL");
	}
	return m_textureBullet.m_pTex2D->GetDrawInfo();
}