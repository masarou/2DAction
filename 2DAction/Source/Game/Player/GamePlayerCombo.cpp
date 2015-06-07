/* ====================================================================== */
/**
 * @brief  �v���C���[�̃R���{�\�����s��
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "System/picojson.h"
#include "Game/GameRecorder.h"
#include "GamePlayerCombo.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

PlayerCombo *PlayerCombo::CreatePlayerCombo()
{
	return NEW PlayerCombo();
}

PlayerCombo::PlayerCombo()
: TaskUnit( "PlayerCombo" )
, m_pNumCounterCombo( NULL )
, m_pNumCounterComboYellow( NULL )
, m_pNumCounterComboRed( NULL )
{
	m_textureFrame.Init();
	m_textureCombo.Init();
	m_textureComboGauge.Init();

	std::string jsonStr = "ComboFrame.json";
	m_textureFrame.m_pTex2D = Game2DBase::Create( jsonStr.c_str() );

	//!�����ʒu�Z�b�g
	m_textureFrame.m_texInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_textureFrame.m_texInfo.m_fileName = jsonStr;
	const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( jsonStr.c_str() );
	m_textureFrame.m_texInfo.m_posOrigin = math::Vector2( WINDOW_WIDTH - static_cast<float>(texInfo.m_sizeWidth/2), WINDOW_HEIGHT - static_cast<float>(texInfo.m_sizeHeight/2) );
	m_textureFrame.m_texInfo.m_usePlayerOffset = false;
	m_textureFrame.m_pTex2D->SetDrawInfo(m_textureFrame.m_texInfo);
}

PlayerCombo::~PlayerCombo(void)
{
}

bool PlayerCombo::DieMain()
{
	m_textureFrame.DeleteAndInit();
	m_textureCombo.DeleteAndInit();
	m_textureComboGauge.DeleteAndInit();
	return true;
}

bool PlayerCombo::Init()
{
	// �X�e�[�^�X���j���[�̃p�[�c���擾
	Utility::GetPartsInfoFromJson( m_textureFrame.m_texInfo.m_fileName, m_partsMap );

	// �e��p�[�c�Z�b�g
	m_textureCombo.Init();
	m_textureCombo.m_pTex2D = Game2DBase::Create("ComboHitStr.json");
	m_textureCombo.m_texInfo.m_fileName = "ComboHitStr.json";
	m_textureCombo.m_texInfo.m_prioity = PRIORITY_HIGH;
	m_textureCombo.m_texInfo.m_posOrigin = GetPartsPos("comboStr");
	m_textureCombo.m_texInfo.m_usePlayerOffset = false;
	m_textureCombo.m_pTex2D->SetDrawInfo(m_textureCombo.m_texInfo);
	
	// �R���{��
	m_pNumCounterCombo = NumberCounter::Create("NumberLarge.json");
	m_pNumCounterComboYellow = NumberCounter::Create("NumberLargeYellow.json");
	m_pNumCounterComboRed = NumberCounter::Create("NumberLargeRed.json");
	TEX_DRAW_INFO comboInfo;
	comboInfo.Init();
	comboInfo.m_prioity = PRIORITY_HIGH;
	comboInfo.m_posOrigin = GetPartsPos("count");
	comboInfo.m_usePlayerOffset = false;
	m_pNumCounterCombo->SetDrawInfo( comboInfo );
	m_pNumCounterComboYellow->SetDrawInfo( comboInfo );
	m_pNumCounterComboRed->SetDrawInfo( comboInfo );

	// �R���{�̌p�����Ԃ�\���R���{�Q�[�W
	m_textureComboGauge.Init();
	m_textureComboGauge.m_pTex2D = Game2DBase::Create("ComboGauge.json");
	m_textureComboGauge.m_texInfo.m_fileName = "ComboGauge.json";
	m_textureComboGauge.m_texInfo.m_prioity = PRIORITY_HIGH;
	m_textureComboGauge.m_texInfo.m_posOrigin = GetPartsPos("comboGauge");
	m_textureComboGauge.m_texInfo.m_usePlayerOffset = false;
	const TEX_INIT_INFO &texInfoComboGauge = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_textureComboGauge.m_texInfo.m_fileName.c_str() );
	m_textureComboGauge.m_texInfo.m_arrangeOrigin = math::Vector2( static_cast<float>(texInfoComboGauge.m_sizeWidth), 0.0f );
	m_textureComboGauge.m_pTex2D->SetDrawInfo(m_textureCombo.m_texInfo);

	return true;
}

/* ================================================ */
/**
 * @brief	�e��Update�֐�
 */
/* ================================================ */
void PlayerCombo::Update()
{
	// ���݂̃R���{���ɂ���ĕ\�����镶���F�ω�	
	uint32_t counter = GameRecorder::GetInstance()->GetCurrentHitCounter();
	if( m_pNumCounterCombo && m_pNumCounterComboYellow && m_pNumCounterComboRed ){
		m_pNumCounterCombo->SetValue( counter );		
		m_pNumCounterComboYellow->SetValue( counter );
		m_pNumCounterComboRed->SetValue( counter );
		if( counter < 30 ){
			m_pNumCounterCombo->SetDrawInvalidFlag( false );		
			m_pNumCounterComboYellow->SetDrawInvalidFlag( true );
			m_pNumCounterComboRed->SetDrawInvalidFlag( true );
		}
		else if( counter < 70 ){
			m_pNumCounterCombo->SetDrawInvalidFlag( true );		
			m_pNumCounterComboYellow->SetDrawInvalidFlag( false );
			m_pNumCounterComboRed->SetDrawInvalidFlag( true );
		}
		else{
			m_pNumCounterCombo->SetDrawInvalidFlag( true );		
			m_pNumCounterComboYellow->SetDrawInvalidFlag( true );
			m_pNumCounterComboRed->SetDrawInvalidFlag( false );
		}
	}
}

void PlayerCombo::DrawUpdate()
{
	if( m_textureFrame.m_pTex2D ){
		// �X�e�[�^�X�O�g�`��
		m_textureFrame.m_pTex2D->DrawUpdate2D();

		// �R���{������
		m_textureCombo.m_pTex2D->DrawUpdate2D();
	}
	if( m_textureComboGauge.m_pTex2D ){
		// �R���{�������ԃQ�[�W
		uint32_t leftTime = GameRecorder::GetInstance()->GetLeftTimeOfCombo();
		float scale = 10.0f;
		scale = 10.0f * ( static_cast<float>(leftTime) / static_cast<float>(GameRecorder::COMBO_COUNT_MAX) );
		m_textureComboGauge.m_texInfo.m_scale.x = scale;
		m_textureComboGauge.m_pTex2D->SetDrawInfo( m_textureComboGauge.m_texInfo );
		m_textureComboGauge.m_pTex2D->DrawUpdate2D();
	}
}

const math::Vector2 PlayerCombo::GetPartsPos( const std::string name ) const
{
	// �X�e�[�^�X���j���[�̍�����W�擾
	const TEX_INIT_INFO &statusMenuInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_textureFrame.m_texInfo.m_fileName.c_str() );
	math::Vector2 retPos = m_textureFrame.m_texInfo.m_posOrigin;
	retPos -= math::Vector2( statusMenuInfo.m_sizeWidth / 2.0f, statusMenuInfo.m_sizeHeight / 2.0f );

	// ��������p�[�c�̈ʒu�𑫂��Z
	Common::PARTS_INFO info = GetPartsInfo(name);
	retPos += info.m_pos;
	return retPos;
}

const Common::PARTS_INFO &PlayerCombo::GetPartsInfo( const std::string name ) const
{
	auto it = m_partsMap.find( name.c_str() );
	if( it != m_partsMap.end() ){
		return (*it).second;
	}
	DEBUG_ASSERT( 0, "�p�[�c��������܂���\n" );
	return (*m_partsMap.begin()).second;
}



