/* ====================================================================== */
/**
 * @brief  �Q�[����ʍ����̃v���C���[�X�e�[�^�X���
 *
 * @note
 *		
 *		
 */
/* ====================================================================== */

#include "System/picojson.h"
#include "GamePlayerStatusMenu.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"

// �Œ�l
static uint32_t ITEM_LEVEL_MAX = 9;

PlayerStatusMenu::PlayerStatusMenu()
: MenuWindow( "MenuPlayerStatus.json", Common::PRIORITY_HIGH, math::Vector2( 240, WINDOW_HEIGHT - 35 ) )
, m_playerLife( 0 )
, m_playerLifeMax( 0 )
, m_itemLevelDamage( 0 )
, m_itemLevelRapid( 0 )
{
}

PlayerStatusMenu::~PlayerStatusMenu(void)
{
}

bool PlayerStatusMenu::InitMenu()
{
	// �e��p�[�c�Z�b�g
	TEX_DRAW_INFO drawInfo;
	m_lifeGauge.Init();
	m_lifeGauge.m_pTex2D = Game2DBase::Create("LifeGauge.json");
	drawInfo.m_fileName = "LifeGauge.json";
	drawInfo.m_prioity = Common::PRIORITY_HIGH;
	drawInfo.m_posOrigin = math::Vector2( 22, WINDOW_HEIGHT - 24 );
	drawInfo.m_usePlayerOffset = false;
	drawInfo.m_arrangeOrigin = math::Vector2( 0.0f, 0.0f );	// �g��k������摜�Ȃ̂ŃZ�b�g���Ă���
	m_lifeGauge.m_pTex2D->SetDrawInfo( drawInfo );

	// �J�E���g�A�b�v�A�j���͍s��Ȃ�
	PartsCounter *pPartsHPCurr = GetPartsCounter("lifeCurr");
	PartsCounter *pPartsHPMax = GetPartsCounter("lifeMax");
	if( pPartsHPCurr && pPartsHPMax ){
		pPartsHPCurr->SetCountAnimFlg( false );
		pPartsHPMax->SetCountAnimFlg( false );
	}

	return true;
}

void PlayerStatusMenu::UpdateMenu()
{
	// ���݂�HP�`��
	PartsCounter *pPartsHPCurr = GetPartsCounter("lifeCurr");
	PartsCounter *pPartsHPMax = GetPartsCounter("lifeMax");
	if( pPartsHPCurr && pPartsHPMax ){
		pPartsHPCurr->SetValue( m_playerLife );
		pPartsHPMax->SetValue( m_playerLifeMax );
	}
}

void PlayerStatusMenu::DrawUpdateMenu()
{
	// ���C�t�Q�[�W�`��
	if( m_lifeGauge.m_pTex2D ){
		TEX_DRAW_INFO &drawInfo = m_lifeGauge.m_pTex2D->UpdateDrawInfo();
		const TEX_INIT_INFO &lifeTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( drawInfo.m_fileName.c_str() );
		float ratio = static_cast<float>(m_playerLife)/static_cast<float>(m_playerLifeMax);
		float lifeValue = ( 450 / lifeTexInfo.m_sizeWidth) * ratio; // 450�̓��C�t�o�[�̒���

		if( math::Absf( drawInfo.m_scale.x - lifeValue ) > 0.3f ){
			drawInfo.m_scale.x *= (drawInfo.m_scale.x - lifeValue < 0.0f) ? 1.02f : 0.98f ;
		}
		else{
			drawInfo.m_scale.x = lifeValue;
		}
		m_lifeGauge.m_pTex2D->DrawUpdate2D();
	}
}

bool PlayerStatusMenu::DieMainMenu()
{
	m_lifeGauge.DeleteAndInit();
	return true;
}

void PlayerStatusMenu::AddItemLevel( Common::ITEM_KIND kind )
{
	switch( kind ){
	default:
		break;
	case Common::ITEM_KIND_DAMAGE_UP:
		if( m_itemLevelDamage < ITEM_LEVEL_MAX ){
			++m_itemLevelDamage;
		}
		break;
		
	case Common::ITEM_KIND_RAPID_BULLET:
		if( m_itemLevelRapid < ITEM_LEVEL_MAX ){
			++m_itemLevelRapid;
		}
		break;
	}
}

/* ================================================ */
/**
 * @brief	���ݕ\�����Ă���A�C�e�����x���擾
 */
/* ================================================ */
const uint32_t PlayerStatusMenu::GetItemLevel( const Common::ITEM_KIND &kind ) const
{
	uint32_t retval = 0;
	switch( kind ){
	default:
		DEBUG_ASSERT( 0, "�A�C�e���̎�ނ��z��O" );
		break;
	case Common::ITEM_KIND_DAMAGE_UP:
		retval = m_itemLevelDamage;
		break;
		
	case Common::ITEM_KIND_RAPID_BULLET:
		retval = m_itemLevelRapid;
		break;
	}
	return retval;
}

/* ================================================ */
/**
 * @brief	�e��Update�֐�
 */
/* ================================================ */
void PlayerStatusMenu::SetPlayerHP( const uint32_t &val, const uint32_t &lifeMax )
{
	m_playerLife = val;
	m_playerLifeMax = lifeMax;
}
