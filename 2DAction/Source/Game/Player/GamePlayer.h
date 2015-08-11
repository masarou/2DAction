/* ====================================================================== */
/**
 * @brief  プレイヤークラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __SYSTEM_GAME_PLAYER__
#define __SYSTEM_GAME_PLAYER__

#include "System/Input/SystemInputWatcher.h"
#include "System/Collision/SystemCollisionUnit.h"
#include "Game/Game2DBase.h"
#include "Game/Item/ItemObject.h"
#include "GamePlayerStatusMenu.h"

class AttackGun;
class AttackBlade;

class GamePlayer : public TaskUnit, public Collision2DUnit, public InputWatcher
{
public:

	static GamePlayer *CreatePlayer();
	~GamePlayer(void);

	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

	virtual void PadEventUp() override;
	virtual void PadEventDown() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;
	virtual void PadEventDecide() override;
	virtual void PadEventCancel() override;
	virtual void PadEventR1() override;
	virtual void PadEventL1() override;
	
	// 情報取得
	const TEX_DRAW_INFO &GetDrawInfo()  const;
	const uint32_t	&GetCurrentLife() const{ return m_playerLife; }
	const uint32_t	&GetPlayerLifeMax() const;

	//! タスクにイベント追加
	void AddEvent( const Common::CMN_EVENT &cmnEvent );

protected:

	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_PLAYER; }
	void EventUpdate( Common::CMN_EVENT &eventId ) override;

private:

	// プレイヤがなりうる異常状態
	enum PLAYER_ABNORMAL_STATE{
		ABNORMAL_STATE_NONE			= 0,	// 通常状態
		ABNORMAL_STATE_MOVE_LOCK	= 1<<0,	// 動けない

		ABNORMAL_STATE_MAX,
	};

	GamePlayer(void);

	// 現在のプレイヤーの状況から再生するアニメタグ取得
	std::string GetAnimTag();

	// ベースとなるステータス＋取得アイテム数を反映させる
	void SetupInitPlayerState();

	// ステータスレベルから実際にセットする値へ変換
	uint32_t ConvertLevelToBaseState( Common::PLAYER_BASE_STATE stateKind, uint32_t level );

	// 移動処理
	void UpdateMove( math::Vector2 &moveVec, bool isForce = false );

	// 引数のベクター分移動ができるかどうかチェック
	bool CanMoveThisPos( const math::Vector2 &nextFlameAddValue ) const;

	// 基底からコールされるCommon::CMN_EVENTに対応した関数
	void EventDamage( const Common::EVENT_MESSAGE &eventKind, const uint32_t &damageValue );
	void PlayerGetItem( const Common::ITEM_KIND &itemKind, bool isCountUp = true );

	// プレイヤーが該当のステータス状態かどうか
	bool IsPlayerState( const PLAYER_ABNORMAL_STATE &checkState ) const;
	void SetPlayerState( const PLAYER_ABNORMAL_STATE &checkState, const bool &flag );

	uint32_t				m_playerLife;	// ユーザーライフ
	uint32_t				m_playerLifeMax;// ユーザーライフ最大値
	uint32_t				m_playerState;	//プレイヤー異常状態ステータス
	uint32_t				m_speedMove;	// 行動速度に倍率をかけた瞬間のスピード
	uint32_t				m_speedMoveBase;// 基本行動速度
	float					m_deffenceLate;	// 防御力
	float					m_speedMultiply;// 行動速度の倍率
	uint32_t				m_invisibleTime;// 何らかの理由で敵の攻撃を受けない時間
	uint32_t				m_invalidCtrlTime;	// 何らかの理由で操作を受け付けない時間
	Common::FORCE_MOVING	m_forceMoveInfo;	// 他のクラスから受ける衝撃

	AttackGun			*m_attackGun;	// マシンガンクラス
	AttackBlade			*m_attackBlade;	// 近接攻撃(剣)クラス

	PlayerStatusMenu	*m_pStatusMenu;	// ステータスメニュー
};

#endif