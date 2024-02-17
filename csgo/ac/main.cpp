#include"head.h"
#include"struct.h"
#include"d3dDraw/D3D绘制.h"

const wchar_t* ProcessName = L"csgo.exe";  //进程名
const wchar_t* ModuleName = L"client.dll";  //模块名
DWORD pid = GPid_Name(ProcessName);  //进程id
HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);  //进程句柄
HWND hWindow = FindWindow(NULL, L"Counter-Strike: Global Offensive - Direct3D 9");  //窗口句柄

int line_width = 2;  //绘制用线粗

RECT WindowRectangle;
int WindowHeigh, WindowWidth;
ViewMatrix VM;
Camera cam;
human ME;
int PlayerAccount;

//绘制函数
void DrawFunction()
{
	绘制开始();

	ME.struct_h = RPMP_D(hProcess, pid, ModuleName, NULL, toStruct_ptr);
	for (int k = 1; RPMP_D(hProcess, pid, ModuleName, NULL, toStruct_ptr + 0x10 * k) != 0; k++) PlayerAccount = k + 1;


	VM.headaddr = GetModuleBaseAddress(pid, ModuleName) + toMatrix_ptr;
	for (int i = 0; i <= 3; i++) {
		for (int j = 0; j <= 3; j++) {
			VM.Matrix[i][j] = RPM_F(hProcess, VM.headaddr + 0x10 * i + 0x4 * j, NULL);
		}
	} // 获取viewMatrix


	//遍历除自己外所有玩家
	human Player;
	ME.camp = RPMP_D(hProcess, pid, ModuleName, NULL, toStruct_ptr, toCamp_ptr);
	for (int i = 1; i <= PlayerAccount - 1; i++) {
		GetWindowRect(hWindow, &WindowRectangle);
		WindowWidth = WindowRectangle.right - WindowRectangle.left;
		WindowHeigh = WindowRectangle.bottom - WindowRectangle.top;
		Player.camp = RPMP_D(hProcess, pid, ModuleName, NULL, toStruct_ptr + 0x10 * i, toCamp_ptr);
		Player.health = RPMP_D(hProcess, pid, ModuleName, NULL, toStruct_ptr + 0x10 * i, toHealth_ptr);
		if (Player.camp == ME.camp or Player.health == 0) continue;
		Player.x = RPMP_F(hProcess, pid, ModuleName, NULL, toStruct_ptr + 0x10 * i, toXSet_ptr);
		Player.y = RPMP_F(hProcess, pid, ModuleName, NULL, toStruct_ptr + 0x10 * i, toXSet_ptr + 0x4);
		Player.z = RPMP_F(hProcess, pid, ModuleName, NULL, toStruct_ptr + 0x10 * i, toXSet_ptr + 0x8);

		cam.z = Player.x * VM.Matrix[3][0] + Player.y * VM.Matrix[3][1] + Player.z * VM.Matrix[3][2] + VM.Matrix[3][3];
		if (cam.z < 0) continue;
		cam.x = WindowWidth / 2 + (Player.x * VM.Matrix[0][0] + Player.y * VM.Matrix[0][1] + Player.z * VM.Matrix[0][2] + VM.Matrix[0][3]) / cam.z * WindowWidth / 2;
		cam.y = WindowHeigh / 2 - (Player.x * VM.Matrix[1][0] + Player.y * VM.Matrix[1][1] + (Player.z - 6) * VM.Matrix[1][2] + VM.Matrix[1][3]) / cam.z * WindowHeigh / 2;
		cam.y2 = WindowHeigh / 2 - (Player.x * VM.Matrix[1][0] + Player.y * VM.Matrix[1][1] + (Player.z + 75) * VM.Matrix[1][2] + VM.Matrix[1][3]) / cam.z * WindowHeigh / 2;
		int squarH = cam.y2 - cam.y;
		int squarW = squarH / 2.5;

		画线(D3DCOLOR_ARGB(255, 0, 0, 255), 20, 20, 66, 66, line_width);
		绘制文字(cam.x, cam.y, "。", D3DCOLOR_ARGB(255, 255, 255, 255));
		//画线(D3DCOLOR_ARGB(255, 0, 0, 255), cam.x - 45, cam.y, cam.x + 45, cam.y, line_width);
		//画线(D3DCOLOR_ARGB(255, 0, 0, 255), cam.x - 45, cam.y, cam.x - 45, cam.y - 170, line_width);
		//画线(D3DCOLOR_ARGB(255, 0, 0, 255), cam.x + 45, cam.y, cam.x + 45, cam.y - 170, line_width);
		//画线(D3DCOLOR_ARGB(255, 0, 0, 255), cam.x - 45, cam.y - 170, cam.x + 45, cam.y - 170, line_width);
		画框(cam.x - squarW / 2, cam.y, squarW, squarH, line_width, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	

	绘制结束();
}

//开始绘制
void Start_Draw()
{
	创建透明窗口(hWindow, DrawFunction);
	窗口消息循环();
}

int main()
{
	while (pid == 0) {
		std::cout << "cant found game." << std::endl;
		Sleep(100);
		system("cls");
	}
	
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)getinfo, NULL, 0, NULL);

	Start_Draw();
}