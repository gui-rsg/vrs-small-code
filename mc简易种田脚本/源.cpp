#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<time.h>

int main()
{
	int fps = 120;

	srand((unsigned int)time(NULL));

	bool start_farm = false;
	int active_type = 0;

	DWORD delta_time = 0;

	DWORD walk_timestart = 0;
	DWORD walk_timeend = 1;
	DWORD walk_time = 0;

	bool walk_stop = true;
	DWORD walk_sustain_start = 0;
	DWORD walk_sustain_end = 0;
	DWORD walk_sustain_time = 0;

	DWORD walk_timestart_left_right = 0;
	DWORD walk_timeend_left_right = 1;
	DWORD walk_left_right = 0;
	char left_right = 'A';

	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;

	printf("注意:\n1.换快捷栏的键位是U和I\n");
	printf("2.停止或结束程序后需要点一下鼠标右键来恢复鼠标右键为抬起状态\n");
	printf("O开始\n");
	printf("P停止\n");
	printf("M结束程序\n");

	bool running = true;
	while (running)
	{
		DWORD start_time = GetTickCount64();

		if (!start_farm)
		{
			start_farm = GetAsyncKeyState('O');
		}
		else
		{
			walk_sustain_end = GetTickCount64();
			walk_sustain_time = walk_sustain_time + (walk_sustain_end - walk_sustain_start);
			walk_sustain_start = GetTickCount64();
			if (walk_stop)
			{
				input.ki.wVk = 'W';
				input.ki.dwFlags = 0;
				SendInput(1, &input, sizeof(INPUT));
				if (walk_sustain_time >= 180)
				{
					walk_sustain_time = 0;
					walk_stop = false;
				}
			}
			else if (!walk_stop)
			{
				input.ki.wVk = 'W';
				input.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &input, sizeof(INPUT));
				if (walk_sustain_time >= 231)
				{
					walk_sustain_time = 0;
					walk_stop = true;
				}
			}
			
			walk_timeend = GetTickCount64();
			walk_time = walk_time + (walk_timeend - walk_timestart);
			walk_timestart = GetTickCount64();
			//printf("[%d]", walk_time);
			if (walk_time >= 8) 
			{
				if (active_type == 0)
				{
					mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
					active_type = 1;
				}
				else if (active_type == 1)
				{
					input.ki.wVk = 'U';
					input.ki.dwFlags = 0;
					SendInput(1, &input, sizeof(INPUT));
					active_type = 2;

					input.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &input, sizeof(INPUT));
				}
				else if (active_type == 2)
				{
					mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
				    mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
					active_type = 3;
				}
				else if (active_type == 3)
				{
					input.ki.wVk = 'I';
					input.ki.dwFlags = 0;
					SendInput(1, &input, sizeof(INPUT));
					active_type = 0;

					input.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &input, sizeof(INPUT));
				}
				walk_time = 0;
			}

			/*walk_timeend_left_right = GetTickCount64();
			walk_left_right = walk_left_right + (walk_timeend_left_right - walk_timestart_left_right);
			walk_timestart_left_right = GetTickCount64();
			if (walk_left_right >= 500)
			{
				input.ki.wVk = left_right;
				input.ki.dwFlags = KEYEVENTF_KEYUP;

				input.ki.wVk = left_right;
				input.ki.dwFlags = 0;
				SendInput(1, &input, sizeof(INPUT));

				if (left_right == 'A')
				{
					left_right = 'D';
				}
				else
				{
					left_right = 'A';
				}
				walk_left_right = 0;
			}*/
			


			if (GetAsyncKeyState('P'))
			{
				start_farm = false;
			}
		}

		if (GetAsyncKeyState('M'))
		{
			//input.ki.wVk = 'W';
			//input.ki.dwFlags = KEYEVENTF_KEYUP;
			//SendInput(1, &input, sizeof(INPUT));
			mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
			break;
		}

		DWORD end_time = GetTickCount64();
		delta_time = end_time - start_time;
		if (delta_time < (1000 / fps))
		{
			Sleep(1000 / fps - delta_time);
		}
	}

	return 0;
}