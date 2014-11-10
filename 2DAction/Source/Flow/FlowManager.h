/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�t���[�̂��ׂĂ��Ǘ�����Manager
 */
/* ====================================================================== */
#ifndef FLOW_MANAGER
#define FLOW_MANAGER

#include "System/Task/SystemTaskUnit.h"
#include "Flow/FlowBase.h"

class FlowManager : public TaskUnit
{
	friend FlowBase;

public:

	static void Create();
	static FlowManager *GetInstance();

	//! ���j�b�g�̓o�^
	bool AddUnit(TaskUnit *unit);

private:

	//! �Ǘ��t���[�̃X�e�b�v
	enum FLOW_STATUS{
		FLOW_FADEIN_CHECK,	// �t�F�[�h�C���Z�b�g
		FLOW_FADEIN_WAIT,	// �t�F�[�h�C���҂�
		FLOW_CHANGE_WAIT,	// ���S�҂�
		FLOW_INIT,			// �������O
		FLOW_FADEOUT_CHECK,	// �t�F�[�h�A�E�g�Z�b�g
		FLOW_FADEOUT_WAIT,	// �t�F�[�h�A�E�g�҂�
		FLOW_ACTION,		// �N����

		FLOW_MAX,
	};

	FlowManager();
	~FlowManager();

	virtual bool Init();
	virtual void Update();
	virtual void DrawUpdate();
	virtual bool DieMain();

	//! �t���[�ύX
	void ChangeFlow(const char* filePath);

	static FlowManager	*m_pInstance;
	std::string			m_nextFilePath;	//!<�J�ڐ�̉�ʂ̃t�@�C����
	FlowBase			*m_pFlow;		//!<�����ݕ\�����Ă���t���[�y�^�C�g����Q�[�����z
	FLOW_STATUS			m_step;			//!<�t�F�[�h�A�j���̌��݂̃X�e�b�v
	FLOW_STATUS			m_fadeNext;		//!<�t�F�[�h�A�j���̎��̃X�e�b�v

};
#endif
// FLOW_MANAGER