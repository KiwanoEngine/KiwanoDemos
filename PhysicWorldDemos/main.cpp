// Copyright (C) 2019 Nomango

#include "Demo1.h"
#include "Demo2.h"
#include "Demo3.h"

namespace
{
	struct Demo
	{
		String title;
		StagePtr(*Create)();
	};

	Demo s_Demos[] = {
		{ L"��̬�붯̬����", Demo1::Create },
		{ L"�ؽ����˶�", Demo2::Create },
		{ L"�Ӵ�����", Demo3::Create },
	};
	int s_CurrIndex = -1;
	int s_DemoNum = sizeof(s_Demos) / sizeof(Demo);
}

class DemoApp
	: public Application
{
public:
	DemoApp()
	{
		Config config;
		config.window.title = L"��������ʾ��";
		config.window.width = 1200;
		config.window.height = 900;

		Init(config);
	}

	void OnReady() override
	{
		// �л�����һ����̨
		ChangeDemoStage(0);
	}

	void ChangeDemoStage(int index)
	{
		if (s_CurrIndex != index)
		{
			s_CurrIndex = index;

			String title = s_Demos[index].title;
			Window::GetInstance()->SetTitle(L"��������ʾ�� - " + title);

			StagePtr scene = s_Demos[index].Create();
			Director::GetInstance()->EnterStage(scene);

			// ��Ӱ�������
			scene->AddListener(event::KeyUp, Closure(this, &DemoApp::KeyPressed));

			// ��ʾ��ʾ����
			String intro_str = String::format(L"���� 1~%d ���л�ʾ��\n", s_DemoNum);
			TextPtr intro = new Text(intro_str + title);
			intro->SetFontSize(16.f);
			intro->SetPosition(10, 10);
			scene->AddChild(intro);
		}
	}

	void KeyPressed(Event& evt)
	{
		KGE_ASSERT(evt.type == event::KeyUp);

		auto key_evt = dynamic_cast<KeyUpEvent&>(evt);
		if (key_evt.code > KeyCode::Num0&&
			key_evt.code <= (KeyCode::Num0 + s_DemoNum))
		{
			int index = key_evt.code - KeyCode::Num1;
			ChangeDemoStage(index);
		}
	}
};

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	try
	{
		DemoApp app;
		app.Run();
	}
	catch (std::exception & e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
