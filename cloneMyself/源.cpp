#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<wchar.h>
#include<locale.h>
#include<io.h>

int main()
{
	setlocale(LC_ALL, "chs");

	//�����ж��ļ��Ƿ���ȷ��
	errno_t source_open;
	errno_t destination_open;

	FILE* source;
	FILE* destination;

	wchar_t self_path[MAX_PATH];
	DWORD path_length;

	const int fps = 2;
	bool running = true;
	while (running)
	{
		DWORD start_time = GetTickCount64();

		wprintf(L"�������ˣ��찴P����\n");

		//��õ�ǰ���г���ľ���·��
		path_length = GetModuleFileNameW(NULL, self_path, MAX_PATH);
		wprintf(L"%ls\n", self_path);
		if (path_length == 0 || path_length > MAX_PATH)
		{
			wprintf(L"faile to get path of exe");
		}

		//��Դ�ļ�
		source_open = _wfopen_s(&source, self_path, L"rb");
		if (source_open == NULL)
		{
			wprintf(L"fiale to open source file\n");
		}

		//��ָ���ƶ����ļ�β
		fseek(source, 0, SEEK_END);
		//����ļ����ݳ���
		long long int size = ftell(source);
		//��ָ���ƶ����ļ�ͷ
		rewind(source);

		//����Ŀ���ļ����ļ���
		wchar_t destination_filename[16];
		srand((unsigned int)time(NULL));
		for (int i = 0; i < 11; i++)
		{
			destination_filename[i] = rand() % 9 + 48;
		}
		destination_filename[11] = '.';
		destination_filename[12] = 'e';
		destination_filename[13] = 'x';
		destination_filename[14] = 'e';
		destination_filename[15] = '\0';

		//��Ŀ���ļ�
		destination_open = _wfopen_s(&destination, destination_filename, L"wb");
		if (destination_open == NULL)
		{
			wprintf(L"fiale to creat destination file\n");
		}

		HANDLE hFile = (HANDLE)_get_osfhandle(_fileno(destination));

		//��ȡԴ�ļ����ݲ�д��Ŀ���ļ�
		wchar_t file_content;
		wprintf(L"%lld\n", size);
		for (int i = 0; i < size; i++)
		{
			file_content = fgetwc(source);
			fputwc(file_content, destination);
		}

		//���˵���Ч����β
		fseek(destination, -(size), SEEK_CUR);
		//�ض��ļ�����
		SetEndOfFile(hFile);

		//�ر��ļ����
		FindClose(hFile);

		//�ر��ļ�
		fclose(source);
		fclose(destination);

		DWORD end_time = GetTickCount64();
		DWORD delta_time = end_time - start_time;
		if (delta_time < 1000 / fps)
		{
			Sleep(1000 / fps - delta_time);
		}

		system("cls");

		if (GetAsyncKeyState('P'))
		{
			exit(0);
			break;
		}
	}

	return 0;
}