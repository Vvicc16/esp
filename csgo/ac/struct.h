#pragma once
//模块 client.dll
DWORD toStruct_ptr = 0x4E051DC; // ->人物结构体
DWORD toHealth_ptr = 0x100; // ->血量
DWORD toXSet_ptr = 0xAC; // ->x坐标
DWORD toCamp_ptr = 0xF4; // ->阵营

DWORD toMatrix_ptr = 0x4DF6134; // ->矩阵头

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

