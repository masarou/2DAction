/* ====================================================================== */
/**
 * @brief  �v���C���[�N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "GamePlayer.h"
#include "Game/Effect/GameEffect.h"

#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/Sound/SystemSoundManager.h"
#include "System/Message/SystemMessageManager.h"
#include "Game/Enemy/EnemyManager.h"
#include "Game/Item/ItemObject.h"
#include "Game/GameRegister.h"
#include "Game/GameRecorder.h"
#include "Common/Utility/CommonGameUtility.h"

// �Œ�l
static uint32_t DAMAGE_INVISIBLE_TIME	= 120;
static uint32_t LIFE_POINT_MAX			= 200;
static uint32_t WARNING_LIFE			= 50;
static uint32_t EMERGENCY_LIFE			= 20;
static uint32_t BULLET_INTERBAL_MIN		= 4;
static uint32_t BULLET_DAMAGE_MAX		= 100;
static uint32_t DEFAULT_POS_X			= 1000;
static uint32_t DEFAULT_POS_Y			= 1000;

// �A�j���^�O
static char *ANIM_TAG_UP	= "up";
static char *ANIM_TAG_DOWN	= "down";
static char *ANIM_TAG_RIGHT	= "right";
static char *ANIM_TAG_LEFT	= "left";

GamePlayer *GamePlayer::CreatePlayer()
{
	return NEW GamePlayer();
}

GamePlayer::GamePlayer(void)
: TaskUnit("Player")
, Collision2DUnit( "player.json" )
, m_speedMove( 0 )
, m_speedMoveBase( 3 )
, m_speedMultiply( 0.0f )
, m_invisibleTime(0)
, m_playerLife(LIFE_POINT_MAX)
, m_pStatusMenu(NULL)
{
	m_pStatusMenu = NEW PlayerStatusMenu();
}


GamePlayer::~GamePlayer(void)
{
}

bool GamePlayer::Init()
{
	// �`��N���X�Z�b�g�A�b�v
	m_drawTexture.m_texInfo.m_prioity = PRIORITY_ABOVE_NORMAL;
	m_drawTexture.m_texInfo.m_usePlayerOffset = false;
	m_drawTexture.m_texInfo.m_posOrigin = math::Vector2( WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f );
	m_drawTexture.m_texInfo.m_fileName = "player.json";
	Utility::GetBelongAreaInMap( m_drawTexture.m_texInfo );
	m_drawTexture.m_pTex2D->SetDrawInfo(m_drawTexture.m_texInfo);


	// �U���}�V���K���N���X�Z�b�g
	m_attackGun = AttackGun::CreateGun( Common::OWNER_PLAYER );

	// �A�C�e���擾���𔽉f
	for( uint32_t i = 0; i < ItemObject::ITEM_KIND_MAX ; ++i ){
		uint32_t itemNum = GameRecorder::GetInstance()->GetItemCount( static_cast<ItemObject::ITEM_KIND>(i) );
		for( uint32_t j = 0; j < itemNum ; ++j ){
			PlayerGetItem( static_cast<ItemObject::ITEM_KIND>(i), /*isCountUp=*/false );
		}
	}

	// �摜�̐^�񒆂��I�t�Z�b�g�ʒu�ɂȂ�悤�ɒ������Ă���(�v���C���[�̏����ʒu�Z�b�g)
	const TEX_INIT_INFO &playerTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_drawTexture.m_texInfo.m_fileName.c_str() );
	GameAccesser::GetInstance()->SetPlayerOffSet( playerTexInfo.m_sizeWidth / 2.0f, playerTexInfo.m_sizeHeight / 2.0f );
	math::Vector2 vec = math::Vector2( static_cast<float>(DEFAULT_POS_X), static_cast<float>(DEFAULT_POS_Y) );
	GameAccesser::GetInstance()->AddPlayerOffSet( vec );

	return true;
}

bool GamePlayer::DieMain(){
	return true;
}

/* ================================================ */
/**
 * @brief	�X�V�֘A�֐�
 */
/* ================================================ */
void GamePlayer::Update()
{
	if( m_playerLife == 0 ){
		return;
	}

	{
		// ���t���[���l���ς��悤�ȍ��ڂ͂�����
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

	// ���݂̃��C�t�Z�b�g
	if( m_pStatusMenu ){
		m_pStatusMenu->SetPlayerHP( m_playerLife, LIFE_POINT_MAX );
	}

	// �p�b�h�ɑΉ�����������ݒ�
	CallPadEvent();

	// �Đ��A�j���^�O�Z�b�g
	m_drawTexture.m_pTex2D->SetAnim(GetAnimTag());

	// �U������
	if( GetStickInfoRight().m_vec != DEFAULT_VECTOR2 ){
		math::Vector2 pos = math::Vector2( m_drawTexture.m_texInfo.m_posOrigin.x, m_drawTexture.m_texInfo.m_posOrigin.y ) + GameAccesser::GetInstance()->GetPlayerOffSet();
		math::Vector2 vec = GetStickInfoRight().m_vec;
		vec.Normalize();
		m_attackGun->ShootBullet( pos, vec );
	}
}
void GamePlayer::DrawUpdate()
{
	if( m_invisibleTime % 3 == 1 ){
		// �_���[�W���󂯂Ȃ����ԑтȂ��3�t���Ɉ��`�悹���_�ł�����
	}
	else{
		// �v���C���[�`��	
		m_drawTexture.m_pTex2D->SetDrawInfo(m_drawTexture.m_texInfo);
		m_drawTexture.m_pTex2D->DrawUpdate2D();
	}
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
		return;
	}
	GameAccesser::GetInstance()->AddPlayerOffSet( moveVal );
}

void GamePlayer::PadEventDown()
{
	math::Vector2 moveVal = math::Vector2(0.0f, 1.0f*m_speedMove);

	// �ړ��悪�����Ȃ��Ȃ�Έړ����Ȃ�
	if( !CanMoveThisPos( moveVal ) ){
		return;
	}
	GameAccesser::GetInstance()->AddPlayerOffSet( moveVal );
}

void GamePlayer::PadEventRight()
{
	math::Vector2 moveVal = math::Vector2(1.0f*m_speedMove, 0.0f);

	// �ړ��悪�����Ȃ��Ȃ�Έړ����Ȃ�
	if( !CanMoveThisPos( moveVal ) ){
		return;
	}
	GameAccesser::GetInstance()->AddPlayerOffSet(1.0f*m_speedMove, 0.0f);
}

void GamePlayer::PadEventLeft()
{
	math::Vector2 moveVal = math::Vector2(-1.0f*m_speedMove, 0.0f);

	// �ړ��悪�����Ȃ��Ȃ�Έړ����Ȃ�
	if( !CanMoveThisPos( moveVal ) ){
		return;
	}

	GameAccesser::GetInstance()->AddPlayerOffSet( moveVal );
}

void GamePlayer::PadEventDecide()
{
}

void GamePlayer::PadEventCancel()
{
}

void GamePlayer::PadEventR1()
{
	m_speedMultiply = 7.0f;

	// �_�b�V�����ʉ�
	SoundManager::GetInstance()->PlaySE("Dash");
}

void GamePlayer::PadEventL1()
{
	m_speedMultiply = 7.0f;

	// �_�b�V�����ʉ�
	SoundManager::GetInstance()->PlaySE("Dash");
}

/* ================================================ */
/**
 * @brief	���擾�֐�
 */
/* ================================================ */
const TEX_DRAW_INFO &GamePlayer::GetDrawInfo() const
{
	if( m_drawTexture.m_pTex2D == NULL ){
		DEBUG_ASSERT( 0, "�v���C���[�̕`��N���X��NULL");
	}
	return m_drawTexture.m_pTex2D->GetDrawInfo();
}

/* ================================================ */
/**
 * @brief	�^�X�N�ɃC�x���g�ǉ�
 */
/* ================================================ */
void GamePlayer::AddEvent( const Common::CMN_EVENT &cmnEvent )
{
	SystemMessageManager::GetInstance()->PushMessage( GetUniqueId(), cmnEvent );
}

/* ================================================ */
/**
 * @brief	���݂̃v���C���[�̏󋵂���Đ�����A�j���^�O�擾
 */
/* ================================================ */
std::string GamePlayer::GetAnimTag()
{
	// �ǂ̕������ނ��Ă��邩
	const STICK_INFO &stickInfo = GetStickInfoLeft();
	std::string retAnim = "";

	switch( Utility::GetDirection( stickInfo.m_vec.x, stickInfo.m_vec.y ) ){
	default:
		break;
	case InputWatcher::BUTTON_UP:
		retAnim = "up";
		break;
	case InputWatcher::BUTTON_DOWN:
		retAnim = "down";
		break;
	case InputWatcher::BUTTON_LEFT:
		retAnim = "left";
		break;
	case InputWatcher::BUTTON_RIGHT:
		retAnim = "right";
		break;
	}

	if( retAnim.compare("") == 0 ){
		retAnim = m_drawTexture.m_pTex2D->GetPlayAnim();
	}
	return retAnim;
}

/* ================================================ */
/**
 * @brief	���݂̃X�e�[�^�X�Ńv���C���[�������̃x�N�^�[���ړ����ł��邩�ǂ����`�F�b�N
 */
/* ================================================ */
bool GamePlayer::CanMoveThisPos( const math::Vector2 &nextFlameAddValue )
{
	bool ret = false;

	const TEX_INIT_INFO &playerTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_drawTexture.m_texInfo.m_fileName.c_str() );

	math::Vector2 nextFlamePos;
	GameAccesser::GetInstance()->GetPlayerOffSet( nextFlamePos.x, nextFlamePos.y );

	// offset�̐��l���ʒu�������ĕ`�悵�Ă���̂Ŏ��ۂɒ��ׂ�Ƃ��͑����Ă��
	// ����Ƀv���C���[�摜�̉摜���S�ʒu�ɂȂ�悤�ɑ����Ă��
	nextFlamePos += nextFlameAddValue;
	nextFlamePos.x += static_cast<float>(WINDOW_WIDTH / 2.0f);
	nextFlamePos.y += static_cast<float>(WINDOW_HEIGHT / 2.0f);

	math::Vector2 up = nextFlamePos;
	up.y -= playerTexInfo.m_sizeHeight/2.0f;
	math::Vector2 down = nextFlamePos;
	down.y += playerTexInfo.m_sizeHeight/2.0f;
	math::Vector2 left = nextFlamePos;
	left.x -= playerTexInfo.m_sizeWidth/2.0f;
	math::Vector2 right = nextFlamePos;
	right.x += playerTexInfo.m_sizeWidth/2.0f;

	// �ړ��悪�����Ȃ��Ȃ�Έړ����Ȃ�
	if( Utility::GetMapHeight( up ) == 0
		&& Utility::GetMapHeight( down ) == 0
		&& Utility::GetMapHeight( left ) == 0
		&& Utility::GetMapHeight( right ) == 0){
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
	case Common::EVENT_HIT_ENEMY_AAA:
	case Common::EVENT_HIT_ENEMY_BBB:
	case Common::EVENT_HIT_ENEMY_CCC:
	case Common::EVENT_HIT_BULLET_ENEMY:
		if( m_invisibleTime == 0 ){
			EventDamage( eventId.m_event, eventId.m_eventValue );
		}
		break;
	case Common::EVENT_GET_ITEM_BULLET:
		PlayerGetItem( ItemObject::ITEM_KIND_RAPID_BULLET );
		break;
	case Common::EVENT_GET_ITEM_LIFE:
		PlayerGetItem( ItemObject::ITEM_KIND_LIFE_UP );
		break;
	case Common::EVENT_GET_ITEM_DAMAGE:
		PlayerGetItem( ItemObject::ITEM_KIND_DAMAGE_UP );
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
void GamePlayer::EventDamage( Common::EVENT_MESSAGE eventKind, uint32_t damageValue )
{
	// �_���[�W��
	if( eventKind != Common::EVENT_HIT_BULLET_ENEMY ){
		SoundManager::GetInstance()->PlaySE("DamageDirect");
	}
	else{
		SoundManager::GetInstance()->PlaySE("DamageBullet");
	}

	// �_���[�W���󂯂����莞�ԃ_���[�W���󂯂Ȃ�
	m_invisibleTime = DAMAGE_INVISIBLE_TIME;

	// ���C�t�����炷
	if( m_playerLife > damageValue ){
		m_playerLife -= damageValue;
	}
	else{
		m_playerLife = 0;
	}

	// ���C�t�c�ʂɂ����SE��炷
	if( m_playerLife == 0 ){

	}
	else if( m_playerLife <= EMERGENCY_LIFE ){
		// HP�����Ȃ��x����炷
		SoundManager::GetInstance()->PlaySE("Emergency");
	}
	else if( m_playerLife <= WARNING_LIFE ){
		// HP�����Ȃ��x����炷
		SoundManager::GetInstance()->PlaySE("Warning");
	}
}

// �A�C�e���擾
void GamePlayer::PlayerGetItem( const ItemObject::ITEM_KIND &itemKind, bool isCountUp )
{
	// �A�C�e�����x���`��N���X�Ɏ擾��m�点�邩�ǂ���
	bool reflectDisp = true;

	switch( itemKind ){
	default:
	case ItemObject::ITEM_KIND_RAPID_BULLET:
		{
			// �e�̔��ˊԊu�����߂�
			AttackGun::GunState &gunState = m_attackGun->UpdateGunState();
			if( gunState.m_shootInterval > BULLET_INTERBAL_MIN ){
				gunState.m_shootInterval -= 2;
			}
			else{
				reflectDisp = false;
			}

			if( isCountUp ){
				// �擾�A�C�e�������J�E���g
				GameRecorder::GetInstance()->AddItem( ItemObject::ITEM_KIND_RAPID_BULLET );
				// �A�C�e���擾����炷
				SoundManager::GetInstance()->PlaySE("GetItem");
			}
		}
		break;
	case ItemObject::ITEM_KIND_LIFE_UP:
		{
			// ���C�t��
			m_playerLife += 30;
			if( m_playerLife > LIFE_POINT_MAX ){
				m_playerLife = LIFE_POINT_MAX;
			}

			if( isCountUp ){
				// �擾�A�C�e�������J�E���g
				GameRecorder::GetInstance()->AddItem( ItemObject::ITEM_KIND_LIFE_UP );
				// �A�C�e���擾����炷
				SoundManager::GetInstance()->PlaySE("GetItemHeal");
			}
		}
		break;
	case ItemObject::ITEM_KIND_DAMAGE_UP:
		{
			// �_���[�W��UP
			AttackGun::GunState &gunState = m_attackGun->UpdateGunState();
			if( gunState.m_damage < BULLET_DAMAGE_MAX ){
				gunState.m_damage += 20;
			}
			else{
				reflectDisp = false;
			}

			if( isCountUp ){
				// �擾�A�C�e�������J�E���g
				GameRecorder::GetInstance()->AddItem( ItemObject::ITEM_KIND_DAMAGE_UP );
				// �A�C�e���擾����炷
				SoundManager::GetInstance()->PlaySE("GetItem");
			}
		}
		break;
	}

	// �\�������ɓ`����
	if( m_pStatusMenu && reflectDisp ){
		m_pStatusMenu->AddItemLevel( itemKind );
	}
}
