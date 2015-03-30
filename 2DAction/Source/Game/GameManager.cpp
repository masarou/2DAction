/* ====================================================================== */
/**
 * @brief  �G�̐�����A�C�e��������ݒ�ɂȂ���ĊǗ�����N���X
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "System/SystemDefine.h"
#include "GameManager.h"
#include "GameRegister.h"
#include "System/picojson.h"
#include "Common/Utility/CommonGameUtility.h"


// �A�C�e���A�G�̏o���p�x�ő�l
#define OBJECT_FREQUECY_MAX 9

GameManager *GameManager::CreateGameManager( const Common::GAME_FLOW &currentKind )
{
	return NEW GameManager( currentKind );
}

GameManager::GameManager( const Common::GAME_FLOW &currentKind )
: TaskUnit("GameManager")
, m_type( TYPE_MAX )
, m_gameTimer( 0 )
, m_gameTimeMax( 0 )
, m_destroyNum( 0 )
, m_destroyMax( 0 )
, m_enemyMax( 0 )	
, m_enemyFrequency( 0 )
, m_itemMax( 0 )
, m_itemFrequency( 0 )
{
	switch( currentKind ){
		default:
			DEBUG_ASSERT( 0, "�z��O�̉ӏ��ŃQ�[���ݒ�t�@�C�����ǂݍ��܂�Ă���");
			/* fall-through */
		case Common::FLOW_STAGE01:
			m_settingFileStr = "gameSettings01.json";
			break;
		case Common::FLOW_STAGE02:
			m_settingFileStr = "gameSettings02.json";
			break;
		case Common::FLOW_STAGE03:
			m_settingFileStr = "gameSettings03.json";
			break;
	}
	// �}�b�v�̊��擾
	LoadGameSettings( m_settingFileStr.c_str() );
}

GameManager::~GameManager(void)
{
}

bool GameManager::DieMain()
{
	return true;
}

bool GameManager::Init()
{
	return true;
}

/* ================================================ */
/**
 * @brief	�X�e�[�W�N���A�����𖞂��������ǂ���
 */
/* ================================================ */
bool GameManager::IsGameOver() const
{
	bool retVal = false;
	if( m_type == TYPE_TIME ){
		if( m_gameTimer > m_gameTimeMax ){
			retVal = true;
		}
	}
	else if( m_type == TYPE_DESTROY ){
		if( m_destroyNum >= m_destroyMax ){
			retVal = true;
		}
	}
	return retVal;
}

/* ================================================ */
/**
 * @brief	�|�����G�̐����J�E���g
 */
/* ================================================ */
void GameManager::AddDestroyCount()
{
	++m_destroyNum;
}

/* ================================================ */
/**
 * @brief	�X�e�[�W�^�C�v�擾
 */
/* ================================================ */
const GameManager::STAGE_TYPE GameManager::GetStageType() const
{
	return m_type;
}

/* ================================================ */
/**
 * @brief	�c�莞��or�r�Ő��̎擾
 */
/* ================================================ */
const uint32_t GameManager::GetGameLeftTimeByFrame() const
{
	uint32_t retVal = 0;
	if( m_gameTimeMax > m_gameTimer ){
		retVal = m_gameTimeMax - m_gameTimer;
	}

	return retVal;
}
const uint32_t GameManager::GetGameLeftTimeBySec() const
{
	uint32_t retVal = 0;
	if( m_gameTimeMax > m_gameTimer ){
		retVal = m_gameTimeMax - m_gameTimer;
		retVal += 59;
		retVal /= 60;
	}

	return retVal;
}
const uint32_t GameManager::GetGameLeftDestroy() const
{
	uint32_t retVal = 0;
	if( m_gameTimeMax > m_gameTimer ){
		retVal = m_destroyMax - m_destroyNum;
	}

	return retVal;
}

/* ================================================ */
/**
 * @brief	���t���[������
 */
/* ================================================ */
void GameManager::Update()
{
	++m_gameTimer;

	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
	ItemManager *pItemManager = GameRegister::GetInstance()->UpdateManagerItem();

	// �G�̐���
	if( IsCreateEnemy( m_enemyMax, m_enemyFrequency ) ){
		pEnemyManager->AddEnemy( Common::ENEMY_KIND_BOSS );
		return;
		if( Utility::GetRandamValue( 5, 0 ) != 0 ){
			pEnemyManager->AddEnemy( Common::ENEMY_KIND_AAA );
		}
		else if( Utility::GetRandamValue( 3, 0 ) != 0 ){
			pEnemyManager->AddEnemy( Common::ENEMY_KIND_CCC );
		}
		else{
			pEnemyManager->AddEnemy( Common::ENEMY_KIND_BBB );
		}
	}
	// �A�C�e���̐���
	if( IsCreateItem( m_itemMax, m_itemFrequency ) ){
		uint32_t kind = Utility::GetRandamValue( ItemObject::ITEM_KIND_MAX-1, 0 );
		pItemManager->CreateItem( static_cast<ItemObject::ITEM_KIND>(kind) );
	}
}

/* ================================================ */
/**
 * @brief	�N���X������
 */
/* ================================================ */
void GameManager::ResetManageValue()
{
	m_type = TYPE_MAX;
	m_gameTimer			= 0;	// �Q�[���̃v���C����
	m_gameTimeMax		= 0;	// �Q�[���I���܂ł̎���(/fps)
	m_destroyNum		= 0;	// ���݂܂ł̓G�r�Ő�
	m_destroyMax		= 0;	// �X�e�[�W�I���܂ł̓G�r�Ő�
	m_enemyMax			= 0;	// �ő�o���G��
	m_enemyFrequency	= 0;	// �G�̏o����(10�i�K0~9)
	m_itemMax			= 0;	// �ő�o���A�C�e����
	m_itemFrequency		= 0;	// �A�C�e���̏o����(10�i�K0~9)
	m_settingFileStr	= "";	// �ǂݍ��ރX�e�[�W�ݒ�t�@�C��
}

/* ================================================ */
/**
 * @brief	�����擾
 */
/* ================================================ */
void GameManager::LoadGameSettings( const char *jsonFile )
{
	std::string path = JSON_OTHER_PATH;
	path += jsonFile;
	std::ifstream ifs(path.c_str());

	picojson::value root;
	picojson::parse( root, ifs);

	//!��{�ƂȂ���擾
	picojson::value stageData = root.get("stageKind");
	std::string typeStr = stageData.get(0).get("type").get<std::string>();
	m_type	= ( typeStr == "DESTROY" ? TYPE_DESTROY : TYPE_TIME );
	m_gameTimeMax	= static_cast<uint32_t>(stageData.get(1).get("TimeS").get<double>());
	m_destroyMax	= static_cast<uint32_t>(stageData.get(2).get("destroyMax").get<double>());
	m_gameTimeMax *= 60;
	
	picojson::value condData = root.get("gameCondition");
	m_enemyMax			= static_cast<uint32_t>(condData.get(0).get("EnemyMax").get<double>());
	m_enemyFrequency	= static_cast<uint32_t>(condData.get(0).get("frequency").get<double>());
	m_itemMax			= static_cast<uint32_t>(condData.get(1).get("ItemMax").get<double>());
	m_itemFrequency		= static_cast<uint32_t>(condData.get(1).get("frequency").get<double>());

	if( m_enemyFrequency > OBJECT_FREQUECY_MAX ){
		m_enemyFrequency = OBJECT_FREQUECY_MAX;
	}
	if( m_itemFrequency > OBJECT_FREQUECY_MAX ){
		m_itemFrequency = OBJECT_FREQUECY_MAX;
	}
}

bool GameManager::IsCreateEnemy( uint32_t enemyLimit, uint32_t frequency )
{
	bool isCreate = false;
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();	
	// �G�̐���
	if( pEnemyManager ){
		uint32_t currEnemy = pEnemyManager->CountEnemy();
		uint32_t under = frequency + 1;
		if( currEnemy < (enemyLimit / under) || currEnemy == 0 ){
			// ���̊����ȉ��Ȃ�G����
			isCreate = true;
		}
		else if( currEnemy < enemyLimit ){
			// �o���p�x�ɂ���Đ���
			uint32_t rand = static_cast<uint32_t>( Utility::GetRandamValue( 1000, 0 ) );
			if( rand < ( 5 + (5*frequency) )){
				isCreate = true;	
			}
		}
	}
	return isCreate;
}

bool GameManager::IsCreateItem( uint32_t itemLimit, uint32_t frequency )
{
	bool isCreate = false;
	ItemManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerItem();	
	// �G�̐���
	if( pEnemyManager ){
		uint32_t currItem = pEnemyManager->CountItem();
		if( currItem <= itemLimit ){
			// �o���p�x�ɂ���Đ���
			uint32_t rand = static_cast<uint32_t>( Utility::GetRandamValue( 1000, 0 ) );
			if( rand < ( 5 + (5*frequency) )){
				isCreate = true;	
			}
		}
	}
	return isCreate;
}

