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
#include "Game/Effect/GameEffect.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Sound/SystemSoundManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"

static uint32_t CRYSTAL_DISTANCE_TO_PLAYER = 150;
static uint32_t CRYSTAL_DISTANCE_TO_ENEMY = 100;

AIWizard *AIWizard::Create()
{
	AIWizard *tmpAI = NEW AIWizard();
	return tmpAI;
}

AIWizard::AIWizard(void)
: m_nextWorpPos( math::Vector2() )
, m_currAction( ACTION_WAY_BULLET )
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
	DEBUG_PRINT( "AIWizard::ExecMain開始\n" );

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

	if( m_currAction != ACTION_WORP ){
		// 移動
		enemyInfo.m_posOrigin += vec * static_cast<float>( GetEnemySPD() );
	}

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
	DEBUG_PRINT( "AIWizard::m_currAction = %d\n", m_currAction );
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
	case ACTION_WORP:
		isFinish = ActionEnemyWorp( enemyInfo );
		break;
	}

	// コルーチンリセット
	if( isFinish ){
		coroutine tmp;
		m_coro = tmp;

		DEBUG_PRINT( "AIWizardの実行アクション終了\n" );

		// 次のアクションがセットされていれば変更
		if( m_nextAction != ACTION_MAX ){
			m_currAction = m_nextAction;
			m_nextAction = ACTION_MAX;
			DEBUG_PRINT( "次のアクションは%dです\n", m_currAction );
		}
	}

	DEBUG_PRINT( "AIWizard::ExecMain終了\n" );
}

void AIWizard::EnemyRecievedEvent( const Common::CMN_EVENT &eventInfo )
{
	if( GetEnemeyMine()->GetStatus() != TaskUnit::TASK_ALIVE ){
		return;
	}

	switch( eventInfo.m_event ){
	case Common::EVENT_HIT_BULLET_PLAYER:	// Playerの弾に当たった
		break;

	case Common::EVENT_HIT_BLADE_PLAYER:	// Playerの斬撃に当たった
		if( m_currAction != ACTION_WORP ){
			SetInterruptAction( ACTION_WORP );
			GameEffect::CreateEffect( GameEffect::EFFECT_WORP, GetEnemeyMine()->GetDrawInfo().m_posOrigin );
			SoundManager::GetInstance()->PlaySE("Worp");
		}
		break;
	}
}

AIWizard::ACTION_TYPE AIWizard::GetRandamNextAction()
{
	static const ACTION_TYPE s_attackAction[] = {
		ACTION_NORMAL,
		ACTION_CONTINUE_BULLET,
		ACTION_WAY_BULLET,
		ACTION_SET_CRYSTAL_PLAYER,
	};

	uint32_t type = Utility::GetRandamValue( NUMBEROF(s_attackAction), 0 );
	DEBUG_PRINT( "AIWizard::GetRandamNextAction is Selected No%d\n", type );
	return static_cast<AIWizard::ACTION_TYPE>( type );
}

void AIWizard::ChangeActionType( const ACTION_TYPE &nextAction )
{
	if( !GetEnemeyMine() || GetEnemeyMine()->GetKind() != Common::ENEMY_KIND_WIZARD ){
		DEBUG_ASSERT( 0, "敵クラスがNULL,もしくは敵クラスの種類が想定外");
		return;
	}

	if( m_nextAction != ACTION_MAX ){
		DEBUG_PRINT( "先約のACTION予約あり\n" );
		return;
	}

	DEBUG_PRINT( "Wizard ActionType変更 To %d\n", static_cast<uint32_t>(nextAction) );
	m_nextAction = nextAction;
	uint32_t crystalDistance = CRYSTAL_DISTANCE_TO_ENEMY;
	switch( m_nextAction ){
	case ACTION_SET_CRYSTAL_PLAYER:
		{
			EnemyWizard *pWizard = static_cast<EnemyWizard*>( UpdateEnemyMine() );
			pWizard->SetCrystalAroundTarget( EnemyWizard::CRYSTAL_AROUND_PLAYER );
			m_waitCounter = ACTION_INTERVAL;
			crystalDistance = CRYSTAL_DISTANCE_TO_PLAYER;
		}
		break;
	default:
		{
			EnemyWizard *pWizard = static_cast<EnemyWizard*>( UpdateEnemyMine() );
			pWizard->SetCrystalAroundTarget( EnemyWizard::CRYSTAL_AROUND_MINE );
		}
		break;
	}

	EnemyWizard *pWizard = static_cast<EnemyWizard*>( UpdateEnemyMine() );
	for( uint32_t i = 0; i < pWizard->GetCrystalIndex() ; ++i ){
		pWizard->SetCrystalAroundDistance( crystalDistance );
	}
}

void AIWizard::SetInterruptAction( const ACTION_TYPE &interruptAction )
{
	m_waitCounter = 0;
	m_currAction = interruptAction;
}

bool AIWizard::ActionNormal()
{
	EnemyWizard *pWizard = static_cast<EnemyWizard*>( UpdateEnemyMine() );
	for( uint32_t i = 0; i < pWizard->GetCrystalIndex() ; ++i ){
		ShootBullet( pWizard->GetCrystalPos( i ), math::Vector2(), pWizard->GetBulletDamage() );
	}
	
	ChangeActionType( GetRandamNextAction() );

	m_waitCounter = ACTION_INTERVAL;
	return true;
}


bool AIWizard::ActionSetCrystalPlayer()
{
	if( Utility::GetRandamValue( 2, 0 ) == 0 ){
		return ActionContinueBullet();
	}
	else{
		ActionNormal();
	}

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
	
	ChangeActionType( GetRandamNextAction() );

	m_waitCounter = ACTION_INTERVAL;
	return true;
}


