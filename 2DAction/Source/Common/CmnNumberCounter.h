/* ====================================================================== */
/**
 * @brief  �X�R�A�\���N���X
 *
 * @note
 *		�Q�[�����ɍ���ɏo���X�e�[�W�X�R�A
 */
/* ====================================================================== */
#ifndef __SCORE_COUNTER__
#define __SCORE_COUNTER__

#include "System/SystemDefine.h"
#include "System/Task/SystemTaskUnit.h"
#include "Game/Game2DBase.h"

class NumberCounter : public TaskUnit
{
public:

	~NumberCounter(void);
	
	static NumberCounter *Create( const char *readJson );

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
	// �`�斳���ɂ��邩�t���O�Z�b�g
	void SetDrawInvalidFlag( const bool &flg ){ m_invalidDraw = flg; }
	// ���l���ς�鎞��SE��炳�Ȃ����t���O�Z�b�g
	void SetCountUpSeInvalidFlag( const bool &flg ){ m_invalidCallCountUpSE = flg; }

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

private:

	NumberCounter( const char *readJson );

	void UpdateScore( const uint32_t &score );

	bool		m_invalidDraw;			// �����̕`����s�����ǂ���(true�ŕ`�悵�Ȃ�)
	bool		m_invalidCallCountUpSE;	// �������ω�����Ƃ���SE��炷���ǂ���(true�Ŗ炳�Ȃ�)
	std::string	m_readFile;				// �ǂݍ���ł���json
	uint32_t	m_counter;				// ���l���オ���Ă���Ƃ���SE�����I�ɂȂ炷�J�E���^
	int32_t		m_value;
	int32_t		m_currDispValue;

	std::vector<Game2DBase*>	m_pNumber2DArray;	// ���_�̉摜���Ǘ�
	TEX_DRAW_INFO				m_numberInfo;		// �`��֌W���

};

#endif