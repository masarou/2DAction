/* ====================================================================== */
/**
 * @brief  敵管理クラス
 *
 * @note
 *		
 */
/* ====================================================================== */

#ifndef __ENEMY_WIZARD__
#define __ENEMY_WIZARD__

#include "EnemyBase.h"
#include "System/Task/SystemTaskUnit.h"
#include "System/Collision/SystemCollisionUnit.h"

class WizardCrystal;

class EnemyWizard : public EnemyBase
{

public:

	// クリスタルの回転の中心となるオブジェクト種類
	enum CRYSTAL_AROUND{
		CRYSTAL_AROUND_MINE,	// クリスタルを自分の周りで回す
		CRYSTAL_AROUND_PLAYER,	// クリスタルをプレイヤの周りで回す

		CRYSTAL_AROUND_MAX,
	};

	// 現在有効なダメージ種類
	enum DAMAGE_TYPE{
		DAMAGE_TYPE_SLASH,	// 斬撃
		DAMAGE_TYPE_BULLET,	// マシンガン

		DAMAGE_TYPE_MAX,
	};

	static EnemyWizard *Create( const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override;	// プレイヤー衝突時のダメージ

	const uint32_t GetCrystalIndex() const{ return m_pCrystalVec.size(); };	// 所持クリスタル数取得
	const math::Vector2 GetCrystalPos( const uint32_t &index) const;		// 指定Indexのクリスタル位置取得

	// マシンガンダメージ取得
	uint32_t GetBulletDamage() const;

	// クリスタルの中心対象取得セット
	const CRYSTAL_AROUND GetCrystalAroundTarget() const{ return m_crystalAround; }
	void SetCrystalAroundTarget( const CRYSTAL_AROUND &type );

	// クリスタルの回転半径セット
	void SetCrystalAroundDistance( const uint32_t &distance );

	// ダメージタイプ取得セット
	const DAMAGE_TYPE GetCurrentDamageType() const{ return m_damageType; }
	void SetCurrentDamageType( const DAMAGE_TYPE &type ){ m_damageType = type; }

protected:
	
	virtual bool InitMain() override;			// 派生先での初期化
	virtual void UpdateCustom() override;		// 派生先での更新処理
	virtual bool DieMainCustom();				// 派生先での死亡時処理
	
	// クラスの敵タイプ
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_WIZARD; }

	// 敵クラスのデフォルト値取得
	virtual const uint32_t	GetEnemyDefaultHP() const override;
	virtual const float		GetEnemyDefaultSPD() const override;
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_ATTACK_WIZARD;}	// 敵クラスのデフォルトSPD取得

	// 敵の種類、レベル等でダメージ軽減処理
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;
	virtual void HitPlayreSlashing( const uint32_t &damageValue ) override;
	
	// HPがなくなり倒されたときに呼ばれる
	virtual void  EnemyDeath() override;


private:

	EnemyWizard( const std::string &fileName, const uint32_t &uniqueID, const uint32_t &enemyLevel, const math::Vector2 &enemyPos );
	~EnemyWizard(void);

	CRYSTAL_AROUND					m_crystalAround;	// クリスタルが何中心に回っているか
	DAMAGE_TYPE						m_damageType;		// 有効なダメージ種類
	std::vector< WizardCrystal* >	m_pCrystalVec;		// クリスタルクラス
};


class WizardCrystal : public TaskUnit, public Collision2DUnit
{
public:

	static WizardCrystal *Create( uint32_t startRot );
	
	virtual const Common::TYPE_OBJECT GetTypeObject() const{ return Common::TYPE_WIZARD_CRYSTAL; };

	// 各種パラメータ変更
	const math::Vector2 GetPos() const;
	void SetPos( const math::Vector2 &centerPos );
	void SetRadius( const uint32_t &newRadius ){ m_circleRadius = newRadius; }

	void TaskDie(){ TaskStartDie(); }

protected:

	virtual void Update() override;			// 移動等の内部数値の更新
	virtual void DrawUpdate() override;		// 描画更新

private:

	WizardCrystal( uint32_t startRot );
	~WizardCrystal();

	uint32_t	m_startRot;		// 回転の開始角度
	uint32_t	m_rotCounter;	// 回転カウンタ
	uint32_t	m_circleRadius;	// 回転円の半径

};

#endif //__ENEMY_WIZARD__