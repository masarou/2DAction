
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
		TYPE_ITEM_BATTLE_POINT,
		TYPE_BLADE_PLAYER,
		TYPE_BLADE_ENEMY,
		TYPE_BULLET_PLAYER,
		TYPE_BULLET_ENEMY,
		
		TYPE_EXPLOSION_PLAYER,
		TYPE_EXPLOSION_ENEMY,

		TYPE_MAX,
	};

	// オブジェクト同士のイベントメッセージ
	enum EVENT_MESSAGE{
		EVENT_HIT_PLAYER,		// プレイヤーキャラクタ
		EVENT_HIT_ENEMY_AAA,	// 敵との接触
		EVENT_HIT_ENEMY_BBB,	// 敵との接触
		EVENT_HIT_ENEMY_CCC,	// 敵との接触

		EVENT_GET_ITEM_BULLET,		// アイテム取得
		EVENT_GET_ITEM_LIFE,		// アイテム取得
		EVENT_GET_ITEM_DAMAGE,		// アイテム取得
		EVENT_GET_ITEM_BATTLE_POINT,// アイテム取得
		EVENT_HIT_BLADE_PLAYER,		// プレイヤーの斬撃に当たった
		EVENT_HIT_BLADE_ENEMY,		// 敵の斬撃に当たった
		EVENT_HIT_BULLET_PLAYER,	// プレイヤーの攻撃弾に当たった
		EVENT_HIT_BULLET_ENEMY,		// 敵の攻撃弾に当たった
		EVENT_HIT_EXPLOSION_PLAYER,	// プレイヤーの爆発攻撃に当たった
		EVENT_HIT_EXPLOSION_ENEMY,	// 敵の爆発攻撃に当たった

		// その他

		EVENT_MESSAGE_MAX,
	};

	// ゲームがなに状態かを示す
	enum GAME_FLOW{
		FLOW_TITLE,			// タイトル
		FLOW_SCORE,			// スコア
		FLOW_POWER_UP,		// ユーザーパワーアップ
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

	// 画面クラス内のパーツ種類
	enum PARTS_TYPE_MENU{
		PARTS_SINGLE_DRAW,	// アニメ情報をもつ程度のシンプルなパーツ
		PARTS_NUM_COUNTER,	// 与えられた数値を描画し続けるパーツ

		PARTS_MAX,
	};

	// 画面クラスで使用するパーツの情報構造体
	struct PARTS_INFO{
		std::string		m_jsonStr;
		math::Vector2	m_pos;
		PARTS_TYPE_MENU	m_type;
		void Init(){
			m_jsonStr = "";
			m_pos = math::Vector2(0,0);
			m_type = PARTS_SINGLE_DRAW;
		}
	};

	enum ENEMY_KIND{
		ENEMY_KIND_AAA,
		ENEMY_KIND_BBB,
		ENEMY_KIND_CCC,
		ENEMY_KIND_BOSS,

		ENEMY_KIND_MAX,
	};

	enum ITEM_KIND{
		ITEM_KIND_RAPID_BULLET,	// 連射速度UP
		ITEM_KIND_LIFE_UP,		// ライフ回復
		ITEM_KIND_DAMAGE_UP,	// 弾の威力UP
		ITEM_KIND_BATTLE_POINT,	// ベースステータスLvをあげるポイント

		ITEM_KIND_MAX,
	};

	enum ENEMY_AI{
		AI_NONE,		// 何もしない
		AI_SEARCHING,	// プレイヤーを探している
		AI_MOVE_PLAYER,	// プレイヤーに近づく(体当たり攻撃)
		AI_SHOOTING,	// 遠距離攻撃(遠距離攻撃)

		// 以下、ボス用AI
		AI_MOVING_WAYPOINT,	// 目的地に移動
		AI_ATTACK_NEAR,		// 斬撃(近距離攻撃)
		AI_ATTACK_MIDDLE,	// 弾発射(中距離攻撃)
		AI_ATTACK_FAR,		// 爆発魔法(遠距離攻撃)
		AI_CREATE_ENEMY,	// 敵キャラ生成

		AI_MAX,
	};
	
	// 所有者
	enum OWNER_TYPE{
		OWNER_PLAYER,
		OWNER_ENEMY,

		OWNER_MAX,
	};

	// バトルポイントを使用してあげることのできるプレイヤーステータス
	enum PLAYER_BASE_STATE{
		BASE_STATE_LIFE,		// ライフの最大値を決める
		BASE_STATE_MOVE_SPEED,	// ダッシュ時間
		BASE_STATE_DEFFENCE,	// 被ダメージを決める
		BASE_STATE_BULLET_SPD,	// マシンガンの間隔
		BASE_STATE_BULLET_DMG,	// マシンガンのダメージ
		BASE_STATE_BLADE_LEVEL,	// 斬撃のダメージ

		BASE_STATE_MAX,
	};

	// 以下、セーブデータとして保持する
	const uint32_t RANKING_RECORD_MAX = 5;
	struct SAVE_DATA{
		uint32_t m_battlePoint;	// ゲームプレイで手にいてたポイント(パワーアップに必要)
		uint32_t m_scoreRanking[RANKING_RECORD_MAX];

		uint32_t m_playerBaseStateLv[BASE_STATE_MAX];
	};
}
#endif