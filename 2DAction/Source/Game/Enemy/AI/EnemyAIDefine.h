
/* ====================================================================== */
/**
 * @brief  敵AIからAI本体に受け渡しをする情報構造体
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_AI_DEFINE__
#define __ENEMY_AI_DEFINE__

#include "Common/CommonDefine.h"

// 格納している情報のタイプ
enum AI_TYPE{
	
	AI_SHOOT_BULLET,

	AI_MAX,
};

// 以下各行動に必要な情報を格納した構造体

// 攻撃弾発射情報構造体
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

// 情報格納共有体
union AI_INFO{

	AI_INFO_SHOOT m_infoShoot;

	void Init(){
		m_infoShoot.Init();
	}
};

// AIにPushされる行動ひとつの構造体
struct ACTION_INFO{

	AI_TYPE			m_AItype;	// 現在のm_AIInfoのタイプ
	AI_INFO			m_AIInfo;	// 行動に必要な情報
	Common::EVENT_MESSAGE	m_EventMessage;	// Eventとして必要なMessageを格納

	ACTION_INFO::ACTION_INFO(){
		Init();
	}

	void Init(){
		m_AItype		= AI_MAX;
		m_AIInfo.Init();
		m_EventMessage	= Common::EVENT_MESSAGE_MAX;
	}
};

// 行動配列
struct ACTION_ARRAY{

	std::vector<ACTION_INFO>	m_pushEventArray;	// Eventとして必要なMessageを格納

	void Init(){
		m_pushEventArray.clear();
	}
};


#endif
