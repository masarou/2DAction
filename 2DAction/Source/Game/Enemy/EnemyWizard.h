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

	static EnemyWizard *Create( const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos );
	virtual const uint32_t GetPlayerHitDamage() const override;	// プレイヤー衝突時のダメージ

	math::Vector2 GetCrystalPosFirst() const;
	math::Vector2 GetCrystalPosSecond() const;

protected:
	
	virtual bool InitMain() override;									// 派生先での初期化
	virtual const Common::TYPE_OBJECT GetTypeObject() const override{ return Common::TYPE_ENEMY_WIZARD; }
	virtual const uint32_t GetEnemyDefaultHP() const override;			// HP取得
	virtual const uint32_t GetEnemyDefaultSPD() const override;			// SPD取得
	virtual void  ReduceDamage( Common::CMN_EVENT &eventId ) override;	// 敵の種類、レベル等でダメージ軽減処理
	virtual const Common::ENEMY_AI GetEnemyDefaultAI() const{return Common::AI_ATTACK_WIZARD;}	// 敵クラスのデフォルトSPD取得

	// このクラスで定義する仮想関数
	virtual bool DieMainCustom();

private:

	EnemyWizard( const std::string &fileName, const uint32_t &enemyLevel, const uint32_t &uniqueID, const math::Vector2 &enemyPos );
	~EnemyWizard(void);

	WizardCrystal	*m_pCrystal;
	WizardCrystal	*m_pCrystalSec;
};


class WizardCrystal : public TaskUnit, public Collision2DUnit
{
public:

	static WizardCrystal *Create();
	
	virtual const Common::TYPE_OBJECT GetTypeObject() const{ return Common::TYPE_WIZARD_CRYSTAL; };

	//
	const math::Vector2 GetPos() const;
	void SetPos( const math::Vector2 &centerPos );

	void TaskDie(){ TaskStartDie(); }

protected:

	virtual void Update() override;			// 移動等の内部数値の更新
	virtual void DrawUpdate() override;		// 描画更新

private:

	WizardCrystal();
	~WizardCrystal();


};

#endif //__ENEMY_WIZARD__