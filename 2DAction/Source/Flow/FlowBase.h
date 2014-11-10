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

class FlowBase : public TaskManagerBase
{
public:

	FlowBase(std::string fileName);
	virtual ~FlowBase(void);
	
	//! �����������L��
	virtual bool Init(){return true;}

	//! �����������L��
	virtual bool Finish();

	//! ���t���[���Ă΂��
	virtual void FlowUpdate() = 0;
	void ChildUpdate();

	//! �^�X�N�ǉ�
	void AddChildTask(TaskUnit *pTask);

	//! json�ǂݍ���
	void LoadFlowFile();

protected:

	void StartFade(const char *eventStr);

private:

	struct FLOW_DATA{
		std::string eventStr;	//!< �C�x���g��
		std::string filePath;	//!< �C�x���g�ɑ΂���t�@�C����
	};

	std::string m_fileName;		//!< json��
	std::vector<FLOW_DATA> m_vEventName;
};
#endif

