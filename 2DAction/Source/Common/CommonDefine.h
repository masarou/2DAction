
#ifndef _COMMON_DEFINE_
#define _COMMON_DEFINE_

#include "System/SystemDefine.h"

namespace Common{

	enum EVENT_MESSAGE{
		// プレイヤーキャラクタ
		EVENT_HIT_ENEMY,	// 敵との接触
		EVENT_GET_ITEM,		// アイテム取得

		// 敵キャラクター
		EVENT_HIT_BULLET,	// 攻撃弾に当たった

		// その他

		EVENT_MESSAGE_MAX,
	};
	struct CMN_EVENT{
		EVENT_MESSAGE	m_event;		// 何が起こったか？
		uint32_t		m_eventValue;	// 汎用番号
		uint32_t		m_delayTime;	// 実際に実行されるタイミング(0=すぐに実行)
		void			*m_exInfo;		// 他に必要な情報があれば
		void Init(){
			m_event			= EVENT_MESSAGE_MAX;
			m_eventValue	= INVALID_VALUE;
			m_delayTime		= 0;
			m_exInfo		= NULL;
		}
	};

	enum ENEMY_KIND{
		KIND_AAA,
		KIND_BBB,
		KIND_CCC,
		KIND_DDD,

		KIND_MAX,
	};


	enum ENEMY_AI{
		AI_NONE,		// 何もしない
		AI_SEARCHING,	// プレイヤーを探している
		AI_MOVE_PLAYER,	// プレイヤーに近づく(体当たり攻撃)
		AI_SHOOTING,	// 遠距離攻撃(遠距離攻撃)

		AI_MAX,
	};


	const uint32_t RANKING_RECORD_MAX = 5;
	struct SAVE_SCORE{
		uint32_t m_scoreTimeAttack[RANKING_RECORD_MAX];
		uint32_t m_scoreEndless[RANKING_RECORD_MAX];
	};
}
#endif