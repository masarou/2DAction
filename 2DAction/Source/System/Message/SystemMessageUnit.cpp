
#include "SystemMessageUnit.h"
#include "SystemMessageManager.h"
#include "Common/Utility/CommonGameUtility.h"

SystemMessageUnit::SystemMessageUnit(void)
: m_uniqueUnitId( INVALID_VALUE )
{
	m_eventVec.clear();

	// ほかのメッセージユニットとかぶらないIDを生成する
	for(;;){
		m_uniqueUnitId = Utility::GetRandamValue( INT_MAX, 0 );
		if( !SystemMessageManager::GetInstance()->IsNumberOverlapID(m_uniqueUnitId) ){
			// ユニークなID決定
			break;
		}
	}

	// 自分を登録しておく
	SystemMessageManager::GetInstance()->AddMessagTask( this );

}


SystemMessageUnit::~SystemMessageUnit(void)
{
	// 自分を削除しておく
	SystemMessageManager::GetInstance()->RemoveMessagTask( this );
}


/* ================================================ */
/**
 * @brief	MessageTaskのPush
 */
/* ================================================ */
void SystemMessageUnit::PushEvent( const Common::CMN_EVENT &eventInfo )
{
	if( eventInfo.m_event != Common::EVENT_MESSAGE_MAX ){
		m_nextEventVec.push_back( eventInfo );
	}
}

/* ================================================ */
/**
 * @brief	Messageイベントの開始
 */
/* ================================================ */
void SystemMessageUnit::StartEventAction()
{
	// 積まれたイベントをこのタイミングでpush
	for( uint32_t i = 0; i < m_nextEventVec.size() ; ++i ){
		m_eventVec.push_back( m_nextEventVec.at(i) );
	}
	m_nextEventVec.clear();

	auto it = m_eventVec.begin();
	for( uint32_t i = 0; it != m_eventVec.end() ; ++i ){
		if( (*it).m_delayTime == 0 ){
			EventUpdate( (*it) );
			it = m_eventVec.erase(it);
		}
		else{
			--(*it).m_delayTime;
			++it;
		}
	}
}
