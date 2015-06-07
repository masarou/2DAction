/* ====================================================================== */
/**
 * @brief  �e��ʃp�[�c�N���X
 *
 * @note
 *		
 */
/* ====================================================================== */
#include "SystemMenuPartsCounter.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/Sound/SystemSoundManager.h"
#include "Common/Utility/CommonGameUtility.h"

MenuParts *PartsCounter::Create( const std::string &partsStr, const std::string &jsonStr, const math::Vector2 &originalPos )
{
	return NEW PartsCounter( partsStr, jsonStr, originalPos );
}

PartsCounter::PartsCounter( const std::string &partsStr, const std::string &jsonStr, const math::Vector2 &originalPos )
: MenuParts( partsStr, jsonStr, originalPos )
, m_counter( 0 )
, m_value( 0 )
, m_currDispValue( 0 )
{
	Reset();
	UpdateScore(0);
}


PartsCounter::~PartsCounter(void)
{
}

void PartsCounter::Reset()
{
	m_counter		= 0;
	m_currDispValue	= 0;
	m_value		= 0;

	for( uint32_t i = 0; i < m_partsArray.size() ; ++i ){
		TEX_DRAW_INFO &info = m_partsArray.at(i)->GetTexDrawInfo();
		info.m_prioity = PRIORITY_ABOVE_NORMAL;
	}
}

void PartsCounter::AddValue( const int32_t &addValue )
{
	// �\���X�R�A��ێ����Ă���
	m_value += addValue;

	m_counter = 0;
}

void PartsCounter::SetValue( const uint32_t &setValue )
{
	m_value = setValue;
}

void PartsCounter::CountAnimEnd()
{
	// �߂��l�ɂ��Ă����Ύ��̍X�V�ŃA�j�����I������
	m_currDispValue = m_value + 1;
}

bool PartsCounter::IsPlayCountAnim()
{
	if( m_currDispValue == m_value ){
		return false;
	}
	return true;
}

void PartsCounter::UpdateParts()
{
	int32_t diff = m_value-m_currDispValue;

	if( m_currDispValue == m_value ){
		// �X�V���Ȃ���Ή������Ȃ�
		return;
	}
	else if( math::Abs(diff) < 30 ){
		m_currDispValue = m_value;
	}
	else{
		if( m_counter%10 == 0 ){	
			// �J�E���gSE�炷
			SoundManager::GetInstance()->PlaySE("Count");
		}
		++m_counter;

		m_currDispValue += static_cast<int32_t>(diff*0.05);
	}

	// �\���X�V
	UpdateScore( m_currDispValue );
}

void PartsCounter::UpdateScore( const uint32_t &score )
{
	uint32_t rest = score;
	for( uint32_t i = 0; i < m_partsArray.size() ; ++i ){
		uint8_t underDigit = rest%10;
		std::string anim, partsName;
		anim += '0' + underDigit;
		partsName += '0' + i;
		
		// �A�j�����Z�b�g����p�[�c�擾(m_partsArray�ɒ��ڃA�N�Z�X�ł��邪�O�̂���...)
		MenuParts *pParts = GetPartsRecursive( partsName.c_str() );
		if( !pParts ){
			DEBUG_ASSERT( 0, "�p�[�c���Ȃ�!!");
			continue;
		}

		if( rest != 0 ){
			pParts->SetDrawFlag( true );
			SetPartsAnim( partsName.c_str(), anim.c_str() );
		}
		else{
			// �`�悵�Ȃ�
			pParts->SetDrawFlag( false );
		}

		rest /= 10;
	}
}
