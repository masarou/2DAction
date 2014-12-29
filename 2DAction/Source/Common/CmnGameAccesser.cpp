/* ====================================================================== */
/**
 * @brief  �Q�[���ŋ��L���K�v�ȏ��i�[�N���X
 *
 * @note
 *		�V�[���A�N���X���܂����ŏ���ێ��������ꍇ�ɃZ�b�g
 */
/* ====================================================================== */


#include "CmnGameAccesser.h"
#include "System/SystemDefine.h"

GameAccesser *GameAccesser::s_pInstance = NULL;

void GameAccesser::Create()
{
	if(s_pInstance){
		DEBUG_ASSERT( 0, "s_pInstance is Already Created");
		return;
	}
	s_pInstance = NEW GameAccesser();
}

GameAccesser *GameAccesser::GetInstance()
{
	if(!s_pInstance){
		DEBUG_ASSERT( 0, "s_pInstance is NULL");
	}
	return s_pInstance;
}

GameAccesser::GameAccesser()
{
	m_offsetPlayer.Set(0.0f,0.0f);
}

GameAccesser::~GameAccesser()
{
	s_pInstance = NULL;
}

void GameAccesser::DeleteGameAccesser()
{
	SAFE_DELETE( s_pInstance );
}

void GameAccesser::InitAll()
{
	m_offsetPlayer.Set(0.0f,0.0f);
}

void GameAccesser::SetPlayerOffSet(const math::Vector2 &setPos)
{
	m_offsetPlayer = setPos;
}

void GameAccesser::SetPlayerOffSet(const float &posX, const float &posY)
{
	m_offsetPlayer.x = posX;
	m_offsetPlayer.y = posY;
}

void GameAccesser::AddPlayerOffSet(const math::Vector2 &addPos)
{
	m_offsetPlayer += addPos;
}

void GameAccesser::AddPlayerOffSet(const float posX, const float posY)
{
	m_offsetPlayer.x += posX;
	m_offsetPlayer.y += posY;
}

const math::Vector2 &GameAccesser::GetPlayerOffSet() const
{
	return m_offsetPlayer;
}

void GameAccesser::GetPlayerOffSet( float &posX, float &posY)
{
	posX = m_offsetPlayer.x;
	posY = m_offsetPlayer.y;
}