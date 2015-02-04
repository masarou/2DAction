
/* ====================================================================== */
/**
 * @brief  音に関するまとめクラス
 *
 * @note
 *
 */
/* ====================================================================== */


#ifndef __SYSTEM_SOUND_MANAGER__
#define __SYSTEM_SOUND_MANAGER__

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"

class SoundManager : public TaskUnit
{
public:

	~SoundManager(void);
	static void Create();
	static SoundManager *GetInstance(){return m_pInstance;}
	virtual bool DieMain();
	virtual void Update();

	//! SEの処理(あらかじめ読み込んでおくのが前提)
	void PlaySE( const char *tagSE );
	void StopSE( const char *tagSE );

	//! BGMの再生(あらかじめ読み込んでおくのが前提)
	void PlayBGM( const char *tagBGM );
	void StopBGM( const bool isFade = true );
	void SetBgmVolume(const char *tag, uint32_t volume );

	//! ロードするテーブル変更(読み込んでいるものはクリア)
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

	//! ロードするテーブル変更
	void ChangeLoadTableMain( const std::string &tableStr, const bool &isLoadSound = false );

	//! 指定した識別名のハンドルIDを取得
	const uint32_t &GetHandleId( const char *tag );

	SoundManager(void);

	static SoundManager *m_pInstance;

	uint32_t				m_fadeTag;
	std::vector<SOUND_INFO>	m_vBGM;		//!< BGM情報ベクタ
	std::vector<SOUND_INFO>	m_vSE;		//!< SE情報ベクタ

};

#endif