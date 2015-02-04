/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�t���[�̋��ʍ��ځAjson�ǂݎ�蓙�̋@�\�����������N���X
 */
/* ====================================================================== */
#ifndef __FLOW_BASE__
#define __FLOW_BASE__

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskManager.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Input/SystemInputWatcher.h"
#include "FlowManager.h"

class FlowBase : public TaskManagerBase, public InputWatcher 
{
	friend class FlowManager;

protected:

	FlowBase(std::string fileName);
	virtual ~FlowBase(void);
	
	//! �����������L��
	virtual bool Init(){return true;}

	//! �h����ł̍X�V�֐�
	virtual void UpdateFlowPreChildTask(){}
	virtual void UpdateFlowAfterChildTask(){}

	//! �h����I�������L��
	virtual bool FinishFlow(){ return true; }

	//! ���݂̃t���[���擾
	const std::string &GetFlowFilePath() const{ return m_filePath; }

protected:
	
	// ���̉�ʂɑJ��
	void StartFade( const char *eventStr );

private:

	//!	�I�������L��
	bool Finish();

	//! json�ǂݍ���
	void LoadFlowFile();
	
	//! ���t���[���Ă΂��
	void UpdateFlow();

	//! �^�X�N�ǉ�
	void AddChildTask(TaskUnit *pTask);

	//! �Ԃ牺�����Ă���q�̍X�V
	void ChildUpdate();


	struct FLOW_DATA{
		std::string eventStr;	//!< �C�x���g��
		std::string filePath;	//!< �C�x���g�ɑ΂���t�@�C����
	};

	std::string m_filePath;		//!< �ǂݍ���ł���json�t�@�C���p�X
	std::vector<FLOW_DATA> m_vEventName;
};
#endif

