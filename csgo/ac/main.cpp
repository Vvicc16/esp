#include"head.h"
#include"struct.h"
#include"d3dDraw/D3D����.h"

const wchar_t* ProcessName = L"csgo.exe";  //������
const wchar_t* ModuleName = L"client.dll";  //ģ����
DWORD pid = GPid_Name(ProcessName);  //����id
HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);  //���̾��
HWND hWindow = FindWindow(NULL, L"Counter-Strike: Global Offensive - Direct3D 9");  //���ھ��

int line_width = 2;  //�������ߴ�

RECT WindowRectangle;
int WindowHeigh, WindowWidth;
ViewMatrix VM;
Camera cam;
human ME;
int PlayerAccount;

//���ƺ���
void DrawFunction()
{
	���ƿ�ʼ();

	ME.struct_h = RPMP_D(hProcess, pid, ModuleName, NULL, toStruct_ptr);
	for (int k = 1; RPMP_D(hProcess, pid, ModuleName, NULL, toStruct_ptr + 0x10 * k) != 0; k++) PlayerAccount = k + 1;


	VM.headaddr = GetModuleBaseAddress(pid, ModuleName) + toMatrix_ptr;
	for (int i = 0; i <= 3; i++) {
		for (int j = 0; j <= 3; j++) {
			VM.Matrix[i][j] = RPM_F(hProcess, VM.headaddr + 0x10 * i + 0x4 * j, NULL);
		}
	} // ��ȡviewMatrix


	//�������Լ����������
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

		����(D3DCOLOR_ARGB(255, 0, 0, 255), 20, 20, 66, 66, line_width);
		��������(cam.x, cam.y, "��", D3DCOLOR_ARGB(255, 255, 255, 255));
		//����(D3DCOLOR_ARGB(255, 0, 0, 255), cam.x - 45, cam.y, cam.x + 45, cam.y, line_width);
		//����(D3DCOLOR_ARGB(255, 0, 0, 255), cam.x - 45, cam.y, cam.x - 45, cam.y - 170, line_width);
		//����(D3DCOLOR_ARGB(255, 0, 0, 255), cam.x + 45, cam.y, cam.x + 45, cam.y - 170, line_width);
		//����(D3DCOLOR_ARGB(255, 0, 0, 255), cam.x - 45, cam.y - 170, cam.x + 45, cam.y - 170, line_width);
		����(cam.x - squarW / 2, cam.y, squarW, squarH, line_width, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	

	���ƽ���();
}

//��ʼ����
void Start_Draw()
{
	����͸������(hWindow, DrawFunction);
	������Ϣѭ��();
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