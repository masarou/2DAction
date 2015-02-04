
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g����ʊǗ��N���X
 */
/* ====================================================================== */
#ifndef __FLOW_TITLE__
#define __FLOW_TITLE__

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "System/Task/SystemTaskUnit.h"

class Title2D;

class FlowTitle : public FlowBase
{
public:

	static FlowBase *Create( const std::string &fileName );

protected:

	virtual bool Init() override;
	virtual void PadEventDecide() override;

private:

	FlowTitle( const std::string &fileName );
	virtual ~FlowTitle(void);

	Title2D	*m_pTitleTex;

};

/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g���I�����N���X
 */
/* ====================================================================== */
class Title2D : public TaskUnit, InputWatcher
{
public:
	
	enum{
		SELECT_START,
		SELECT_SCORE,
		SELECT_EXIT,

		SELECT_MAX,
	};

	static Title2D *CreateTitle2D();

	const uint32_t &GetSelectedNo(){ return m_selectNo; }

protected:
	
	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;		// �`��X�V
	
	virtual void PadEventUp() override;
	virtual void PadEventDown() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;

private:

	Title2D();
	virtual ~Title2D(void);
	
	Texture2D			m_textureTitle;	// �^�C�g���ꖇ�G
	
	uint32_t			m_selectNo;
	Game2DBase			*m_pTexChoiceArray[SELECT_MAX];		// �I����
	Game2DBase			*m_pTexChoiceBGArray[SELECT_MAX];	// �I�����w�i
	TEX_DRAW_INFO		m_texInfo;							// �I�����`����
};
#endif

