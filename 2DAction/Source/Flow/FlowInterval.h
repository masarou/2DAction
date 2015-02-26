/* ====================================================================== */
/**
 * @brief  
 *		Q[IΉγgCAΙiή©mF·ιζΚ
 * @note
 *		
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
 *		FlowIntervalΜ\¦¨g
 * @note
 *		
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
	virtual void DrawUpdate() override;		// `ζXV
	
	virtual void PadEventUp() override;
	virtual void PadEventDown() override;
	virtual void PadEventRight() override;
	virtual void PadEventLeft() override;

private:

	Interval2D();
	virtual ~Interval2D(void);
	
	Texture2D			m_textureRetry;	// ^CgκG
	
	uint32_t			m_selectNo;
	Game2DBase			*m_pTexChoiceArray[SELECT_RETRY_MAX];	// Iπ
	Game2DBase			*m_pTexChoiceBGArray[SELECT_RETRY_MAX];	// Iπwi
	TEX_DRAW_INFO		m_texInfo;								// Iπ`ζξρ
};

#endif