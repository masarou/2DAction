/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�Q�[�����ʉ�ʃN���X
 */
/* ====================================================================== */
#ifndef FLOW_RESULT
#define FLOW_RESULT

#include "FlowBase.h"
#include "Game/Game2DBase.h"
#include "Common/CmnNumberCounter.h"
#include "System/Task/SystemTaskUnit.h"

class Result2D;

class FlowResult : public FlowBase
{
public:

	static FlowBase *Create( const std::string &fileName );

private:

	virtual bool Init() override;
	virtual void PadEventDecide() override;

	void UpdateSortRanking( Common::SAVE_SCORE &scoreData );

	bool CheckSaveData( Common::SAVE_SCORE &saveData );

	FlowResult( const std::string &fileName );
	~FlowResult(void);

	Result2D	*m_pResultTex;
};


/* ====================================================================== */
/**
 * @brief  
 *
 * @note
 *		�^�C�g���I�����N���X
 */
/* ====================================================================== */
class Result2D : public TaskUnit, InputWatcher
{
public:
	
	enum{
		SELECT_START,
		SELECT_SCORE,
		SELECT_EXIT,

		SELECT_MAX,
	};

	static Result2D *CreateResult2D();

protected:
	
	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;		// �`��X�V

private:

	Result2D();
	virtual ~Result2D(void);
	
	Texture2D			m_textureResult;	// �^�C�g���ꖇ�G

	NumberCounter		*m_pNumCounter;		// �X�R�A�\��
	TEX_DRAW_INFO		m_numberInfo;		// �`��֌W���
};


#endif