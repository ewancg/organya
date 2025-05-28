#define _WIN32_WINNT	0x0400
#define WINVER			0x0400
#define _WIN32_WINDOWS	0x0410
#define _WIN32_IE		0x0400
#define BUF_SIZE 256
//#define HENKOU_NO_SHIRUSHI	" [�ύX�A��]"

#define MAIN_WINDOW "WINDOW"
#define COMMON_WINDOW "COMMONDIALOG"
#define TRACK_WINDOW "TRACK"
#define PLAY_WINDOW "PLAYWINDOW"
#define COPY_WINDOW "COPYWINDOW"
#define INIT_DATA	"INIT_PAN_VOL"
#define MIDI_EXPORT	"STANDARD_MIDI_EXPORT"

#define CDCD_INIT 0
#define CDCD_EXIT 1
#define CDCD_LOAD 2


#include <windows.h>
#include <winuser.h>

#include "Setting.h"
#include "DefOrg.h"
#include "resource.h"
#include "Gdi.h"
#include "OrgData.h"
#include "Scroll.h"
#include "Mouse.h"
#include "Click.h"
#include <stdio.h>
#include <string.h>
#include "Filer.h"
#include "rxoFunction.h"

#include "Sound.h"
#include "Timer.h"

//���C���v���V�[�W��
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK DialogSetting(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogDefault(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogDelete(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogCopy(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogCopy2(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogPan(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogTrans(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogVolume(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogWave(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogPlayer(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogTrack(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogNoteUsed(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogMemo(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogHelp(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogEZCopy(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogSwap(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam);

void SetTitlebarText(char *name);
void SetTitlebarChange(void);
void ResetTitlebarChange(void);
void CheckLoupeMenu(void);

//�����ɍL��ϐ���錾����
HINSTANCE hInst;//�C���X�^���X�n���h��
HWND hWnd;//���C���E�B���h�E�n���h��
HWND hDlgPlayer;
HWND hDlgTrack;
HWND hDlgEZCopy;
HWND hDlgHelp = NULL;
BOOL actApp;


int WWidth = WINDOWWIDTH, WHeight = WINDOWHEIGHT;

char lpszName[MAX_PATH+15];// = "�I���K�[�j���Q - ";//�E�C���h�E�Y��ɓo�^���閼�O

ORGDATA org_data;//���C���f�[�^
SCROLLDATA scr_data;//�X�N���[���f�[�^
MOUSEDATA mouse_data;//�}�E�X�f�[�^
extern char music_file[];//�t�@�C���l�[��
extern int sGrid;	//�͈͑I���̓O���b�h�P�ʂ�
extern int sACrnt;	//�͈͑I���͏�ɶ����ׯ�

extern void ChangeTrack(HWND hdwnd, int iTrack);
extern void ChangeTrackPlus(HWND hdwnd, int iValue);
extern char timer_sw; //���t���H
extern int EZCopyWindowState; //�C�[�W�[�R�s�[�̏��
extern void ClearEZC_Message(); //EZ���b�Z�[�W�Ɣ͈͂�����
extern RECT CmnDialogWnd;
extern int SaveWithInitVolFile;	//�ȃf�[�^�Ɓc�Z�[�u���邩�B
extern int Menu_Recent[];
extern int iDragMode;
extern int iDlgRepeat; //�_�C�A���O����擾�����J��Ԃ���
extern char strMIDI_TITLE[256];
extern char strMIDI_AUTHOR[256];
extern unsigned char ucMIDIProgramChangeValue[MAXTRACK];

//�E�B���h�E�T�C�Y�ۑ��p
RECT WinRect;
CHAR *buf;
CHAR app_path[BUF_SIZE];
CHAR num_buf[BUF_SIZE];
//�����܂�


void SaveIniFile();

//�I�[�v�j���O�t���b�V��
//#define WAITFLASH	256
//BOOL    CALLBACK DialogFlash(HWND, UINT, WPARAM, LPARAM);
int gWidthWindow;
int gHeightWindow;

int gDrawDouble;	//�����̃g���b�N�O���[�v��`�悷��

int iChgTrackKey[16] = {
	ID_AC_1,	ID_AC_2,	ID_AC_3,	ID_AC_4,	ID_AC_5,	ID_AC_6,	ID_AC_7,	ID_AC_8,
	ID_AC_Q,	ID_AC_W,	ID_AC_E,	ID_AC_R,	ID_AC_T,	ID_AC_Y,	ID_AC_U,	ID_AC_I,
};

int iMuteKey[16]={
	ID_AC_S1, ID_AC_S2, ID_AC_S3, ID_AC_S4, ID_AC_S5, ID_AC_S6, ID_AC_S7, ID_AC_S8,
	ID_AC_SQ, ID_AC_SW, ID_AC_SE, ID_AC_SR, ID_AC_ST, ID_AC_SY, ID_AC_SU, ID_AC_SI, 
};

TCHAR strSize[128]; //for Debug	// 2010.08.14 A

int iKeyPhase[128];
int iCurrentPhase;
int iCast[256];
int iPushShift[2];

int iKeyPushDown[256]; // 2010.09.22 A

char *strMIDIFile;

int CancelDeleteCurrentData(int iMessagePattern = 1){
	if(iChangeFinish!=0){	// A 2010.09.22
		char cc[512],*chn;
		GetWindowText(hWnd,cc,512);
		//chn = strstr(cc, HENKOU_NO_SHIRUSHI);	// 2014.10.19 D
		chn = strstr(cc, MessageString[IDS_MODIFIED]);	// 2014.10.19 A
		if(chn!=NULL){
			//�ύX���������Ƃ��͏I���m�F����B // A 2010.09.22
			if(iMessagePattern == 0){
				//if(MessageBox(hWnd,"�ۑ����Ă��Ȃ����e�͔j������܂��B���������܂����H", "�������m�F",MB_OKCANCEL | MB_ICONASTERISK)==IDCANCEL)return 1;	// 2014.10.19 D
				if(msgbox(hWnd,IDS_NOTIFY_INITIALIZE, IDS_NOTIFY_TITLE_INITALIZE,MB_OKCANCEL | MB_ICONASTERISK)==IDCANCEL)return 1;	// 2014.10.19 A
			}else if(iMessagePattern == 1){
				//if(MessageBox(hWnd,"�ۑ����Ă��Ȃ����e�͔j������܂��B�I�����܂����H", "�I���m�F",MB_OKCANCEL | MB_ICONASTERISK)==IDCANCEL)return 1;	// 2014.10.19 D
				if(msgbox(hWnd,IDS_NOTIFY_EXIT, IDS_NOTIFY_TITLE_EXIT,MB_OKCANCEL | MB_ICONASTERISK)==IDCANCEL)return 1;	// 2014.10.19 A
			}else if(iMessagePattern == 2){
				//if(MessageBox(hWnd,"�ۑ����Ă��Ȃ����e�͔j������܂��B�t�@�C�������[�h���܂����H", "���[�h�m�F",MB_OKCANCEL | MB_ICONASTERISK)==IDCANCEL)return 1;	// 2014.10.19 D
				if(msgbox(hWnd,IDS_NOTIFY_LOAD, IDS_NOTIFY_TITLE_LOAD,MB_OKCANCEL | MB_ICONASTERISK)==IDCANCEL)return 1;	// 2014.10.19 A
			}
		}
	} 
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR dropfile,int nCmdShow)
{//���C���֐�
	MSG msg;//���̃A�v�����g���p�����[�^	
	WNDCLASSEX wc;
//	MessageBox(hWnd,dropfile,"Drap",MB_OK);
	InitMMTimer();  // 2010.09.21
	strSize[0]=0;	// 2010.08.14 A
	for(int jjj=0;jjj<128;jjj++)iKeyPhase[jjj]=-1;
	iCurrentPhase=0;
	iPushShift[0]=0;
	iPushShift[1]=0;
	int i, vvv;
	for(vvv=0;vvv<256;vvv++){
		iCast[vvv]=0xFFFF;
		iKeyPushDown[vvv]=0;
	}

	//���b�Z�[�W�p�X�g�����O�����[�h����
	AllocMessageStringBuffer();

	//�����t�@�C����
	strcpy(music_file, MessageString[IDS_DEFAULT_ORG_FILENAME]);

	iCast['Z']= 33;
	iCast['S']= 34;
	iCast['X']= 35;
	iCast['C']= 36; //C �c C�̉�
	iCast['F']= 37;
	iCast['V']= 38; //     D
	iCast['G']= 39;
	iCast['B']= 40; //     E
	iCast['N']= 41; //     F
	iCast['J']= 42;
	iCast['M']= 43; //     G
	iCast['K']= 44;
	iCast[0xBC]=45; //,    A
	iCast['L']= 46;
	iCast[0xBE]=47; //.    B
	iCast[0xBF]=48; //�^   C
	iCast[0xBA]=49; //:
	iCast[0xE2]=50; //��
	iCast[0xDD]=51; //]
	strMIDIFile = (char *)malloc(MAX_PATH);

	HACCEL Ac; //�V���[�g�J�b�g�L�[�p

	LoadString(GetModuleHandle(NULL), IDS_TITLE, lpszName, sizeof(lpszName) / sizeof(lpszName[0]));

	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = 0;//CS_DBLCLKS| CS_OWNDC;//�A�v���P�[�V�����̃X�^�C��
	wc.lpfnWndProc   = (WNDPROC)WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInst = hInstance;
	wc.hIcon         = LoadIcon(hInst,"ICON");//�傫���A�C�R��
	wc.hIconSm       = LoadIcon(hInst,"ICON");//�������A�C�R��
	wc.hCursor       = LoadCursor(hInst,"CURSOR");//�J�[�\��
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//�E�C���h�E�̋C�{�F
	wc.lpszMenuName  = "ORGANYAMENU";//���j	���[
	wc.lpszClassName = lpszName;

	int wnd_width;///������Window�̍L�����w�肵�܂��B
	int wnd_height;
	gWidthWindow = wnd_width = GetSystemMetrics(SM_CXFRAME)*2+//�t���[���̕�
		GetSystemMetrics(SM_CXHSCROLL)+//�X�N���[���o�[�̕�
		WWidth;
	gHeightWindow = wnd_height = GetSystemMetrics(SM_CYFRAME)*2 +//�t���[���̍���
		GetSystemMetrics(SM_CYCAPTION)+//�L���v�V�����̍���
		GetSystemMetrics(SM_CYMENU)+//���j���[�o�[�̍���
		GetSystemMetrics(SM_CYVSCROLL)+//�X�N���[���o�[�̍���
		WHeight;

	if(!RegisterClassEx(&wc)) return FALSE;
	
	GetModuleFileName(hInstance, app_path,BUF_SIZE - 1);
	buf=app_path + lstrlen(app_path) - 4;
	if(lstrcmpi(buf,".exe")==0){
		lstrcpy(buf,".ini");
	}else{
		lstrcat(app_path,".ini");
	}
	//NoteWidth = 16; //���w�聚
	NoteWidth =         GetPrivateProfileInt(MAIN_WINDOW,"NoteWidth",16,app_path);
	NoteWidth = (NoteWidth > 16) ? 16: ( (NoteWidth<4) ? 4: NoteWidth );

	WinRect.left=       GetPrivateProfileInt(MAIN_WINDOW,"left",0,app_path);
	WinRect.top=        GetPrivateProfileInt(MAIN_WINDOW,"top",0,app_path);
	WinRect.right=      GetPrivateProfileInt(MAIN_WINDOW,"right",560,app_path);
	WinRect.bottom=     GetPrivateProfileInt(MAIN_WINDOW,"bottom",560,app_path);
	CmnDialogWnd.left=	GetPrivateProfileInt(COMMON_WINDOW,"left",	20,app_path);
	CmnDialogWnd.top=	GetPrivateProfileInt(COMMON_WINDOW,"top",	20,app_path);
	CmnDialogWnd.right=	GetPrivateProfileInt(COMMON_WINDOW,"right",	550,app_path);
	CmnDialogWnd.bottom=GetPrivateProfileInt(COMMON_WINDOW,"bottom",560,app_path);
	iDlgRepeat =        GetPrivateProfileInt(MIDI_EXPORT,"Repeat",1,app_path);

	char strauthtmp[128];
	SYSTEMTIME stTime; GetLocalTime(&stTime); //stTime.wYear �ŔN�擾	// 2014.10.18
	strcpy(strauthtmp, "(C) AUTHOR xxxxx,                 ");
	sprintf(strauthtmp + 18, "%d", stTime.wYear); //,�̌��ɐ��������

	//GetPrivateProfileString(MIDI_EXPORT, "Author", "(C) AUTHOR xxxxx, 2014", strMIDI_AUTHOR, 255, app_path);	// 2045.01.18 D
	GetPrivateProfileString(MIDI_EXPORT, "Author", strauthtmp, strMIDI_AUTHOR, 255, app_path);	// 2045.01.18 A
	GetPrivateProfileString(MIDI_EXPORT, "Title", MessageString[IDS_DEFAULT_MIDI_TITLE], strMIDI_TITLE, 255, app_path);
	for(i=0;i<8;i++)ucMIDIProgramChangeValue[i]=255;

	//SetWindowPos(hWnd,HWND_TOP,WinRect.left,WinRect.top,WinRect.right,WinRect.bottom,SWP_HIDEWINDOW);

	unsigned long ul;
	ul = WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU|WS_THICKFRAME|WS_MAXIMIZEBOX;

	//���C���E�C���h�E�̐���
	hWnd = CreateWindow(lpszName,
			"�I���K�[�j��",//�\�������"���O"
			ul,
			//WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU|WS_THICKFRAME|WS_MAXIMIZEBOX,
//            WS_CAPTION|WS_VISIBLE|WS_SYSMENU,//�E�B���h�E�̃X�^�C��
/*
            32,//Window��X
			32,//Window��Y
            wnd_width,//��
            wnd_height,//����
			*/
			WinRect.left, WinRect.top, WinRect.right, WinRect.bottom,
            NULL, NULL, hInst, NULL);
	if(!hWnd) return FALSE;

//	DialogBox(hInst,"DLGFLASH",NULL,DialogFlash);

	Ac=LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR1)); //�A�N�Z�X�L�[

//�摜�̏�����//////////
	StartGDI(hWnd);//GDI����
	InitBitmap("MUSIC",BMPMUSIC);//�s�A�m���[��
	InitBitmap("NOTE",BMPNOTE);//����
	InitBitmap("NUMBER",BMPNUMBER);//����
	InitBitmap("PAN",BMPPAN);//�p���ƃ{�����[��
//�T�E���h�̏�����///////
	InitDirectSound(hWnd);
//	LoadWaveData100(); //�t�@�C��"Wave.dat"���烍�[�h����
	InitWaveData100(); //���W�X�g��"WAVE100"���烍�[�h����
	scr_data.InitScroll();
	
    hDlgPlayer = CreateDialog(hInst,"PLAYER",hWnd,(DLGPROC)DialogPlayer);
    hDlgTrack = CreateDialog(hInst,"TRACK",hWnd,(DLGPROC)DialogTrack);
    hDlgEZCopy = CreateDialog(hInst,"COPYBD",hWnd,(DLGPROC)DialogEZCopy);
	//hDlgShortCutList = CreateDialog(hInst,"DLGSHORTCUTINFO",hWnd,DialogShortCut);
	WinRect.left=GetPrivateProfileInt(TRACK_WINDOW,"left",200,app_path);
	WinRect.top=GetPrivateProfileInt(TRACK_WINDOW,"top",200,app_path);
	SetWindowPos(hDlgTrack,NULL,WinRect.left,WinRect.top,0,0,SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
	WinRect.left=GetPrivateProfileInt(PLAY_WINDOW,"left",280,app_path);
	WinRect.top=GetPrivateProfileInt(PLAY_WINDOW,"top",280,app_path);
	SetWindowPos(hDlgPlayer,NULL,WinRect.left,WinRect.top,0,0,SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
	WinRect.left=GetPrivateProfileInt(COPY_WINDOW,"left",180,app_path);
	WinRect.top=GetPrivateProfileInt(COPY_WINDOW,"top",380,app_path);
	SetWindowPos(hDlgEZCopy,NULL,WinRect.left,WinRect.top,0,0,SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
	EZCopyWindowState = GetPrivateProfileInt(COPY_WINDOW,"show",1,app_path);
	if(EZCopyWindowState==0)ShowWindow(hDlgEZCopy, SW_HIDE);
	SaveWithInitVolFile = GetPrivateProfileInt(INIT_DATA,"autosave",0,app_path);
	ChangeAutoLoadMode(SaveWithInitVolFile);
	org_data.InitOrgData();

	
	CheckLoupeMenu();
	LoadRecentFromIniFile();
	ChangeGridMode(GetPrivateProfileInt(MAIN_WINDOW,"GridMode",0,app_path));
	ChangeSelAlwaysCurrent(GetPrivateProfileInt(MAIN_WINDOW,"AlwaysCurrent",0,app_path));
	ChangeDrawDouble(GetPrivateProfileInt(MAIN_WINDOW,"DrawDouble",1,app_path));
	ChangeDragMode(GetPrivateProfileInt(MAIN_WINDOW,"DragMode",1,app_path));
	ChangeEnablePlaying(GetPrivateProfileInt(MAIN_WINDOW,"EnablePlaying",1,app_path));
	ChangeFinish(GetPrivateProfileInt(MAIN_WINDOW,"QuitMessage",0,app_path));
	ChangeSlideOverlapNoteMode(GetPrivateProfileInt(MAIN_WINDOW,"SlideOverlapNoteMode",1,app_path));
	ChangePushStratchNOTE(GetPrivateProfileInt(MAIN_WINDOW,"EnablePressNoteStretch",1,app_path));
	ChangeNoteEnlarge(GetPrivateProfileInt(MAIN_WINDOW,"NoteEnlarge",1,app_path)); //2014.05.28 �k���\�����A�����̓���ڗ�������(&O)
	
	org_data.PutMusic();//�y����\��

	if(GetPrivateProfileInt(MAIN_WINDOW,"WindowState",0,app_path)==1){
		ShowWindow(hWnd,SW_MAXIMIZE);
	}else{
		ShowWindow(hWnd,nCmdShow);//���C���E�C���h�E�\��
	}
	UpdateWindow(hWnd);//���C���E�B���h�E�̍X�V

	DragAcceptFiles(hWnd,TRUE);//����Ńh���b�O����
	//�v���C���[�_�C�A���O(���[�_���X)�̐���

	//if(GetPrivateProfileInt(MAIN_WINDOW,"WindowState",0,app_path)==1){
	//	ShowWindow(hWnd,SW_MAXIMIZE);
	//}

	SetTitlebarText(music_file);//�^�C�g���l�[���Z�b�g
	ClearVirtualCB(); //���@�[�`�����N���b�v�{�[�h������
	ClearUndo();

			//�v���C���[�ɕ\��
	MUSICINFO mi;
			org_data.GetMusicInfo( &mi );
			SetDlgItemInt(hDlgTrack,IDE_VIEWWAIT,mi.wait,TRUE );
			//SetDlgItemInt(hDlgTrack,IDE_VIEWTRACK,0,TRUE );
			SetDlgItemText(hDlgTrack,IDE_VIEWTRACK,"1");
	FILE *fp;
	char kfn[MAX_PATH],gfn[MAX_PATH];
	if(dropfile[0]!=0){
		strcpy(kfn,dropfile);
		int ttt;
		if(dropfile[0]=='\"'){	//���p������菜��
			ttt = 1;
		}else{
			ttt = 0;
		}
		strcpy(gfn,&kfn[ttt]);
		int ggg = strlen(gfn)-ttt;
		gfn[ggg]=0;
		//MessageBox(hWnd,gfn,"Error(Load)",MB_OK);
		fp=fopen(gfn,"rb");
		if(fp==NULL){
			//MessageBox(hWnd,"�ǂݍ��݂Ɏ��s���܂���","Error(Load)",MB_OK); //D 2010.09.28
		}else{
			char pass_chek[3];
			bool b_OrgFile = false;
			fread(&pass_chek[0], sizeof(char), 3, fp);
			b_OrgFile = (pass_chek[0]==0x4F) && (pass_chek[1]==0x72) && (pass_chek[2]==0x67); //'O', 'r', 'g'
			fclose(fp);
			//memcpy(music_file,dropfile,MAX_PATH);
			if(b_OrgFile){ //A 2010.09.25 �Ȃ�ƂȂ�Org�`���̃t�@�C���������ꍇ
				strcpy(music_file, gfn);
				if(org_data.LoadMusicData()==TRUE){ //C 2010.09.25 ����ǉ�
					SetTitlebarText(music_file);//�^�C�g���l�[���Z�b�g
					org_data.GetMusicInfo( &mi );
					SetDlgItemInt(hDlgTrack,IDE_VIEWWAIT,mi.wait,TRUE );
					SetDlgItemText(hDlgTrack,IDE_VIEWTRACK,"1");
					ClearEZC_Message();	SelectReset();	org_data.PutMusic();
				}else{
					//ORG�t�H�[�}�b�g�̃t�@�C���ł͂Ȃ������̂� //A 2010.9.25
					//�t�@�C�����N���A
					strcpy(music_file, MessageString[IDS_DEFAULT_ORG_FILENAME]);
				}
			}
		}
	}
	QuitMMTimer(); //A 2010.09.21
	//���b�Z�[�W���[�v�𐶐�(���C�����[�v)
	while(GetMessage(&msg,NULL,0,0)){
//		if(!TranslateAccelerator(hwnd,hAccel,&msg)){
		//�_�C�A���O�����̃��b�Z�[�W����Ȃ����
		if(!TranslateAccelerator(hWnd,Ac,&msg))
        {
			if(!IsDialogMessage(hDlgPlayer,&msg)){
				if(!IsDialogMessage(hDlgTrack,&msg)){
					if(!IsDialogMessage(hDlgEZCopy,&msg)){
						if(!IsDialogMessage(hDlgHelp,&msg)){
							TranslateMessage(&msg);//�L�[�{�[�h�g�p�\
							DispatchMessage(&msg);//�����Windows�ɖ߂�
						}
					}				
				}
			}
				//TranslateMessage(&msg);
				//DispatchMessage(&msg);
        }
	}
	//MessageBox(NULL, "���b�Z�[�W���[�v�𔲂��܂���", "OK", MB_OK);

	DestroyAcceleratorTable (Ac);
	return msg.wParam;//�����ŃA�v���P�[�V�����͏I��
}
//���C���v���V�[�W��
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
//	char str[64];
	int i, j;	// 2014.10.18 j��ǉ�
	char res;
	bool AfterReSize=false;
	RECT rect = {0,0,WWidth,WHeight};//�X�V����̈�(�g���b�N�ύX)
	MUSICINFO mi;
	MINMAXINFO *pmmi;

	


//	char name[MAX_PATH];	// 2010.12.01 MOVED
//	strcpy(name, MessageString[IDS_DEFAULT_ORG_FILENAME]);
	char str[128];
	
	switch(message){
	case WM_COMMAND:
		for(i=0;i<16;i++){
			if(LOWORD(wParam) == iChgTrackKey[i]){
				ChangeTrack(hDlgTrack,i);
				//return FALSE;
				//SendDlgItemMessage(hDlgTrack , IDC_TRACK0 , BM_CLICK , 0, 0);
			}
		}
		for(i=0;i<16;i++)if(LOWORD(wParam) == iMuteKey[i]){MuteTrack(i);
		//return FALSE;
		}
		if(LOWORD(wParam)==IDM_EZCOPYVISIBLE || LOWORD(wParam)==ID_AC_SHOWEZCOPY){
			if(EZCopyWindowState==0){
				EZCopyWindowState=1;
				ShowWindow(hDlgEZCopy, SW_SHOWNOACTIVATE);
			}else{
				EZCopyWindowState=0;
				ShowWindow(hDlgEZCopy, SW_HIDE);

			}
		}
		for(i=0;i<10;i++){	//�ŋߎg�����t�@�C��
			if(LOWORD(wParam)==Menu_Recent[i]){
				if(CancelDeleteCurrentData(CDCD_LOAD))break;
				SetLoadRecentFile(i);	org_data.InitOrgData();	org_data.LoadMusicData();
				SetTitlebarText(music_file);//�^�C�g���l�[���Z�b�g
				org_data.GetMusicInfo( &mi );
				SetDlgItemInt(hDlgTrack,IDE_VIEWWAIT,mi.wait,TRUE );
				SetDlgItemText(hDlgTrack,IDE_VIEWTRACK,"1");
				ClearEZC_Message();	SelectReset();	org_data.PutMusic();
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				//for(i=0;i<8;i++)ucMIDIProgramChangeValue[i]=255;	// 2014.10.18 D
				for(j=0;j<8;j++)ucMIDIProgramChangeValue[j]=255;	// 2014.10.18 A
				break;	// 2014.10.18 A
			}
		}
		if(timer_sw==0){ //���t���͂����
			switch(LOWORD(wParam)){
			case ID_AC_LOAD_MOST_RECENT:
				SendMessage(hWnd, WM_COMMAND, Menu_Recent[0],0);
				break;
			case IDM_SORTMUSICNOTE: //�\�[�g����
				SetUndo();
				SortMusicNote();
				break;
			case IDM_DLGSETTING://�ݒ�_�C�A���O��\��
			case ID_AC_SETTEMPO:
                DialogBox(hInst,"DLGSETTING",hwnd,(DLGPROC)DialogSetting);
				break;
			case IDM_DLGDEFAULT://�f�t�H���g�_�C�A���O��\��
			case ID_AC_DEFAULT:
                DialogBox(hInst,"DLGDEFAULT",hwnd,(DLGPROC)DialogDefault);
				break;
			case IDM_DLGDELETE://
			case ID_AC_DELETE:
                DialogBox(hInst,"DLGDELETE",hwnd,(DLGPROC)DialogDelete);
				break;
			case IDM_DLGCOPY://
			case ID_AC_COPY:
                DialogBox(hInst,"DLGCOPY",hwnd,(DLGPROC)DialogCopy);
				break;
			case IDM_DLGCOPY2://
			case ID_AC_COPY2:
                DialogBox(hInst,"DLGCOPY2",hwnd,(DLGPROC)DialogCopy2);
				break;
			case IDM_DLGPAN://
			case ID_AC_DLG_PAN:
                DialogBox(hInst,"DLGPAN",hwnd,(DLGPROC)DialogPan);
				break;
			case IDM_DLGTRANS://
			case ID_AC_DLG_TRANS:
                DialogBox(hInst,"DLGTRANS",hwnd,(DLGPROC)DialogTrans);
				break;
			case ID_AC_STPLAY:
				SendMessage(hDlgPlayer , WM_COMMAND , IDC_PLAY , NULL);
				break;
			case ID_AC_STBACK:
				SendMessage(hDlgPlayer , WM_COMMAND , IDC_START , NULL);
				break;
			case IDM_DLGVOL://
			case ID_AC_DLG_VOL:
                DialogBox(hInst,"DLGVOLUME",hwnd,(DLGPROC)DialogVolume);
				break;
			case IDM_DLGUSED://
                DialogBox(hInst,"DLGUSED",hwnd,(DLGPROC)DialogNoteUsed);
				break;
			case IDM_DLGWAVE://�ݒ�_�C�A���O��\��
			case ID_AC_WAVESELECT:
                DialogBox(hInst,"DLGWAVE",hwnd,(DLGPROC)DialogWave);
				break;
			case IDM_DLGSWAP:
			case ID_AC_DLG_SWAP:
                DialogBox(hInst,"DLGSWAP",hwnd,(DLGPROC)DialogSwap);
				break;
			case IDM_DLGMEMO://
				PlaySound( "CAT" , GetModuleHandle(NULL),SND_RESOURCE | SND_ASYNC); 
                DialogBox(hInst,"DLGMEMO",hwnd,(DLGPROC)DialogMemo);
				break;
			case IDM_DLGHELP://
			case ID_AC_HELP:
				//LoadFromResource(IDR_HELPHTML);
				if(!hDlgHelp){
                    hDlgHelp = CreateDialog(hInst,"DLGHELP",hwnd,(DLGPROC)DialogHelp);
				}
				ShowWindow(hDlgHelp, SW_SHOWNOACTIVATE);
				//DialogBox(hInst,"DLGHELP",hwnd,DialogHelp);
				break;
			case IDM_SAVEOVER:
			case ID_AC_MENUOVERSAVE:
				org_data.SaveMusicData();
				ResetTitlebarChange();
				break;							  
			case IDM_SAVENEW://�ʖ��ŕۑ�
			case ID_AC_MENUNEWSAVE:
				res = GetFileNameSave(hwnd,MessageString[IDS_STRING62]); //"�ʖ��ŕۑ�"
				if(res == MSGCANCEL)break;
				if(res == MSGEXISFILE){
					//if(MessageBox(hwnd,"�㏑�����܂����H","�������O�̃t�@�C��������܂�",MB_YESNO | MB_ICONEXCLAMATION)	// 2014.10.19 D
					if(msgbox(hwnd,IDS_NOTIFY_OVERWRITE,IDS_INFO_SAME_FILE,MB_YESNO | MB_ICONEXCLAMATION)	// 2014.10.19 A
						==IDNO)break;
				}
				org_data.SaveMusicData();
				SetTitlebarText(music_file);//�^�C�g���l�[���Z�b�g
				ResetTitlebarChange();
				break;
			case IDM_EXPORT_MIDI: //����߰� 2014.05.11
			case ID_AC_MIDI:
				
				res = GetFileNameMIDI(hwnd,MessageString[IDS_STRING63], strMIDIFile );//"�W��MIDI�`���Ŵ���߰�"
				if(res == MSGCANCEL)break;
				if(res == MSGEXISFILE){
					//if(MessageBox(hwnd,"�㏑�����܂����H","�������O�̃t�@�C��������܂�",MB_YESNO | MB_ICONEXCLAMATION)	// 2014.10.19 D
					if(msgbox(hwnd,IDS_NOTIFY_OVERWRITE,IDS_INFO_SAME_FILE,MB_YESNO | MB_ICONEXCLAMATION)	// 2014.10.19 A
						==IDNO)break;
				}
				org_data.ExportMIDIData(strMIDIFile, iDlgRepeat);
				//SetTitlebarText(music_file);//�^�C�g���l�[���Z�b�g
				//ResetTitlebarChange();
				break;
			case IDM_DUMMY_TATE_SEPARATOR: //�������Ȃ�
				break;
			case IDM_LOAD:
			case ID_AC_MENUOPEN:
			case IDM_LOAD2:
			case ID_AC_LOAD2:
				if(CancelDeleteCurrentData(CDCD_LOAD))break;
				i = 0;
				if(LOWORD(wParam)==IDM_LOAD2 || LOWORD(wParam)==ID_AC_LOAD2)i=1; 
				if(GetFileNameLoad(hWnd,MessageString[IDS_STRING61],i) != MSGLOADOK)break;//"�ȃf�[�^�̓ǂݍ���"
				
				ClearUndo();
				org_data.InitOrgData();
				org_data.LoadMusicData();
				SetTitlebarText(music_file);//�^�C�g���l�[���Z�b�g
				//�v���C���[�ɕ\��
				org_data.GetMusicInfo( &mi );
				SetDlgItemInt(hDlgTrack,IDE_VIEWWAIT,mi.wait,TRUE );
				//SetDlgItemInt(hDlgTrack,IDE_VIEWTRACK,0,TRUE );
				SetDlgItemText(hDlgTrack,IDE_VIEWTRACK,"1");

				ClearEZC_Message();
				SelectReset();
				org_data.PutMusic();
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				for(i=0;i<8;i++)ucMIDIProgramChangeValue[i]=255;
				break;
			case IDM_EXIT:
				//if(iChangeFinish!=0){	// A 2010.09.22
				//	char cc[512],*chn;
				//	GetWindowText(hWnd,cc,512);
				//	chn = strstr(cc, HENKOU_NO_SHIRUSHI);
				//	if(chn!=NULL){
				//		//�ύX���������Ƃ��͏I���m�F����B // A 2010.09.22
				//		if(MessageBox(hwnd,"�ۑ����Ă��Ȃ����e�͔j������܂��B�I�����܂����H","�I���m�F",MB_OKCANCEL | MB_ICONASTERISK)==IDCANCEL)break;
				//	}
				//}
				if(CancelDeleteCurrentData(CDCD_EXIT))break;
				SaveIniFile();
				QuitMMTimer();
				PostQuitMessage(0);
				EndDirectSound();
				org_data.ReleaseNote();
				EndGDI();
				if(!hDlgPlayer)DestroyWindow(hDlgPlayer);
				if(!hDlgTrack)DestroyWindow(hDlgTrack);
				if(!hDlgEZCopy)DestroyWindow(hDlgEZCopy);
				if(!hDlgHelp)DestroyWindow(hDlgHelp);
				
				if(!hwnd)DestroyWindow(hwnd);
				PostQuitMessage(0);
				break;
			case IDM_2BAI:
				SetUndo();
				org_data.EnlargeAllNotes(2);
				scr_data.SetHorzScroll(0);org_data.SetPlayPointer(0);SetFocus(hWnd);//���o��
				org_data.PutMusic();
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				org_data.GetMusicInfo( &mi );
				itoa(mi.wait,str,10);
				SetDlgItemText(hDlgTrack,IDE_VIEWWAIT,str);
				break;
			case IDM_3BAI:
				SetUndo();
				org_data.EnlargeAllNotes(3);
				scr_data.SetHorzScroll(0);org_data.SetPlayPointer(0);SetFocus(hWnd);//���o��
				org_data.PutMusic();
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				org_data.GetMusicInfo( &mi );
				itoa(mi.wait,str,10);
				SetDlgItemText(hDlgTrack,IDE_VIEWWAIT,str);
				break;
			case IDM_2BUNNO1:
				SetUndo();
				org_data.ShortenAllNotes(2);
				scr_data.SetHorzScroll(0);org_data.SetPlayPointer(0);SetFocus(hWnd);//���o��				
				org_data.PutMusic();
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				org_data.GetMusicInfo( &mi );
				itoa(mi.wait,str,10);
				SetDlgItemText(hDlgTrack,IDE_VIEWWAIT,str);
				break;
			case IDM_3BUNNO1:
				SetUndo();
				org_data.ShortenAllNotes(3);
				scr_data.SetHorzScroll(0);org_data.SetPlayPointer(0);SetFocus(hWnd);//���o��				
				org_data.PutMusic();
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				org_data.GetMusicInfo( &mi );
				itoa(mi.wait,str,10);
				SetDlgItemText(hDlgTrack,IDE_VIEWWAIT,str);
				break;
			case IDM_CT_L1: //���`����            IDM_CT_L1�`9�͘A�Ԃł��邱�ƁI
			case IDM_CT_L2: //��ɓʐ���
			case IDM_CT_L3: //���ɓʐ���
			case IDM_CT_L4: //�m�[�}���C�Y
			case IDM_CT_L5: //�O���f�[�V����
			case IDM_CT_L6: //�r�u���[�g
			case IDM_CT_L7: //
			case IDM_CT_L8: //
			case IDM_CT_L9: //
			case IDM_CT_L10:
			case IDM_CT_L11:
			case IDM_CT_L12:
			case IDM_CT_L13:
			case IDM_CT_L14:
			case IDM_CT_L15:
			case IDM_CT_L16:
			case IDM_CT_L17:
			case IDM_CT_L18:
			case IDM_CT_L19:
				SetUndo(); VolumeDecayEdit(1, -4, LOWORD(wParam) - IDM_CT_L1 + 1); //��������0�ɂ���Ƌ�return �����̂�
				break;
			case IDM_CT_S0: 
			case IDM_CT_S1: 
			case IDM_CT_S2: 
			case IDM_CT_S3: 
			case IDM_CT_S4: //������
			case IDM_CT_S5: 
			case IDM_CT_S6: 
			case IDM_CT_S7: 
			case IDM_CT_S8: 
			case IDM_CT_S9: 
			case IDM_CT_S10: 
			case IDM_CT_S11: 
			case IDM_CT_S12: 
			case IDM_CT_S13: 
			case IDM_CT_S14: 
			case IDM_CT_S15: 
			case IDM_CT_S16: 
			case IDM_CT_S17: 
			case IDM_CT_S18: 
			case IDM_CT_S19: 
			case IDM_CT_S20: //�ر
				SetUndo(); VolumeDecayEdit(1, -4, LOWORD(wParam) - IDM_CT_S1 + 1 + 20);
				break;
			case ID_AC_C0://�ر
				SetUndo(); VolumeDecayEdit(1, -4, IDM_CT_S20     - IDM_CT_S1 + 1 + 20); 
				break;
			case ID_AC_C1: //Ctrl+1
			case ID_AC_C2:
			case ID_AC_C3:
			case ID_AC_C4:
			case ID_AC_C5:
			case ID_AC_C6:
			case ID_AC_C7:
			case ID_AC_C8:
			case ID_AC_C9:
				SetUndo(); VolumeDecayEdit(1, -4, LOWORD(wParam) - ID_AC_C1 + 1 + 20);
				break;
			case ID_AC_CA0:
			case ID_AC_CA1:
			case ID_AC_CA2:
			case ID_AC_CA3:
				SetUndo(); VolumeDecayEdit(1, -4, LOWORD(wParam) - ID_AC_CA0 + 1 + 20 + 9);
				break;
			case ID_AC_PRESS_LEFT: //���N���b�N�̑���
				ClickProcL(wParam, lParam);
				break;
			case ID_AC_PRESS_RIGHT: //�E�N���b�N�̑���
				ClickProcR(wParam, lParam);
				break;
			case ID_AC_CT_KEY_OCT_DOWN: //�I�N�^�[�u������
			case IDM_CT_OCT_DOWN:
				SetUndo();
				TransportNote(-12 , -4 );
				break;
			case ID_AC_CT_KEY_OCT_UP:
			case IDM_CT_OCT_UP:
				SetUndo();
				TransportNote(12 , -4 );
				break;
			case ID_AC_CT_PAN_R:
			case IDM_CT_PAN_R:
				SetUndo();
				PanEdit( 1,-4 );
				break;
			case ID_AC_CT_PAN_L:
			case IDM_CT_PAN_L:
				SetUndo();
				PanEdit( -1,-4 );
				break;
			case IDM_CT_PAN_REVERSE: //PAN�t�]
			case ID_AC_PAN_REVERSE:
				SetUndo(); PanEdit( 254, -4 );
				break;
			case ID_AC_CT_KEY_UP:
			case IDM_CT_TRANS_UP:
				SetUndo();
				TransportNote(1 , -4 );
				break;
			case ID_AC_CT_KEY_DOWN:
			case IDM_CT_TRANS_DOWN:
				SetUndo();
				TransportNote(-1 , -4 );
				break;
			case IDM_CT_VOL_PLUS:
			case ID_AC_CT_VOL_UP:
				SetUndo();
				VolumeEdit(5,-4);
				break;
			case IDM_CT_VOL_MINUS:
			case ID_AC_CT_VOL_DOWN:
				SetUndo();
				VolumeEdit(-5,-4);
				break;
			case IDM_CT_VOLWARIAI_UP:
			case ID_AC_VOLWARIAI_UP:
				SetUndo();
				VolumeWariaiEdit(144,-4);
				break;
			case IDM_CT_VOLWARIAI_DOWN:
			case ID_AC_VOLWARIAI_DOWN:
				SetUndo();
				VolumeWariaiEdit(112,-4);
				break;
			case IDM_ML_PAN_R:
				SetUndo();
				PanEdit( 1,-1 );
				break;
			case IDM_ML_PAN_L:
				SetUndo();
				PanEdit( -1,-1 );
				break;
			case IDM_ML_TRANS_UP:
			case ID_AC_ML_KEY_UP:
				SetUndo();
				TransportNote(1 , -1 );
				break;
			case IDM_ML_TRANS_DOWN:
			case ID_AC_ML_KEY_DOWN:
				SetUndo();
				TransportNote(-1 , -1 );
				break;
			case IDM_ML_VOL_PLUS:
			case ID_AC_ML_VOL_UP:
				SetUndo();
				VolumeEdit(5,-1);
				break;
			case IDM_ML_VOL_MINUS:
			case ID_AC_ML_VOL_DOWN:
				SetUndo();
				VolumeEdit(-5,-1);
				break;
			case IDM_DR_VOL_PLUS:
			case ID_AC_DR_VOL_UP:
				SetUndo();
				VolumeEdit(5,-2);
				break;
			case IDM_DR_VOL_MINUS:
			case ID_AC_DR_VOL_DOWN:
				SetUndo();
				VolumeEdit(-5,-2);
				break;
			case IDM_UNDO:
			case ID_AC_UNDO:
				ReplaseUndo();
				org_data.PutMusic();//�y����\��
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				org_data.GetMusicInfo( &mi );
				itoa(mi.wait,str,10);
				SetDlgItemText(hDlgTrack,IDE_VIEWWAIT,str);
				break;
			case IDM_REDO:
			case ID_AC_REDO:
				ReplaceRedo();
				org_data.PutMusic();//�y����\��
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				org_data.GetMusicInfo( &mi );
				itoa(mi.wait,str,10);
				SetDlgItemText(hDlgTrack,IDE_VIEWWAIT,str);
				break;
			case IDM_SELECT_ALL:
			case ID_AC_SELECT_ALL:
				SelectAll(0);
				break;
			case IDM_SELECT_RESET:
			case ID_AC_SELECT_RESET:
				SelectReset();
				break;
			case IDM_GRIDMODE:
			case ID_AC_GRIDMODE:
				ChangeGridMode();
				break;
			case IDM_AUTOLOADPVI:
				ChangeAutoLoadMode();
				break;
			case IDM_ALWAYS_CURRENT:
			case ID_AC_ALWAYS_CURRENT:
				ChangeSelAlwaysCurrent();
				org_data.PutMusic();//�y����\��
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				break;
			case ID_AC_DRAWDOUBLE:
			case IDM_DRAWDOUBLE:
				ChangeDrawDouble();
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				break;
			case IDM_NOTE_ENLARGE: //    �k���\�����A�����̓���ڗ�������(&O) 2014.05.28
				ChangeNoteEnlarge();
				org_data.PutMusic();//�y����\��
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				break;
			case IDM_ENABLEPLAYING: //���t���Ɍ��Ղ�@���� 2010.09.23 A
				ChangeEnablePlaying();
				break;
			case IDM_CHANGEFINISH: //�I�����Ɋm�F���� 2010.09.23 A
				ChangeFinish();
				break;
			case ID_AC_HOMEBACK: //�z�[��
				SendMessage(hDlgPlayer , WM_COMMAND , IDC_START , NULL);
				break;
			case IDM_RECENT_CLEAR:
				ClearRecentFile();
				break;
			case IDM_STOPNOWALL:
				Rxo_StopAllSoundNow();
				break;
			case ID_AC_DRAGMODE:
			case IDM_DRAGMODE:
				ChangeDragMode();
				break;
			case IDM_SLIDEOVERLAPNOTES: //2014.05.06 A
			case ID_AC_SLIDEOVERLAPNOTES:
				ChangeSlideOverlapNoteMode();
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				break;
			case IDM_PRESSNOTE:
			case ID_AC_PRESSNOTE:
				ChangePushStratchNOTE();
				break;
			case IDM_INIT:
			case ID_AC_INIT:
				//if(MessageBox(hwnd,"�ۑ����Ă��Ȃ����e�͔j������܂��B���������܂����H","�������m�F",MB_OKCANCEL)==IDCANCEL)break; //2010.09.25 A
				if(CancelDeleteCurrentData(CDCD_INIT))break;
				ClearUndo();
				memset(music_file, 0 , MAX_PATH);
				strcpy(music_file, MessageString[IDS_DEFAULT_ORG_FILENAME]);
				//for(i = 0; i < 12; i++){
				//	music_file[i] = name[i];
				//}
				org_data.InitOrgData();
				org_data.SetPlayPointer(0);
				scr_data.SetHorzScroll(0);
				//�v���C���[�ɔ��f
				SetDlgItemText(hDlgPlayer,IDE_VIEWWAIT,"128");
				SetDlgItemText(hDlgPlayer,IDE_VIEWMEAS,"0");
				SetDlgItemText(hDlgPlayer,IDE_VIEWXPOS,"0");
				SetTitlebarText(music_file);
				//MessageBox(hwnd,"���������܂���","Message",MB_OK);
				ClearEZC_Message(); //EZ���b�Z�[�W�Ɣ͈͂�����
				SelectReset();
				org_data.PutMusic();
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				for(i=0;i<8;i++)ucMIDIProgramChangeValue[i]=255;

				break;
				//��	// 2010.12.01 A
			case ID_AC_SELECT_CLEAR: //�I��͈͂�����
			case ID_AC_SELECT_CLEAR2: //�I��͈͂����� //2014.04.13
				ClearEZC_Message();
				org_data.PutMusic();
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				break;
			case ID_AC_SELECT_BACKDEL: //2014.04.13
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_DELETEBUTTON_2  , NULL);
				break;
			case ID_AC_SELECT_INSERT: //2014.04.13
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_INSERTBUTTON  , NULL);
				break;
			//�e���L�[����ɂ��͈͑I�𑀍�
			case ID_AC_NUM1:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_CTB1 , NULL);
				break;
			case ID_AC_NUM2:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_CTB2 , NULL);
				break;
			case ID_AC_NUM3:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_CTB3 , NULL);
				break;
			case ID_AC_NUM4:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_CTB4 , NULL);
				break;
			case ID_AC_NUM5:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_CTB5 , NULL);
				break;
			case ID_AC_NUM6:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_CTB6 , NULL);
				break;
			case ID_AC_C_NUM1:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_CTB7 , NULL);
				break;
			case ID_AC_C_NUM2:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_CTB8 , NULL);
				break;
			case ID_AC_C_NUM3:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_CTB9 , NULL);
				break;
			case ID_AC_C_NUM4:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_CTB10 , NULL);
				break;
			case ID_AC_C_NUM5:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_CTB11 , NULL);
				break;
			case ID_AC_C_NUM6:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_CTB12 , NULL);
				break;
			case ID_AC_NUM7:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_PST1 , NULL);
				break;
			case ID_AC_NUM8:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_PST2 , NULL);
				break;
			case ID_AC_NUM9:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_PST3 , NULL);
				break;
			case ID_AC_NUMPLUS:
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_PST4 , NULL);
				break;
			case ID_AC_DELETEKEY: //Add 2014/04/12
				SendMessage(hDlgEZCopy , WM_COMMAND , IDC_DELETEBUTTON , NULL);
				break;
			}
		}else{
			//���t���̂�
			//���ɖ���
		}
		//���t���ł���
		switch(LOWORD(wParam)){
		case IDM_LOUPE_MINUS:
		case ID_AC_LOUPE_MINUS:
			NoteWidth -= 2; if(NoteWidth<4)NoteWidth=4;
			org_data.PutBackGround();
			org_data.PutMusic();//�y����\��
			RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
			switch(NoteWidth){
			case 4:		SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[25.%]"); break;
			case 6:		SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[37.5%]"); break;
			case 8:		SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[50%]"); break;
			case 10:	SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[62.5%]"); break;
			case 12:	SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[75%]"); break;
			case 14:	SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[87.5%]"); break;
			case 16:	SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[100%]"); break;
			}
			CheckLoupeMenu();
			break;
		case IDM_LOUPE_PLUS:
		case ID_AC_LOUPE_PLUS:
			NoteWidth += 2; if(NoteWidth>16)NoteWidth=16;
			org_data.PutBackGround();
			org_data.PutMusic();//�y����\��
			RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
			switch(NoteWidth){
			case 4:		SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[25%]"); break;
			case 6:		SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[37.5%]"); break;
			case 8:		SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[50%]"); break;
			case 10:	SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[62.5%]"); break;
			case 12:	SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[75%]"); break;
			case 14:	SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[87.5%]"); break;
			case 16:	SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, "[100%]"); break;
			}
			CheckLoupeMenu();
			break;
		}

		break;
	case WM_ACTIVATEAPP:
		actApp = wParam;
		break;
	case WM_MENUSELECT:

		switch(LOWORD(wParam)){
		default: SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, ""); break;
		case ID_MENUITEM40265:      SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING78]); break;
		case IDM_EXPORT_MIDI:       SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING79]); break;
		case IDM_LOAD2:             SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING80]); break;
		case IDM_SAVEOVER:          SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, music_file); break; 
		case IDM_SAVENEW:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING81]); break; 
		case IDM_RECENT1:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, RecentFileName[0]); break;
		case IDM_RECENT2:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, RecentFileName[1]); break;
		case IDM_RECENT3:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, RecentFileName[2]); break;
		case IDM_RECENT4:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, RecentFileName[3]); break;
		case IDM_RECENT5:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, RecentFileName[4]); break;
		case IDM_RECENT6:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, RecentFileName[5]); break;
		case IDM_RECENT7:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, RecentFileName[6]); break;
		case IDM_RECENT8:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, RecentFileName[7]); break;
		case IDM_RECENT9:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, RecentFileName[8]); break;
		case IDM_RECENT0:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, RecentFileName[9]); break;
		case IDM_EXIT:              SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING82]); break;
		case IDM_DLGWAVE:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING83]); break;
		case IDM_DLGSETTING:        SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING84]); break;
		case IDM_DLGDEFAULT:        SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING85]); break;
		case IDM_CT_L0:             SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING86]); break;
		case IDM_CT_S0:             SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING87]); break;
		case IDM_DLGUSED:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING88]); break;
		case IDM_DRAWDOUBLE:        SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING89]); break;
		case IDM_SLIDEOVERLAPNOTES: SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING90]); break;
		case IDM_LOUPE_PLUS:        SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING91]); break;
		case IDM_LOUPE_MINUS:       SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING92]); break;
		case IDM_PRESSNOTE:         SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING93]); break;
		case IDM_GRIDMODE:          SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING94]); break;
		case IDM_ALWAYS_CURRENT:    SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING95]); break;
		case IDM_DRAGMODE:          SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING96]); break;
		case IDM_ENABLEPLAYING:     SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING97]); break;
		case IDM_SORTMUSICNOTE:     SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING98]); break;
		case IDM_STOPNOWALL:        SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING99]); break;
		case IDM_RECENT_CLEAR:      SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING100]); break;
		case IDM_AUTOLOADPVI:       SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING101]); break;
		case IDM_DLGHELP:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING102]); break;
		case IDM_DLGMEMO:           SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING103]); break;		}
		break;
	case WM_DROPFILES://�t�@�C���h���b�v
		//SetWindowPos(hWnd, HWND_TOP, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE );
		//SendMessage(hWnd, WM_PAINT, 0, 0);
		//SetFocus(hWnd);
		SetForegroundWindow(hwnd);
		if(CancelDeleteCurrentData(CDCD_LOAD))break;
		//DragQueryFile((HDROP)wParam,0,music_file,MAX_PATH);	// 2014.05.22 D
		DragQueryFile((HDROP)wParam,0,strMIDIFile,MAX_PATH);	// 2014.05.22 A
		if(org_data.FileCheckBeforeLoad(strMIDIFile)){
			SetDlgItemText(hDlgEZCopy, IDC_MESSAGE, MessageString[IDS_STRING64]); //<!>�t�@�C�����J���Ȃ��A�܂��́A�`�����s���ł��B
			break;
		}
		strcpy(music_file, strMIDIFile);

		ClearUndo();
//		MessageBox(hWnd,music_file,"",MB_OK);
		org_data.InitOrgData();
		org_data.LoadMusicData();
		org_data.PutMusic();//�y����\��
		RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
			//�v���C���[�ɕ\��
			org_data.GetMusicInfo( &mi );
			SetDlgItemInt(hDlgTrack,IDE_VIEWWAIT,mi.wait,TRUE );
			//SetDlgItemInt(hDlgTrack,IDE_VIEWTRACK,0,TRUE );
			SetDlgItemText(hDlgTrack,IDE_VIEWTRACK,"1");
		SetTitlebarText(music_file);//�^�C�g���l�[���Z�b�g
		break;
	case WM_PAINT://�\�����b�Z�[�W
		HDC hdc;
		PAINTSTRUCT ps;

		hdc = BeginPaint(hWnd,&ps);
		RefleshScreen(hdc);
		//static TCHAR strSize[128];
		//wsprintf(strSize , "width = %d : height = %d" , WWidth , WHeight);
		TextOut(hdc , 10 , 10 , strSize , lstrlen(strSize));
		EndPaint(hWnd,&ps);


		break;
	case WM_CLOSE:
		//if(iChangeFinish!=0){	// A 2010.09.22
		//	char cc[512],*chn;
		//	GetWindowText(hWnd,cc,512);
		//	chn = strstr(cc, HENKOU_NO_SHIRUSHI);
		//	if(chn!=NULL){
		//		//�ύX���������Ƃ��͏I���m�F����B // A 2010.09.22
		//		if(MessageBox(hwnd,"�ۑ����Ă��Ȃ����e�͔j������܂��B�I�����܂����H","�I���m�F",MB_OKCANCEL| MB_ICONASTERISK)==IDCANCEL)break;
		//	}
		//}
		if(CancelDeleteCurrentData(CDCD_EXIT))break;

		SaveIniFile();
		QuitMMTimer();
		PostQuitMessage(0);
		break;
	case WM_QUIT:
		break;
	case WM_DESTROY://�㏈���p
		EndDirectSound();
		org_data.ReleaseNote();
		DeleteWaveData100(); //�ǉ�20140401 �ʏ�AWM_CLOSE �� WM_DESTROY �� WM_QUIT �̏��ɌĂ΂��炵���B
		EndGDI();
		if(!hDlgPlayer)DestroyWindow(hDlgPlayer);
		if(!hDlgTrack)DestroyWindow(hDlgTrack);
		if(!hDlgEZCopy)DestroyWindow(hDlgEZCopy);
		if(!hwnd)DestroyWindow(hwnd);
		PostQuitMessage(0);
		free(strMIDIFile); //2014.05.11
		FreeMessageStringBuffer();	// 2014.10.19 
		break;
	case WM_KEYDOWN://�L�[�{�[�h�������ꂽ
		switch(wParam){
		case VK_UP:
			scr_data.KeyScroll(DIRECTION_UP);
			break;
		case VK_DOWN:
			scr_data.KeyScroll(DIRECTION_DOWN);
			break;
		case VK_LEFT:
			scr_data.KeyScroll(DIRECTION_LEFT);
			break;
		case VK_RIGHT:
			scr_data.KeyScroll(DIRECTION_RIGHT);
			break;
		case VK_F5:
		case VK_NUMPAD0:
			if(timer_sw == 0)SendMessage(hDlgPlayer , WM_COMMAND , IDC_PLAY , NULL);
			else SendMessage(hDlgPlayer , WM_COMMAND , IDC_STOP , NULL);
			break;
		//case VK_HOME:
		//	SendMessage(hDlgPlayer , WM_COMMAND , IDC_START , NULL);
		//	break;
		case 'Z':
		case 'S':
		case 'X':
		case 'C':
		case 'F':
		case 'V':
		case 'G':
		case 'B':
		case 'N':
		case 'J':
		case 'M':
		case 'K':
		case 0xBC:
		case 'L':
		case 0xBE:
		case 0xBF:
		case 0xBA:
		case 0xE2:
		case 0xDD:

			if((lParam & 0x40000000) ==0 && (timer_sw==0 || iChangeEnablePlaying!=0)){
				iKeyPhase[iCast[wParam]]=iCurrentPhase;
				iCurrentPhase=-iCurrentPhase + 1;
				Rxo_PlayKey(iCast[wParam] + iPushShift[0]*12 -iPushShift[1]*12 , org_data.track, 1000, iKeyPhase[iCast[wParam]]);
				iKeyPushDown[iCast[wParam]+ iPushShift[0]*12 -iPushShift[1]*12] = 1;
				org_data.PutMusic();//�y���̍ĕ`��
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);

			}

			break;
		case VK_TAB:
			if((lParam & 0x40000000) ==0 && (timer_sw==0 || iChangeEnablePlaying!=0)) iPushShift[0]=1;
			Rxo_StopAllSoundNow();
			break;
		case VK_SHIFT:
			if((lParam & 0x40000000) ==0 && (timer_sw==0 || iChangeEnablePlaying!=0)) iPushShift[1]=1;
			Rxo_StopAllSoundNow();
			break;
		}
		break;
//				wsprintf(strSize , "U lParam = %x" , lParam);
//				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
	case WM_KEYUP: //�L�[�{�[�h�������ꂽ
		switch(wParam){
		case 'Z':
		case 'S':
		case 'X':
		case 'C':
		case 'F':
		case 'V':
		case 'G':
		case 'B':
		case 'N':
		case 'J':
		case 'M':
		case 'K':
		case 0xBC:
		case 'L':
		case 0xBE:
		case 0xBF:
		case 0xBA:
		case 0xE2:
		case 0xDD:
			if((timer_sw==0 || iChangeEnablePlaying!=0)){
				Rxo_StopKey(iCast[wParam]+ iPushShift[0]*12 -iPushShift[1]*12, org_data.track, iKeyPhase[iCast[wParam]]);
				iKeyPhase[iCast[wParam]] = -1;
				iKeyPushDown[iCast[wParam]+ iPushShift[0]*12 -iPushShift[1]*12] = 0;
				org_data.PutMusic();//�y���̍ĕ`��
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
			}
			break;
		case VK_TAB:
			if((timer_sw==0 || iChangeEnablePlaying!=0)){
				iPushShift[0]=0;
				for(i=0;i<256;i++)iKeyPushDown[i]=0;
				org_data.PutMusic();//�y���̍ĕ`��
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				Rxo_StopAllSoundNow();
			}
			break;
		case VK_SHIFT:
			if((timer_sw==0 || iChangeEnablePlaying!=0)){
				iPushShift[1]=0;
				for(i=0;i<256;i++)iKeyPushDown[i]=0;
				org_data.PutMusic();//�y���̍ĕ`��
				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
				Rxo_StopAllSoundNow();
			}
			break;
		}
		break;

    case WM_LBUTTONDOWN://�}�E�X(��)�������ꂽ
		ClickProcL(wParam, lParam);
		break;
    case WM_RBUTTONDOWN://�}�E�X(�E)�������ꂽ
		ClickProcR(wParam, lParam);
		break;
    case WM_MBUTTONDOWN://�}�E�X(����)�������ꂽ
		ClickProcM(wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		MouseDrag(wParam, lParam);
		break;
    case WM_LBUTTONUP://�}�E�X(��)�������ꂽ
		LButtonUP(wParam, lParam);
		break;
    case WM_RBUTTONUP://�}�E�X(�E)�������ꂽ
		RButtonUP(wParam, lParam);
		RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
		break;
	case WM_HSCROLL:
		scr_data.HorzScrollProc(wParam);
		break;
	case WM_VSCROLL:
		scr_data.VertScrollProc(wParam);
		break;
	case WM_MOUSEWHEEL:
		scr_data.WheelScrollProc(lParam, wParam);		
		break;
	case WM_GETMINMAXINFO:
			pmmi = (MINMAXINFO *)lParam;
			if ( pmmi )
			{
				pmmi->ptMinTrackSize.x = 420;  // �ŏ���
				pmmi->ptMinTrackSize.y = 480;  // �ŏ���
				//pmmi->ptMaxTrackSize.x = 1024; // �ő啝
				//pmmi->ptMaxTrackSize.y = 768;  // �ő卂
			}
		
		break;
	case 0x020C: //���ȃ{�^������
		//MessageBox(hWnd,"�ǂݍ��݂Ɏ��s���܂���","Error(Load)",MB_OK);
		switch HIWORD(wParam){
		case 0x0001: //��
			ChangeTrackPlus(hDlgTrack , 1);
			break;
		case 0x0002: //��
			ChangeTrackPlus(hDlgTrack , -1);
			break;
		}
		break;
	case WM_SIZE:
		WWidth = LOWORD(lParam);	//�N���C�A���g�̈�̃T�C�Y
		WHeight = HIWORD(lParam);
		rect.right = WWidth;		//A 2008/05/14
		rect.bottom = WHeight;		//A 2008/05/14
		if(!org_data.PutBackGround())break;
		org_data.PutMusic();
		
//				wsprintf(strSize , "Height = %d" , (WHeight - 158)/12);
//				RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
		scr_data.ChangeVerticalRange(WHeight);
		RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);

		break;
	case WM_SIZING: //�傫����ύX��
		//org_data.PutBackGround();
		//org_data.PutMusic();
		
		//RedrawWindow(hWnd,&rect,NULL,RDW_INVALIDATE|RDW_ERASENOW);
		
		//AfterReSize = true;
		break;
	//�ȉ��̓e�X�g�p
/*	case WM_MOUSEMOVE:
		char str[80];
		long mouse_x,mouse_y;
//		HDC hdc;
		hdc = GetDC(hWnd);
		mouse_data.GetMousePosition(&mouse_x,&mouse_y);
		sprintf(str,"Mouse X:%4d Mouse Y:%4d  ",mouse_x,mouse_y);
		TextOut(hdc,500,16,str,strlen(str));
		ReleaseDC(hWnd,hdc);
		break;
*/
	default:return DefWindowProc(hwnd, message, wParam, lParam);//�ȊO��������
	}

	return FALSE;
}
//�^�C�g���o�[�Ƀt�@�C���l�[����\��
void SetTitlebarText(char *name)
{
	int i,j;
	char set_name[MAX_PATH+15];//�^�C�g���ɕ\����߰�
	char file_name[MAX_PATH];//���O�����H�i�f�B���N�g����r���j

	i = 0;
	while(name[i] != NULL)i++;//�܂��͐K�܂�
	while(i != 0 && name[i-1] != '\\')i--; //���X�g�̉~�}�[�N
	
	//�t�@�C����������
	j = 0;
	while(name[i] != NULL){
		file_name[j] = name[i];
		i++;
		j++;
	}
	file_name[j] = NULL;
	//�A�v���^�C�g���𗬂�����
	for(i = 0; i < 15; i++){
		set_name[i] = lpszName[i];
	}
	//�t�@�C�����𗬂�����
	for(j = 0; j < MAX_PATH; j++){
		set_name[i] = file_name[j];
		if(set_name[i] == NULL)break;
		i++;
	}
	SetWindowText(hWnd,&set_name[0]);
}


//�ύX�A���̈�B
void SetTitlebarChange(void)
{
	char cc[512],*chn;
	GetWindowText(hWnd,cc,512);
	//chn = strstr(cc, HENKOU_NO_SHIRUSHI);	// 2014.10.19 D
	chn = strstr(cc, MessageString[IDS_MODIFIED]);	// 2014.10.19 A
	if(chn==NULL){
		//strcat(cc, HENKOU_NO_SHIRUSHI);	// 2014.10.19 D
		strcat(cc, MessageString[IDS_MODIFIED]);	// 2014.10.19 A
		SetWindowText(hWnd,cc);
	}

}
void ResetTitlebarChange(void)
{
	char cc[512],*chn;
	GetWindowText(hWnd,cc,512);
	//chn = strstr(cc, HENKOU_NO_SHIRUSHI);	// 2014.10.19 D
	chn = strstr(cc, MessageString[IDS_MODIFIED]);	// 2014.10.19 A
	if(chn!=NULL){
		*chn = '\0';//����
		*(chn+1) = '\0';//����
		SetWindowText(hWnd,cc);
	}
}

void SaveIniFile()
{
	int i;
	WINDOWPLACEMENT WPM;
	WPM.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hWnd, &WPM);
	if(WPM.showCmd == SW_SHOWMAXIMIZED)i=1;else i=0;
	ShowWindow( hWnd, SW_RESTORE );	//���̃T�C�Y�j

	GetWindowRect(hWnd,(LPRECT)&WinRect);
	wsprintf(num_buf,"%d",WinRect.left);
	WritePrivateProfileString(MAIN_WINDOW,"left",num_buf,app_path);
	wsprintf(num_buf,"%d",WinRect.top);
	WritePrivateProfileString(MAIN_WINDOW,"top",num_buf,app_path);
	wsprintf(num_buf,"%d",WinRect.right - WinRect.left);
	WritePrivateProfileString(MAIN_WINDOW,"right",num_buf,app_path);
	wsprintf(num_buf,"%d",WinRect.bottom - WinRect.top);
	WritePrivateProfileString(MAIN_WINDOW,"bottom",num_buf,app_path);
	wsprintf(num_buf,"%d",i);
	WritePrivateProfileString(MAIN_WINDOW,"WindowState",num_buf,app_path);
	wsprintf(num_buf,"%d",sGrid);
	WritePrivateProfileString(MAIN_WINDOW,"GridMode",num_buf,app_path);
	wsprintf(num_buf,"%d",sACrnt);
	WritePrivateProfileString(MAIN_WINDOW,"AlwaysCurrent",num_buf,app_path);
	wsprintf(num_buf,"%d",gDrawDouble);
	WritePrivateProfileString(MAIN_WINDOW,"DrawDouble",num_buf,app_path);
	wsprintf(num_buf,"%d",iDragMode);
	WritePrivateProfileString(MAIN_WINDOW,"DragMode",num_buf,app_path);
	wsprintf(num_buf,"%d",iChangeEnablePlaying);
	WritePrivateProfileString(MAIN_WINDOW,"EnablePressNoteStretch",num_buf,app_path);
	wsprintf(num_buf,"%d",iPushStratch);
	WritePrivateProfileString(MAIN_WINDOW,"EnablePlaying",num_buf,app_path);
	wsprintf(num_buf,"%d",iChangeFinish);
	WritePrivateProfileString(MAIN_WINDOW,"QuitMessage",num_buf,app_path);
	wsprintf(num_buf,"%d",iSlideOverlapNotes);
	WritePrivateProfileString(MAIN_WINDOW,"SlideOverlapNoteMode",num_buf,app_path);
	wsprintf(num_buf,"%d",NoteWidth);
	WritePrivateProfileString(MAIN_WINDOW,"NoteWidth",num_buf,app_path);
	wsprintf(num_buf,"%d",NoteEnlarge_Until_16px);
	WritePrivateProfileString(MAIN_WINDOW,"NoteEnlarge",num_buf,app_path);


	GetWindowRect(hDlgTrack,(LPRECT)&WinRect);
	wsprintf(num_buf,"%d",WinRect.left);
	WritePrivateProfileString(TRACK_WINDOW,"left",num_buf,app_path);
	wsprintf(num_buf,"%d",WinRect.top);
	WritePrivateProfileString(TRACK_WINDOW,"top",num_buf,app_path);

	GetWindowRect(hDlgPlayer,(LPRECT)&WinRect);
	wsprintf(num_buf,"%d",WinRect.left);
	WritePrivateProfileString(PLAY_WINDOW,"left",num_buf,app_path);
	wsprintf(num_buf,"%d",WinRect.top);
	WritePrivateProfileString(PLAY_WINDOW,"top",num_buf,app_path);
	GetWindowRect(hDlgEZCopy,(LPRECT)&WinRect);
	wsprintf(num_buf,"%d",WinRect.left);
	WritePrivateProfileString(COPY_WINDOW,"left",num_buf,app_path);
	wsprintf(num_buf,"%d",WinRect.top);
	WritePrivateProfileString(COPY_WINDOW,"top",num_buf,app_path);
	wsprintf(num_buf,"%d",EZCopyWindowState);
	WritePrivateProfileString(COPY_WINDOW,"show",num_buf,app_path);

	wsprintf(num_buf,"%d",CmnDialogWnd.left);
	WritePrivateProfileString(COMMON_WINDOW,"left",num_buf,app_path);
	wsprintf(num_buf,"%d",CmnDialogWnd.top);
	WritePrivateProfileString(COMMON_WINDOW,"top",num_buf,app_path);
	wsprintf(num_buf,"%d",CmnDialogWnd.right );
	WritePrivateProfileString(COMMON_WINDOW,"right",num_buf,app_path);
	wsprintf(num_buf,"%d",CmnDialogWnd.bottom );
	WritePrivateProfileString(COMMON_WINDOW,"bottom",num_buf,app_path);
	wsprintf(num_buf,"%d",iDlgRepeat );
	WritePrivateProfileString(MIDI_EXPORT,"Repeat",num_buf,app_path);

	WritePrivateProfileString(MIDI_EXPORT,"Author", strMIDI_AUTHOR, app_path);
	WritePrivateProfileString(MIDI_EXPORT,"Title", strMIDI_TITLE, app_path);

	wsprintf(num_buf,"%d",SaveWithInitVolFile );
	WritePrivateProfileString(INIT_DATA,"autosave",num_buf,app_path);

	SaveRecentFilesToInifile();
	scr_data.SetIniFile();

}

/*
//�I�[�v�j���O�t���b�V��
BOOL CALLBACK DialogFlash(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
	case WM_INITDIALOG:
		int i;
		long screen_w;
		long screen_h;
		long my_w;
		long my_h;
		long x,y,w,h;

		//�X�N���[���T�C�Y
		screen_w = GetSystemMetrics(SM_CXSCREEN);
		screen_h = GetSystemMetrics(SM_CYSCREEN);
		//�E�C���h�E�T�C�Y
		my_w = gWidthWindow;
		my_h = gHeightWindow;

		MoveWindow( hDlg, screen_w/2, screen_h/2, 1, 1, TRUE );
		ShowWindow( hDlg, SW_SHOW );
		for( i = 1; i <= WAITFLASH; i++ ){
			x = screen_w/2 - ( my_w/2 * i/WAITFLASH );
			y = screen_h/2 - ( my_h/2 * i/WAITFLASH );
			w = my_w * i / WAITFLASH;
			h = my_h * i / WAITFLASH;
			MoveWindow( hDlg, x, y, w, h, TRUE );
		}
		ShowWindow(hWnd,SW_SHOW);
		EndDialog(hDlg,0);
		return 1;
	default:return FALSE;
	}
	return TRUE;
}
*/

void CheckLoupeMenu(void)
{
	if(NoteWidth == 4){
		EnableMenuItem(GetMenu(hWnd), IDM_LOUPE_MINUS, MF_GRAYED);
	}else{
		EnableMenuItem(GetMenu(hWnd), IDM_LOUPE_MINUS, MF_ENABLED);
	}
	if(NoteWidth == 16){
		EnableMenuItem(GetMenu(hWnd), IDM_LOUPE_PLUS, MF_GRAYED);
	}else{
		EnableMenuItem(GetMenu(hWnd), IDM_LOUPE_PLUS, MF_ENABLED);
	}

}
