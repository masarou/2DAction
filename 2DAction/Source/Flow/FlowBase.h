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
#include "System/Sound/SystemSoundManager.h"
#include "FlowManager.h"

class ProcessBase;

class FlowBase : public TaskManagerBase, public InputWatcher 
{
	friend class FlowManager;

protected:

	FlowBase( const std::string &fileName);
	virtual ~FlowBase(void);
	
	//! �����������L��
	virtual bool Init(){return true;}

	//! �h����ł̍X�V�֐�
	virtual void UpdateFlowPreChildTask(){}
	virtual void CheckNextFlow(){}
	virtual void UpdateFlowAfterChildTask(){}

	//! �h����I�������L��
	virtual bool FinishFlow(){ return true; }

	//! ���݂̃t���[���擾
	const std::string &GetFlowFilePath() const{ return m_filePath; }

	//! �X�e�[�W�G�t�F�N�gAdd
	void PushStageEffect( ProcessBase *pEffect ){ m_vStageEffect.push_back( pEffect ); }
	bool IsPlaySpecialEffect(){ return ( m_vStageEffect.size() == 0 ) ? false : true ; }

protected:
	
	// ���̉�ʂɑJ��
	bool StartFade( const char *eventStr );

	// �Q�[�����|�[�Y��Ԃ�
	virtual void PadEventStart() override;

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

	bool m_isInvalidPadCtrl;	//!< �p�b�h���얳���t���O
	std::string m_filePath;		//!< �ǂݍ���ł���json�t�@�C���p�X
	std::vector<FLOW_DATA> m_vEventName;

	std::vector<ProcessBase*>	m_vStageEffect;	// �Q�[���S�̂��~�߂Č����鉉�o(�Q�[��������)
};
#endif

