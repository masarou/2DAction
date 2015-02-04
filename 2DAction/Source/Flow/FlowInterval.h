/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[���I���ナ�g���C�m�F��ʃN���X
 */
/* ====================================================================== */
#ifndef __FLOW_RETRY__
#define __FLOW_RETRY__

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class Interval2D;

class FlowInterval : public FlowBase 
{
public:

	static FlowBase *Create( const std::string &fileName);

protected:
	
	virtual bool Init() override;
	virtual void PadEventDecide() override;

private:

	std::string GetNextFadeStr();

	FlowInterval( const std::string &fileName );
	~FlowInterval(void);
	
	Interval2D	*m_pRetryTex;
};

/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		���g���C�I�����N���X
 */
/* ====================================================================== */
class Interval2D : public TaskUnit, InputWatcher
{
public:
	
	enum{
		SELECT_NEXT,
		SELECT_TITLE,

		SELECT_RETRY_MAX,
	};

	static Interval2D *CreateRetry2D();

	const uint32_t &GetSelectedNo() const{ return m_selectNo; }

protected:
	
	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;		// �`��X�V
	
	virtual void PadEventUp() override;
	virtual void PadEventDown() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;

private:

	Interval2D();
	virtual ~Interval2D(void);
	
	Texture2D			m_textureRetry;	// �^�C�g���ꖇ�G
	
	uint32_t			m_selectNo;
	Game2DBase			*m_pTexChoiceArray[SELECT_RETRY_MAX];	// �I����
	Game2DBase			*m_pTexChoiceBGArray[SELECT_RETRY_MAX];	// �I�����w�i
	TEX_DRAW_INFO		m_texInfo;								// �I�����`����
};

#endif