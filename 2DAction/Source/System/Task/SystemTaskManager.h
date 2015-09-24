
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

#ifndef __SYSTEM_TASK_MANAGER__
#define __SYSTEM_TASK_MANAGER__

#include <vector>
#include "System/SystemDefine.h"

class TaskUnit;

class TaskManagerBase
{
public:

	TaskManagerBase();
	virtual ~TaskManagerBase(void);

	//! ���j�b�g�̓o�^
	void AddUnit( TaskUnit *unit );

	//! �S�^�X�N�I��
	bool ReleaseAllEnd();

protected:

	//! �X�V����
	void Exec();
	void DrawUpdate();

	void DeleteDieUnit();
	std::vector<TaskUnit*> m_vTaskUnit;

};

class TaskManager : public TaskManagerBase
{
public:

	virtual ~TaskManager(void){};
	static TaskManager *GetInstance(){return m_pInstance;}

	void ExecApp(){
		Exec();
		DrawUpdate();
	}

private:

	TaskManager(){};
	static TaskManager *m_pInstance;

};

#endif