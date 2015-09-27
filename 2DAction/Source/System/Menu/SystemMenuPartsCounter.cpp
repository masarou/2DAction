/* ====================================================================== */
/**
 * @brief  �p�[�c�N���X�̂ЂƂ�(���l�J�E���^�[)
 *
 * @note
 *		
 */
/* ====================================================================== */
#include "SystemMenuPartsCounter.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/Sound/SystemSoundManager.h"
#include "Common/Utility/CommonGameUtility.h"

MenuParts *PartsCounter::Create( const std::string &partsStr, const std::string &jsonStr, const Common::PRIORITY &priority, const math::Vector2 &originalPos )
{
	return NEW PartsCounter( partsStr, jsonStr, priority, originalPos );
}

PartsCounter::PartsCounter( const std::string &partsStr, const std::string &jsonStr, const Common::PRIORITY &priority, const math::Vector2 &originalPos )
: MenuParts( partsStr, jsonStr, priority, originalPos )
, m_counter( 0 )
, m_value( 0 )
, m_currDispValue( 0 )
, m_isInvalidCountAnim( false )
{
	Reset();
	UpdateScore(0);
}


PartsCounter::~PartsCounter(void)
{
}

/* ================================================ */
/**
 * @brief	������
 */
/* ================================================ */
void PartsCounter::Reset()
{
	m_counter		= 0;
	m_currDispValue	= 0;
	m_value		= 0;
}

/* ================================================ */
/**
 * @brief	�X�R�A�ǉ�,�Z�b�g
 */
/* ================================================ */
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

/* ================================================ */
/**
 * @brief	�J�E���g�A�j�����f,�I��
 */
/* ================================================ */
void PartsCounter::CountAnimEnd()
{
	// �߂��l�ɂ��Ă����Ύ��̍X�V�ŃA�j�����I������
	m_currDispValue = m_value + 1;
}

/* ================================================ */
/**
 * @brief	�J�E���g�A�j�������ǂ���
 */
/* ================================================ */
bool PartsCounter::IsPlayCountAnim()
{
	if( m_currDispValue == m_value ){
		return false;
	}
	return true;
}

/* ================================================ */
/**
 * @brief	���t���[���Ă΂��N���X�X�V�̋N�_
 */
/* ================================================ */
void PartsCounter::UpdateParts()
{
	int32_t diff = m_value-m_currDispValue;

	if( m_currDispValue == m_value ){
		// �X�V���Ȃ���Ή������Ȃ�
		return;
	}
	else if( m_isInvalidCountAnim || math::Abs(diff) < 30 ){
		m_currDispValue = m_value;
	}
	else{
		if( m_counter%10 == 0 ){	
			// �J�E���gSE�炷
			SoundManager::GetInstance()->PlaySE("Count", 8500);
		}
		++m_counter;

		m_currDispValue += static_cast<int32_t>(diff*0.05);
	}

	// �\���X�V
	UpdateScore( m_currDispValue );
}
/* ================================================ */
/**
 * @brief	�\���X�R�A�A�j���X�V
 */
/* ================================================ */
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

		if( rest != 0 || i == 0 ){
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
