/* ====================================================================== */
/**
 * @brief  �v���C���[�N���X
 *
 * @note
 *		�a��	Lv1~5	���a��
				Lv6~9	��a��

		�_�b�V��	Lv2����g�p�\
					Lv4�ŃG�t�F�N�g����
					Lv9�ŉ������ςȂ��_�b�V�����\��

		�}�V���K��	�ђʂɂ���H
					���ߍU������H

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
#include "Game/Attack/GameAttackGun.h"
#include "Game/Attack/GameAttackBlade.h"
#include "Common/Utility/CommonGameUtility.h"

#include "Game/Attack/Slashing.h"

#include "System/SystemFPSManager.h"

// �Œ�l
static uint32_t DAMAGE_INVISIBLE_TIME	= 40;

static uint32_t LIFE_POINT_DEFAULT_MAX	= 200;
static uint32_t MOVE_SPEED_DEFAULT		= 3;
static float DASH_MULTIPLY_DEFAULT		= 5.0f;
static uint32_t DASH_SPEED_MAX			= 15;
static uint32_t WARNING_LIFE			= 40;
static uint32_t EMERGENCY_LIFE			= 20;

static uint32_t BULLET_INTERBAL_MIN		= 1;
static uint32_t BULLET_DAMAGE_MAX		= 100;

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
, Collision2DUnit( "Player.json" )
, m_lifeLv( 0 )
, m_speedLv( 0 )
, m_defenceLv( 0 )
, m_playerLife( LIFE_POINT_DEFAULT_MAX )
, m_playerLifeMax( LIFE_POINT_DEFAULT_MAX )
, m_playerState( ABNORMAL_STATE_NONE )
, m_speedMove( 0 )
, m_speedMoveBase( MOVE_SPEED_DEFAULT )
, m_deffenceLate( 1.0f )
, m_speedMultiply( 0.0f )
, m_invisibleTime( 0 )
, m_invalidCtrlTime( 0 )
, m_attackGun(NULL)
, m_attackBlade(NULL)
, m_pStatusMenu(NULL)
{
	m_forceMoveInfo.Init();
	m_pStatusMenu = NEW PlayerStatusMenu();
}


GamePlayer::~GamePlayer(void)
{
}

bool GamePlayer::Init()
{
	// �`��N���X�Z�b�g�A�b�v
	TEX_DRAW_INFO drawInfo;
	drawInfo.m_prioity = Common::PRIORITY_ABOVE_NORMAL;
	drawInfo.m_usePlayerOffset = false;
	drawInfo.m_posOrigin = math::Vector2( WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f );
	drawInfo.m_fileName = "Player.json";
	m_drawTexture.m_pTex2D->SetDrawInfo( drawInfo );

	// �������U���}�V���K���N���X�Z�b�g
	m_attackGun = AttackGun::CreateGun( Common::OWNER_PLAYER );
	
	// �ߐڍU�����N���X�Z�b�g
	m_attackBlade = AttackBlade::CreateAttackBlade( Common::OWNER_PLAYER );

	// �x�[�X�ƂȂ�X�e�[�^�X�ƃA�C�e���擾���𔽉f������
	SetupInitPlayerState();

	// �摜�̐^�񒆂��I�t�Z�b�g�ʒu�ɂȂ�悤�ɒ������Ă���(�v���C���[�̏����ʒu�Z�b�g)
	const TEX_INIT_INFO &playerTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_drawTexture.m_pTex2D->GetDrawInfo().m_fileName.c_str() );
	GameAccesser::GetInstance()->SetPlayerOffSet( playerTexInfo.m_sizeWidth / 2.0f, playerTexInfo.m_sizeHeight / 2.0f );
	math::Vector2 startPos = GameRegister::GetInstance()->GetGameMap()->GetPlayerStartPos() - drawInfo.m_posOrigin;
	GameAccesser::GetInstance()->AddPlayerOffSet( startPos );

	if( m_speedLv == Common::STATUS_LEVEL_MAX ){
		SetPadButtonState( InputWatcher::BUTTON_R1, InputWatcher::EVENT_PRESS );
		SetPadButtonState( InputWatcher::BUTTON_L1, InputWatcher::EVENT_PRESS );
	}
	else{
		SetPadButtonState( InputWatcher::BUTTON_R1, InputWatcher::EVENT_PUSH );
		SetPadButtonState( InputWatcher::BUTTON_L1, InputWatcher::EVENT_PUSH );
	}
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

		// �_�b�V���֘A
		m_speedMove = m_speedMoveBase * static_cast<uint32_t>(m_speedMultiply + 0.5f);
		if( m_speedMultiply > 1.0f ){
			m_speedMultiply *= 0.94f;
			
			// 3�t���[���Ɉ��c���G�t�F�N�g�`��
			if( FpsManager::GetUpdateCounter() % 3 == 0 && m_speedLv >= 3 ){
				GameEffect::CreateEffect( GameEffect::EFFECT_DASH_SMOKE, Utility::GetPlayerPos() );
			}
		}
		// ��萔�ɂȂ�����؂�̂�(�I��)
		if( m_speedMultiply < 1.2f || m_speedLv == Common::STATUS_LEVEL_MAX ){
			m_speedMultiply = 1.0f;
		}
		// ��������ꍇ�͐؂�̂�
		if( m_speedMove > DASH_SPEED_MAX ){
			m_speedMove = DASH_SPEED_MAX;
		}

		// ���G���Ԓ��Ȃ�f�N�������g
		if( m_invisibleTime > 0 ){
			--m_invisibleTime;
		}

		// ����s�\���Ԓ��Ȃ�f�N�������g
		if( m_invalidCtrlTime > 0 ){
			--m_invalidCtrlTime;
		}
	}

	// ���݂̃��C�t�Z�b�g
	if( m_pStatusMenu ){
		m_pStatusMenu->SetPlayerHP( m_playerLife, m_playerLifeMax );
	}

	// ����s�\���łȂ��Ȃ�p�b�h�C�x���g���R�[��
	if( m_invalidCtrlTime == 0 ){
		// �p�b�h�ɑΉ�����������ݒ�
		CallPadEvent();
		
		// �U������
		if( GetStickInfoRight().m_vec != DEFAULT_VECTOR2 ){
			const TEX_DRAW_INFO &drawInfo = m_drawTexture.m_pTex2D->GetDrawInfo();
			math::Vector2 pos = math::Vector2( drawInfo.m_posOrigin.x, drawInfo.m_posOrigin.y ) + GameAccesser::GetInstance()->GetPlayerOffSet();
			math::Vector2 vec = GetStickInfoRight().m_vec;
			vec.Normalize();
			m_attackGun->ShootBullet( pos, vec );
		}
	}

	// �O����̃_���[�W���ł̋����ړ�
	if( m_forceMoveInfo.m_posX != 0.0f 
		&& m_forceMoveInfo.m_posY != 0.0f ){
		if( m_forceMoveInfo.m_forcePower <= 1.0f ){
			m_forceMoveInfo.Init();
		}
		else{
			math::Vector2 forceVec = math::Vector2( m_forceMoveInfo.m_posX, m_forceMoveInfo.m_posY );
			UpdateMove( forceVec*m_forceMoveInfo.m_forcePower, /*bool isForce=*/true );
			m_forceMoveInfo.m_forcePower *= 0.9f;
		}
	}

	// �Đ��A�j���^�O�Z�b�g
	m_drawTexture.m_pTex2D->SetAnim(GetAnimTag());
}
void GamePlayer::DrawUpdate()
{
	if( m_invisibleTime % 3 == 1 ){
		// �_���[�W���󂯂Ȃ����ԑтȂ��3�t���Ɉ��`�悹���_�ł�����
	}
	else{
		// �v���C���[�`��	
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
	UpdateMove( moveVal );
}

void GamePlayer::PadEventDown()
{
	math::Vector2 moveVal = math::Vector2(0.0f, 1.0f*m_speedMove);
	UpdateMove( moveVal );
}

void GamePlayer::PadEventRight()
{
	math::Vector2 moveVal = math::Vector2(1.0f*m_speedMove, 0.0f);
	UpdateMove( moveVal );
}

void GamePlayer::PadEventLeft()
{
	math::Vector2 moveVal = math::Vector2(-1.0f*m_speedMove, 0.0f);
	UpdateMove( moveVal );
}

void GamePlayer::UpdateMove( math::Vector2 &moveVec, bool isForce )
{
	// �a���U�����Ȃ瑬�x�𗎂Ƃ�
	if( m_attackBlade && !isForce && m_attackBlade->IsSlashingAnimPlay() ){
		moveVec /= 2.0f;
	}

	// �ړ��悪�����Ȃ��Ȃ�Έړ����Ȃ�
	if( !CanMoveThisPos( moveVec ) ){
		return;
	}

	GameAccesser::GetInstance()->AddPlayerOffSet( moveVec );
}

void GamePlayer::PadEventDecide()
{
	if( !m_attackBlade ){
		return;
	}

	// �a��
	const STICK_INFO &stickInfo = GetStickInfoLeft();
	if( stickInfo.m_vec == math::Vector2() ){
		math::Vector2 vec = math::Vector2();
		std::string anim = GetAnimTag();
		if( anim.compare("up") == 0 ){
			vec = math::Vector2( 0.0f, -1.0f );
		}
		if( anim.compare("down") == 0 ){
			vec = math::Vector2( 0.0f, 1.0f );
		}
		if( anim.compare("left") == 0 ){
			vec = math::Vector2( -1.0f, 0.0f );
		}
		if( anim.compare("right") == 0 ){
			vec = math::Vector2( 1.0f, 0.0f );
		}
		m_attackBlade->CreateSlashing( Utility::GetPlayerPos(), vec );
	}
	else{
		m_attackBlade->CreateSlashing( Utility::GetPlayerPos(), stickInfo.m_vec );
	}
}

void GamePlayer::PadEventR1()
{
	ActionPlayerDash();
}

void GamePlayer::PadEventL1()
{
	ActionPlayerDash();
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

const uint32_t	&GamePlayer::GetPlayerLifeMax() const
{
	return m_playerLifeMax;
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
 * @brief	�_�b�V������̏����܂Ƃߊ֐�
 */
/* ================================================ */
void GamePlayer::ActionPlayerDash()
{
	if( GetStickInfoLeft().m_vec == DEFAULT_VECTOR2 ){
		// �����L�[�̓��͂������Ă��Ȃ��̂ŉ������Ȃ�
		return;
	}

	if( m_speedLv == 0 ){
		// �_�b�V��Lv��0�Ȃ̂ŉ������Ȃ�
		return;
	}

	// �_�b�V��Lv���ő�̏ꍇ�͏�ɗL��
	if( m_speedLv == Common::STATUS_LEVEL_MAX ){
		// ���ʉ���30f�Ɉ��炷
		if( FpsManager::GetUpdateCounter() % 30 == 0 || !IsPreFrameButtonPress( InputWatcher::BUTTON_L1 ) ){
			SoundManager::GetInstance()->PlaySE("Dash");
		}
		// �W���Z�b�g
		m_speedMultiply = DASH_MULTIPLY_DEFAULT + static_cast<uint32_t>( 0.5 * m_speedLv );
	}
	else if( m_speedMultiply <= 1.0f ){
		SoundManager::GetInstance()->PlaySE("Dash");
		// �W���Z�b�g
		m_speedMultiply = DASH_MULTIPLY_DEFAULT + static_cast<uint32_t>( 0.5 * m_speedLv );
	}
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
 * @brief	�x�[�X�ƂȂ�X�e�[�^�X�{�擾�A�C�e�����𔽉f������
 */
/* ================================================ */
void GamePlayer::SetupInitPlayerState()
{
	// �x�[�X�X�e�[�^�X�𔽉f
	Common::SAVE_DATA playData;
	Utility::GetSaveData( playData );

	// ���C�t�𔽉f
	m_lifeLv		= playData.m_playerBaseStateLv[Common::BASE_STATE_LIFE];
	m_playerLifeMax	= LIFE_POINT_DEFAULT_MAX + Utility::ConvertLevelToBaseState( Common::BASE_STATE_LIFE, m_lifeLv );
	m_playerLife	= m_playerLifeMax;

	// �s�����x�𔽉f
	m_speedLv		= playData.m_playerBaseStateLv[Common::BASE_STATE_MOVE_SPEED];
	m_speedMoveBase = MOVE_SPEED_DEFAULT + Utility::ConvertLevelToBaseState( Common::BASE_STATE_MOVE_SPEED, m_speedLv );

	// �h��͂𔽉f
	m_defenceLv		= playData.m_playerBaseStateLv[Common::BASE_STATE_DEFFENCE];
	m_deffenceLate = ( 100.0f - static_cast<float>( Utility::ConvertLevelToBaseState( Common::BASE_STATE_DEFFENCE, m_defenceLv ) ) ) / 100.0f;

	// �}�V���K���N���X�𔽉f
	if( m_attackGun ){
		m_attackGun->SetGunLevel( playData.m_playerBaseStateLv[Common::BASE_STATE_BULLET_DMG], playData.m_playerBaseStateLv[Common::BASE_STATE_BULLET_SPD] );
	}

	// ���N���X�𔽉f
	if( m_attackBlade ){
		m_attackBlade->SetBladeLevel( playData.m_playerBaseStateLv[Common::BASE_STATE_BLADE_LEVEL] );
	}

	// �A�C�e���擾���𔽉f
	for( uint32_t i = 0; i < Common::ITEM_KIND_MAX ; ++i ){
		uint32_t itemNum = GameRecorder::GetInstance()->GetItemCount( static_cast<Common::ITEM_KIND>(i) );
		for( uint32_t j = 0; j < itemNum ; ++j ){
			PlayerGetItem( static_cast<Common::ITEM_KIND>(i), /*isCountUp=*/false );
		}
	}
}

/* ================================================ */
/**
 * @brief	���݂̃X�e�[�^�X�Ńv���C���[�������̃x�N�^�[���ړ����ł��邩�ǂ����`�F�b�N
 */
/* ================================================ */
bool GamePlayer::CanMoveThisPos( const math::Vector2 &nextFlameAddValue ) const
{
	bool ret = false;

	const TEX_INIT_INFO &playerTexInfo = TextureResourceManager::GetInstance()->GetLoadTextureInfo( m_drawTexture.m_pTex2D->GetDrawInfo().m_fileName.c_str() );

	math::Vector2 nextFlamePos;
	GameAccesser::GetInstance()->GetPlayerOffSet( nextFlamePos.x, nextFlamePos.y );

	// offset�̐��l���ʒu�������ĕ`�悵�Ă���̂Ŏ��ۂɒ��ׂ�Ƃ��͑����Ă��
	// ����Ƀv���C���[�摜�̉摜���S�ʒu�ɂȂ�悤�ɑ����Ă��
	nextFlamePos += nextFlameAddValue;
	nextFlamePos.x += static_cast<float>(WINDOW_WIDTH / 2.0f);
	nextFlamePos.y += static_cast<float>(WINDOW_HEIGHT / 2.0f);

	// �ړ��悪�����Ȃ��Ȃ�Έړ����Ȃ�
	if( Utility::IsMovable( m_drawTexture.m_pTex2D->GetDrawInfo().m_fileName, nextFlamePos ) ){
		ret = true;
	}
	return ret;
}


/* ================================================ */
/**
 * @brief	���̃N���X����̃C�x���g�R�[��
 */
/* ================================================ */
void GamePlayer::EventUpdate( Common::CMN_EVENT &eventId )
{
	switch( eventId.m_event ){
	case Common::EVENT_HIT_ENEMY_SLIME:
	case Common::EVENT_HIT_ENEMY_AHRIMAN:
	case Common::EVENT_HIT_ENEMY_COW:
	case Common::EVENT_HIT_ENEMY_BOSS:
	case Common::EVENT_HIT_ENEMY_SLIME_KING:
	case Common::EVENT_HIT_ENEMY_WIZARD:
	case Common::EVENT_HIT_WIZARD_CRYSTAL:
	case Common::EVENT_HIT_BULLET_ENEMY:
	case Common::EVENT_HIT_BLADE_ENEMY:
	case Common::EVENT_HIT_EXPLOSION_ENEMY:
		if( m_invisibleTime == 0 ){
			EventDamage( eventId );
		}
		break;
	case Common::EVENT_GET_ITEM_BULLET:
		PlayerGetItem( Common::ITEM_KIND_RAPID_BULLET );
		break;
	case Common::EVENT_GET_ITEM_LIFE:
		PlayerGetItem( Common::ITEM_KIND_LIFE_UP );
		break;
	case Common::EVENT_GET_ITEM_DAMAGE:
		PlayerGetItem( Common::ITEM_KIND_DAMAGE_UP );
		break;
	case Common::EVENT_GET_ITEM_BATTLE_POINT:
		PlayerGetItem( Common::ITEM_KIND_BATTLE_POINT );
		break;
	case Common::EVENT_ADD_FORCE_MOVE:
		// �K�v�ȏ������o��
		m_forceMoveInfo = eventId.GetExInfoForceMove();
		m_invalidCtrlTime	= 10;
		m_invisibleTime		= 60;
		
		SetPlayerState( ABNORMAL_STATE_MOVE_LOCK, false );
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

void GamePlayer::EventDamage( Common::CMN_EVENT &eventId )
{
	Common::EVENT_MESSAGE	eventKind	= eventId.m_event;
	uint32_t				damageValue	= eventId.m_eventValue;

	// �_���[�W��
	switch( eventKind ){
	case Common::EVENT_HIT_BULLET_ENEMY:
		SoundManager::GetInstance()->PlaySE("DamageBullet");
		break;
	default:
		SoundManager::GetInstance()->PlaySE("DamageDirect");
		break;
	}

	// �_���[�W���󂯂����莞�ԃ_���[�W���󂯂Ȃ�
	m_invisibleTime = DAMAGE_INVISIBLE_TIME;

	// �e�_���[�W��ނɂ����ꏈ��
	switch( eventKind ){
	case Common::EVENT_HIT_ENEMY_SLIME_KING:
		{
			if( !IsPlayerState( ABNORMAL_STATE_MOVE_LOCK ) ){
				// �O�b�قǓ������Ƃ肱�܂��A���̌�͂����o�����
				Common::CMN_EVENT forceEvent;
				forceEvent.Init();
				forceEvent.m_event = Common::EVENT_ADD_FORCE_MOVE;
				forceEvent.m_delayTime = 120;
				Common::EX_FORCE_MOVE moveInfo;
				math::Vector2 vec	= math::Vector2( Utility::GetRandamValueFloat( 100, -100 ) / 100.0f, Utility::GetRandamValueFloat( 100, -100 ) / 100.0f );
				vec.Normalize();
				moveInfo.m_posX = vec.x;
				moveInfo.m_posY = vec.y;
				moveInfo.m_forcePower	= 20.0f;
				forceEvent.SetExInfoForceMove( moveInfo );
				AddEvent( forceEvent );

				// �����Ȃ��X�e�[�^�X�ɃZ�b�g
				SetPlayerState( ABNORMAL_STATE_MOVE_LOCK, true );
			}
			else{
				// ���Ƀ��b�N��ԂȂ̂łȂɂ����Ȃ�(�_���[�W��������)
			}
		}
		break;
	case Common::EVENT_HIT_ENEMY_SLIME:
	case Common::EVENT_HIT_ENEMY_AHRIMAN:
	case Common::EVENT_HIT_ENEMY_COW:
	case Common::EVENT_HIT_ENEMY_BOSS:
	case Common::EVENT_HIT_ENEMY_WIZARD:
	case Common::EVENT_HIT_WIZARD_CRYSTAL:
		{
			// ������ԕ�����ݒ肵�ăC�x���g�Ƃ��ăZ�b�g���Ă���
			math::Vector2 plPos		= Utility::GetPlayerPos();
			math::Vector2 enemyPos	= math::Vector2( eventId.GetExInfoCmn().m_posX, eventId.GetExInfoCmn().m_posY );			
			math::Vector2 forceAngle = plPos - enemyPos;
			forceAngle.Normalize();

			Common::CMN_EVENT forceEvent;
			forceEvent.Init();
			forceEvent.m_event = Common::EVENT_ADD_FORCE_MOVE;
			Common::EX_FORCE_MOVE moveInfo;
			moveInfo.m_posX = forceAngle.x;
			moveInfo.m_posY = forceAngle.y;
			moveInfo.m_forcePower	= ( eventKind == Common::EVENT_HIT_ENEMY_COW ) ? 25.0f : 10.0f ;
			forceEvent.SetExInfoForceMove( moveInfo );
			AddEvent( forceEvent );
		}
		break;
	default:
		break;
	}

	// ����̍U�����󂯂����莞�ԑ���s�\
	switch( eventKind ){
	case Common::EVENT_HIT_BLADE_ENEMY:
		m_invalidCtrlTime = 10;
		break;
	case Common::EVENT_HIT_ENEMY_SLIME_KING:
		m_invalidCtrlTime = 200;
		break;
	default:
		break;
	}

	// �h��͂ɉ����ă_���[�W�����炷
	uint32_t totalDamage = static_cast<uint32_t>( damageValue * m_deffenceLate );

	// ���C�t�����炷
	if( m_playerLife > totalDamage ){
		m_playerLife -= totalDamage;
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
void GamePlayer::PlayerGetItem( const Common::ITEM_KIND &itemKind, bool isCountUp )
{
	// �A�C�e�����x���`��N���X�Ɏ擾��m�点�邩�ǂ���
	bool reflectDisp = true;

	switch( itemKind ){
	default:
	case Common::ITEM_KIND_RAPID_BULLET:
		{
			// �e�̔��ˊԊu�����߂�
			AttackGun::GunState &gunState = m_attackGun->UpdateGunState();
			if( gunState.m_shootInterval > BULLET_INTERBAL_MIN ){
				gunState.m_shootInterval -= 1;
			}
			else{
				reflectDisp = false;
			}

			if( isCountUp ){
				// �擾�A�C�e�������J�E���g
				GameRecorder::GetInstance()->AddItem( Common::ITEM_KIND_RAPID_BULLET );
				// �A�C�e���擾����炷
				SoundManager::GetInstance()->PlaySE("GetItem");
			}
		}
		break;
	case Common::ITEM_KIND_LIFE_UP:
		{
			// ���C�t��
			m_playerLife += 30;
			if( m_playerLife > m_playerLifeMax ){
				m_playerLife = m_playerLifeMax;
			}

			if( isCountUp ){
				// �擾�A�C�e�������J�E���g
				GameRecorder::GetInstance()->AddItem( Common::ITEM_KIND_LIFE_UP );
				// �A�C�e���擾����炷
				SoundManager::GetInstance()->PlaySE("GetItemHeal");
			}
		}
		break;
	case Common::ITEM_KIND_DAMAGE_UP:
		{
			// �_���[�W��UP
			AttackGun::GunState &gunState = m_attackGun->UpdateGunState();
			if( gunState.m_damage < BULLET_DAMAGE_MAX ){
				gunState.m_damage += 5;
			}
			else{
				reflectDisp = false;
			}

			if( gunState.m_damage % 4 == 0 ){// 20�̔{���ɂȂ邲�ƂɎa���̍U���͂�UP
				AttackBlade::BladeState &bladeState = m_attackBlade->UpdateBladeState();
				bladeState.m_damage += 5;
			}

			if( isCountUp ){
				// �擾�A�C�e�������J�E���g
				GameRecorder::GetInstance()->AddItem( Common::ITEM_KIND_DAMAGE_UP );
				// �A�C�e���擾����炷
				SoundManager::GetInstance()->PlaySE("GetItem");
			}
		}
		break;
	case Common::ITEM_KIND_BATTLE_POINT:
		if( isCountUp ){
			// �擾�A�C�e�������J�E���g
			GameRecorder::GetInstance()->AddItem( Common::ITEM_KIND_BATTLE_POINT );
		}
		break;
	}

	// �\�������ɓ`����
	if( m_pStatusMenu && reflectDisp ){
		m_pStatusMenu->AddItemLevel( itemKind );
	}
}


bool GamePlayer::IsPlayerState( const PLAYER_ABNORMAL_STATE &checkState ) const
{
	if( m_playerState & checkState ){
		return true;
	}
	return false;
}

void GamePlayer::SetPlayerState( const PLAYER_ABNORMAL_STATE &checkState, const bool &flag )
{
	if( flag ){
		m_playerState |= checkState;
	}
	else{
		m_playerState &= ~checkState;
	}
}
