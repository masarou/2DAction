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

	// �����ɍ���̃v���C�f�[�^�𔽉f���������L���O���Z�b�g
	void UpdateSortRanking( Common::SAVE_SCORE &scoreData );
	
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

	static Result2D *CreateResult2D();

	bool ProceedNext(){ return m_dispState == DISP_ALL ? true : false ;}		// �S�ĕ\�����I���Ď��ɐi��ł������ǂ���

protected:
	
	virtual bool Init() override;
	virtual void Update() override;
	virtual void DrawUpdate() override;		// �`��X�V
	virtual void PadEventDecide() override;

private:

	enum DISP_STATE{
		DISP_RESULT,
		DISP_BONUS,
		DISP_TOTAL,
		DISP_ALL,

		DISP_MAX,
	};

	Result2D();
	virtual ~Result2D(void);

	DISP_STATE			m_dispState;		// �ǂ��܂ŕ\�����Ă��邩
	Texture2D			m_textureResult;	// �w�i�ꖇ�G
	TEX_DRAW_INFO		m_numberInfo;		// �`��֌W���
	
	NumberCounter		*m_pNumCounterResult;		// �X�R�A�\��
	NumberCounter		*m_pNumCounterBonus;		// �X�R�A�\��
	NumberCounter		*m_pNumCounterTotal;		// �X�R�A�\��
};


#endif