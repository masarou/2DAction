
#include "SystemMessageUnit.h"
#include "SystemMessageManager.h"
#include "Common/Utility/CommonGameUtility.h"

SystemMessageUnit::SystemMessageUnit(void)
: m_uniqueUnitId( INVALID_VALUE )
{
	m_eventVec.clear();

	// �ق��̃��b�Z�[�W���j�b�g�Ƃ��Ԃ�Ȃ�ID�𐶐�����
	for(;;){
		m_uniqueUnitId = GetRandamValue( INT_MAX, 0 );
		if( !SystemMessageManager::GetInstance()->IsNumberOverlapID(m_uniqueUnitId) ){
			// ���j�[�N��ID����
			break;
		}
	}

	// ������o�^���Ă���
	SystemMessageManager::GetInstance()->AddMessagTask( this );

}


SystemMessageUnit::~SystemMessageUnit(void)
{
	// �������폜���Ă���
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

	// �C�x���g�����I��
	m_eventVec.clear();
}
