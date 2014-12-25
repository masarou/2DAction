
#include "SystemMessageUnit.h"
#include "SystemMessageManager.h"
#include "Common/Utility/CommonGameUtility.h"

SystemMessageUnit::SystemMessageUnit(void)
: m_uniqueUnitId( INVALID_VALUE )
{
	m_eventVec.clear();

	// ほかのメッセージユニットとかぶらないIDを生成する
	for(;;){
		m_uniqueUnitId = GetRandamValue( INT_MAX, 0 );
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

void SystemMessageUnit::PushEvent( const Common::CMN_EVENT &eventInfo )
{
	if( eventInfo.m_event != Common::EVENT_MESSAGE_MAX ){
		m_eventVec.push_back( eventInfo );
	}
}

void SystemMessageUnit::StartEventAction()
{
	for( uint32_t i = 0; i < m_eventVec.size() ; ++i ){
		EventUpdate( m_eventVec.at(i) );
	}

	// イベント処理終了
	m_eventVec.clear();
}
