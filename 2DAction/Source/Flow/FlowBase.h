/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�t���[�̋��ʍ��ځAjson�ǂݎ�蓙�̋@�\�����������N���X
 */
/* ====================================================================== */
#ifndef FLOW_BASE
#define FLOW_BASE

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskManager.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Input/SystemInputWatcher.h"

class FlowBase : public TaskManagerBase, public InputWatcher 
{
public:

	FlowBase(std::string fileName);
	virtual ~FlowBase(void);
	
	//! �����������L��
	virtual bool Init(){return true;}

	//!	�I�������L��
	virtual bool Finish();

	//! ���t���[���Ă΂��
	virtual void UpdateFlow();

	//! �Ԃ牺�����Ă���q�̍X�V
	void ChildUpdate();

	//! �^�X�N�ǉ�
	void AddChildTask(TaskUnit *pTask);

	//! json�ǂݍ���
	void LoadFlowFile();

	//! ���݂̃t���[���擾
	const std::string &GetFlowFilePath(){ return m_filePath; }

protected:

	void StartFade(const char *eventStr);

private:

	struct FLOW_DATA{
		std::string eventStr;	//!< �C�x���g��
		std::string filePath;	//!< �C�x���g�ɑ΂���t�@�C����
	};

	std::string m_filePath;		//!< �ǂݍ���ł���json�t�@�C���p�X
	std::vector<FLOW_DATA> m_vEventName;
};
#endif

