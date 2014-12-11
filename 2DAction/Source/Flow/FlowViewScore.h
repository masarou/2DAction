/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[������\���N���X
 */
/* ====================================================================== */
#ifndef FLOW_VIEW_SCORE
#define FLOW_VIEW_SCORE

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "Common/CmnNumberCounter.h"

class ViewScore2D;

class FlowViewScore : public FlowBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual void PadEventCancel() override;

	FlowViewScore( const std::string &fileName );
	~FlowViewScore(void);
	

	ViewScore2D	*m_pViewScoreTex;
};


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g���I�����N���X
 */
/* ====================================================================== */
class ViewScore2D : public TaskUnit, InputWatcher
{
public:
	
	enum{
		SELECT_START,
		SELECT_SCORE,
		SELECT_EXIT,

		SELECT_MAX,
	};

	static ViewScore2D *CreateViewScore2D();

protected:
	
	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;		// �`��X�V

private:

	ViewScore2D();
	virtual ~ViewScore2D(void);
	
	Common::SAVE_SCORE	m_scoreData;

	Texture2D			m_textureResult;	// �^�C�g���ꖇ�G
	NumberCounter		*m_pNumCounter[Common::RANKING_RECORD_MAX];	// �X�R�A�\��
	TEX_DRAW_INFO		m_numberInfo;								// �`��֌W���
};
#endif