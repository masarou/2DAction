
#include "System/SystemDefine.h"
#include "System/Task/SystemTaskManager.h"
#include "System/Message/SystemMessageManager.h"
#include "System/Collision/SystemCollisionManager.h"
#include "System/Sound/SystemSoundManager.h"
#include "System/SystemFPSManager.h"
#include "System/Draw2D/SystemDraw2DManager.h"
#include "System/Draw2D/SystemDraw2DResource.h"
#include "System/SystemFadeManager.h"
#include "System/SystemPauseFilter.h"
#include "System/SystemBgManager.h"
#include "Game/GameRecorder.h"
#include "Common/CmnGameAccesser.h"
#include "Flow/FlowManager.h"

void GameMain();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
	//! ���[�N�`�F�b�N
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//! �c�w���C�u����������
	SetAlwaysRunFlag(true);

	// �E�C���h�E�^�C�g���ݒ�
	SetWindowText( "MonsterBuster" );

	// �E�C���h�E�T�C�Y�ݒ�
	SetGraphMode( WINDOW_WIDTH, WINDOW_HEIGHT, 32);

	if( ChangeWindowMode(true) != 0 ){
		return 0;
	}
	if( DxLib_Init() != 0 ){
		return 0;
	}

	// �f�t�H���g�t�H���g�ݒ�
	SetDefaultFontState( "���C���I", FONT_DEFAULT_SIZE, 6 );

	// �A�v�����O�o�͂Ȃ�
	//SetOutApplicationLogValidFlag( false );

	//! TaskUnit��p���풓��
	GameAccesser::Create();
	Draw2DManager::Create();
	TextureResourceManager::Create();
	SystemMessageManager::Create();
	CollisionManager::Create();

	//! TaskUnit�p���풓��
	FlowManager::Create();
	SoundManager::Create();
	FadeManager::Create();
	PauseFilter::Create();
	GameRecorder::Create();
	BgManager::Create();

	//! �����p��
	srand( static_cast<uint32_t>(time(NULL)) );

	while(1){

		//! FPS�ɂ��X�V�҂�
		if( !FpsManager::GetInstance()->IsUpdateTiming() ){
			Sleep(1);
			continue;
		}

		GameMain();

		if( ProcessMessage() != 0 ){ //!< ���b�Z�[�W����
			break;//�E�B���h�E�́~�{�^���������ꂽ�烋�[�v�𔲂���
		}
	};

	//! �^�X�N�}�l�[�W���폜
	for(;;){
		if(TaskManager::GetInstance()->ReleaseAllEnd()){
			TaskManager *pTask = TaskManager::GetInstance();
			SAFE_DELETE(pTask);
			break;
		}
	}

	//! �풓���폜
	CollisionManager::GetInstance()->DeleteCollisionManager();
	SystemMessageManager::GetInstance()->DeleteMessageManager();
	TextureResourceManager::GetInstance()->DeleteResourceManager();
	Draw2DManager::GetInstance()->DeleteDraw2DManager();
	GameAccesser::GetInstance()->DeleteGameAccesser();

	// �c�w���C�u�����g�p�̏I��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}

void GameMain()
{
	//! �`��N���A
	ClearDrawScreen();

	//! �`���𗠉�ʂɂ���
	SetDrawScreen( DX_SCREEN_BACK ) ;

	TaskManager::GetInstance()->ExecApp();

	//! ���ׂĂ�2D�`��͂����ōs��
	Draw2DManager::GetInstance()->Action();

	//! ����ʂ̓��e��\��ʂɔ��f������
	ScreenFlip() ;
}
