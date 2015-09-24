/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "EnemyWizard.h"
#include "Game/GameMap.h"
#include "Game/GameRegister.h"
#include "System/Draw2D/SystemDraw2DDefine.h"

EnemyWizard *EnemyWizard::Create( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos )
{
	std::string fileStr = "EnemyWizard.json";
	return NEW EnemyWizard( fileStr, enemyLevel, uniqueID, enemyPos );
}

EnemyWizard::EnemyWizard( const std::string &fileName, const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos )
	: EnemyBase( fileName, uniqueID, Common::ENEMY_KIND_WIZARD, enemyLevel, enemyPos )
{
}

EnemyWizard::~EnemyWizard(void)
{
}

bool EnemyWizard::InitMain()
{
	math::Vector2 pos = GetDrawInfo().m_posOrigin;
	m_pCrystal = WizardCrystal::Create();
	m_pCrystal->SetPos( math::Vector2( pos.x - 200 , pos.y ) );
	m_pCrystalSec = WizardCrystal::Create();
	m_pCrystalSec->SetPos( math::Vector2( pos.x + 200 , pos.y ) );
	return true;
}

bool EnemyWizard::DieMainCustom()
{
	if( m_pCrystal ){
		m_pCrystal->TaskDie();
	}
	if( m_pCrystalSec ){
		m_pCrystalSec->TaskDie();
	}
	m_pCrystal = NULL;
	m_pCrystalSec = NULL;
	return true;
}

 const uint32_t EnemyWizard::GetEnemyDefaultHP() const
{
	// Lvによって最大ライフ変更
	return 50 + ( 150 * ( GetEnemyLevel() - 1 ) );
}

 const uint32_t EnemyWizard::GetEnemyDefaultSPD() const
{
	return 1 + ( GetEnemyLevel() / 3 );
}

const uint32_t EnemyWizard::GetPlayerHitDamage() const
{
	return 10 + ( 5 * GetEnemyLevel() );
}

void EnemyWizard::ReduceDamage( Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:
		// マシンガン攻撃は効きにくい( 0.9~4.5 )
		eventId.m_eventValue *= 0.9f - ( static_cast<float>( GetEnemyLevel()*0.05f ) );
		break;
	case Common::EVENT_HIT_BLADE_PLAYER:
		// 斬撃はダメージを増やす
		eventId.m_eventValue *= 1.2f;
		break;
	}
}

math::Vector2 EnemyWizard::GetCrystalPosFirst() const
{
	if( m_pCrystal ){
		return m_pCrystal->GetPos();
	}
	return math::Vector2();
}

math::Vector2 EnemyWizard::GetCrystalPosSecond() const
{
	if( m_pCrystalSec ){
		return m_pCrystalSec->GetPos();
	}
	return math::Vector2();
}




WizardCrystal *WizardCrystal::Create()
{
	return NEW WizardCrystal();
}

WizardCrystal::WizardCrystal()
{
	m_drawTexture.m_pTex2D = Game2DBase::Create( "EnemyWizardCrystal.json" );
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = math::Vector2();
}

WizardCrystal::~WizardCrystal()
{

}

const math::Vector2 WizardCrystal::GetPos() const
{
	if( m_drawTexture.m_pTex2D ){
		return m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin;
	}
	return math::Vector2();
}

void WizardCrystal::SetPos( const math::Vector2 &centerPos )
{
	// 指定位置から半径xのところをぐるぐる回る
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = centerPos;
}

void WizardCrystal::Update()
{

}
void WizardCrystal::DrawUpdate()
{
	m_drawTexture.m_pTex2D->DrawUpdate2D();
}

