
#ifndef __COMMON_DEFINE__
#define __COMMON_DEFINE__

#include "System/SystemDefine.h"

namespace Common{

	// 当たり判定が絡んでくる表示物
	enum TYPE_OBJECT{
		TYPE_PLAYER,
		TYPE_ENEMY_SLIME,
		TYPE_ENEMY_SLIME_ANOTHER,
		TYPE_ENEMY_AHRIMAN,
		TYPE_ENEMY_COW,
		TYPE_ENEMY_RANGER,
		TYPE_ENEMY_SLIME_KING,
		TYPE_ENEMY_WIZARD,
		TYPE_WIZARD_CRYSTAL,
		TYPE_DRAGON,
		TYPE_LAST_BOSS,
		TYPE_LAST_BOSS_LEFT,
		TYPE_LAST_BOSS_RIGHT,
		TYPE_ITEM_BULLET,
		TYPE_ITEM_LIFE,
		TYPE_ITEM_DAMAGE,
		TYPE_ITEM_ANTIDOTE,
		TYPE_BLADE_PLAYER,
		TYPE_BLADE_ENEMY,
		TYPE_BULLET_PLAYER,
		TYPE_BULLET_ENEMY,
		TYPE_FIRE,
		TYPE_FIRE_BALL,
		TYPE_POISON,
		TYPE_EXPLOSION_PLAYER,
		TYPE_EXPLOSION_ENEMY,

		TYPE_MAX,
	};

	// オブジェクト同士のイベントメッセージ
	enum EVENT_MESSAGE{
		EVENT_HIT_PLAYER,				// プレイヤーキャラクタ
		EVENT_HIT_ENEMY_SLIME,			// 敵との接触
		EVENT_HIT_ENEMY_SLIME_ANOTHER,	// 敵との接触
		EVENT_HIT_ENEMY_AHRIMAN,		// 敵との接触
		EVENT_HIT_ENEMY_COW,			// 敵との接触
		EVENT_HIT_ENEMY_BOSS,			// 敵との接触
		EVENT_HIT_ENEMY_SLIME_KING,		// 敵との接触
		EVENT_HIT_ENEMY_WIZARD,			// 敵との接触
		EVENT_HIT_WIZARD_CRYSTAL,		// 敵との接触
		EVENT_HIT_DRAGON,				// 敵との接触
		EVENT_HIT_BOSS,					// 敵との接触
		EVENT_HIT_BOSS_LEFT,			// 敵との接触
		EVENT_HIT_BOSS_RIGHT,			// 敵との接触
		EVENT_GET_ITEM_BULLET,			// アイテム取得
		EVENT_GET_ITEM_LIFE,			// アイテム取得
		EVENT_GET_ITEM_DAMAGE,			// アイテム取得
		EVENT_GET_ITEM_ANTIDOTE,		// アイテム取得
		EVENT_HIT_BLADE_PLAYER,			// プレイヤーの斬撃に当たった
		EVENT_HIT_BLADE_ENEMY,			// 敵の斬撃に当たった
		EVENT_HIT_BULLET_PLAYER,		// プレイヤーの攻撃弾に当たった
		EVENT_HIT_BULLET_ENEMY,			// 敵の攻撃弾に当たった
		EVENT_HIT_FIRE,					// 火炎弾後の延焼
		EVENT_HIT_FIRE_BALL,			// 火炎弾
		EVENT_HIT_POISON,				// 毒
		EVENT_HIT_EXPLOSION_PLAYER,		// プレイヤーの爆発攻撃に当たった
		EVENT_HIT_EXPLOSION_ENEMY,		// 敵の爆発攻撃に当たった

		EVENT_ADD_FORCE_MOVE,		// ダメージ等でのふっとび

		EVENT_MESSAGE_MAX,
	};

	// 当たり判定オブジェクトとそれによって発生するイベントをまとめたもの
	struct COLLISION_OBJ
	{
		TYPE_OBJECT		m_type;
		EVENT_MESSAGE	m_eventMessage;
	} static const s_collisionInfo[ TYPE_MAX ] = {
		{	TYPE_PLAYER,				EVENT_HIT_PLAYER				},	// プレイヤーキャラクタ
		{	TYPE_ENEMY_SLIME,			EVENT_HIT_ENEMY_SLIME			},	// 敵との接触
		{	TYPE_ENEMY_SLIME_ANOTHER,	EVENT_HIT_ENEMY_SLIME_ANOTHER	},	// 敵との接触
		{	TYPE_ENEMY_AHRIMAN,			EVENT_HIT_ENEMY_AHRIMAN			},	// 敵との接触
		{	TYPE_ENEMY_COW,				EVENT_HIT_ENEMY_COW				},	// 敵との接触
		{	TYPE_ENEMY_RANGER,			EVENT_HIT_ENEMY_BOSS			},	// 敵との接触
		{	TYPE_ENEMY_SLIME_KING,		EVENT_HIT_ENEMY_SLIME_KING		},	// 敵との接触
		{	TYPE_ENEMY_WIZARD,			EVENT_HIT_ENEMY_WIZARD			},	// 敵との接触
		{	TYPE_WIZARD_CRYSTAL,		EVENT_HIT_WIZARD_CRYSTAL		},	// 敵との接触
		{	TYPE_DRAGON,				EVENT_HIT_DRAGON				},	// 敵との接触
		{	TYPE_LAST_BOSS,				EVENT_HIT_BOSS					},	// 敵との接触
		{	TYPE_LAST_BOSS_LEFT,		EVENT_HIT_BOSS_LEFT				},	// 敵との接触
		{	TYPE_LAST_BOSS_RIGHT,		EVENT_HIT_BOSS_RIGHT			},	// 敵との接触
		{	TYPE_ITEM_BULLET,			EVENT_GET_ITEM_BULLET			},	// アイテム取得
		{	TYPE_ITEM_LIFE,				EVENT_GET_ITEM_LIFE				},	// アイテム取得
		{	TYPE_ITEM_DAMAGE,			EVENT_GET_ITEM_DAMAGE			},	// アイテム取得
		{	TYPE_ITEM_ANTIDOTE,			EVENT_GET_ITEM_ANTIDOTE			},	// アイテム取得
		{	TYPE_BLADE_PLAYER,			EVENT_HIT_BLADE_PLAYER			},	// プレイヤーの斬撃に当たった
		{	TYPE_BLADE_ENEMY,			EVENT_HIT_BLADE_ENEMY			},	// 敵の斬撃に当たった
		{	TYPE_BULLET_PLAYER,			EVENT_HIT_BULLET_PLAYER			},	// プレイヤーの攻撃弾に当たった
		{	TYPE_BULLET_ENEMY,			EVENT_HIT_BULLET_ENEMY			},	// 敵の攻撃弾に当たった
		{	TYPE_FIRE,					EVENT_HIT_FIRE					},	// 火炎弾後の延焼
		{	TYPE_POISON,				EVENT_HIT_POISON				},	// 毒
		{	TYPE_FIRE_BALL,				EVENT_HIT_FIRE_BALL				},	// 火炎弾
		{	TYPE_EXPLOSION_PLAYER,		EVENT_HIT_EXPLOSION_PLAYER		},	// プレイヤーの爆発攻撃に当たった
		{	TYPE_EXPLOSION_ENEMY,		EVENT_HIT_EXPLOSION_ENEMY		},	// 敵の爆発攻撃に当たった
	};

	// ゲームがなに状態かを示す
	enum GAME_FLOW{
		FLOW_TITLE,			// タイトル
		FLOW_SCORE,			// スコア
		FLOW_POWER_UP,		// ユーザーパワーアップ
		FLOW_STAGE_SEL,		// 開始ステージ選択
		FLOW_STAGE01,		// ステージ01
		FLOW_STAGE02,		// ステージ02
		FLOW_STAGE03,		// ステージ03
		FLOW_STAGE04,		// ステージ04
		FLOW_STAGE05,		// ステージ05
		FLOW_STAGE06,		// ステージ06
		FLOW_STAGE07,		// ステージ07
		FLOW_STAGE08,		// ステージ08
		FLOW_STAGE09,		// ステージ09
		FLOW_STAGE10,		// ステージ10
		FLOW_RESULT,		// リザルト
		FLOW_INTERVAL,		// 休憩
		FLOW_RESULT_TOTAL,	// すべてのステージを終えてのリザルト
		FLOW_GAME_CLEAR,	// ゲーム終了
		FLOW_EXIT,			// ゲーム終了

		FLOW_MAX
	};

	//描画の種類 優先
	enum PRIORITY{
		PRIORITY_LOWEST,
		PRIORITY_LOW,
		PRIORITY_BELOW_NORMAL,	// 標準以下
		PRIORITY_NORMAL,		// 標準
		PRIORITY_ABOVE_NORMAL,	// 標準以上
		PRIORITY_HIGH,
		PRIORITY_HIGHEST,
		PRIORITY_SCENE_FADE,
	
		PRIORITY_MAX,
	};

	enum EXTRA_KIND{
		KIND_ENEMY_POS,
		KIND_FORCE_MOVE,

		KIND_MAX,
	};

	struct EX_COMMON{
		float	m_posX;			// 相手のオブジェクトの位置
		float	m_posY;			// 相手のオブジェクトの位置
		void Init(){
			m_posX = 0.0f;
			m_posY = 0.0f;
		}
	};

	struct EX_FORCE_MOVE{
		float	m_posX;			// 強制移動の方向
		float	m_posY;			// 強制移動の方向
		float	m_forcePower;	// 強制移動の大きさ
		void Init(){
			m_posX = 0.0f;
			m_posY = 0.0f;
			m_forcePower = 0.0f;
		}
	};

	union EXTRA_INFO{
		EX_COMMON		m_common;
		EX_FORCE_MOVE	m_forceMove;
	};

	struct CMN_EVENT{
	public:
		EVENT_MESSAGE	m_event;		// 何が起こったか？
		uint32_t		m_eventValue;	// 汎用番号
		uint32_t		m_delayTime;	// 実際に実行されるタイミング(0=すぐに実行)
		void Init(){
			m_event			= EVENT_MESSAGE_MAX;
			m_eventValue	= 0;
			m_delayTime		= 0;
			m_exkind		= KIND_MAX;
		}

		const EXTRA_KIND &GetExInfoKind() const{ return m_exkind; }

		void SetExInfoCmn( const EX_COMMON &info ){
			m_exkind = KIND_ENEMY_POS;
			m_exInfo.m_common = info;
		}
		void SetExInfoForceMove( const EX_FORCE_MOVE &info ){
			m_exkind = KIND_FORCE_MOVE;
			m_exInfo.m_forceMove = info;
		}

		const EX_COMMON &GetExInfoCmn() const{
			if( m_exkind == KIND_ENEMY_POS ){
				return m_exInfo.m_common;
			}
			DEBUG_ASSERT( 0, "共有体の実体と期待している型が異なります" );
			return m_exInfo.m_common;
		}
		const EX_FORCE_MOVE &GetExInfoForceMove() const{
			if( m_exkind == KIND_FORCE_MOVE ){
				return m_exInfo.m_forceMove;
			}
			DEBUG_ASSERT( 0, "共有体の実体と期待している型が異なります" );
			return m_exInfo.m_forceMove;
		}
	private:
		EXTRA_KIND		m_exkind;
		EXTRA_INFO		m_exInfo;		// その他情報
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
		Common::PRIORITY		m_priority;
		math::Vector2	m_pos;
		PARTS_TYPE_MENU	m_type;
		void Init(){
			m_jsonStr = "";
			m_pos = math::Vector2(0,0);
			m_priority = PRIORITY_NORMAL;
			m_type = PARTS_SINGLE_DRAW;
		}
	};

	enum ENEMY_KIND{
		ENEMY_KIND_SLIME,
		ENEMY_KIND_SLIME_ANOTHER,
		ENEMY_KIND_AHRIMAN,
		ENEMY_KIND_COW,
		ENEMY_KIND_RANGER,
		ENEMY_KIND_SLIME_KING,
		ENEMY_KIND_WIZARD,
		ENEMY_KIND_DRAGON,
		ENEMY_KIND_LAST_BOSS,

		ENEMY_KIND_MAX,
	};

	// ユニークモンスター配列(基本的に一体ずつしかでない)
	static ENEMY_KIND s_uniqueEnemyKind[] = {
		Common::ENEMY_KIND_WIZARD,
		Common::ENEMY_KIND_DRAGON,
		Common::ENEMY_KIND_RANGER,
	};
	
	// ライトモンスター配列(複数体存在することあり)
	static ENEMY_KIND s_lightEnemyKind[] = {
		Common::ENEMY_KIND_SLIME,
		Common::ENEMY_KIND_SLIME_ANOTHER,
		Common::ENEMY_KIND_AHRIMAN,
		Common::ENEMY_KIND_COW,
	};

	enum ITEM_KIND{
		ITEM_KIND_RAPID_BULLET,	// 連射速度UP
		ITEM_KIND_LIFE_UP,		// ライフ回復
		ITEM_KIND_DAMAGE_UP,	// 弾の威力UP
		ITEM_KIND_ANTIDOTE,		// 解毒剤

		ITEM_KIND_MAX,
	};

	enum ENEMY_AI{
		AI_NONE,		// 何もしない
		AI_SEARCHING,	// プレイヤーを探している
		AI_MOVE_PLAYER,	// プレイヤーに近づく(体当たり攻撃)
		AI_DASH_TACKLE,	// プレイヤーに高速体当たり
		AI_SHOOTING,	// 遠距離攻撃(遠距離攻撃)

		// 以下、ボス用AI
		AI_SEARCHING_SLIME_KING,	// プレイヤー探索(スライムキング)
		AI_MOVE_PLAYER_SLIME_KING,	// プレイヤーに近づく(スライムキング)
		AI_ATTACK_NEAR,				// レンジャーAI
		AI_ATTACK_WIZARD,			// 魔法使いAI
		AI_ATTACK_DRAGON,			// ドラゴンAI
		AI_ATTACK_LAST_BOSS,		// ラスボスAI
		AI_ATTACK_LAST_BOSS_HAND,	// ラスボス両手AI
		AI_MAX,
	};
	
	// 所有者
	enum OWNER_TYPE{
		OWNER_PLAYER,
		OWNER_ENEMY,

		OWNER_MAX,
	};

	// バトルポイントを使用してあげることのできるプレイヤーステータス
	const uint32_t STATUS_LEVEL_MAX = 9;
	enum PLAYER_BASE_STATE{
		BASE_STATE_LIFE,		// ライフの最大値を決める
		BASE_STATE_MOVE_SPEED,	// ダッシュ時間
		BASE_STATE_DEFFENCE,	// 被ダメージを決める
		BASE_STATE_BULLET_SPD,	// マシンガンの間隔
		BASE_STATE_BULLET_DMG,	// マシンガンのダメージ
		BASE_STATE_BLADE_LEVEL,	// 斬撃のダメージ
		BASE_STATE_CONTINUE,	// コンティニュー(途中から再開)

		BASE_STATE_MAX,
	};

	// 以下、セーブデータとして保持する
	const uint32_t RANKING_RECORD_MAX = 5;
	struct SAVE_DATA{
		bool		m_isFirst;			// 初めてのパワーアップ画面かどうか
		bool		m_isFirstGamePlay;	// 初めてのゲームプレイかどうか
		bool		m_isClearPhase01;	// フェーズ1クリアフラグ
		bool		m_isClearPhase02;	// フェーズ2クリアフラグ
		uint32_t	m_battlePoint;	// ゲームプレイで手にいてたポイント(パワーアップに必要)
		uint32_t	m_scoreRanking[RANKING_RECORD_MAX];
		uint32_t	m_playerBaseStateLv[BASE_STATE_MAX];
	};
}
#endif