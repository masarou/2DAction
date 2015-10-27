/* ====================================================================== */
/**
 * @brief  ラスボスAI
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "Game/GameRegister.h"
#include "Game/Enemy/EnemyManager.h"

#include "EnemyAILastBoss.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "Game/Effect/GameEffect.h"

// 本体から見た両手の位置
static const math::Vector2 BASE_LEFT_HAND_POS = math::Vector2( 150.0f, 50.0f );
static const math::Vector2 BASE_RIGHT_HAND_POS = math::Vector2( -150.0f, 50.0f );

EnemyAILastBoss *EnemyAILastBoss::Create()
{
	EnemyAILastBoss *tmpAI = NEW EnemyAILastBoss();
	return tmpAI;
}

EnemyAILastBoss::EnemyAILastBoss(void)
: m_pLeftHand( NULL )
, m_pRightHand( NULL )
, m_moveBasicPos( math::Vector2() )
{
}


EnemyAILastBoss::~EnemyAILastBoss(void)
{
}

bool EnemyAILastBoss::InitAI()
{

	// 基準点更新
	math::Vector2 letfPos = GetEnemyPos();
	letfPos += BASE_LEFT_HAND_POS;
	math::Vector2 rightPos = GetEnemyPos();
	rightPos += BASE_RIGHT_HAND_POS;

	m_pRightHand	= LastBossRight::Create( "EnemyLastBossRight.json", rightPos );
	m_pRightHand->SetAnotherHand( m_pLeftHand );
	m_pLeftHand		= LastBossLeft::Create( "EnemyLastBossLeft.json", letfPos );
	m_pLeftHand->SetAnotherHand( m_pRightHand );
	return true;
}

void EnemyAILastBoss::ExecMain( TEX_DRAW_INFO &enemyInfo, ACTION_ARRAY &actionInfo )
{
	// 最初を除き基本的にm_moveBasicPosで上書き
	// その後、ふわふわ浮いている分の数値を足してやる
	if( m_moveBasicPos == math::Vector2() ){
		m_moveBasicPos = enemyInfo.m_posOrigin;
	}
	else{
		enemyInfo.m_posOrigin = m_moveBasicPos;
	}

	// 両腕の更新(新たな行動タイプ設定や、定位置の更新)
	ExecHandsUpdate( enemyInfo );

	// ふわふわ浮いている分の数値
	static uint32_t counter = 0;
	enemyInfo.m_posOrigin.y += math::Sin( static_cast<float>( counter ) ) * 10.0f;
	++counter;

}

void EnemyAILastBoss::EnemyIsDead()
{
	// 両手クラス死亡
	m_pRightHand->StartDie();
	m_pLeftHand->StartDie();
}




/* ================================================ */
/**
 * @brief	両腕更新
 */
/* ================================================ */
void EnemyAILastBoss::ExecHandsUpdate( TEX_DRAW_INFO &enemyInfo )
{
	// 基準点更新
	math::Vector2 letfPos = enemyInfo.m_posOrigin;
	letfPos += BASE_LEFT_HAND_POS;
	math::Vector2 rightPos = enemyInfo.m_posOrigin;
	rightPos += BASE_RIGHT_HAND_POS;

	if( m_pRightHand && m_pLeftHand ){
		m_pRightHand->SetBasicPos( rightPos );
		m_pLeftHand->SetBasicPos( letfPos );
	}
}




/* ====================================================================== */
/**
 * @brief  ラスボス右手左手クラス
 *
 * @note
 *		
 */
/* ====================================================================== */


uint32_t LastBossHand::m_createUniqueEnemyCount = 0;

LastBossHand::LastBossHand( const std::string &readFileName, const math::Vector2 &enemyPos )
: TaskUnit("Enemy")
, Collision2DUnit( readFileName.c_str() )
, m_basicPos( enemyPos )
, m_moveTargetPos( math::Vector2() )
, m_waitCounter( 0 )
, m_currAction( ACTION_SUMMON )
, m_nextAction( ACTION_NONE )
, m_pAnotherHand( NULL )
{


}

LastBossHand::~LastBossHand()
{
	m_createUniqueEnemyCount = 0;
}

void LastBossHand::SetBasicPos( math::Vector2 basicPos )
{
	m_basicPos = basicPos;
}

bool LastBossHand::Init()
{
	if( m_drawTexture.m_pTex2D ){
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = m_basicPos;
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_rot = GetRotateDefault();
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	}
	return true;
}

bool LastBossHand::ExecAction()
{
	if( m_waitCounter > 0){
		// 次の行動待ち
		--m_waitCounter;
		return false;
	}

	bool isActionEnd = false;
	switch( m_currAction ){
	case ACTION_NONE:
		isActionEnd = true;
		
		// 定位置に移動
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_posOrigin = m_basicPos;
		break;
	case ACTION_FIST:
		isActionEnd = ExecFist( m_drawTexture.m_pTex2D->UpdateDrawInfo() );
		break;
	case ACTION_GUARD:
		isActionEnd = true;//ExecGuard( m_drawTexture.m_pTex2D->UpdateDrawInfo() );
		break;
	case ACTION_SUMMON:
		isActionEnd = ExecSummonMonster( m_drawTexture.m_pTex2D->UpdateDrawInfo() );
		break;
	case ACTION_SUMMONS:
		isActionEnd = true;//ExecSummonLightMonsters( m_drawTexture.m_pTex2D->UpdateDrawInfo() );
		break;
	}

	if( isActionEnd ){
		// コルーチンリセット
		coroutine tmp;
		m_coro = tmp;

		// 次のアクションがセットされていれば変更
		if( m_nextAction != ACTION_NONE ){
			m_currAction = m_nextAction;
			m_nextAction = ACTION_MAX;
		}
		else{
			m_currAction = GetNextActionKind();
		}
	}

	return isActionEnd;
}

void LastBossHand::SetAnotherHand( LastBossHand *pHand )
{
	if( m_pAnotherHand ){
		DEBUG_ASSERT( 0, "もうすでに片方の腕がセットされている");
		return;
	}
	m_pAnotherHand = pHand;
}

void LastBossHand::Update()
{
	ExecAction();
}


void LastBossHand::DrawUpdate()
{
	if( m_drawTexture.m_pTex2D ){
		m_drawTexture.m_pTex2D->DrawUpdate2D();
	}
}

/* ================================================ */
/**
 * @brief	次の行動選定
 */
/* ================================================ */
LastBossHand::ACTION_KIND LastBossHand::GetNextActionKind()
{
	ACTION_KIND retAction = ACTION_NONE;
	if( CountUniqueMonster() == 0 ){
		// ユニークモンスターが以内なら即生成
		retAction = ACTION_SUMMON;
	}
	else{
		for(;;){
			retAction = static_cast<ACTION_KIND>( Utility::GetRandamValue( ACTION_MAX, ACTION_NONE ) );

			// モンスター召喚の時は召喚してもよいかどうかチェック
			if( retAction == ACTION_SUMMON ){
				if( IsCreateUniqueMonster() ){
					break;
				}
			}
			else if( retAction == ACTION_SUMMONS ){
				if( IsCreateLightMonster() ){
					break;
				}
			}
			else{
				// 行動決定！
				break;
			}
		}
	}

	if( retAction == ACTION_SUMMON ){
		// ユニークモンスター召喚ならば召喚回数に応じて待ち時間設定
		++m_createUniqueEnemyCount;
		m_waitCounter = 60 + ( 20 * m_createUniqueEnemyCount );
		// あんまり長いとバグっぽくなるので
		if( m_waitCounter > 200 ){
			m_waitCounter = 0;
		}
		DEBUG_PRINT( "m_waitCounter = %d\n", m_waitCounter );
	}

	return retAction;
}

/* ================================================ */
/**
 * @brief	生成モンスター配列
 */
/* ================================================ */


/* ================================================ */
/**
 * @brief	ユニークモンスターを生成していいか決定
 */
/* ================================================ */
uint32_t LastBossHand::CountUniqueMonster() const
{
	uint32_t countEnemy = 0;
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();	
	// 敵の生成
	if( pEnemyManager ){
		for( uint32_t i = 0; i < NUMBEROF(Common::s_uniqueEnemyKind) ; ++i ){
			countEnemy += pEnemyManager->CountEnemy( Common::s_uniqueEnemyKind[i] );
		}
	}

	// もう一方の片腕が生成中なら+1
	if( m_pAnotherHand && m_pAnotherHand->GetCurrentActionKind() == ACTION_SUMMON ){
		++countEnemy;
	}

	return countEnemy;
}
bool LastBossHand::IsCreateUniqueMonster() const
{
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();	
	// 敵の生成
	if( pEnemyManager ){

		uint32_t countEnemy = CountUniqueMonster();

		// ユニークモンスターは2体以上召喚しない
		if( countEnemy < 1 ){
			return true;
		}
		if( countEnemy < 2
			&& m_pAnotherHand && m_pAnotherHand->GetCurrentActionKind() != ACTION_SUMMON ){
				return true;
		}
	}
	return false;
}

/* ================================================ */
/**
 * @brief	雑魚モンスターを生成していいか決定
 */
/* ================================================ */
bool LastBossHand::IsCreateLightMonster() const
{
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();	
	// 敵の生成
	if( pEnemyManager ){
		uint32_t countEnemy = 0;
		countEnemy += pEnemyManager->CountEnemy();
		if( countEnemy < 5 ){
			return true;
		}
	}
	return false;
}

/* ================================================ */
/**
 * @brief	生成モンスター決定
 */
/* ================================================ */
Common::ENEMY_KIND LastBossHand::DecideCreateMonster() const
{
	// 生成するモンスターを決定
	// 同じモンスターは避ける
	Common::ENEMY_KIND kind = Common::ENEMY_KIND_SLIME;
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
	if( pEnemyManager ){
		for(;;){
			kind = Common::s_uniqueEnemyKind[ Utility::GetRandamValue( NUMBEROF( Common::s_uniqueEnemyKind ), 0 ) ];
			// 既に同じモンスターが生成されているかどうかチェック
			if( pEnemyManager->CountEnemy( kind ) == 0 ){
				break;
			}
		}
	}
	return kind;
}

Common::ENEMY_KIND LastBossHand::DecideCreateLightMonster() const
{
	// 生成する雑魚モンスターを決定
	Common::ENEMY_KIND kind = Common::ENEMY_KIND_SLIME;
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
	if( pEnemyManager ){
		kind = Common::s_lightEnemyKind[ Utility::GetRandamValue( NUMBEROF( Common::s_lightEnemyKind ), 0 ) ];
	}
	return kind;
}


/* ================================================ */
/**
 * @brief	指定位置に移動し続ける関数(trueが帰ると到達)
 */
/* ================================================ */
bool LastBossHand::MoveToTargetPos( const math::Vector2 &targetPos, const float &maxSpeed, const float &rateSpeed )
{
	TEX_DRAW_INFO &drawInfo = m_drawTexture.m_pTex2D->UpdateDrawInfo();
	math::Vector2 currentPos = drawInfo.m_posOrigin;
	math::Vector2 diff = targetPos - currentPos;

	bool isMoveFinish = false;
	if( diff.GetLength() < 20.0f ){
		// 距離が一定値以下なら目的地に到達
		drawInfo.m_posOrigin = targetPos;
		isMoveFinish = true;
	}
	else{
		// maxSpeedを超えて移動しないようにしながら移動1
		math::Vector2 move = diff * rateSpeed;
		if( move.GetLength() >= maxSpeed ){
			move.Normalize();
			move *= maxSpeed;
		}
		drawInfo.m_posOrigin += move;
	}
	return isMoveFinish;
}

/* ================================================ */
/**
 * @brief	指定の回転角度まで一定量で回転
 */
/* ================================================ */
bool LastBossHand::RotateToTargetAngle( const math::Angle &targetDegree, const bool &forceSet )
{
	math::Angle currDegree = m_drawTexture.m_pTex2D->GetDrawInfo().m_rot;

	float diff = targetDegree.GetDegree() - currDegree.GetDegree();
	
	bool isFinish = false;
	if( math::Absf(diff) < 5.0f || forceSet ){
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_rot = targetDegree;
		isFinish = true;
	}
	else{
		m_drawTexture.m_pTex2D->UpdateDrawInfo().m_rot += diff * 0.03f;
	}
	return isFinish;
}



LastBossRight *LastBossRight::Create( const std::string &readFileName, const math::Vector2 &enemyPos )
{
	return NEW LastBossRight( readFileName, enemyPos );
}

LastBossRight::LastBossRight( const std::string &readFileName, const math::Vector2 &enemyPos )
: LastBossHand( readFileName, enemyPos )
{

}

LastBossRight::~LastBossRight()
{
}


LastBossLeft *LastBossLeft::Create( const std::string &readFileName, const math::Vector2 &enemyPos )
{
	return NEW LastBossLeft( readFileName, enemyPos );
}

LastBossLeft::LastBossLeft( const std::string &readFileName, const math::Vector2 &enemyPos )
: LastBossHand( readFileName, enemyPos )
{

}

LastBossLeft::~LastBossLeft()
{
}