#pragma once
#include"Ԥ����.h"

/*
D3D��ص�һЩ����
���������static������ȫ�ֱ����Ļ���ǧ�����ͷ�ļ���������cpp�ļ�������
����ͷ�ļ�extern ������ ������
*/
static MARGINS Margin;
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
static ID3DXLine* pLine = 0;
static ID3DXFont* Font;

static HWND �������ھ��, GameHwnd;
static RECT ���ھ���;
static int ���ڿ�, ���ڸ�;

//ע�ᴰ����Ҫ�õ��Ĵ�����
static WNDCLASSEX wClass;


//�����Σ�����֮��ĵ����������������
typedef void(*Draw)();
static Draw Render;


//������Ϣ������
LRESULT WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

bool ��ʼ��D3D();

void ����͸������(HWND ��Ϸ���ھ��, Draw ���ƺ���);

void ������Ϣѭ��();

void ����(D3DCOLOR Color, float X1, float Y1, float X2, float Y2, float Width);

void ��������(float X, float Y, const char* Str, D3DCOLOR Color);

void ����(float X, float Y, float W, float H, float Width, D3DCOLOR Color);

void ���ƿ�ʼ();

void ���ƽ���();
