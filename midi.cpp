#include<iostream>
#include<windows.h>
#include<thread>
#include<conio.h>
#pragma comment(lib,"winmm.lib")
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?0:1)
using namespace std;
int i, change;

//枚举音调
enum Note
{
	Rest = 0, C8 = 108, B7 = 107, A7s = 106, A7 = 105, G7s = 104, G7 = 103, F7s = 102, F7 = 101, E7 = 100,
	D7s = 99, D7 = 98, C7s = 97, C7 = 96, B6 = 95, A6s = 94, A6 = 93, G6s = 92, G6 = 91, F6s = 90, F6 = 89,
	E6 = 88, D6s = 87, D6 = 86, C6s = 85, C6 = 84, B5 = 83, A5s = 82, A5 = 81, G5s = 80, G5 = 79, F5s = 78,
	F5 = 77, E5 = 76, D5s = 75, D5 = 74, C5s = 73, C5 = 72, B4 = 71, A4s = 70, A4 = 69, G4s = 68, G4 = 67,
	F4s = 66, F4 = 65, E4 = 64, D4s = 63, D4 = 62, C4s = 61, C4 = 60, B3 = 59, A3s = 58, A3 = 57, G3s = 56,
	G3 = 55, F3s = 54, F3 = 53, E3 = 52, D3s = 51, D3 = 50, C3s = 49, C3 = 48, B2 = 47, A2s = 46, A2 = 45,
	G2s = 44, G2 = 43, F2s = 42, F2 = 41, E2 = 40, D2s = 39, D2 = 38, C2s = 37, C2 = 36, B1 = 35, A1s = 34,
	A1 = 33, G1s = 32, G1 = 31, F1s = 30, F1 = 29, E1 = 28, D1s = 27, D1 = 26, C1s = 25, C1 = 24, B0 = 23,
	A0s = 22, A0 = 21
};

//键入与音调对应关系
Note GetNote(char c)
{
	switch (c)
	{
	case 'Z':return C3; break;
	case 'X':return D3; break;
	case 'C':return E3; break;
	case 'V':return F3; break;
	case 'B':return G3; break;
	case 'N':return A3; break;
	case 'M':return B3; break;
	case 'A':return C4; break;
	case 'S':return D4; break;
	case 'D':return E4; break;
	case 'F':return F4; break;
	case 'G':return G4; break;
	case 'H':return A4; break;
	case 'J':return B4; break;
	case 'Q':return C5; break;
	case 'W':return D5; break;
	case 'E':return E5; break;
	case 'R':return F5; break;
	case 'T':return G5; break;
	case 'Y':return A5; break;
	case 'U':return B5; break;
	default:
		break;
	}
}

//改变音调
void ChangeScale()
{
lable: cout << "请选择音调" << endl;
	char input;
	cin >> input;
	switch (input)
	{
	case 'C': change = 0; break;  // 切换到C大调
	case 'D': change = 2; break;  // 切换到D大调
	case 'E': change = 4; break;  // 切换到E大调
	case 'F': change = 5; break;  // 切换到F大调
	case 'G': change = 7; break;  // 切换到G大调
	case 'A': change = 9; break;  // 切换到A大调
	case 'B': change = 11; break;  // 切换到B大调
	case 'c': change = 3; break;  // 切换到c小调
	case 'd': change = 5; break;  // 切换到d小调
	case 'e': change = 7; break;  // 切换到e小调
	case 'f': change = 8; break;  // 切换到f小调
	case 'g': change = 10; break;  // 切换到g小调
	case 'a': change = 10; break;  // 切换到A#小调
	case 'b': change = 2; break;  // 切换到b小调
	default:cout << "重输"; goto lable;
		break;
	}
}

//播放
void PlayShortNote(HMIDIOUT handle, int note)
{
	midiOutShortMsg(handle, note + change);
}

//检测按键
void Play(char key, const HMIDIOUT& handle, int volume)
{
	short state = GetAsyncKeyState(key);
	Note note = GetNote(key);
	if (state & 0x8000)
	{
		if (KEY_DOWN(key))
		{
			midiOutShortMsg(handle, (volume << 16) + (note + change << 8) + 0x90);
		}
		system("cls");
		printf("已按下%c键\n", key);
		while (KEY_DOWN(key))
			Sleep(1000);
		if (KEY_UP(key))
		{
			midiOutShortMsg(handle, (volume << 16) + (note + change << 8) + 0x80);
		}
	}
}

int main()
{

	int volume;
	char key;
	Note note;

	cout << "按下enter开始弹奏" << endl;
	while (1)
	{
		if (cin.get() == '\n')
			break;
	}

	HMIDIOUT handle;
	unsigned long result = midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
	if (result)
	{
		cout << "error!" << endl;
	}
	cout << "请选择乐器音色" << endl;
	cin >> i;
	midiOutShortMsg(handle, i << 8 | 0xC0);
	cout << "请调节音量" << endl;
	cin >> volume;
	midiOutShortMsg(handle, i << 8 | 0xC0);
	ChangeScale();

	cout << "演奏中可以通过空格键结束运行" << endl;

	while (1)
	{
		key = getch();//获取键入
		if (key == ' ')
			break;
		thread(Play, key, handle, volume).detach();
	}

	midiOutClose(handle);

	if (!result)
	{
		cout << "关闭成功！" << endl;
	}
	else
	{
		cout << "关闭失败！" << endl;
	}
}
