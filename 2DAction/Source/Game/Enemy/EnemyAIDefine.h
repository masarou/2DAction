
/* ====================================================================== */
/**
 * @brief  敵AIからAI本体に受け渡しをする情報構造体
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef _ENEMY_AI_DEFINE_
#define _ENEMY_AI_DEFINE_

#include "Common/CommonDefine.h"

// 格納している情報のタイプ
enum AI_TYPE{
	
	AI_SHOOT_BULLET,

	AI_MAX,
};

// 以下各行動に必要な情報を格納した構造体
struct AI_INFO_SHOOT{
	
	uint32_t m_tmp;

	void Init(){
		m_tmp = 0;
	}
};

// 情報格納共有体
union AI_INFO{

	AI_INFO_SHOOT m_infoShoot;

	void Init(){
		m_infoShoot.Init();
	}
};

// 変数として所有する構造体
struct ACTION_INFO{

	AI_TYPE m_AItype;	// 現在のm_AIInfoのタイプ
	AI_INFO	m_AIInfo;	// 行動に必要な情報
	std::vector<Common::EVENT_MESSAGE>	m_pushEventArray;	// Eventとして必要なMessageを格納

	void Init(){
		m_AItype = AI_MAX;
		m_AIInfo.Init();
		m_pushEventArray.clear();
	}
};


#endif
