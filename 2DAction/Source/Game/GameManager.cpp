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
#include "GameRecorder.h"
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
			m_settingFileStr = "GameSettings01.json";
			break;
		case Common::FLOW_STAGE02:
			m_settingFileStr = "GameSettings02.json";
			break;
		case Common::FLOW_STAGE03:
			m_settingFileStr = "GameSettings03.json";
			break;
		case Common::FLOW_STAGE04:
			m_settingFileStr = "GameSettings04.json";
			break;
		case Common::FLOW_STAGE05:
			m_settingFileStr = "GameSettings05.json";
			break;
		case Common::FLOW_STAGE06:
			m_settingFileStr = "GameSettings06.json";
			break;
		case Common::FLOW_STAGE07:
			m_settingFileStr = "GameSettings07.json";
			break;
		case Common::FLOW_STAGE08:
			m_settingFileStr = "GameSettings08.json";
			break;
		case Common::FLOW_STAGE09:
			m_settingFileStr = "GameSettings09.json";
			break;
		case Common::FLOW_STAGE10:
			m_settingFileStr = "GameSettings10.json";
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
	// �����z�u�ɐݒ肳��Ă���G�L�����𐶐�
	for( uint32_t i = 0; i < m_initEnemyInfoVec.size(); ++i ){
		math::Vector2 pos;
		if( m_initEnemyInfoVec.at(i).m_initPos == math::Vector2() ){
			pos = Utility::GetMapRandamPos( /*bool allowInWindow=*/true );
		}
		else{
			pos = math::Vector2( m_initEnemyInfoVec.at(i).m_initPos.x, m_initEnemyInfoVec.at(i).m_initPos.y );
		}
		CreateEnemy( m_initEnemyInfoVec.at(i).m_kind, m_initEnemyInfoVec.at(i).m_level, true, pos );
	}
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
		EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
		if( m_destroyNum >= m_destroyMax && pEnemyManager->CountEnemy() == 0 ){
			retVal = true;
		}
	}
	else if( m_type == TYPE_DESTROY_BOSS ){
		EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
		if( pEnemyManager ){
			// �{�X�Ƃ��Ďw�肳�ꂽ�G�����邩�ǂ����m�F
			for( uint32_t i = 0; i < m_bossKindVec.size() ; ++i ){
				if( pEnemyManager->CountEnemy( m_bossKindVec.at(i) ) == 0 ){
					retVal = true;
				}
				else{
					retVal = false;
					break;
				}
			}
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
 * @brief	�A�C�e�������˗�
 */
/* ================================================ */
void GameManager::CreateItem( const Common::ITEM_KIND &kind, const math::Vector2 &pos )
{
	if( IsCreateItem( m_itemMax ) ){
		ItemManager *pItemManager = GameRegister::GetInstance()->UpdateManagerItem();
		pItemManager->CreateItem( kind, pos );
	}
}

/* ================================================ */
/**
 * @brief	�G�����˗�
 */
/* ================================================ */
void GameManager::CreateEnemy( const Common::ENEMY_KIND &kind, const uint32_t &level, bool isForce, const math::Vector2 &enemyPos )
{
	if( IsCreateEnemy( m_enemyMax ) || isForce ){
		EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();
		pEnemyManager->AddEnemy( kind, level, enemyPos );
	}
}

/* ================================================ */
/**
 * @brief	���t���[������
 */
/* ================================================ */
void GameManager::Update()
{
	++m_gameTimer;

	// �G�̐���
	if( IsCreateEnemy( m_enemyMax, m_enemyFrequency ) ){
		uint32_t seedRandamValue = Utility::GetRandamValue( 100, 1 );
		uint32_t ratio = 0;
		for( uint32_t i = 0; i < m_enemyInfoVec.size() ; ++i ){
			ratio += m_enemyInfoVec.at(i).m_freequency;
			if( seedRandamValue < ratio ){
				CreateEnemy( m_enemyInfoVec.at(i).m_kind, m_enemyInfoVec.at(i).m_level );
				break;
			}
		}
	}
	// �A�C�e���̐���
	if( IsCreateItem( m_itemMax, m_itemFrequency ) ){
		uint32_t kind = Utility::GetRandamValue( Common::ITEM_KIND_ANTIDOTE-1, 0 );
		CreateItem( static_cast<Common::ITEM_KIND>(kind) );
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
	m_enemyInfoVec.clear();		// �o������G���
}

/* ================================================ */
/**
 * @brief	�����擾
 */
/* ================================================ */
void GameManager::LoadGameSettings( const char *jsonFile )
{
	picojson::value null;
	std::string path = JSON_OTHER_PATH;
	path += jsonFile;
	idxfstream ifs(path.c_str());

	picojson::value root;
	picojson::parse( root, ifs);

	//!��{�ƂȂ���擾(�X�e�[�W�̃N���A����)
	{
		picojson::value stageData = root.get("stageKind");
		std::string typeStr = stageData.get(0).get("type").get<std::string>();
		if( typeStr.compare("TIME") == 0 ){
			m_type = TYPE_TIME;
			m_gameTimeMax	= static_cast<uint32_t>(stageData.get(1).get("TimeS").get<double>());
			m_gameTimeMax *= 60;
		}
		else if( typeStr.compare("DESTROY") == 0 )
		{
			m_type = TYPE_DESTROY;
			m_destroyMax	= static_cast<uint32_t>(stageData.get(2).get("destroyMax").get<double>());
		}
		else if( typeStr.compare("DESTROY_BOSS") == 0 )
		{
			m_type = TYPE_DESTROY_BOSS;
			picojson::value bossData = root.get("bossInfo");
			picojson::value null;
			for(uint32_t i = 0;; ++i){
				if( bossData == null || bossData.get(i) == null){
					break;
				}
				
				// �{�X�J�E���g����G�̎�ނ��擾
				Common::ENEMY_KIND kind = GetEnemyKindFromStr( bossData.get(i).get("enemyKind").get<std::string>() );
				if( kind != Common::ENEMY_KIND_MAX ){
					m_bossKindVec.push_back( kind );
				}

				// �����z�u����{�X���Z�b�g
				uint32_t enemyIndex = static_cast<uint32_t>( bossData.get(i).get("num").get<double>() );
				for( uint32_t j = 0; j < enemyIndex; ++j ){
					ExistEnemyState enemyState;
					enemyState.Init();
					enemyState.m_kind = kind;
					enemyState.m_level = static_cast<uint32_t>( bossData.get(i).get("level").get<double>() );
					if( bossData.get(i).get("initPosX") != null && bossData.get(i).get("initPosY") != null ){
						enemyState.m_initPos.x = static_cast<float>( bossData.get(i).get("initPosX").get<double>() );
						enemyState.m_initPos.y = static_cast<float>( bossData.get(i).get("initPosY").get<double>() );
					}
					m_initEnemyInfoVec.push_back( enemyState );
				}
			}
		}
	}

	// �����z�u�̓G�ݒ�
	{
		picojson::value initEnemy = root.get("initEnemy");
		for( uint32_t i = 0 ;; ++i ){
			if( initEnemy == null || initEnemy.get(i) == null ){
				break;
			}
			uint32_t enemyIndex = static_cast<uint32_t>( initEnemy.get(i).get("num").get<double>() );
			for( uint32_t j = 0; j < enemyIndex ; ++j ){
				ExistEnemyState enemyState;
				enemyState.Init();
				enemyState.m_kind = GetEnemyKindFromStr( initEnemy.get(i).get("enemyKind").get<std::string>() );
				enemyState.m_level = static_cast<uint32_t>( initEnemy.get(i).get("level").get<double>() );
				if( initEnemy.get(i).get("initPosX") != null && initEnemy.get(i).get("initPosY") != null ){
					enemyState.m_initPos.x = static_cast<float>( initEnemy.get(i).get("initPosX").get<double>() );
					enemyState.m_initPos.y = static_cast<float>( initEnemy.get(i).get("initPosY").get<double>() );
				}
				m_initEnemyInfoVec.push_back( enemyState );
			}
		}
	}

	// �o���G�̕p�x
	{
		picojson::value enemyData = root.get("appearEnemy");
		uint32_t totalFreequency = 0;	// �o���W���̃g�[�^��
		for( uint32_t i = 0 ;; ++i ){
			if( enemyData == null || enemyData.get(i) == null ){
				break;
			}
			ExistEnemyState eneInfo;
			eneInfo.m_kind = GetEnemyKindFromStr( enemyData.get(i).get("enemyKind").get<std::string>() );
			eneInfo.m_level = static_cast<uint32_t>(enemyData.get(i).get("level").get<double>());
			eneInfo.m_freequency = static_cast<uint32_t>(enemyData.get(i).get("frequency").get<double>());
			if( eneInfo.m_level == 0 ){
				DEBUG_ASSERT( 0, "eneInfo.m_level is 0" );
				eneInfo.m_level = 1;
			}
			m_enemyInfoVec.push_back( eneInfo );

			totalFreequency += eneInfo.m_freequency;
		}
		// �o���W���̍��v���炻�ꂼ��̓G�̏o���������߂ăZ�b�g���Ȃ���
		for( uint32_t i = 0; i < m_enemyInfoVec.size() ; ++i ){
			float ratio =  m_enemyInfoVec.at(i).m_freequency / static_cast<float>( totalFreequency );
			m_enemyInfoVec.at(i).m_freequency = static_cast<uint32_t>( (ratio * 100.0f) + 0.5f );
		}
	}

	// �G�A�A�C�e���̏o���p�x
	{
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

	// �擾�ł���o�g���|�C���g���
	{
		picojson::value pointData = root.get("pointInfo");
		for( uint32_t i = 0 ;; ++i ){
			if( pointData == null || pointData.get(i) == null ){
				break;
			}
			uint32_t score = static_cast<uint32_t>(pointData.get(i).get("score").get<double>());
			uint32_t point = static_cast<uint32_t>(pointData.get(i).get("value").get<double>());
			GameRecorder::GetInstance()->SetClearBattlePoint( score, point );
		}
	}
}

/* ================================================ */
/**
 * @brief	�G�̐������f
 */
/* ================================================ */
bool GameManager::IsCreateEnemy( uint32_t enemyLimit, uint32_t frequency )
{
	bool isCreate = false;
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();	
	// �G�̐���
	if( pEnemyManager ){
		uint32_t currEnemy = pEnemyManager->CountEnemy();
		uint32_t under = frequency + 1;
		if( currEnemy < (enemyLimit / under) || currEnemy == 0 ){
			// ���݂̏o���G�������̊����ȉ��Ȃ�G����
			isCreate = true;
		}
		else if( currEnemy < enemyLimit ){
			// �o���p�x�ɂ���Đ���
			uint32_t rand = static_cast<uint32_t>( Utility::GetRandamValue( 1000, 0 ) );
			if( rand < ( 5 + (5*frequency) )){
				isCreate = true;	
			}
		}

		if( m_type == TYPE_DESTROY ){
			if( m_destroyNum + currEnemy >= m_destroyMax ){
				// ���ɓG���X�e�[�W�N���A�����̌��j���������ς݂Ȃ̂ł���ȏ�o�������Ȃ�
				isCreate = false;
			}
		}
	}
	return isCreate;
}
bool GameManager::IsCreateEnemy( uint32_t enemyLimit )
{
	bool isCreate = false;
	EnemyManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerEnemy();	
	// �G�̐���
	if( pEnemyManager ){
		uint32_t currEnemy = pEnemyManager->CountEnemy();
		if( currEnemy < enemyLimit ){
			isCreate = true;
		}
	}
	return isCreate;
}

/* ================================================ */
/**
 * @brief	�A�C�e���̐������f
 */
/* ================================================ */
bool GameManager::IsCreateItem( uint32_t itemLimit, uint32_t frequency )
{
	bool isCreate = false;
	ItemManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerItem();	
	// �A�C�e���̐���
	if( pEnemyManager ){
		uint32_t currItem = pEnemyManager->CountItem();
		if( currItem <= itemLimit ){
			// �o���p�x�ɂ���Đ���
			uint32_t rand = static_cast<uint32_t>( Utility::GetRandamValue( 1000, 1 ) );
			if( rand < ( 5*frequency )){
				isCreate = true;	
			}
		}
	}
	return isCreate;
}
bool GameManager::IsCreateItem( uint32_t itemLimit )
{
	bool isCreate = false;
	ItemManager *pEnemyManager = GameRegister::GetInstance()->UpdateManagerItem();	
	// �A�C�e���̐���
	if( pEnemyManager ){
		uint32_t currItem = pEnemyManager->CountItem();
		if( currItem <= itemLimit ){
			isCreate = true;
		}
	}
	return isCreate;
}


/* ================================================ */
/**
 * @brief	�G�̎�ނ𕶎��񂩂�擾
 */
/* ================================================ */
Common::ENEMY_KIND GameManager::GetEnemyKindFromStr( const std::string str )
{
	static struct enemyKindPair{
		Common::ENEMY_KIND	kind;
		std::string			kindStr;
	} s_enemyTypeStr[] = {
		{ Common::ENEMY_KIND_SLIME,			"ENEMY_KIND_SLIME" },
		{ Common::ENEMY_KIND_SLIME_ANOTHER,	"ENEMY_KIND_SLIME_ANOTHER" },
		{ Common::ENEMY_KIND_AHRIMAN,		"ENEMY_KIND_AHRIMAN" },
		{ Common::ENEMY_KIND_COW,			"ENEMY_KIND_COW" },
		{ Common::ENEMY_KIND_RANGER,		"ENEMY_KIND_RANGER" },
		{ Common::ENEMY_KIND_SLIME_KING,	"ENEMY_KIND_SLIME_KING" },
		{ Common::ENEMY_KIND_WIZARD,		"ENEMY_KIND_WIZARD" },
		{ Common::ENEMY_KIND_DRAGON,		"ENEMY_KIND_DRAGON" },
		{ Common::ENEMY_KIND_LAST_BOSS,		"ENEMY_KIND_LAST_BOSS" },
	};
	for( uint32_t i = 0; i < NUMBEROF(s_enemyTypeStr); ++i ){
		if( s_enemyTypeStr[i].kindStr.compare( str ) == 0 ){
			return s_enemyTypeStr[i].kind;
		}
	}
	DEBUG_ASSERT( 0, "json����ǂݍ��񂾓G�̃^�C�v���z��O");
	return Common::ENEMY_KIND_MAX;
}

