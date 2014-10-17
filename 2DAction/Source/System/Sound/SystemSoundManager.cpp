
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
	//! BGM�̃t�F�[�h����
	if(m_fadeTag != INVALID_VALUE){
		int32_t volume = GetVolumeSoundMem(m_fadeTag);
		if(volume > 5000){
			SetVolumeSoundMem(volume - VOLUME_FADE_SPEED, m_fadeTag);
		}
		else{
			//! BGM���~�߂�
			StopSoundMem(m_fadeTag);
			m_fadeTag = INVALID_VALUE;
		}
	}
}

/* ================================================ */
/**
 * @brief	SE�̏���(���炩���ߓǂݍ���ł����̂��O��)
 */
/* ================================================ */
void SoundManager::PlaySE(const char *tagSE)
{
	const uint32_t handle = GetHandleId(tagSE);
	if(handle == INVALID_VALUE){
		DEBUG_PRINT("�y�Đ����悤�Ƃ���SE�̓��[�h����Ă��Ȃ��H�z\n");
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
 * @brief	BGM�̏���(���炩���ߓǂݍ���ł����̂��O��)
 */
/* ================================================ */
void SoundManager::PlayBGM(const char *tagBGM)
{
	uint32_t handle = GetHandleId(tagBGM);
	if(handle == INVALID_VALUE){
		DEBUG_PRINT("�y�Đ����悤�Ƃ���BGM�̓��[�h����Ă��Ȃ��H�z\n");
		return;
	}

	//! �Đ����łȂ����m�F
	if(!CheckSoundMem(handle)){
		for(uint32_t i = 0; i < m_vBGM.size(); ++i){
			//! ����BGM�͂��ׂĎ~�߂�
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
			//! BGM�͈�Ȃ����Ȃ�Ȃ��O��Ȃ̂ōĐ��Ȃ��������甲����
			playBGMId = m_vBGM.at(i).m_soundId;
			break;
		}
	}

	if(isFade){
		//! �t�F�[�h������Ȃ�Update�ŏ������s��
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
 * @brief	���[�h����e�[�u���ύX(�ǂݍ���ł�����̂̓N���A)
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
	//! �܂�������
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

	//! �T�E���h�e�[�u���̓ǂݍ��݂Ƃ�������t�@�C���̃��[�h
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
				DEBUG_PRINT("�yBGM���[�h tag : %s ,FileName : %s�z\n", data.m_soundTag.c_str(), data.m_soundName.c_str());
			}
			else{
				m_vSE.push_back(data);
				DEBUG_PRINT("�ySE���[�h tag : %s ,FileName : %s�z\n", data.m_soundTag.c_str(), data.m_soundName.c_str());
			}
		}
		else{
			DEBUG_PRINT("�y�T�E���h�t�@�C�����[�h�G���[ : %s�z\n", filePath.c_str());
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

	DEBUG_PRINT("�ytag����擾���悤�Ƃ���handle�͌�����܂���ł����z\n");
	return m_vBGM.at(0).m_soundId;
}
