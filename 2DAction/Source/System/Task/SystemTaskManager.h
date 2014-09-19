
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

#ifndef SYSTEM_TASK_MANAGER
#define SYSTEM_TASK_MANAGER

#include <vector>
#include "System/SystemDefine.h"
#include "System/SystemMessage.h"

class TaskUnit;

class TaskManagerBase
{
public:

	TaskManagerBase();
	virtual ~TaskManagerBase(void);

	//! ���j�b�g�̓o�^
	void AddUnit(TaskUnit *unit);

	//! �X�V����
	void Update();
	void EventUpdate();
	void DrawUpdate();

	//! �S�^�X�N�I��
	bool ReleaseAllEnd();

protected:

	void DeleteDieUnit();
	std::vector<TaskUnit*> m_vTaskUnit;

};

class TaskManager : public TaskManagerBase
{
public:

	virtual ~TaskManager(void){};
	static TaskManager *GetInstance(){return m_pInstance;}

private:

	TaskManager(){};
	static TaskManager *m_pInstance;

};

#endif