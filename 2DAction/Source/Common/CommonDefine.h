
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

		CMN_EVENT_MAX,
	};
	struct CMN_EVENT{
		EVENT_MESSAGE	m_event;		// 何が起こったか？
		uint32_t		m_eventValue;	// 汎用番号
	};

	enum ENEMY_KIND{
		KIND_AAA,
		KIND_BBB,
		KIND_CCC,
		KIND_DDD,

		ENEMY_MAX,
	};

}
#endif