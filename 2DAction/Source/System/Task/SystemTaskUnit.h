/* ====================================================================== */
/**
 * @brief  �e�N���X�̍ŏ��P�ʂ̃N���X
 *
 * @note
 *		��{�I�ɂ͂��̃N���X��h�����������̂��g�p����B
 *		m_status�ɂ���Ď����I��update���Ă΂ꂽ��delete���ꂽ�肷��B
 *		setStatus��TASK_PRE_DIE�ɂ��Ă��Ǝ����I�ɍ폜�����B
 *		�֐�����
 *			Init()		: �ŏ��ɕK���Ă΂��Atrue��Ԃ��܂ŌĂ΂ꑱ����
 *			Update()	: ���t���[���Ă΂��A�X�V��������
 *			DrawUpdate(): ���t���[���Ă΂��A�`�擙��
 *			DieMain()	: m_status��UNIT_DEAD�ɂ�����Ă΂��B
 *						  true��Ԃ��܂ŌĂ΂ꑱ���A���̌�N���X����������
 *
 *		SetChildUnit()�Őe�q�t������TaskUnit�p���N���X�͐e�����ʂƎ����I�Ɏ��ɂ܂�
 */
/* ====================================================================== */

#ifndef __SYSTEM_TASK_UNIT__
#define __SYSTEM_TASK_UNIT__

#include <string>
#include <vector>
#include "System/SystemDefine.h"
#include "Common/CommonDefine.h"
#include "System/SystemMessage.h"

class TaskManager;

class TaskUnit
{
	friend class TaskManagerBase;
	friend class FlowBase;

public:

	//! �^�X�N�̃X�e�[�^�X
	enum TASK_STATUS{
		TASK_IDLE,		//!< �������Ȃ�(�|�[�Y��)
		TASK_INIT,		//!< ������
		TASK_ALIVE,		//!< ���ʂ̏��
		TASK_PRE_DIE,	//!< CanDie���Ă΂��O
		TASK_DIE,		//!< CanDie���Ă΂ꂽ��

		TASK_MAX
	};

	const std::string &GetName(){return m_name;}
	const TASK_STATUS &GetStatus() const;

	// ��������ł���or���ꂩ�玀�ʂ��ǂ���
	bool IsDie();

	//! ��{�I�Ɏg�p�֎~
	void SetDieStateFromParent( TaskUnit *pParent );

protected:

	TaskUnit( std::string name = "No Name", TASK_STATUS status = TASK_INIT );
	virtual ~TaskUnit(void);

	virtual bool Init(){return true;}
	virtual void Update(){};			// �ړ����̓������l�̍X�V
	virtual void DrawUpdate(){};		// �`��X�V
	virtual bool DieMain(){return true;}

	// �h����ł̃��b�Z�[�W����
	virtual void MessageReceive( const Message &msg ){}

	// ���ʎ��͂�����Ă�ŉ�����
	void TaskStartDie();

	//! �e�N���X�̓o�^
	void AddConnectionParent( TaskUnit *unit );

	//! �q�N���X�̓o�^
	void SetChildUnit( TaskUnit *parent );

	//! �e�q�֌W�̉���
	void ReleaseConnection( TaskUnit *unit );

	//! �e�Ƀ��b�Z�[�W�𓊂���
	void SendMessageToParent( const Message &msg );

private:

	//! �X�e�[�^�X�ύX
	void SetStatus( const TASK_STATUS &status );

	//! �N���X�̏��
	std::string m_name;
	TASK_STATUS m_status;

	//! �e�N���X
	TaskUnit *m_pParent;

	//! �q�N���X
	std::vector<TaskUnit*> m_pChildVec;

};

#endif //SYSTEM_TASK_UNIT
