/* ====================================================================== */
/**
 * @brief  �X�R�A�\���N���X
 *
 * @note
 *		
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
, m_readFile( readJson )
, m_counter( 0 )
, m_currDispValue( 0 )
, m_value( 0 )
{
	m_numberInfo.Init();
	m_numberInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_numberInfo.m_pos.x = WINDOW_WIDTH;
	m_numberInfo.m_pos.y = 16;
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
	m_numberInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_numberInfo.m_pos.x = WINDOW_WIDTH;
	m_numberInfo.m_pos.y = 16;
	m_numberInfo.m_usePlayerOffset = false;
}

void NumberCounter::AddValue( int32_t addValue )
{
	// �\���X�R�A��ێ����Ă���
	m_value += addValue;

	m_counter = 0;
}

void NumberCounter::SetDrawInfo( const TEX_DRAW_INFO &drawInfo )
{
	m_numberInfo = drawInfo;
}

void NumberCounter::CountAnimEnd()
{
	// �߂��l�ɂ��Ă����Ύ��̍X�V�ŃA�j�����I������
	m_currDispValue = m_value + 1;
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
	uint32_t diff = m_value-m_currDispValue;

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

		m_currDispValue += static_cast<uint32_t>(diff*0.05);
	}

	// �\���X�V
	UpdateScore( m_currDispValue );
}

void NumberCounter::DrawUpdate()
{
	// �����̕`��
	for( uint32_t i = 0; i < m_pNumber2DArray.size() ; ++i ){
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

	// score��0�Ȃ�0�ƕ\�����邽��1���̐����Ƃ��Ĉ���
	if( digitNum == 0 ){
		++digitNum;
	}

	for(;m_pNumber2DArray.size() < digitNum;){
		Game2DBase *tmp = NEW Game2DBase( m_readFile.c_str() );
		const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo("number.json");
		m_numberInfo.m_pos.x -= texInfo.m_sizeWidth;
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