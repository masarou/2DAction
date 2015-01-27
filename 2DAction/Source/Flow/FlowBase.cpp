/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�t���[�̋��ʍ��ځAjson�ǂݎ�蓙�̋@�\�����������N���X
 */
/* ====================================================================== */

#include "System/picojson.h"
#include "FlowBase.h"
#include "FlowManager.h"
#include "System/Message/SystemMessageManager.h"
#include "System/Collision/SystemCollisionManager.h"

FlowBase::FlowBase(std::string fileName)
	: m_filePath(fileName)
{
	LoadFlowFile();
}

FlowBase::~FlowBase(void)
{
}

/* ============================================== */
/**
 * @brief	�I������
 */
/* ================================================ */
bool FlowBase::Finish()
{
	if(ReleaseAllEnd()){
		return true;
	}
	//! �܂��q�^�X�N�̏I���҂�
	return false;
}


/* ============================================== */
/**
 * @brief	�q�^�X�N�X�V����
 */
/* ================================================ */
void FlowBase::UpdateFlow()
{
	// �p�b�h���͎擾
	CallPadEvent();

	// �q�̍X�V
	ChildUpdate();
}

/* ============================================== */
/**
 * @brief	�q�^�X�N�X�V����
 */
/* ================================================ */
void FlowBase::ChildUpdate()
{
	Exec();				//! �ʒu���̍X�V
	CollisionManager::GetInstance()->CollisionUpdate();			// �Փ˔���X�V+�e�N���X�ɃC�x���g���s	
	SystemMessageManager::GetInstance()->StartMessageEvent();	// �e�N���X�̑��݃C�x���g�������s��
	DrawUpdate();		//! �`�擙�̍X�V
}

/* ============================================== */
/**
 * @brief	�q�^�X�N�ǉ�
 */
/* ================================================ */
void FlowBase::AddChildTask(TaskUnit *pTask)
{
	if(pTask){
		m_vTaskUnit.push_back(pTask);
	}
}

/* ============================================== */
/**
 * @brief	json�t�@�C���̓ǂݍ���
 */
/* ================================================ */
void FlowBase::LoadFlowFile()
{
	std::ifstream ifs(m_filePath.c_str());

	picojson::value root;
	picojson::parse( root, ifs);
	
	picojson::value sceneData = root.get("eventdata");
	for(uint32_t i = 0;; ++i){
		picojson::value null;
		if(sceneData.get(i) == null){
			break;
		}
		FLOW_DATA data;
		data.eventStr = sceneData.get(i).get("event").get<std::string>();
		data.filePath = sceneData.get(i).get("path").get<std::string>();
		m_vEventName.push_back(data);
	}

	if(m_vEventName.empty()){
		DEBUG_ASSERT( 0, "m_vEventName is empty!!");
	}

}

/* ================================================ */
/**
 * @brief	���̉�ʂ�
 */
/* ================================================ */
void FlowBase::StartFade(const char* eventStr)
{
	for(uint32_t i = 0; i < m_vEventName.size(); ++i){
		if(m_vEventName.at(i).eventStr.compare(eventStr) == 0){
			FlowManager::GetInstance()->ChangeFlow(m_vEventName.at(i).filePath.c_str());
			return;
		}
	}
	DEBUG_ASSERT( 0, "event�����Ȃ�");
}
