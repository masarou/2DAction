/* ====================================================================== */
/**
 * @brief  �p�[�c�N���X�̂ЂƂ�(���l�J�E���^�[)
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

	// ������
	void Reset();

	// �X�R�A�ǉ�,�Z�b�g
	void AddValue( const int32_t &addValue );
	void SetValue( const uint32_t &setValue );

	// �X�R�A�擾
	const int32_t &GetValue() const{ return m_value; }

	// �J�E���g�A�j�����s�����ǂ���
	void SetCountAnimFlg( bool isCount ){ m_isInvalidCountAnim = !isCount; }

	// �J�E���g�A�j������߂�
	void CountAnimEnd();

	// �J�E���g�A�j�������ǂ���
	bool IsPlayCountAnim();

	// �p�[�c�^�C�v�擾
	virtual Common::PARTS_TYPE_MENU GetPartsType() override{ return Common::PARTS_NUM_COUNTER; }

protected:

	// ���t���[���Ă΂��N���X�X�V�̋N�_
	virtual void UpdateParts() override;

private:
	
	// �\���X�R�A�A�j���X�V
	void UpdateScore( const uint32_t &score );

	uint32_t	m_counter;			// ���l���オ���Ă���Ƃ���SE�����I�ɂȂ炷�J�E���^
	int32_t		m_value;
	int32_t		m_currDispValue;

	bool		m_isInvalidCountAnim;	// ���l�̃J�E���g�A�j���𖳌��ɂ��邩�ǂ���
};
#endif // __SYSTEM_MENU_PARTS_COUNTER__