/* ====================================================================== */
/**
 * @brief  メッセージイベントを管理するクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef MESSAGE_MANAGER
#define MESSAGE_MANAGER

#include <map>
#include "Common/CommonDefine.h"

class SystemMessageUnit;

class SystemMessageManager
{

public:

	static SystemMessageManager *Create();
	static SystemMessageManager *GetInstance();

	// 特定のタスクにメッセージイベントをPush
	bool PushMessage( const uint32_t &uniqueId, const Common::CMN_EVENT &kind );

	// 特定のタスクを削除
	void AddMessagTask( SystemMessageUnit *unit );
	void RemoveMessagTask( SystemMessageUnit *unit );

	// 指定IDが他とかぶっているかチェック
	bool IsNumberOverlapID( const uint32_t &checkID ) const;

	// 登録してあるMessageTaskのイベント処理開始
	void StartMessageEvent();
	
	// 子のクラスの解放
	static void DeleteMessageManager();

private:

	SystemMessageManager(void);
	virtual ~SystemMessageManager(void);

	static SystemMessageManager *s_pInstance;

	std::map<uint32_t, SystemMessageUnit*>	m_messageUnitMap;

};

#endif