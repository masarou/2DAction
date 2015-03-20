
#ifndef __COMMON_DEFINE__
#define __COMMON_DEFINE__

#include "System/SystemDefine.h"

namespace Common{

	// 当たり判定が絡んでくる表示物
	enum TYPE_OBJECT{
		TYPE_PLAYER,
		TYPE_EVENMY_AAA,
		TYPE_EVENMY_BBB,
		TYPE_EVENMY_CCC,

		TYPE_ITEM_BULLET,
		TYPE_ITEM_LIFE,
		TYPE_ITEM_DAMAGE,
		TYPE_BLADE_PLAYER,
		TYPE_BLADE_ENEMY,
		TYPE_BULLET_PLAYER,
		TYPE_BULLET_ENEMY,

		TYPE_MAX,
	};

	enum EVENT_MESSAGE{
		EVENT_HIT_PLAYER,		// プレイヤーキャラクタ
		EVENT_HIT_ENEMY_AAA,	// 敵との接触
		EVENT_HIT_ENEMY_BBB,	// 敵との接触
		EVENT_HIT_ENEMY_CCC,	// 敵との接触

		EVENT_GET_ITEM_BULLET,	// アイテム取得
		EVENT_GET_ITEM_LIFE,	// アイテム取得
		EVENT_GET_ITEM_DAMAGE,	// アイテム取得
		EVENT_HIT_BLADE_PLAYER,	// プレイヤーの斬撃に当たった
		EVENT_HIT_BLADE_ENEMY,	// 敵の斬撃に当たった
		EVENT_HIT_BULLET_PLAYER,// プレイヤーの攻撃弾に当たった
		EVENT_HIT_BULLET_ENEMY,	// 敵の攻撃弾に当たった
		EVENT_SHOOT_BULLET,		// 攻撃弾を発射

		// その他

		EVENT_MESSAGE_MAX,
	};

	// ゲームがなに状態かを示す
	enum GAME_FLOW{
		FLOW_TITLE,			// タイトル
		FLOW_SCORE,			// スコア
		FLOW_STAGE01,		// ステージ01
		FLOW_STAGE02,		// ステージ02
		FLOW_STAGE03,		// ステージ03
		FLOW_RESULT,		// リザルト
		FLOW_INTERVAL,		// 休憩
		FLOW_RESULT_TOTAL,	// すべてのステージを終えてのリザルト
		FLOW_EXIT,			// ゲーム終了

		FLOW_MAX
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

	// 画面クラスで使用するパーツの情報構造体
	struct PARTS_INFO{
		math::Vector2 m_pos;
		void Init(){
			m_pos = math::Vector2(0,0);
		}
	};

	enum ENEMY_KIND{
		ENEMY_KIND_AAA,
		ENEMY_KIND_BBB,
		ENEMY_KIND_CCC,
		ENEMY_KIND_DDD,

		ENEMY_KIND_MAX,
	};


	enum ENEMY_AI{
		AI_NONE,		// 何もしない
		AI_SEARCHING,	// プレイヤーを探している
		AI_MOVE_PLAYER,	// プレイヤーに近づく(体当たり攻撃)
		AI_SHOOTING,	// 遠距離攻撃(遠距離攻撃)

		AI_MAX,
	};
	
	// 所有者
	enum OWNER_TYPE{
		OWNER_PLAYER,
		OWNER_ENEMY,

		OWNER_MAX,
	};

	const uint32_t RANKING_RECORD_MAX = 5;
	struct SAVE_SCORE{
		uint32_t m_scoreTimeAttack[RANKING_RECORD_MAX];
		uint32_t m_scoreEndless[RANKING_RECORD_MAX];
	};
}
#endif