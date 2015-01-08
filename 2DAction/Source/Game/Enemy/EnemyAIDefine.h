
/* ====================================================================== */
/**
 * @brief  �GAI����AI�{�̂Ɏ󂯓n����������\����
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef _ENEMY_AI_DEFINE_
#define _ENEMY_AI_DEFINE_

#include "Common/CommonDefine.h"

// �i�[���Ă�����̃^�C�v
enum AI_TYPE{
	
	AI_SHOOT_BULLET,

	AI_MAX,
};

// �ȉ��e�s���ɕK�v�ȏ����i�[�����\����
struct AI_INFO_SHOOT{
	
	uint32_t m_tmp;

	void Init(){
		m_tmp = 0;
	}
};

// ���i�[���L��
union AI_INFO{

	AI_INFO_SHOOT m_infoShoot;

	void Init(){
		m_infoShoot.Init();
	}
};

// �ϐ��Ƃ��ď��L����\����
struct ACTION_INFO{

	AI_TYPE m_AItype;	// ���݂�m_AIInfo�̃^�C�v
	AI_INFO	m_AIInfo;	// �s���ɕK�v�ȏ��
	std::vector<Common::EVENT_MESSAGE>	m_pushEventArray;	// Event�Ƃ��ĕK�v��Message���i�[

	void Init(){
		m_AItype = AI_MAX;
		m_AIInfo.Init();
		m_pushEventArray.clear();
	}
};


#endif
