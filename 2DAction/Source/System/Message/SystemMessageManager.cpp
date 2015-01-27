/* ====================================================================== */
/**
 * @brief  メッセージイベントを管理するクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#include "Common/CommonDefine.h"
#include "SystemMessageUnit.h"

SystemMessageManager *SystemMessageManager::s_pInstance = NULL;

SystemMessageManager *SystemMessageManager::Create()
{
	if( s_pInstance ){
		DEBUG_ASSERT( 0, "既にインスタンス作成済み");
	}
	else{
		s_pInstance = NEW SystemMessageManager();
	}
	return s_pInstance;
}

SystemMessageManager *SystemMessageManager::GetInstance()
{
	return s_pInstance;
}

SystemMessageManager::SystemMessageManager(void)
{
	m_messageUnitMap.clear();
}


SystemMessageManager::~SystemMessageManager(void)
{
	s_pInstance = NULL;
}

/* ================================================ */
/**
 * @brief	特定のMessageタスクにメッセージをPush
 */
/* ================================================ */
bool SystemMessageManager::PushMessage( const uint32_t &uniqueId, const Common::CMN_EVENT &kind )
{
	auto it = m_messageUnitMap.find( uniqueId );
	if( it != m_messageUnitMap.end() ){
		(*it).second->PushEvent( kind );
		return true;
	}
	return false;
}

/* ================================================ */
/**
 * @brief	MessageTaskの登録と削除
 */
/* ================================================ */
void SystemMessageManager::AddMessagTask( SystemMessageUnit *unit )
{
	if( unit ){
		m_messageUnitMap.insert( std::make_pair( unit->GetUniqueId(), unit ) );
	}
}

void SystemMessageManager::RemoveMessagTask( SystemMessageUnit *unit )
{
	uint32_t uniqueId = unit->GetUniqueId();
	m_messageUnitMap.erase( uniqueId );
}

/* ================================================ */
/**
 * @brief	登録する際にIDの重複判定関数
 */
/* ================================================ */
bool SystemMessageManager::IsNumberOverlapID( const uint32_t &checkID ) const
{
	// IDの重複チェック
	if( m_messageUnitMap.find( checkID ) == m_messageUnitMap.end() ){
		return false;
	}
	return true;
}

/* ================================================ */
/**
 * @brief	IDイベント処理スタート
 */
/* ================================================ */
void SystemMessageManager::StartMessageEvent()
{
	auto it = m_messageUnitMap.begin();
	for( uint32_t i = 0 ;; ++i ){
		if( it != m_messageUnitMap.end() ){
			(*it).second->StartEventAction();
			++it;
		}
		else{
			break;
		}
	}
}


/* ================================================ */
/**
 * @brief	このクラスの削除
 */
/* ================================================ */
void SystemMessageManager::DeleteMessageManager()
{
	DEBUG_PRINT("/_/_/SystemMessageManager 削除/_/_/\n");

	SAFE_DELETE( s_pInstance );
	
	DEBUG_PRINT("/_/_/SystemMessageManager 完了/_/_/\n");
}
