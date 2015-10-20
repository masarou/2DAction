/* ====================================================================== */
/**
 * @brief  スコア表示クラス
 *
 * @note
 *		ゲーム中に左上に出すステージスコア
 */
/* ====================================================================== */

#include "CmnNumberCounter.h"
#include "System/Sound/SystemSoundManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"

NumberCounter *NumberCounter::Create( const char *readJson )
{
	return NEW NumberCounter( readJson );
}

NumberCounter::NumberCounter( const char *readJson )
: TaskUnit("GameScoreRecorder")
, m_invalidDraw( false )
, m_invalidCallCountUpSE( false )
, m_readFile( readJson )
, m_counter( 0 )
, m_currDispValue( 0 )
, m_value( 0 )
{
	m_numberInfo.Init();
	m_numberInfo.m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	m_numberInfo.m_posOrigin.x = WINDOW_WIDTH;
	m_numberInfo.m_posOrigin.y = 16;
	m_numberInfo.m_usePlayerOffset = false;
}


NumberCounter::~NumberCounter(void)
{
}

void NumberCounter::Reset()
{
	auto it = m_pNumber2DArray.begin();
	while(m_pNumber2DArray.empty() == 0){
		Game2DBase *numTex = (*it);
		it = m_pNumber2DArray.erase(it);
		SAFE_DELETE(numTex);
	}
	m_pNumber2DArray.clear();

	m_counter		= 0;
	m_currDispValue	= 0;
	m_value		= 0;

	m_numberInfo.Init();
	m_numberInfo.m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	//m_numberInfo.m_posOrigin.x = WINDOW_WIDTH;
	//m_numberInfo.m_posOrigin.y = 16;
	m_numberInfo.m_usePlayerOffset = false;
}

void NumberCounter::AddValue( const int32_t &addValue )
{
	// 表示スコアを保持しておく
	m_value += addValue;

	m_counter = 0;
}

void NumberCounter::SetValue( const uint32_t &setValue )
{
	m_value = setValue;
}

void NumberCounter::SetDrawInfo( const TEX_DRAW_INFO &drawInfo )
{
	m_numberInfo = drawInfo;
}

void NumberCounter::SetDrawPos( const math::Vector2 &pos )
{
	m_numberInfo.m_posOrigin = pos;
}

void NumberCounter::CountAnimEnd()
{
	// 近い値にしておけば次の更新でアニメが終了する
	m_currDispValue = m_value + 1;
}

bool NumberCounter::IsPlayCountAnim()
{
	if( m_currDispValue == m_value ){
		return false;
	}
	return true;
}

bool NumberCounter::Init()
{
	UpdateScore(0);
	return true;
}

bool NumberCounter::DieMain()
{
	Reset();
	return true;
}

void NumberCounter::Update()
{
	int32_t diff = m_value-m_currDispValue;

	if( m_currDispValue == m_value ){
		// 更新がなければ何もしない
		return;
	}
	else if( math::Abs(diff) < 30 ){
		m_currDispValue = m_value;
	}
	else{
		if( m_counter%10 == 0 && !m_invalidCallCountUpSE ){	
			// カウントSE鳴らす
			SoundManager::GetInstance()->PlaySE("Count", 8500);
		}
		++m_counter;

		m_currDispValue += static_cast<int32_t>(diff*0.05);
	}

	// 表示更新
	UpdateScore( m_currDispValue );
}

void NumberCounter::DrawUpdate()
{
	if( m_invalidDraw ){
		// 描画しない命令が出ているので描画しない
		return;
	}

	// 数字の描画
	for( uint32_t i = 0; i < m_pNumber2DArray.size() ; ++i ){
		if( i != 0 && i > std::log10(static_cast<double>(m_currDispValue))){
			break;
		}
		m_pNumber2DArray.at(i)->DrawUpdate2D();
	}
}

void NumberCounter::UpdateScore( const uint32_t &score )
{
	uint32_t digitNum = 0;
	uint32_t rest = score;
	for(;rest != 0;++digitNum){
		rest /= 10;
	}

	// scoreが0なら0と表示するため1桁の数字として扱う
	if( digitNum == 0 ){
		++digitNum;
	}

	for(;m_pNumber2DArray.size() < digitNum;){
		Game2DBase *tmp = Game2DBase::Create( m_readFile.c_str() );
		const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_readFile.c_str() );
		m_numberInfo.m_posOrigin.x -= texInfo.m_sizeWidth*m_numberInfo.m_scale.x;
		tmp->SetDrawInfo(m_numberInfo);
		m_pNumber2DArray.push_back(tmp);
	}

	rest = score;
	for( uint32_t i = 0; i < m_pNumber2DArray.size() ; ++i ){
		uint8_t underDigit = rest%10;
		std::string anim;
		anim += '0' + underDigit;
		m_pNumber2DArray.at(i)->SetAnim( anim.c_str() );

		rest /= 10;
	}
}