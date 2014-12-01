/* ====================================================================== */
/**
 * @brief  スコア記録クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GameScoreRecorder.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/Sound/SystemSoundManager.h"
#include "Flow/FlowManager.h"

ScoreRecorder *ScoreRecorder::s_pInstance = NULL;

ScoreRecorder *ScoreRecorder::Create()
{
	if( !s_pInstance ){
		s_pInstance = NEW ScoreRecorder();
	}
	else{
		DEBUG_ASSERT( 0, "インスタンスが既にあります : ScoreRecorder" );
	}
	return s_pInstance;
}

ScoreRecorder *ScoreRecorder::GetInstance()
{
	if( !s_pInstance ){
		s_pInstance = Create();
	}
	return s_pInstance;
}

ScoreRecorder::ScoreRecorder()
: TaskUnit("GameScoreRecorder")
, m_counter( 0 )
, m_currDispScore( 0 )
, m_gameScore( 0 )
{
	m_numberInfo.Init();
	m_numberInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_numberInfo.m_pos.x = WINDOW_WIDTH;
	m_numberInfo.m_pos.y = 16;
	m_numberInfo.m_usePlayerOffset = false;
}

ScoreRecorder::~ScoreRecorder()
{
	s_pInstance = NULL;
}

bool ScoreRecorder::Init()
{
	return true;
}

void ScoreRecorder::ScoreReset()
{
	auto it = m_pNumber2DArray.begin();
	while(m_pNumber2DArray.empty() == 0){
		Game2DBase *numTex = (*it);
		it = m_pNumber2DArray.erase(it);
		SAFE_DELETE(numTex);
	}
	m_pNumber2DArray.clear();

	m_counter		= 0;
	m_currDispScore	= 0;
	m_gameScore		= 0;

	m_numberInfo.Init();
	m_numberInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_numberInfo.m_pos.x = WINDOW_WIDTH;
	m_numberInfo.m_pos.y = 16;
	m_numberInfo.m_usePlayerOffset = false;
}

void ScoreRecorder::Update()
{
	uint32_t diff = m_gameScore-m_currDispScore;

	if( m_currDispScore == m_gameScore ){
		// 更新がなければ何もしない
		return;
	}
	else if( math::Abs(diff) < 30 ){
		m_currDispScore = m_gameScore;
	}
	else{
		if( m_counter%10 == 0 ){	
			// カウントSE鳴らす
			SoundManager::GetInstance()->PlaySE("Count");
		}
		++m_counter;

		m_currDispScore += static_cast<uint32_t>(diff*0.1);
	}

	// 表示更新
	UpdateScore( m_currDispScore );
}

void ScoreRecorder::ScoreEvent( const SCORE_KIND &kind )
{
	uint32_t addValue = 0;
	switch( kind ){

	default:
	case POINT_MAX:
		DEBUG_ASSERT( 0, "スコア加算の種類がおかしい" );
		return;
	case ENEMY_AAA_DEATH:
		addValue = 100;
		break;
	case ENEMY_BBB_DEATH:
		addValue = 100;
		break;
	case ENEMY_CCC_DEATH:
		addValue = 100;
		break;
	}
	AddScore( addValue );

	++m_scoreDetail[static_cast<uint32_t>(kind)];
}

void ScoreRecorder::AddScore( uint32_t addValue )
{
	// 表示スコアを保持しておく
	m_gameScore += addValue;

	m_counter = 0;
}

void ScoreRecorder::DrawUpdate()
{
	// 描画が必要なのはゲーム時のみ
	std::string gameFlow = "FlowGameMain.json";
	if( gameFlow.compare( FlowManager::GetInstance()->GetCurrentFlow() ) != 0 ){
		return;
	}

	// 数字の描画
	for( uint32_t i = 0; i < m_pNumber2DArray.size() ; ++i ){
		m_pNumber2DArray.at(i)->DrawUpdate2D();
	}
}

bool ScoreRecorder::DieMain()
{
	ScoreReset();
	return true;
}

void ScoreRecorder::UpdateScore( const uint32_t &score )
{
	uint32_t digitNum = 0;
	uint32_t rest = score;
	for(;rest != 0;++digitNum){
		rest /= 10;
	}

	for(;m_pNumber2DArray.size() < digitNum;){
		Game2DBase *tmp = NEW Game2DBase("number.json");
		const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo("number.json");
		m_numberInfo.m_pos.x -= texInfo.m_sizeWidth;
		tmp->SetDrawInfo(m_numberInfo);
		m_pNumber2DArray.push_back(tmp);
	}

	rest = score;
	for( uint32_t i = 0; rest != 0 ; ++i ){
		uint8_t underDigit = rest%10;
		std::string anim;
		anim += '0' + underDigit;
		m_pNumber2DArray.at(i)->SetAnim( anim.c_str() );

		rest /= 10;
	}
}

