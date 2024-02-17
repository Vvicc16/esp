#pragma once
//ģ�� client.dll
DWORD toStruct_ptr = 0x4E051DC; // ->����ṹ��
DWORD toHealth_ptr = 0x100; // ->Ѫ��
DWORD toXSet_ptr = 0xAC; // ->x����
DWORD toCamp_ptr = 0xF4; // ->��Ӫ

DWORD toMatrix_ptr = 0x4DF6134; // ->����ͷ

struct human
{
	DWORD struct_h = 0;
	DWORD health = 0;
	DWORD camp = 0;
	FLOAT x = 0;
	FLOAT y = 0;
	FLOAT z = 0;
};

struct ViewMatrix
{
	DWORD_PTR headaddr = 0;
	FLOAT Matrix[4][4];
};

struct Camera
{
	FLOAT x, y, y2, z;
};

