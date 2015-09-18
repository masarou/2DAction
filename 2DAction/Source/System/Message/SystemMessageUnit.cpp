
#include "SystemMessageUnit.h"
#include "SystemMessageManager.h"
#include "Common/Utility/CommonGameUtility.h"

SystemMessageUnit::SystemMessageUnit(void)
: m_uniqueUnitId( INVALID_VALUE )
{
	m_eventVec.clear();

	// �ق��̃��b�Z�[�W���j�b�g�Ƃ��Ԃ�Ȃ�ID�𐶐�����
	for(;;){
		m_uniqueUnitId = Utility::GetRandamValue( INT_MAX, 0 );
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


/* ================================================ */
/**
 * @brief	MessageTask��Push
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
 * @brief	Message�C�x���g�̊J�n
 */
/* ================================================ */
void SystemMessageUnit::StartEventAction()
{
	// �ς܂ꂽ�C�x���g�����̃^�C�~���O��push
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
