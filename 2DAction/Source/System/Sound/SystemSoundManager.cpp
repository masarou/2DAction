
#include "System/picojson.h"
#include "SystemSoundManager.h"

#define VOLUME_FADE_SPEED 10
#define VOLUME_MAX 10000

SoundManager *SoundManager::m_pInstance = NULL;

SoundManager::SoundManager(void)
: TaskUnit("Sound Manager")
, m_fadeTag(INVALID_VALUE)
{
	ChangeLoadTableBGM("SoundTest.json");
}


SoundManager::~SoundManager(void)
{
}


void SoundManager::Create()
{
	if( m_pInstance == NULL ){
		m_pInstance = NEW SoundManager;
	}
}


bool SoundManager::DieMain()
{
	InitSoundMem();
	return true;
}

void SoundManager::Update()
{
	//! BGMのフェード処理
	if(m_fadeTag != INVALID_VALUE){
		int32_t volume = GetVolumeSoundMem(m_fadeTag);
		if(volume > 5000){
			SetVolumeSoundMem(volume - VOLUME_FADE_SPEED, m_fadeTag);
		}
		else{
			//! BGMを止める
			StopSoundMem(m_fadeTag);
			m_fadeTag = INVALID_VALUE;
		}
	}
}

/* ================================================ */
/**
 * @brief	SEの処理(あらかじめ読み込んでおくのが前提)
 */
/* ================================================ */
void SoundManager::PlaySE(const char *tagSE)
{
	const uint32_t handle = GetHandleId(tagSE);
	if(handle == INVALID_VALUE){
		DEBUG_PRINT("【再生しようとしたSEはロードされていない？】\n");
		return;
	}

	PlaySoundMem(GetHandleId(tagSE), DX_PLAYTYPE_BACK);
}

void SoundManager::StopSE(const char *tagSE)
{
	uint32_t handle = GetHandleId(tagSE);
	if(handle == INVALID_VALUE){
		return;
	}

	StopSoundMem(handle);
}

/* ================================================ */
/**
 * @brief	BGMの処理(あらかじめ読み込んでおくのが前提)
 */
/* ================================================ */
void SoundManager::PlayBGM(const char *tagBGM)
{
	uint32_t handle = GetHandleId(tagBGM);
	if(handle == INVALID_VALUE){
		DEBUG_PRINT("【再生しようとしたBGMはロードされていない？】\n");
		return;
	}

	//! 再生中でないか確認
	if(!CheckSoundMem(handle)){
		for(uint32_t i = 0; i < m_vBGM.size(); ++i){
			//! 他のBGMはすべて止める
			StopSoundMem(m_vBGM.at(i).m_soundId);
		}
		SetVolumeSoundMem( VOLUME_MAX, handle);
		PlaySoundMem(handle, DX_PLAYTYPE_LOOP);
		m_fadeTag = INVALID_VALUE;
	}
}

void SoundManager::StopBGM(bool isFade)
{
	uint32_t playBGMId = INVALID_VALUE;
	for(uint32_t i = 0; i < m_vBGM.size(); ++i){
		if(CheckSoundMem(m_vBGM.at(i).m_soundId)){
			//! BGMは一曲しかならない前提なので再生曲を見つけたら抜ける
			playBGMId = m_vBGM.at(i).m_soundId;
			break;
		}
	}

	if(isFade){
		//! フェードがありならUpdateで処理を行う
		m_fadeTag = playBGMId;
	}
	else{
		StopSoundMem(playBGMId);
	}
}

void SoundManager::SetBgmVolume(const char *tag, uint32_t volume)
{
	uint32_t handle = GetHandleId(tag);
	if(handle == INVALID_VALUE){
		return;
	}

	ChangeVolumeSoundMem( volume, handle);
}

/* ================================================ */
/**
 * @brief	ロードするテーブル変更(読み込んでいるものはクリア)
 */
/* ================================================ */
void SoundManager::ChangeLoadTableBGM( const std::string &tableStr )
{
	ChangeLoadTableMain( tableStr, true );
}

void SoundManager::ChangeLoadTableSE( const std::string &tableStr )
{
	ChangeLoadTableMain( tableStr, false );
}

void SoundManager::ChangeLoadTableMain( const std::string &tableStr, const bool &isLoadSound )
{
	//! まず初期化
	if(isLoadSound){
		for(uint32_t i = 0; i < m_vBGM.size(); ++i){
			DeleteSoundMem(m_vBGM.at(i).m_soundId);
		}
		m_vBGM.clear();
	}
	else{
		for(uint32_t i = 0; i < m_vSE.size(); ++i){
			DeleteSoundMem(m_vSE.at(i).m_soundId);
		}
		m_vSE.clear();
	}

	//! サウンドテーブルの読み込みとそこからファイルのロード
	std::string pathStr = JSON_SOUND_PATH;
	pathStr += tableStr;
	std::ifstream ifs(pathStr.c_str());

	picojson::value root;
	picojson::parse( root, ifs);
	
	picojson::value sceneData = root.get("soundTable");
	for(uint32_t i = 0;; ++i){
		picojson::value null;
		if(sceneData.get(i) == null){
			break;
		}
		SOUND_INFO data;
		data.Init();
		data.m_soundTag = sceneData.get(i).get("tag").get<std::string>();
		data.m_soundName = sceneData.get(i).get("file").get<std::string>();
		std::string filePath = "";
		if(isLoadSound){
			filePath = BGM_PATH;
		}
		else{
			filePath = SE_PATH;
		}
		filePath += data.m_soundName.c_str();
		int32_t soundId = LoadSoundMem(filePath.c_str());
		if(soundId != -1){
			data.m_soundId = soundId;
			if(isLoadSound){
				m_vBGM.push_back(data);
				DEBUG_PRINT("【BGMロード tag : %s ,FileName : %s】\n", data.m_soundTag.c_str(), data.m_soundName.c_str());
			}
			else{
				m_vSE.push_back(data);
				DEBUG_PRINT("【SEロード tag : %s ,FileName : %s】\n", data.m_soundTag.c_str(), data.m_soundName.c_str());
			}
		}
		else{
			DEBUG_PRINT("【サウンドファイルロードエラー : %s】\n", filePath.c_str());
		}
	}
}



const uint32_t &SoundManager::GetHandleId(const char *tag)
{
	for(uint32_t i = 0; i < m_vSE.size(); ++i){
		if(m_vSE.at(i).m_soundTag.compare(tag) == 0){
			return m_vSE.at(i).m_soundId;
		}
	}
	for(uint32_t i = 0; i < m_vBGM.size(); ++i){
		if(m_vBGM.at(i).m_soundTag.compare(tag) == 0){
			return m_vBGM.at(i).m_soundId;
		}
	}

	DEBUG_PRINT("【tagから取得しようとしたhandleは見つかりませんでした】\n");
	return m_vBGM.at(0).m_soundId;
}
