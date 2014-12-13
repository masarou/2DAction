

#include "GamePlayer.h"
#include "Game/Effect/GameEffect.h"

#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/Sound/SystemSoundManager.h"
#include "Game/Enemy/EnemyManager.h"
#include "Game/GameRegister.h"
#include "Common/Utility/CommonGameUtility.h"

static uint32_t DAMAGE_INVISIBLE_TIME = 120;
static uint32_t LIFE_POINT_MAX = 100;

GamePlayer *GamePlayer::CreatePlayer()
{
	return NEW GamePlayer();
}

GamePlayer::GamePlayer(void)
: TaskUnit("Player")
, m_speedMove( 0 )
, m_speedMoveBase( 3 )
, m_speedMultiply( 0.0f )
, m_invisibleTime(0)
, m_playerLife(LIFE_POINT_MAX)
{
}


GamePlayer::~GamePlayer(void)
{
}

bool GamePlayer::Init()
{
	// �`��N���X�Z�b�g�A�b�v
	m_texturePlayer.Init();
	m_texturePlayer.m_pTex2D = NEW Game2DBase("player.json");
	m_texturePlayer.m_texInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_texturePlayer.m_texInfo.m_usePlayerOffset = false;
	m_texturePlayer.m_texInfo.m_pos = math::Vector2( WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f );
	m_texturePlayer.m_texInfo.m_fileName = "player.json";
	GetBelongAreaInMap( m_texturePlayer.m_texInfo );
	m_texturePlayer.m_pTex2D->SetDrawInfo(m_texturePlayer.m_texInfo);

	// �`��N���X�Z�b�g�A�b�v
	m_textureLife.Init();
	m_textureLife.m_pTex2D = NEW Game2DBase("lifeGauge.json");
	m_textureLife.m_texInfo.Init();
	m_textureLife.m_texInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_textureLife.m_texInfo.m_usePlayerOffset = false;
	m_textureLife.m_texInfo.m_pos.x = 0.0f;
	const TEX_INIT_INFO &lifeTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( "lifeGauge.json" );
	m_textureLife.m_texInfo.m_pos.y = static_cast<float>(WINDOW_HEIGHT - lifeTexInfo.m_sizeHeight);
	m_textureLife.m_texInfo.m_arrangeOrigin = math::Vector2( 0.0f, 0.0f );
	m_textureLife.m_pTex2D->SetDrawInfo(m_textureLife.m_texInfo);

	m_textureLifeFrame.Init();
	m_textureLifeFrame.m_pTex2D = NEW Game2DBase("lifeFrame.json");
	m_textureLifeFrame.m_texInfo.m_prioity = PRIORITY_HIGH;
	m_textureLifeFrame.m_texInfo.m_usePlayerOffset = false;
	m_textureLifeFrame.m_texInfo.m_pos = m_textureLife.m_texInfo.m_pos;
	m_textureLifeFrame.m_texInfo.m_arrangeOrigin = m_textureLife.m_texInfo.m_arrangeOrigin;
	m_textureLifeFrame.m_pTex2D->SetDrawInfo(m_textureLifeFrame.m_texInfo);

	// �U���}�V���K���N���X�Z�b�g
	m_attackGun = NEW AttackGun();
	SetChildUnit( m_attackGun );

	// �摜�̐^�񒆂��I�t�Z�b�g�ʒu�ɂȂ�悤�ɒ������Ă���
	const TEX_INIT_INFO &playerTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_texturePlayer.m_texInfo.m_fileName.c_str() );
	GameAccesser::GetInstance()->SetPlayerOffSet( playerTexInfo.m_sizeWidth / 2.0f, playerTexInfo.m_sizeHeight / 2.0f );

	return true;
}

void GamePlayer::Update()
{
	if( m_playerLife == 0 ){
		return;
	}

	{
		// �}�C�t���[���l���ς��悤�ȍ��ڂ͂�����
		m_speedMove = m_speedMoveBase * static_cast<uint32_t>(m_speedMultiply + 0.5f);
		if( m_speedMultiply > 1.0f ){
			m_speedMultiply *= 0.95f;
		}
		if( m_speedMultiply < 1.0f ){
			m_speedMultiply = 1.0f;
		}

		// ���G���Ԓ��Ȃ�f�N�������g
		if( m_invisibleTime > 0 ){
			--m_invisibleTime;
		}
	}

	CallPadEvent();

	// �U������
	if( GetStickInfoRight().m_vec != math::Vector2() ){
		math::Vector2 pos = math::Vector2( m_texturePlayer.m_texInfo.m_pos.x, m_texturePlayer.m_texInfo.m_pos.y ) + GameAccesser::GetInstance()->GetPlayerOffSet();
		math::Vector2 vec = GetStickInfoRight().m_vec;
		vec.Normalize();
		m_attackGun->ShootBullet( pos, vec );
	}
}

void GamePlayer::CollisionUpdate()
{
	// �A�C�e���A�G�Ƃ̓����蔻��͊e�}�l�[�W���N���X���s���A
	// �C�x���g��push���Ă����̂ł����ł͉������Ȃ�
}

void GamePlayer::DrawUpdate()
{
	//EnemyManager *pEnemyMan = GameRegister::GetInstance()->UpdateManagerEnemy();
	//pEnemyMan->CreateEnemy( Common::KIND_AAA );

	if( m_invisibleTime % 3 == 1 ){
		// �_���[�W���󂯂Ȃ����ԑтȂ��3�t���Ɉ��`�悹���_�ł�����
	}
	else{
		// �v���C���[�`��	
		m_texturePlayer.m_pTex2D->SetDrawInfo(m_texturePlayer.m_texInfo);
		m_texturePlayer.m_pTex2D->DrawUpdate2D();
	}

	// ���C�t�Q�[�W�`��
	{
		const TEX_INIT_INFO &lifeTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_textureLife.m_texInfo.m_fileName.c_str() );
		float ratio = static_cast<float>(m_playerLife)/static_cast<float>(LIFE_POINT_MAX);
		float lifeValue = (WINDOW_WIDTH / lifeTexInfo.m_sizeWidth) * ratio;
		
		if( math::Absf( m_textureLife.m_texInfo.m_scale.x - lifeValue ) > 0.2f ){
			m_textureLife.m_texInfo.m_scale.x *= (m_textureLife.m_texInfo.m_scale.x - lifeValue < 0.0f) ? 1.02f : 0.98f ;
		}
		else{
			m_textureLife.m_texInfo.m_scale.x = lifeValue;
		}

		m_textureLife.m_pTex2D->SetDrawInfo(m_textureLife.m_texInfo);
		m_textureLife.m_pTex2D->DrawUpdate2D();
		m_textureLifeFrame.m_pTex2D->DrawUpdate2D();
	}
}

bool GamePlayer::DieMain(){
	m_texturePlayer.DeleteAndInit();
	m_textureLife.DeleteAndInit();
	m_textureLifeFrame.DeleteAndInit();
	return true;
}

/* ================================================ */
/**
 * @brief	�p�b�h����֐�
 */
/* ================================================ */
void GamePlayer::PadEventUp()
{
	math::Vector2 moveVal = math::Vector2(0.0f, -1.0f*m_speedMove);

	// �ړ��悪�����Ȃ��Ȃ�Έړ����Ȃ�
	if( !CanMoveThisPos( moveVal ) ){
		if(!IsButtonPress(InputWatcher::BUTTON_DOWN)
		&& !IsButtonPress(InputWatcher::BUTTON_RIGHT)
		&& !IsButtonPress(InputWatcher::BUTTON_LEFT)){
			// �ق��ɕ����L�[��������Ă��Ȃ���΃A�j�������ς��Ă���
			m_texturePlayer.m_pTex2D->SetAnim("up");
		}
		return;
	}

	GameAccesser::GetInstance()->AddPlayerOffSet( moveVal );

	if(IsButtonPush(InputWatcher::BUTTON_UP)){
		m_texturePlayer.m_pTex2D->SetAnim("up");
	}
}

void GamePlayer::PadEventDown()
{
	math::Vector2 moveVal = math::Vector2(0.0f, 1.0f*m_speedMove);

	// �ړ��悪�����Ȃ��Ȃ�Έړ����Ȃ�
	if( !CanMoveThisPos( moveVal ) ){
		if(!IsButtonPress(InputWatcher::BUTTON_UP)
		&& !IsButtonPress(InputWatcher::BUTTON_RIGHT)
		&& !IsButtonPress(InputWatcher::BUTTON_LEFT)){
			// �ق��ɕ����L�[��������Ă��Ȃ���΃A�j�������ς��Ă���
			m_texturePlayer.m_pTex2D->SetAnim("down");
		}
		return;
	}

	GameAccesser::GetInstance()->AddPlayerOffSet( moveVal );

	if(IsButtonPush(InputWatcher::BUTTON_DOWN)){
		m_texturePlayer.m_pTex2D->SetAnim("down");
	}
}

void GamePlayer::PadEventRight()
{
	math::Vector2 moveVal = math::Vector2(1.0f*m_speedMove, 0.0f);

	// �ړ��悪�����Ȃ��Ȃ�Έړ����Ȃ�
	if( !CanMoveThisPos( moveVal ) ){
		if(!IsButtonPress(InputWatcher::BUTTON_DOWN)
		&& !IsButtonPress(InputWatcher::BUTTON_UP)
		&& !IsButtonPress(InputWatcher::BUTTON_LEFT)){
			// �ق��ɕ����L�[��������Ă��Ȃ���΃A�j�������ς��Ă���
			m_texturePlayer.m_pTex2D->SetAnim("right");
		}
		return;
	}

	GameAccesser::GetInstance()->AddPlayerOffSet(1.0f*m_speedMove, 0.0f);

	if(IsButtonPush(InputWatcher::BUTTON_RIGHT)){
		m_texturePlayer.m_pTex2D->SetAnim("right");
	}
}

void GamePlayer::PadEventLeft()
{
	math::Vector2 moveVal = math::Vector2(-1.0f*m_speedMove, 0.0f);

	// �ړ��悪�����Ȃ��Ȃ�Έړ����Ȃ�
	if( !CanMoveThisPos( moveVal ) ){
		if(!IsButtonPress(InputWatcher::BUTTON_DOWN)
		&& !IsButtonPress(InputWatcher::BUTTON_RIGHT)
		&& !IsButtonPress(InputWatcher::BUTTON_UP)){
			// �ق��ɕ����L�[��������Ă��Ȃ���΃A�j�������ς��Ă���
			m_texturePlayer.m_pTex2D->SetAnim("left");
		}
		return;
	}

	GameAccesser::GetInstance()->AddPlayerOffSet( moveVal );

	if(IsButtonPush(InputWatcher::BUTTON_LEFT)){
		m_texturePlayer.m_pTex2D->SetAnim("left");
	}
}

void GamePlayer::PadEventDecide()
{
	GameEffect *effect = NEW GameEffect( GameEffect::EFFECT_BOMB, 50,50 );
	ItemManager *pItemMan = GameRegister::GetInstance()->UpdateManagerItem();
	pItemMan->CreateItem( ItemObject::ITEM_RAPID_BULLET );
}

void GamePlayer::PadEventCancel()
{
	for( uint32_t i = 0; i < 1000 ;++i ){
		EnemyManager *pEnemyMan = GameRegister::GetInstance()->UpdateManagerEnemy();
		pEnemyMan->CreateEnemy( Common::KIND_AAA );
	}
}

void GamePlayer::PadEventR1()
{
	m_speedMultiply = 7.0f;
}

void GamePlayer::PadEventL1()
{
	m_speedMultiply = 7.0f;
}

/* ================================================ */
/**
 * @brief	���擾�֐�
 */
/* ================================================ */
const TEX_DRAW_INFO &GamePlayer::GetDrawInfo() const
{
	if( m_texturePlayer.m_pTex2D == NULL ){
		DEBUG_ASSERT( 0, "�v���C���[�̕`��N���X��NULL");
	}
	return m_texturePlayer.m_pTex2D->GetDrawInfo();
}

/* ================================================ */
/**
 * @brief	�^�X�N�ɃC�x���g�ǉ�
 */
/* ================================================ */
void GamePlayer::AddEvent( const Common::CMN_EVENT &cmnEvent )
{
	TaskUnit::AddEvent( cmnEvent );
}

/* ================================================ */
/**
 * @brief	���݂̃X�e�[�^�X�Ńv���C���[�������̃x�N�^�[���ړ����ł��邩�ǂ����`�F�b�N
 */
/* ================================================ */
bool GamePlayer::CanMoveThisPos( const math::Vector2 &nextFlameAddValue )
{
	bool ret = false;

	const TEX_INIT_INFO &playerTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_texturePlayer.m_texInfo.m_fileName.c_str() );

	math::Vector2 nextFlamePos;
	GameAccesser::GetInstance()->GetPlayerOffSet( nextFlamePos.x, nextFlamePos.y );

	// offset�̐��l���ʒu�������ĕ`�悵�Ă���̂Ŏ��ۂɒ��ׂ�Ƃ��͑����Ă��
	// ����Ƀv���C���[�摜�̉摜���S�ʒu�ɂȂ�悤�ɑ����Ă��
	nextFlamePos += nextFlameAddValue;
	nextFlamePos.x = static_cast<float>(WINDOW_WIDTH / 2.0f) + nextFlamePos.x + (playerTexInfo.m_sizeWidth / 2.0f);
	nextFlamePos.y = static_cast<float>(WINDOW_HEIGHT / 2.0f) + nextFlamePos.y + (playerTexInfo.m_sizeHeight / 2.0f);

	// �ړ��悪�����Ȃ��Ȃ�Έړ����Ȃ�
	const GameMap *pMap = GameRegister::GetInstance()->GetGameMap();
	if( pMap && pMap->GetTileHeight( nextFlamePos ) == 0 ){
		ret = true;
	}
	return ret;
}


/* ================================================ */
/**
 * @brief	���̃N���X����̃C�x���g�R�[��
 */
/* ================================================ */
void GamePlayer::EventUpdate( const Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	case Common::EVENT_HIT_ENEMY:
		if( m_invisibleTime == 0 ){
			EventDamage();
		}
		break;
	case Common::EVENT_GET_ITEM:
		PlayerGetItem( static_cast<ItemObject::ITEM_KIND>(eventId.m_eventValue) );
		break;
	default:

		break;
	}

}

/* ================================================ */
/**
 * @brief	�C�x���g�ɑΉ������֐��Q
 */
/* ================================================ */

// �G�ƐڐG����
void GamePlayer::EventDamage()
{
	// �_���[�W��
	SoundManager::GetInstance()->PlaySE("Damage");

	// �_���[�W���󂯂����莞�ԃ_���[�W���󂯂Ȃ�
	m_invisibleTime = DAMAGE_INVISIBLE_TIME;

	// ���C�t�����炷
	if( m_playerLife > 50 ){
		m_playerLife -= 50;
	}
	else{
		m_playerLife = 0;
	}
}

// �A�C�e���擾
void GamePlayer::PlayerGetItem( const ItemObject::ITEM_KIND &itemKind )
{
	switch( itemKind ){
	default:
	case ItemObject::ITEM_RAPID_BULLET:
		{
			// �e�̔��ˊԊu�����߂�
			AttackGun::GunState &gunState = m_attackGun->UpdateGunState();
			gunState.m_shootInterval = ( gunState.m_shootInterval < 2 ) ? 0 : gunState.m_shootInterval - 2;
		}
		break;
	}
}
