/* ====================================================================== */
/**
 * @brief  �G�t�F�N�g�N���X ��{���Đ�+���E
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GameEffect.h"


GameEffect::GameEffect( const EFFECT_KIND &kind, const uint32_t &posX, const uint32_t &posY )
	: TaskUnit("GameEffect")
	, m_kind( kind )
	, m_pEffect( NULL )
{
	// �`��N���X�Z�b�g�A�b�v
	m_pEffect = NEW Game2DBase(SelectEffectFile().c_str());
	m_effectInfo.Init();
	m_effectInfo.m_fileName = SelectEffectFile();
	m_effectInfo.m_pos.x = static_cast<float>(posX);
	m_effectInfo.m_pos.y = static_cast<float>(posY);
	m_pEffect->SetAnim(SelectEffectAnimTag());
}

GameEffect::~GameEffect(void)
{
}

bool GameEffect::DieMain()
{
	SAFE_DELETE(m_pEffect);
	return true;
}

bool GameEffect::Init()
{
	m_pEffect->SetDrawInfo( m_effectInfo );
	return true;
}

void GameEffect::Update()
{
}

void GameEffect::DrawUpdate()
{
	if( std::string("").compare(m_pEffect->GetPlayAnim()) == 0 ){
		// �A�j���͏I������̂Ŏ��E
		TaskStartDie();
		return;
	}

	m_pEffect->DrawUpdate2D();
}

std::string GameEffect::SelectEffectFile()
{
	std::string rtn = "";
	switch(m_kind){
	case EFFECT_BOMB:
		rtn = "effectBomb.json";
		break;

	default:
		DEBUG_ASSERT( "�G�t�F�N�g��ނ��z��O" );
		// �Ƃ肠������Ԗ���Ȃ��̂��Z�b�g
		rtn = "effectBomb.json";
		break;
	}
	return rtn;
}

std::string GameEffect::SelectEffectAnimTag()
{
	std::string rtn = "";
	switch(m_kind){
	case EFFECT_BOMB:
		rtn = "bomb";
		break;

	default:
		DEBUG_ASSERT( "�G�t�F�N�g��ނ��z��O" );
		rtn = "";
		break;
	}
	return rtn;
}
