/* ====================================================================== */
/**
 * @brief  敵の生成やアイテム生成を設定にならって管理するクラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __GAME_MANAGER__
#define __GAME_MANAGER__

#include "System/Task/SystemTaskUnit.h"
#include "Game/Item/ItemObject.h"

class GameManager : public TaskUnit
{
public:

	enum STAGE_TYPE{
		TYPE_TIME,
		TYPE_DESTROY,
		TYPE_DESTROY_BOSS,

		TYPE_MAX,
	};

	static GameManager *CreateGameManager( const Common::GAME_FLOW &currentKind );
	~GameManager(void);

	// ステージクリア条件を満たしたかどうか
	bool	IsGameOver() const;

	// 敵を倒した数をカウント
	void AddDestroyCount();

	// ステージタイプ取得
	const STAGE_TYPE GetStageType() const;

	// 残り時間or殲滅数の取得
	const uint32_t GetGameLeftTimeByFrame() const;
	const uint32_t GetGameLeftTimeBySec() const;
	const uint32_t GetGameLeftDestroy() const;
	
	// アイテム生成依頼
	void CreateItem( const Common::ITEM_KIND &kind, const math::Vector2 &pos = math::Vector2() );
	// 敵生成依頼
	void CreateEnemy( const Common::ENEMY_KIND &kind, const uint32_t &level = 0, bool isForce = false, const math::Vector2 &enemyPos = DEFAULT_VECTOR2 );

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual bool DieMain() override;

private:

	struct ExistEnemyState{
		uint32_t			m_level;		// 目安となる強さ
		uint32_t			m_freequency;	// 出現頻度(すべての敵の中での出現割合%)
		Common::ENEMY_KIND	m_kind;			// 種類
		math::Vector2		m_initPos;		// 初期位置(math::Vector2ならランダム位置)
		void Init(){
			m_level = 0;
			m_freequency = 0;
			m_kind = Common::ENEMY_KIND_MAX;
			m_initPos = math::Vector2();
		}
	};

	GameManager( const Common::GAME_FLOW &currentKind );
	
	void ResetManageValue();										// 初期化
	void LoadGameSettings( const char *jsonFile );					// ゲーム環境の読み込み
	bool IsCreateEnemy( uint32_t enemyLimit, uint32_t frequency );	// 敵の生成判断
	bool IsCreateEnemy( uint32_t enemyLimit	);						// 敵の生成判断(ランダム要素なし)
	bool IsCreateItem( uint32_t itemLimit, uint32_t frequency );	// アイテムの生成判断
	bool IsCreateItem( uint32_t enemyLimit );						// アイテムの生成判断(ランダム要素なし)
	Common::ENEMY_KIND GetEnemyKindFromStr( const std::string str );// 敵の種類を文字列から取得

	STAGE_TYPE		m_type;				// ステージクリアのタイプ

	// ステージが制限時間をもつものだった場合
	uint32_t		m_gameTimer;		// ゲームのプレイ時間
	uint32_t		m_gameTimeMax;		// ゲーム終了までの時間(/fps)

	// ステージが敵殲滅数をクリア条件とする場合
	uint32_t		m_destroyNum;		// 現在までの敵殲滅数
	uint32_t		m_destroyMax;		// ステージ終了までの敵殲滅数

	// ステージがボス殲滅をクリア条件とする場合
	std::vector<Common::ENEMY_KIND>	m_bossKindVec;	// ボスカウントする敵の種類

	// ステージ環境周り変数
	uint32_t		m_enemyMax;			// 最大出現敵数
	uint32_t		m_enemyFrequency;	// 敵の出現率(10段階0~9)
	uint32_t		m_itemMax;			// 最大出現アイテム数
	uint32_t		m_itemFrequency;	// アイテムの出現率(10段階0~9)
	std::string		m_settingFileStr;	// 読み込むステージ設定ファイル

	// 出現する敵情報
	std::vector<ExistEnemyState>	m_enemyInfoVec;

	// 最初に配置する敵情報(ボス等)
	std::vector<ExistEnemyState>	m_initEnemyInfoVec;
};

#endif