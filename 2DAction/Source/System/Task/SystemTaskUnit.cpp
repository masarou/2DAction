
/* ====================================================================== */
/**
 * @brief  �e�N���X�̍ŏ��P�ʂ̃N���X
 *
 * @note
 *		��{�I�ɂ͂��̃N���X��h�����������̂��g�p����B
 *		m_status�ɂ���Ď����I��update���Ă΂ꂽ��delete���ꂽ�肷��B
 *		setStatus��UNIT_DEAD�ɂ��Ă��Ǝ����I�ɍ폜�����B
 *		�֐�����
 *			Init()		: �ŏ��ɕK���Ă΂��Atrue��Ԃ��܂ŌĂ΂ꑱ����
 *			Update()	: ���t���[���Ă΂��A�X�V��������
 *			DrawUpdate(): ���t���[���Ă΂��A�`�擙��
 *			DieMain()	: m_status��UNIT_DEAD�ɂ�����Ă΂��B
 *						  true��Ԃ��܂ŌĂ΂ꑱ���A���̌�N���X����������
 */
/* ====================================================================== */

#include "SystemTaskUnit.h"
#include "SystemTaskManager.h"
#include "Flow/FlowManager.h"

TaskUnit::TaskUnit(std::string name, TASK_STATUS status)
: m_status(status)
, m_name(name)
, m_pParent(NULL)
{
	bool isSet = false;

	//! �t���[���ɃZ�b�g�����݂�
	FlowManager *pFlow = FlowManager::GetInstance();
	if(pFlow){
		isSet = FlowManager::GetInstance()->AddUnit(this);
	}

	//! �Z�b�g�ł��Ȃ�������ėp�փZ�b�g
	if(!isSet){
		TaskManager::GetInstance()->AddUnit(this);
	}
}

const TaskUnit::TASK_STATUS &TaskUnit::GetStatus() const
{
	return m_status;
}

bool TaskUnit::IsDie()
{
	if( GetStatus() < TASK_PRE_DIE ){
		return true;
	}
	return false;
}

TaskUnit::~TaskUnit(void)
{
	//! �e�N���X�Ɏ��������ʂ��Ƃ�`����
	if(m_pParent){
		m_pParent->ReleaseConnection(this);
	}
	for(uint32_t i = 0; i < m_pChildVec.size(); ++i){
		//! �e�����ʂ���q���E��
		m_pChildVec.at(i)->SetDieStateFromParent(this);
	}
	//DEBUG_PRINT("/_/_/TaskUnit��� name : %s /_/_/\n",m_name.c_str());
}

/* ================================================ */
/**
 * @brief	�e�^�X�N�ݒ�
 */
/* ================================================ */
void TaskUnit::TaskStartDie()
{
	if( GetStatus() < TASK_PRE_DIE){
		SetStatus(TASK_PRE_DIE);
	}
}

/* ================================================ */
/**
 * @brief	�e�^�X�N�ݒ�
 */
/* ================================================ */
void TaskUnit::AddConnectionParent( TaskUnit *parent )
{
	//! �e�ɐݒ肳���
	if(!m_pParent){
		m_pParent = parent;
	}
	else{
		//! ���łɂ���ꍇ�̓A�T�[�g
		DEBUG_ASSERT( 0, "�e�����łɂ���̂ɂ܂��Z�b�g���悤�Ƃ��Ă���\n");
	}
}

/* ================================================ */
/**
 * @brief	�q�^�X�N�ݒ�
 */
/* ================================================ */
void TaskUnit::SetChildUnit( TaskUnit *unit )
{
	unit->AddConnectionParent(this);
	m_pChildVec.push_back(unit);
}

/* ================================================ */
/**
 * @brief	�e�q�֌W�̉���
 */
/* ================================================ */
void TaskUnit::ReleaseConnection( TaskUnit *unit )
{
	if(m_pParent == unit){
		m_pParent = NULL;
		return;
	}

	std::vector<TaskUnit*>::iterator it;
	for(it = m_pChildVec.begin(); it != m_pChildVec.end(); ++it){
		if((*it) == unit){
			m_pChildVec.erase(it);
			return;
		}
	}

	DEBUG_ASSERT( 0, "�e�q�֌W�����������AReleaseConnection��������������Ȃ��B\n");
}

/* ================================================ */
/**
 * @brief	�e�Ƀ��b�Z�[�W�𓊂���
 */
/* ================================================ */
void TaskUnit::SendMessageToParent( const Message &msg )
{
	if( m_pParent ){
		m_pParent->MessageReceive(msg);
	}
}

/* ================================================ */
/**
 * @brief	�X�e�[�^�X�ύX
 */
/* ================================================ */
void TaskUnit::SetStatus( const TASK_STATUS &status )
{
	m_status = status;
}

void TaskUnit::SetDieStateFromParent( TaskUnit *pParent )
{
	if( !m_pParent || m_pParent != pParent ){
		DEBUG_ASSERT( 0, "�e�ȊO����Ă΂ꂽ�����������e���Ȃ�\n");
	}

	//! �e�q�֌W���������Ď��ʏ���
	ReleaseConnection( pParent );
	if( GetStatus() < TASK_PRE_DIE ){
		SetStatus( TASK_PRE_DIE );
	}
}
