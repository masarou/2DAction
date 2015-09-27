/* ====================================================================== */
/**
 * @brief  敵AIの通常クラス(プレイヤー未発見)
 *
 * @note
 *		デフォルトAI
 */
/* ====================================================================== */

#include "Game/GameRegister.h"
#include "Game/Enemy/EnemyManager.h"
#include "Game/Enemy/EnemyBase.h"
#include "Game/Enemy/EnemyWizard.h"

#include "EnemyAIWizard.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Game/Effect/GameEffect.h"

AIWizard *AIWizard::Create()
{
	AIWizard *tmpAI = NEW AIWizard();
	return tmpAI;
}

AIWizard::AIWizard(void)
: m_currAction( ACTION_WAY_BULLET )
, m_nextAction( ACTION_MAX )
, m_waitCounter( 0 )
{
}


AIWizard::~AIWizard(void)
{
}

bool AIWizard::InitAI()
{
	return true;
}

void AIWizard::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	// アニメ更新
	std::string animTag = "";
	math::Vector2 vec = Utility::GetPlayerPos() - enemyInfo.m_posOrigin;
	vec.Normalize();
	switch( Utility::GetDirection( vec.x, vec.y ) ){
	default:
		break;
	case InputWatcher::BUTTON_UP:
		animTag = "up";
		break;
	case InputWatcher::BUTTON_DOWN:
		animTag = "down";
		break;
	case InputWatcher::BUTTON_LEFT:
		animTag = "left";
		break;
	case InputWatcher::BUTTON_RIGHT:
		animTag = "right";
		break;
	}
	SetEnemyAnim( animTag );
	SetEnemyEyeSight( vec );

	// 移動
	enemyInfo.m_posOrigin += vec * GetEnemySPD();

	// 以下、攻撃関数
	if( m_waitCounter > 0 ){
		--m_waitCounter;
		return;
	}
	
	if( !GetEnemeyMine() || GetEnemeyMine()->GetKind() != Common::ENEMY_KIND_WIZARD ){
		DEBUG_ASSERT( 0, "敵クラスがNULL,もしくは敵クラスの種類が想定外");
		return;
	}

	bool isFinish = false;
	switch( m_currAction ){
	case ACTION_NORMAL:
		isFinish = ActionNormal();
		break;
	case ACTION_CONTINUE_BULLET:
		isFinish = ActionContinueBullet();
		break;
	case ACTION_SET_CRYSTAL_PLAYER:
		isFinish = ActionSetCrystalPlayer();
		break;
	case ACTION_WAY_BULLET:
		isFinish = ActionWayBullet();
		break;
	}

	// コルーチンリセット
	if( isFinish ){
		coroutine tmp;
		m_coro = tmp;

		// 次のアクションがセットされていれば変更
		if( m_nextAction != ACTION_MAX ){
			m_currAction = m_nextAction;
			m_nextAction = ACTION_MAX;
		}
	}
}

void AIWizard::ChangeActionType( const ACTION_TYPE &nextAction )
{
	if( !GetEnemeyMine() || GetEnemeyMine()->GetKind() != Common::ENEMY_KIND_WIZARD ){
		DEBUG_ASSERT( 0, "敵クラスがNULL,もしくは敵クラスの種類が想定外");
		return;
	}

	m_nextAction = nextAction;

	switch( m_nextAction ){
	case ACTION_SET_CRYSTAL_PLAYER:
		{
			EnemyWizard *pWizard = static_cast<EnemyWizard*>( UpdateEnemyMine() );
			pWizard->SetCrystalAroundTarget( EnemyWizard::CRYSTAL_AROUND_PLAYER );
			m_waitCounter = 30;
		}
		break;
	default:
		{
			EnemyWizard *pWizard = static_cast<EnemyWizard*>( UpdateEnemyMine() );
			pWizard->SetCrystalAroundTarget( EnemyWizard::CRYSTAL_AROUND_MINE );
		}
		break;
	}
}

bool AIWizard::ActionNormal()
{
	EnemyWizard *pWizard = static_cast<EnemyWizard*>( UpdateEnemyMine() );
	for( uint32_t i = 0; i < pWizard->GetCrystalIndex() ; ++i ){
		ShootBullet( pWizard->GetCrystalPos( i ), math::Vector2(), pWizard->GetBulletDamage() );
	}
	
	uint32_t next = Utility::GetRandamValue( ACTION_MAX, 0 );
	ChangeActionType( static_cast<ACTION_TYPE>(next) );

	m_waitCounter = 120;
	return true;
}


bool AIWizard::ActionSetCrystalPlayer()
{
	ActionNormal();

	return true;
}

bool AIWizard::ActionWayBullet()
{
	uint32_t bulletNum = 10;
	EnemyWizard *pWizard = static_cast<EnemyWizard*>( UpdateEnemyMine() );
	for( uint32_t i = 0 ; i < bulletNum ; ++i )
	{
		for( uint32_t j = 0; j < pWizard->GetCrystalIndex() ; ++j ){
			math::Vector2 crystalPos = pWizard->GetCrystalPos( j );
			math::Vector2 vec = Utility::GetPlayerPos() - crystalPos;
			math::Vector2 result = math::Vector2();

			result.x = vec.x * math::Cos( (math::ANGLE_FULL / bulletNum) * i ) - vec.y * math::Sin( (math::ANGLE_FULL / bulletNum) * i );
			result.y = vec.x * math::Sin( (math::ANGLE_FULL / bulletNum) * i ) + vec.y * math::Cos( (math::ANGLE_FULL / bulletNum) * i );
			ShootBullet( crystalPos, result, pWizard->GetBulletDamage() );
		}
	}

	uint32_t next = Utility::GetRandamValue( ACTION_MAX, 0 );
	ChangeActionType( static_cast<ACTION_TYPE>(next) );

	m_waitCounter = 60;
	return true;
}


