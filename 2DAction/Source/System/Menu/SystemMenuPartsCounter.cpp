/* ====================================================================== */
/**
 * @brief  パーツクラスのひとつ(数値カウンター)
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
 * @brief	初期化
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
 * @brief	スコア追加,セット
 */
/* ================================================ */
void PartsCounter::AddValue( const int32_t &addValue )
{
	// 表示スコアを保持しておく
	m_value += addValue;

	m_counter = 0;
}
void PartsCounter::SetValue( const uint32_t &setValue )
{
	m_value = setValue;
}

/* ================================================ */
/**
 * @brief	カウントアニメ中断,終了
 */
/* ================================================ */
void PartsCounter::CountAnimEnd()
{
	// 近い値にしておけば次の更新でアニメが終了する
	m_currDispValue = m_value + 1;
}

/* ================================================ */
/**
 * @brief	カウントアニメ中かどうか
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
 * @brief	毎フレーム呼ばれるクラス更新の起点
 */
/* ================================================ */
void PartsCounter::UpdateParts()
{
	int32_t diff = m_value-m_currDispValue;

	if( m_currDispValue == m_value ){
		// 更新がなければ何もしない
		return;
	}
	else if( m_isInvalidCountAnim || math::Abs(diff) < 30 ){
		m_currDispValue = m_value;
	}
	else{
		if( m_counter%10 == 0 ){	
			// カウントSE鳴らす
			SoundManager::GetInstance()->PlaySE("Count", 8500);
		}
		++m_counter;

		m_currDispValue += static_cast<int32_t>(diff*0.05);
	}

	// 表示更新
	UpdateScore( m_currDispValue );
}
/* ================================================ */
/**
 * @brief	表示スコアアニメ更新
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
		
		// アニメをセットするパーツ取得(m_partsArrayに直接アクセスできるが念のため...)
		MenuParts *pParts = GetPartsRecursive( partsName.c_str() );
		if( !pParts ){
			DEBUG_ASSERT( 0, "パーツがない!!");
			continue;
		}

		if( rest != 0 || i == 0 ){
			pParts->SetDrawFlag( true );
			SetPartsAnim( partsName.c_str(), anim.c_str() );
		}
		else{
			// 描画しない
			pParts->SetDrawFlag( false );
		}

		rest /= 10;
	}
}
