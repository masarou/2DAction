/* ====================================================================== */
/**
 * @brief  パーツクラスのひとつ(数値カウンター)
 *
 * @note
 *		
 */
/* ====================================================================== */
#ifndef __SYSTEM_MENU_PARTS_COUNTER__
#define __SYSTEM_MENU_PARTS_COUNTER__

#include "System/SystemDefine.h"
#include "Common/CommonDefine.h"
#include "SystemMenuParts.h"

class PartsCounter : public MenuParts
{

public:

	static MenuParts *Create( const std::string &partsStr, const std::string &jsonStr, const Common::PRIORITY &priority, const math::Vector2 &originalPos );

	PartsCounter( const std::string &partsStr, const std::string &jsonStr, const Common::PRIORITY &priority, const math::Vector2 &originalPos );
	~PartsCounter(void);

	// 初期化
	void Reset();

	// スコア追加,セット
	void AddValue( const int32_t &addValue );
	void SetValue( const uint32_t &setValue );

	// スコア取得
	const int32_t &GetValue() const{ return m_value; }

	// カウントアニメを行うかどうか
	void SetCountAnimFlg( bool isCount ){ m_isInvalidCountAnim = !isCount; }

	// カウントアニメをやめる
	void CountAnimEnd();

	// カウントアニメ中かどうか
	bool IsPlayCountAnim();

	// パーツタイプ取得
	virtual Common::PARTS_TYPE_MENU GetPartsType() override{ return Common::PARTS_NUM_COUNTER; }

protected:

	// 毎フレーム呼ばれるクラス更新の起点
	virtual void UpdateParts() override;

private:
	
	// 表示スコアアニメ更新
	void UpdateScore( const uint32_t &score );

	uint32_t	m_counter;			// 数値が上がっているときにSEを定期的にならすカウンタ
	int32_t		m_value;
	int32_t		m_currDispValue;

	bool		m_isInvalidCountAnim;	// 数値のカウントアニメを無効にするかどうか
};
#endif // __SYSTEM_MENU_PARTS_COUNTER__