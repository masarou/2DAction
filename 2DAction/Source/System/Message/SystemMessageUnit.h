/* ====================================================================== */
/**
 * @brief  メッセージイベントをやり取りする基底クラス
 *
 * @note
 *		単体で使用することはない
 *		これを基底にして他のクラスからのイベントを処理していく
 */
/* ====================================================================== */

#ifndef MESSAGE_UNIT
#define MESSAGE_UNIT

#include "Common/CommonDefine.h"
#include "System/Message/SystemMessageManager.h"

class SystemMessageUnit
{
public:

	friend SystemMessageManager;
	
	// 識別ID取得
	const uint32_t &GetUniqueId() const{ return m_uniqueUnitId; }

protected:

	SystemMessageUnit(void);
	virtual ~SystemMessageUnit(void);

	virtual void EventUpdate( const Common::CMN_EVENT &eventId ){};

private:

	// 以下の関数は基本的にSystemMessageManagerからのみアクセス
	void PushEvent( const Common::CMN_EVENT &eventInfo );
	void StartEventAction();

	uint32_t						m_uniqueUnitId;	// 識別用ID
	std::vector<Common::CMN_EVENT>	m_eventVec;		// ほかのクラスからのイベント

};

#endif