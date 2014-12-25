/* ====================================================================== */
/**
 * @brief  ���b�Z�[�W�C�x���g���Ǘ�����N���X
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
		DEBUG_ASSERT( 0, "���ɃC���X�^���X�쐬�ς�");
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
}


bool SystemMessageManager::PushMessage( const uint32_t &uniqueId, const Common::CMN_EVENT &kind )
{
	auto it = m_messageUnitMap.find( uniqueId );
	if( it != m_messageUnitMap.end() ){
		(*it).second->PushEvent( kind );
		return true;
	}
	return false;
}

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

bool SystemMessageManager::IsNumberOverlapID( const uint32_t &checkID ) const
{
	// ID�̏d���`�F�b�N
	if( m_messageUnitMap.find( checkID ) == m_messageUnitMap.end() ){
		return false;
	}
	return true;
}


// ID�C�x���g�����X�^�[�g
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

	//for( auto it = m_messageUnitMap.begin() ;it != m_messageUnitMap.end(); ++it ){
	//	if( it != m_messageUnitMap.end() ){
	//		(*it).second->StartEventAction();
	//	}
	//}
}


// �q�̃N���X�̉��
void SystemMessageManager::DeleteMessageManager()
{
	DEBUG_PRINT("/_/_/SystemMessageManager �폜/_/_/\n");

	SAFE_DELETE( s_pInstance );
	
	DEBUG_PRINT("/_/_/SystemMessageManager ����/_/_/\n");
}
