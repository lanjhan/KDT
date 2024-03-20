#include "Engine.h"
#include "Task/Task.h"
#include "MISC/Utils.h"

void FEngine::Run()
{
	Init();

	while (!bTerminated)
	{
		system("pause");
		const EMenuEvent Event = PrintMenu();
		switch (Event)
		{
		case ETerminated:
		{
			bTerminated = true;
			break;
		}
		case ECreateAccount:
		{
			FCreateAccountTask Task;
			break;
		}
		case EDeleteAccount:
		{
			FDeleteAccountTask Task;
			break;
		}
		}
	}

	Release();
}

void FEngine::Init()
{
	cout << format("[System]Start {} \n", __FUNCTION__);
}

void FEngine::Release()
{
	cout << format("[System]Shutdown {} \n", __FUNCTION__);
}

EMenuEvent FEngine::PrintMenu()
{
	system("cls");
	cout << "[---------------------------]" << endl;
	cout << "[ Please input event number ]" << endl;
	cout << "[ 0) 프로그램 종료          ]" << endl;
	cout << "[ 1) 회원 가입              ]" << endl;
	cout << "[ 2) 회원 탈퇴              ]" << endl;
	cout << "[---------------------------]" << endl;
	cout << "--> ";

	int Event = 0;
	cin >> Event;

	return static_cast<EMenuEvent>(Event);
}
