
/* ====================================================================== */
/**
 * @brief  �GAI����AI�{�̂Ɏ󂯓n����������\����
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_DEFINE__
#define __ENEMY_AI_DEFINE__

#include "Common/CommonDefine.h"

// �i�[���Ă�����̃^�C�v
enum AI_TYPE{
	
	AI_SHOOT_BULLET,

	AI_MAX,
};

// �ȉ��e�s���ɕK�v�ȏ����i�[�����\����

// �U���e���ˏ��\����
struct AI_INFO_SHOOT{
	
	float			vecX;
	float			vecY;
	uint32_t		speed;
	uint32_t		damage;

	void Init(){
		vecX	= 0.0f;
		vecY	= 0.0f;
		speed	= 0;
		damage	= 0;
	}
};

// ���i�[���L��
union AI_INFO{

	AI_INFO_SHOOT m_infoShoot;

	void Init(){
		m_infoShoot.Init();
	}
};

// AI��Push�����s���ЂƂ̍\����
struct ACTION_INFO{

	AI_TYPE			m_AItype;	// ���݂�m_AIInfo�̃^�C�v
	AI_INFO			m_AIInfo;	// �s���ɕK�v�ȏ��
	Common::EVENT_MESSAGE	m_EventMessage;	// Event�Ƃ��ĕK�v��Message���i�[

	ACTION_INFO::ACTION_INFO(){
		Init();
	}

	void Init(){
		m_AItype		= AI_MAX;
		m_AIInfo.Init();
		m_EventMessage	= Common::EVENT_MESSAGE_MAX;
	}
};

// �s���z��
struct ACTION_ARRAY{

	std::vector<ACTION_INFO>	m_pushEventArray;	// Event�Ƃ��ĕK�v��Message���i�[

	void Init(){
		m_pushEventArray.clear();
	}
};


#endif
