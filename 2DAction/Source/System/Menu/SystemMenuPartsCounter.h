/* ====================================================================== */
/**
 * @brief  ��ʂ��\������p�[�c�ЂƂ̃N���X
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

	static MenuParts *Create( const std::string &partsStr, const std::string &jsonStr, const math::Vector2 &originalPos );

	PartsCounter( const std::string &partsStr, const std::string &jsonStr, const math::Vector2 &originalPos );
	~PartsCounter(void);

	// ������
	void Reset();

	// �X�R�A�ǉ�,�Z�b�g
	void AddValue( const int32_t &addValue );
	void SetValue( const uint32_t &setValue );

	// �X�R�A�擾
	const int32_t &GetValue() const{ return m_value; }

	// �`����Z�b�g
	void SetDrawInfo( const TEX_DRAW_INFO &drawInfo );
	void SetDrawPos( const math::Vector2 &pos );

	// �J�E���g�A�j������߂�
	void CountAnimEnd();

	// �J�E���g�A�j�������ǂ���
	bool IsPlayCountAnim();

	// �p�[�c�^�C�v�擾
	virtual Common::PARTS_TYPE_MENU GetPartsType() override{ return Common::PARTS_NUM_COUNTER; }

protected:

	virtual void UpdateParts() override;

private:
	
	void UpdateScore( const uint32_t &score );

	uint32_t	m_counter;			// ���l���オ���Ă���Ƃ���SE�����I�ɂȂ炷�J�E���^
	int32_t		m_value;
	int32_t		m_currDispValue;

};
#endif // __SYSTEM_MENU_PARTS_COUNTER__