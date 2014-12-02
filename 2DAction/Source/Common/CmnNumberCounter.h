/* ====================================================================== */
/**
 * @brief  �X�R�A�\���N���X
 *
 * @note
 *		
 */
/* ====================================================================== */
#ifndef SCORE_COUNTER
#define SCORE_COUNTER

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
	// �X�R�A�ǉ�
	void AddValue( int32_t addValue );
	// �X�R�A�擾
	const int32_t &GetValue(){ return m_value; }
	// �`����Z�b�g
	void SetDrawInfo( const TEX_DRAW_INFO &drawInfo );
	// �J�E���g�A�j������߂�
	void CountAnimEnd();

protected:

	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;
	virtual bool DieMain() override;

private:

	NumberCounter( const char *readJson );

	void UpdateScore( const uint32_t &score );

	std::string	m_readFile;			// �ǂݍ���ł���json
	uint32_t	m_counter;			// ���l���オ���Ă���Ƃ���SE�����I�ɂȂ炷�J�E���^
	int32_t		m_value;
	int32_t		m_currDispValue;

	std::vector<Game2DBase*>	m_pNumber2DArray;	// ���_�̉摜���Ǘ�
	TEX_DRAW_INFO				m_numberInfo;		// �`��֌W���

};

#endif