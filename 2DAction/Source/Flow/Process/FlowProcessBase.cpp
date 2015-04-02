#include "FlowProcessBase.h"


ProcessBase::ProcessBase(void)
: m_effectState( STATE_INIT )
{
}


ProcessBase::~ProcessBase(void)
{
}

void ProcessBase::SetStateNext()
{
	switch( m_effectState ){
	default:
		DEBUG_ASSERT( 0, "�z��O�̃X�e�[�^�X" );
		break;
	case STATE_INIT:		// Init��
		m_effectState = STATE_UPDATE;
		break;
	case STATE_UPDATE:		// Update��
		m_effectState = STATE_CANDIE;
		break;
	case STATE_CANDIE:		// CanDie��
		m_effectState = STATE_FLOW_WAIT;
		break;
	case STATE_FLOW_WAIT:	// �G�t�F�N�g�I���B�t���[�̍폜�҂�
		break;
	}
}

void ProcessBase::Exec()
{
	switch( GetState() ){
	default:
		DEBUG_ASSERT( 0, "�z��O�̃X�e�[�^�X" );
		break;
	case STATE_INIT:		// Init��
		if( Init() ){
			SetStateNext();
		}
		break;
	case STATE_UPDATE:		// Update��
		Update();
		break;
	case STATE_CANDIE:		// CanDie��
		if( CanDie() ){
			SetStateNext();
		}
		break;
	case STATE_FLOW_WAIT:	// �G�t�F�N�g�I���B�t���[�̍폜�҂�
		break;
	}

	CallPadEvent();
}