/* ====================================================================== */
/**
 * @brief  
 *		�t���[�̂��ׂĂ��Ǘ�����Manager
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __FLOW_MANAGER__
#define __FLOW_MANAGER__

#include "Common/CommonDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "FlowBase.h"
#include "Process/FlowProcessBase.h"

class FlowManager : public TaskUnit
{
	friend class FlowBase;

public:

	static void Create();
	static FlowManager *GetInstance();

	//! ���j�b�g�̓o�^
	bool AddUnit(TaskUnit *unit);

	//! ���݂̃t���[���擾
	const char *GetCurrentFlow() const;

	//! ���݂̃Q�[���t���[�̎�ގ擾
	const Common::GAME_FLOW &GetCurrentFlowKind() const{ return m_currFlow; }
	const Common::GAME_FLOW &GetPreFlowKind() const{ return m_preFlow; }

	//! �X�e�[�W�G�t�F�N�g�ǉ�
	void SetupSpecialEffect( ProcessBase *pEffect );

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
	virtual bool DieMain();

	//! �t���[�ύX
	bool ChangeFlow(const char* filePath);

	static FlowManager	*m_pInstance;
	std::string			m_nextFilePath;	//!<�J�ڐ�̉�ʂ̃t�@�C����
	FlowBase			*m_pFlow;		//!<�����ݕ\�����Ă���t���[�y�^�C�g����Q�[�����z
	FLOW_STATUS			m_step;			//!<�t�F�[�h�A�j���̌��݂̃X�e�b�v
	FLOW_STATUS			m_fadeNext;		//!<�t�F�[�h�A�j���̎��̃X�e�b�v

	Common::GAME_FLOW	m_currFlow;
	Common::GAME_FLOW	m_preFlow;
};
#endif
// FLOW_MANAGER