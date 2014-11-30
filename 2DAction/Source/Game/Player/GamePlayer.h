/* ====================================================================== */
/**
 * @brief  プレイヤークラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef SYSTEM_GAME_PLAYER
#define SYSTEM_GAME_PLAYER

#include "System/Task/SystemTaskUnit.h"
#include "System/Input/SystemInputWatcher.h"
#include "AttackGun/GamePlayerAttackGun.h"
#include "Game/Game2DBase.h"
#include "Game/Item/ItemObject.h"

class GamePlayer : public TaskUnit, InputWatcher
{
public:

	static GamePlayer *CreatePlayer();
	~GamePlayer(void);

	virtual bool Init() override;
	virtual void Update() override;
	virtual void CollisionUpdate() override;
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
	const TEX_DRAW_INFO &GetDrawInfo();
	const uint32_t	&GetCurrentLife(){ return m_playerLife; }

	//! タスクにイベント追加
	void AddEvent( const Common::CMN_EVENT &cmnEvent ) override;

protected:

	void EventUpdate( const Common::CMN_EVENT &eventId ) override;

private:

	GamePlayer(void);

	// 引数のベクター分移動ができるかどうかチェック
	bool CanMoveThisPos( const math::Vector2 &nextFlameAddValue );

	// 基底からコールされるCommon::CMN_EVENTに対応した関数
	void EventDamage();
	void PlayerGetItem( const ItemObject::ITEM_KIND &itemKind );

	uint32_t			m_playerLife;	// ユーザーライフ
	uint32_t			m_speedMove;	// 行動速度
	uint32_t			m_speedMoveBase;// 基本行動速度
	float				m_speedMultiply;// 行動速度の倍率
	uint32_t			m_invisibleTime;// 何らかの理由で敵の攻撃を受けない時間
	AttackGun			*m_attackGun;	// マシンガンクラス

	// 描画
	Texture2D			m_texturePlayer;	// プレイヤー画像
	Texture2D			m_textureLife;		// ライフ画像
	Texture2D			m_textureLifeFrame;	// ライフ外枠画像
};

#endif