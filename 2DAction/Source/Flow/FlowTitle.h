
/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g����ʊǗ��N���X
 */
/* ====================================================================== */
#ifndef FLOW_TITLE
#define FLOW_TITLE

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
	virtual bool Finish() override;
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
 *		�^�C�g���ꖇ�G�N���X
 */
/* ====================================================================== */
class Title2D : public TaskUnit
{
public:

	static Title2D *CreateTitle2D();

protected:

	virtual void DrawUpdate() override;		// �`��X�V

private:

	Title2D();
	virtual ~Title2D(void);
	
	Texture2D			m_textureTitle;	// �^�C�g���ꖇ�G
};
#endif

