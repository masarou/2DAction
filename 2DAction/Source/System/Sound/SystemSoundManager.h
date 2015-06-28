
/* ====================================================================== */
/**
 * @brief  ���Ɋւ���܂Ƃ߃N���X
 *
 * @note
 *
 */
/* ====================================================================== */


#ifndef __SYSTEM_SOUND_MANAGER__
#define __SYSTEM_SOUND_MANAGER__

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"

#define BGM_VOLUME_MAX 9600
#define SE_VOLUME_MAX 9400

class SoundManager : public TaskUnit
{
public:

	~SoundManager(void);
	static void Create();
	static SoundManager *GetInstance(){return m_pInstance;}
	virtual bool DieMain();
	virtual void Update();

	//! SE�̏���(���炩���ߓǂݍ���ł����̂��O��)
	void PlaySE( const char *tagSE, const uint32_t &volume = SE_VOLUME_MAX );
	void StopSE( const char *tagSE );

	//! BGM�̍Đ�(���炩���ߓǂݍ���ł����̂��O��)
	void PlayBGM( const char *tagBGM, const uint32_t &volume = BGM_VOLUME_MAX );
	void StopBGM( const bool isFade = true );
	void SetBgmVolume(const char *tag, const uint32_t &volume = BGM_VOLUME_MAX );

	//! ���[�h����e�[�u���ύX(�ǂݍ���ł�����̂̓N���A)
	void ChangeLoadTableBGM( const std::string &tableStr );
	void ChangeLoadTableSE( const std::string &tableStr );

private:

	struct SOUND_INFO{
		uint32_t m_soundId;
		std::string m_soundTag;
		std::string m_soundName;
		void Init(){
			m_soundId = INVALID_VALUE;
			m_soundTag = "";
			m_soundName = "";
		}
	};

	//! ���[�h����e�[�u���ύX
	void ChangeLoadTableMain( const std::string &tableStr, const bool &isLoadSound = false );

	//! �w�肵�����ʖ��̃n���h��ID���擾
	const uint32_t &GetHandleId( const char *tag );

	SoundManager(void);

	static SoundManager *m_pInstance;

	uint32_t				m_fadeTag;
	std::vector<SOUND_INFO>	m_vBGM;		//!< BGM���x�N�^
	std::vector<SOUND_INFO>	m_vSE;		//!< SE���x�N�^

};

#endif