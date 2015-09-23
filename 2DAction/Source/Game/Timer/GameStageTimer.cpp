/* ====================================================================== */
/**
 * @brief  �������ԃX�e�[�W�̎c�莞�Ԃ�\���N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "System/picojson.h"
#include "GameStageTimer.h"
#include "Game/GameRegister.h"
#include "Game/Timer/GameCountDownTimer.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

StageTimer *StageTimer::CreateStageTimer( uint32_t startTime )
{
	return NEW StageTimer( startTime );
}

StageTimer::StageTimer( uint32_t startTime )
: TaskUnit( "StageTimer" )
, m_startTime( startTime )
, m_pCountDown( NULL )
{
	m_plateStageTimer.Init();
	m_animClock.Init();

	//!�����ʒu�Z�b�g
	std::string jsonStr = "StageTimer.json";
	m_plateStageTimer.m_pTex2D = Game2DBase::Create( jsonStr.c_str() );
	TEX_DRAW_INFO drawInfo;
	drawInfo.m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	drawInfo.m_fileName = jsonStr;
	const TEX_INIT_INFO &texInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( jsonStr.c_str() );
	drawInfo.m_posOrigin = math::Vector2( texInfo.m_sizeWidth/2.0f, texInfo.m_sizeHeight/2.0f );
	drawInfo.m_usePlayerOffset = false;
	m_plateStageTimer.m_pTex2D->SetDrawInfo( drawInfo );
}

StageTimer::~StageTimer(void)
{
}

bool StageTimer::DieMain()
{
	m_plateStageTimer.DeleteAndInit();
	m_animClock.DeleteAndInit();
	return true;
}

bool StageTimer::Init()
{
	if( !m_plateStageTimer.m_pTex2D ){
		DEBUG_ASSERT( 0, "�K�v�ȃN���X������Ă��Ȃ�" );
		return true;
	}

	// �X�e�[�^�X���j���[�̃p�[�c���擾
	Utility::GetPartsInfoFromJson( m_plateStageTimer.m_pTex2D->GetDrawInfo().m_fileName, m_partsMap );

	// ���v�摜
	TEX_DRAW_INFO drawInfo;
	std::string jsonStr = "ClockTimer.json";
	m_animClock.m_pTex2D = Game2DBase::Create( jsonStr.c_str() );
	drawInfo.m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	drawInfo.m_fileName = jsonStr;
	const Common::PARTS_INFO &clockInfo = GetPartsInfo("timeIcon");
	drawInfo.m_posOrigin = clockInfo.m_pos;
	drawInfo.m_usePlayerOffset = false;
	m_animClock.m_pTex2D->SetDrawInfo( drawInfo );

	// ���ԃN���X�p��
	m_pCountDown = GameCountDownTimer::CreateBySec( m_startTime );
	const Common::PARTS_INFO &numberInfo = GetPartsInfo("numPos");
	m_pCountDown->SetPosition( numberInfo.m_pos );

	return true;
}

/* ================================================ */
/**
 * @brief	�e��Update�֐�
 */
/* ================================================ */
void StageTimer::Update()
{
}

void StageTimer::DrawUpdate()
{
	if( m_plateStageTimer.m_pTex2D ){
		m_plateStageTimer.m_pTex2D->DrawUpdate2D();
	}
	if( m_animClock.m_pTex2D ){
		m_animClock.m_pTex2D->DrawUpdate2D();
	}
}

const math::Vector2 StageTimer::GetPartsPos( const std::string name ) const
{
	if( !m_plateStageTimer.m_pTex2D ){
		DEBUG_ASSERT( 0, "�K�v�ȃN���X������Ă��Ȃ�" );
		return math::Vector2();
	}

	// �X�e�[�^�X���j���[�̍�����W�擾
	const TEX_INIT_INFO &statusMenuInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_plateStageTimer.m_pTex2D->GetDrawInfo().m_fileName.c_str() );
	math::Vector2 retPos = m_plateStageTimer.m_pTex2D->GetDrawInfo().m_posOrigin;
	retPos -= math::Vector2( statusMenuInfo.m_sizeWidth / 2.0f, statusMenuInfo.m_sizeHeight / 2.0f );

	// ��������p�[�c�̈ʒu�𑫂��Z
	Common::PARTS_INFO info = GetPartsInfo(name);
	retPos += info.m_pos;
	return retPos;
}

const Common::PARTS_INFO &StageTimer::GetPartsInfo( const std::string name ) const
{
	auto it = m_partsMap.find( name.c_str() );
	if( it != m_partsMap.end() ){
		return (*it).second;
	}
	DEBUG_ASSERT( 0, "�p�[�c��������܂���\n" );
	return (*m_partsMap.begin()).second;
}



