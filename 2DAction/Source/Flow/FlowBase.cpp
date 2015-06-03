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
#include "Process/FlowProcessBase.h"
#include "Common/Utility/CommonGameUtility.h"
#include "System/SystemFadeManager.h"
#include "System/SystemBgManager.h"
#include "System/Message/SystemMessageManager.h"
#include "System/Collision/SystemCollisionManager.h"


FlowBase::FlowBase(std::string fileName)
: m_filePath(fileName)
, m_isInvalidPadCtrl( false )
{
	LoadFlowFile();
}

FlowBase::~FlowBase(void)
{
}

/* ================================================ */
/**
 * @brief	���̉�ʂ�
 */
/* ================================================ */
bool FlowBase::StartFade(const char* eventStr)
{
	for(uint32_t i = 0; i < m_vEventName.size(); ++i){
		if(m_vEventName.at(i).eventStr.compare(eventStr) == 0){
			bool changeResult = FlowManager::GetInstance()->ChangeFlow(m_vEventName.at(i).filePath.c_str());
			if( changeResult ){
				m_isInvalidPadCtrl = true;
			}
			return changeResult;
		}
	}
	DEBUG_ASSERT( 0, "event�����Ȃ�");
	return false;
}

/* ============================================== */
/**
 * @brief	�I������
 */
/* ================================================ */
bool FlowBase::Finish()
{
	// �q�^�X�N�̏I������
	if( !ReleaseAllEnd() ){
		return false;
	}

	// �X�e�[�W�G�t�F�N�g�I������
	auto it = m_vStageEffect.begin();
	while( it != m_vStageEffect.end() ){
		if( (*it)->CanDie() ){
			ProcessBase *tmp = *it;
			it = m_vStageEffect.erase(it);
			SAFE_DELETE(tmp);
		}
		if( it != m_vStageEffect.end() ){
			++it;
		}
	}
	if( !m_vStageEffect.empty() ){
		return false;
	}

	//! �h����̏I���m�F
	return FinishFlow();
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

	//BG�ύX����
	picojson::value bgData = root.get("bgdata");
	std::string bgStr = bgData.get(0).get("bgid").get<std::string>();
	if( BgManager::GetInstance() ){
		BgManager::GetInstance()->SetNextBg( bgStr );
	}
}

/* ============================================== */
/**
 * @brief	�q�^�X�N�X�V����
 */
/* ================================================ */
void FlowBase::UpdateFlow()
{
	// �h����̎q�^�X�N�X�V�OUpdate
	UpdateFlowPreChildTask();

	if( !m_isInvalidPadCtrl ){
		CallPadEvent();	// �p�b�h���͎擾
	}

	// �q�̍X�V
	ChildUpdate();

	// �h����̎q�^�X�N�X�V��Update
	UpdateFlowAfterChildTask();
}


void FlowBase::PadEventStart()
{
	if( Utility::IsGamePause() ){
		Utility::EndGamePause();
	}
	else{
		Utility::StartGamePause();
	}
};

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
 * @brief	�q�^�X�N�X�V����
 */
/* ================================================ */
void FlowBase::ChildUpdate()
{
	if( Utility::IsGamePause() ){
		// �|�[�Y��
	}
	else if( m_vStageEffect.size() != 0
		&& FadeManager::GetInstance()->GetCurrentState() == FadeManager::STATE_IDLE ){
		// �t���[���o��
		auto it = m_vStageEffect.begin();
		if( (*it)->IsEffectEnd() ){
			ProcessBase *tmp = *it;
			m_vStageEffect.erase( it );
			SAFE_DELETE(tmp);
		}
		else{
			(*it)->Exec();
		}
	}
	else{
		Exec();				//! �ʒu���̍X�V
		CollisionManager::GetInstance()->CollisionUpdate();			// �Փ˔���X�V+�e�N���X�ɃC�x���g���s	
		SystemMessageManager::GetInstance()->StartMessageEvent();	// �e�N���X�̑��݃C�x���g�������s��
	}
	DrawUpdate();		//! �`�擙�̍X�V
}