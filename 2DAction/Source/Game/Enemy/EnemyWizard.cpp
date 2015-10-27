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

#include "Flow/FlowManager.h"
#include "Flow/Process/FlowProcessBossEnemyDeath.h"

EnemyWizard *EnemyWizard::Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
{
	std::string fileStr = "EnemyWizard.json";
	return NEW EnemyWizard( fileStr, uniqueID, enemyLevel, enemyPos );
}

EnemyWizard::EnemyWizard( const std::string &fileName, const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos )
: EnemyBase( fileName, uniqueID, Common::ENEMY_KIND_WIZARD, enemyLevel, enemyPos )
, m_crystalAround( CRYSTAL_AROUND_MINE )
, m_damageType( DAMAGE_TYPE_SLASH )
{
}

EnemyWizard::~EnemyWizard(void)
{
}

bool EnemyWizard::InitMain()
{
	math::Vector2 pos = GetDrawInfo().m_posOrigin;

	uint32_t crystalMax = 2;
	uint32_t randamValue = Utility::GetRandamValue( 360, 0 );
	for( uint32_t i = 0; i < crystalMax ; ++i ){
		uint32_t startAngle = randamValue;
		startAngle += static_cast<int>( math::ANGLE_FULL / static_cast<float>( crystalMax ) ) * i;
		WizardCrystal *pCrystal = WizardCrystal::Create( startAngle );
		m_pCrystalVec.push_back( pCrystal );
	}
	return true;
}

void EnemyWizard::UpdateCustom()
{
	math::Vector2 centerPos = ( m_crystalAround == CRYSTAL_AROUND_MINE ) ? GetDrawInfo().m_posOrigin : Utility::GetPlayerPos() ;

	for( uint32_t i = 0; i < m_pCrystalVec.size() ; ++i ){
		m_pCrystalVec.at( i )->SetPos( centerPos );
	}
}

bool EnemyWizard::DieMainCustom()
{
	for( uint32_t i = 0; i < m_pCrystalVec.size() ; ++i ){
		m_pCrystalVec.at( i )->TaskDie();
	}
	return true;
}

/* ================================================ */
/**
 * @brief	敵クラスのデフォルト値取得
 */
/* ================================================ */
const uint32_t EnemyWizard::GetEnemyDefaultHP() const
{
	// Lvによって最大ライフ変更
	return 1000 + ( 300 * GetEnemyLevel() );
}

 const float EnemyWizard::GetEnemyDefaultSPD() const
{
	return static_cast<float>( 1 + ( GetEnemyLevel() / 3 ) );
}

const uint32_t EnemyWizard::GetPlayerHitDamage() const
{
	return 10 + ( 5 * GetEnemyLevel() );
}

/* ================================================ */
/**
 * @brief	敵の種類、レベル等でダメージ軽減処理
 */
/* ================================================ */
void EnemyWizard::ReduceDamage( Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:
		if( m_damageType == DAMAGE_TYPE_BULLET ){
			eventId.m_eventValue /= 2;
		}
		else{
			eventId.m_eventValue = 0;
			// ダメージ無効エフェクト
			GameEffect::CreateEffect( GameEffect::EFFECT_INVALID_DAMAGE, GetDrawInfo().m_posOrigin );
			// 無効SE
			SoundManager::GetInstance()->PlaySE( "invalidDamage" );
		}
		break;
	case Common::EVENT_HIT_BLADE_PLAYER:
		if( m_damageType == DAMAGE_TYPE_SLASH ){
			eventId.m_eventValue /= 2;
		}
		else{
			eventId.m_eventValue = 0;
			// ダメージ無効エフェクト
			GameEffect::CreateEffect( GameEffect::EFFECT_INVALID_DAMAGE, GetDrawInfo().m_posOrigin );
			// 無効SE
			SoundManager::GetInstance()->PlaySE( "invalidDamage" );
		}
		break;
	}
}

/* ================================================ */
/**
 * @brief	HPがなくなり倒されたときに呼ばれる
 */
/* ================================================ */
void EnemyWizard::EnemyDeath()
{
	// 死亡したので専用演出再生
	ProcessBossEnemyDeath *pDeathEffect = ProcessBossEnemyDeath::Create( m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );
	FlowManager *pFlowMan = FlowManager::GetInstance();
	if( pFlowMan && pDeathEffect ){
		pFlowMan->SetupSpecialEffect( pDeathEffect );
	}
	if( m_drawTexture.m_pTex2D ){
		m_drawTexture.m_pTex2D->SetAnim( "death" );
	}

	// ラストステージならアイテムを落とす
	if( FlowManager::GetInstance()->GetCurrentFlowKind() == Common::FLOW_STAGE10 ){
		GameManager *pGameMan = GameRegister::GetInstance()->UpdateManagerGame();
		if( pGameMan ){
			pGameMan->CreateItem( Common::ITEM_KIND_LIFE_UP, GetDrawInfo().m_posOrigin );
		}
	}
}

/* ================================================ */
/**
 * @brief	指定Indexのクリスタル位置取得
 */
/* ================================================ */
void EnemyWizard::HitPlayreSlashing( const uint32_t &damageValue )
{
	// スタンはなし
	//SetStunTime( 10 );
	GameEffect::CreateEffect( GameEffect::EFFECT_SLASHING_HIT, m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin );
	UpdateEnemyDamage( damageValue );
}

/* ================================================ */
/**
 * @brief	指定Indexのクリスタル位置取得
 */
/* ================================================ */
const math::Vector2 EnemyWizard::GetCrystalPos( const uint32_t &index ) const
{
	if( m_pCrystalVec.size() > index ){
		return m_pCrystalVec.at( index )->GetPos();
	}

	DEBUG_ASSERT( 0, "EnemyWizard::GetCrystalPosが想定外");
	return math::Vector2();
}

/* ================================================ */
/**
 * @brief	クリスタルの発射する弾のダメージ量取得
 */
/* ================================================ */
uint32_t EnemyWizard::GetBulletDamage() const
{
	return 15 + ( 4 * GetEnemyLevel() );
}

/* ================================================ */
/**
 * @brief	クリスタルの回転中心対象セット
 */
/* ================================================ */
void EnemyWizard::SetCrystalAroundTarget( const CRYSTAL_AROUND &type )
{
	m_crystalAround = type;
}

/* ================================================ */
/**
 * @brief	クリスタルの回転半径セット
 */
/* ================================================ */
void EnemyWizard::SetCrystalAroundDistance( const uint32_t &distance )
{
	for( uint32_t i = 0; i < m_pCrystalVec.size() ; ++i )
	{
		m_pCrystalVec.at(i)->SetRadius( distance );
	}
}


WizardCrystal *WizardCrystal::Create( uint32_t startRot )
{
	return NEW WizardCrystal( startRot );
}

WizardCrystal::WizardCrystal( uint32_t startRot )
: m_startRot( startRot )
, m_circleRadius( 100 )
{
	m_drawTexture.m_pTex2D = Game2DBase::Create( "EnemyWizardCrystal.json" );
	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = math::Vector2();
}

WizardCrystal::~WizardCrystal()
{

}

/* ================================================ */
/**
 * @brief	クリスタル位置取得
 */
/* ================================================ */
const math::Vector2 WizardCrystal::GetPos() const
{
	if( m_drawTexture.m_pTex2D ){
		return m_drawTexture.m_pTex2D->GetDrawInfo().m_posOrigin;
	}
	return math::Vector2();
}

/* ================================================ */
/**
 * @brief	クリスタル位置セット
 */
/* ================================================ */
void WizardCrystal::SetPos( const math::Vector2 &centerPos )
{
	// 指定位置から半径xのところをぐるぐる回る
	math::Vector2 startPos = math::Vector2();
	startPos.x += m_circleRadius;

	math::Vector2 currPos = math::Vector2();
	float rotateAngle = static_cast<float>( m_rotCounter + m_startRot );
	currPos.x = startPos.x * math::Cos( rotateAngle ) - startPos.y * math::Sin( rotateAngle );
	currPos.y = startPos.x * math::Sin( rotateAngle ) + startPos.y * math::Cos( rotateAngle );

	currPos += centerPos;

	m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = currPos;
}

void WizardCrystal::Update()
{
	++m_rotCounter;
	if( m_rotCounter > math::ANGLE_FULL ){
		m_rotCounter = 0;
	}
}
void WizardCrystal::DrawUpdate()
{
	m_drawTexture.m_pTex2D->DrawUpdate2D();
}

