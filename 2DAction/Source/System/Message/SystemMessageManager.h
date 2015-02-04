/* ====================================================================== */
/**
 * @brief  メッセージイベントを管理するクラス
 *
 * @note
 *		SystemMessageUnitを継承したクラスが登録されるManagerクラス
 *		他のクラスからのイベントを積んで、特定のタイミングで
 *		SystemMessageUnitのEventUpdateを呼んで各クラスに知らせてあげる
 */
/* ====================================================================== */

#ifndef __MESSAGE_MANAGER__
#define __MESSAGE_MANAGER__

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

	// 特定のタスクの登録と削除
	void AddMessagTask( SystemMessageUnit *unit );
	void RemoveMessagTask( SystemMessageUnit *unit );

	// 指定IDが他とかぶっているかチェック
	bool IsNumberOverlapID( const uint32_t &checkID ) const;

	// 登録してあるMessageTaskのイベント処理開始
	void StartMessageEvent();
	
	// このクラスの解放
	static void DeleteMessageManager();

private:

	SystemMessageManager(void);
	virtual ~SystemMessageManager(void);

	static SystemMessageManager *s_pInstance;

	std::map<uint32_t, SystemMessageUnit*>	m_messageUnitMap;

};

#endif