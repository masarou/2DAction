
/* ====================================================================== */
/**
 * @brief  �N���X�Ԃ̂���������N���X
 *
 * @note �g�p���@
	�V�����o�^����ꍇ��TaskUnitBase���p�����Ă���ꍇ��
	����ɃR���X�g���N�^�œo�^���āA�f�X�g���N�^���Ă΂���
	�폜�����̂œ��Ɉӎ������g�p�\�B
	�o�^����Ɩ��t���[��Update��DrawUpdate���Ă΂��B

 */
/* ====================================================================== */

#include "SystemTaskManager.h"
#include "SystemTaskUnit.h"
#include "System/Draw2D/SystemDraw2DManager.h"

TaskManager *TaskManager::m_pInstance = NEW TaskManager();

TaskManagerBase::TaskManagerBase(void)
{
	m_vTaskUnit.clear();
}

TaskManagerBase::~TaskManagerBase(void)
{
	std::vector<TaskUnit*>::iterator it = m_vTaskUnit.begin();
	while(m_vTaskUnit.empty() == 0){
		//! ��{�I�ɂ��ׂẴ^�X�N�͎����ō폜����̂ł����� m_vTaskUnit.size()��0�̂͂�
		TaskUnit *task = *it;
		DEBUG_PRINT("/_/_/������Y��^�X�N������� : %s/_/_/\n",task->GetName().c_str());
		it = m_vTaskUnit.erase(it);
		SAFE_DELETE(task);
	}
	m_vTaskUnit.clear();
}

/* ================================================ */
/**
 * @brief	�^�X�N�ǉ�
 */
/* ================================================ */
void TaskManagerBase::AddUnit( TaskUnit *unit )
{
	if( !unit ){
		DEBUG_ASSERT( 0, "task��NULL");
	}
	m_vTaskUnit.push_back(unit);
}

/* ================================================ */
/**
 * @brief	�^�X�N�X�V����
 */
/* ================================================ */
void TaskManagerBase::Exec()
{
	for(uint32_t i = 0; i < m_vTaskUnit.size(); ++i){
		TaskUnit *pTask = m_vTaskUnit.at(i);
		switch(pTask->GetStatus()){
		default:
			DEBUG_ASSERT( 0, "/_/_/task status ���z��O/_/_/");
			break;

		case TaskUnit::TASK_IDLE:
			//! �������Ȃ�
			break;

		case TaskUnit::TASK_INIT:
			if(pTask->Init()){
				pTask->SetStatus(TaskUnit::TASK_ALIVE);
			}
			break;

		case TaskUnit::TASK_ALIVE:
			pTask->Update();
			break;

		case TaskUnit::TASK_PRE_DIE:
			if(pTask->DieMain()){
				pTask->SetStatus(TaskUnit::TASK_DIE);
			}
			break;

		case TaskUnit::TASK_DIE:
			DEBUG_ASSERT( 0, "/_/_/�����ɗ���̂͂�������/_/_/");
			break;
		}
	}

	//! ��Ԃ�TASK_PRE_DIE�ɂȂ����^�X�N���������
	DeleteDieUnit();
}

void TaskManagerBase::DrawUpdate()
{
	for(uint32_t i = 0; i < m_vTaskUnit.size(); ++i){
		TaskUnit *pTask = m_vTaskUnit.at(i);
		if(pTask->GetStatus() == TaskUnit::TASK_ALIVE){
			pTask->DrawUpdate();
		}
	}
}

/* ================================================ */
/**
 * @brief	���S�^�X�N���
 */
/* ================================================ */
void TaskManagerBase::DeleteDieUnit()
{
	std::vector<TaskUnit*>::iterator it = m_vTaskUnit.begin();
	while(it != m_vTaskUnit.end()){
		TaskUnit *task = *it;
		if(task->GetStatus() == TaskUnit::TASK_DIE){
			it = m_vTaskUnit.erase(it);
			SAFE_DELETE(task);
			continue;
		}
		if(it != m_vTaskUnit.end()){
			++it;
		}
	}
}

/* ================================================ */
/**
 * @brief	�I�����̃^�X�N����҂��֐�
 */
/* ================================================ */
bool TaskManagerBase::ReleaseAllEnd()
{
	for(unsigned int i = 0; i < m_vTaskUnit.size(); ++i){
		TaskUnit *pTask = m_vTaskUnit.at(i);
		if(pTask->DieMain()){
			pTask->SetStatus(TaskUnit::TASK_DIE);
		}
	}
	//! ��Ԃ�TASK_PRE_DIE�ɂȂ����^�X�N���������
	DeleteDieUnit();

	if(m_vTaskUnit.size() == 0){
		return true;
	}
	return false;
}
