// Copyright (C) 2019 Nomango

#include "Demo1.h"
#include "Demo2.h"
#include "Demo3.h"
#include "Demo4.h"
#include "Demo5.h"
#include "resource.h"

namespace
{
	struct Demo
	{
		String title;
		StagePtr(*Create)();
	};

	Demo s_Demos[] = {
		{ L"����ʾ��", Demo1::Create },
		{ L"����ʾ��", Demo2::Create },
		{ L"��Ƶ����ʾ��", Demo3::Create },
		{ L"֡����ʾ��", Demo4::Create },
		{ L"��������ʾ��", Demo5::Create },
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
		// ʹ�� Audio ���
		Use(Audio::GetInstance());

		// ʹ�� HttpClient ���
		Use(HttpClient::GetInstance());

		Config config;
		config.window.title = L"Kiwanoʾ������";
		config.window.width = WINDOW_WIDTH;
		config.window.height = WINDOW_HEIGHT;
		config.window.icon = IDI_ICON1;

		Init(config);
	}

	void OnReady() override
	{
		// �� JSON �ļ��ж�ȡ��Դ��Ϣ
		//ResourceCache::GetInstance()->LoadFromJsonFile(L"res/index.json");

		// �� XML �ļ��ж�ȡ��Դ��Ϣ
		ResourceCache::GetInstance()->LoadFromXmlFile(L"res/index.xml");

		// �л�����һ����̨
		ChangeDemoStage(0);
	}

	void ChangeDemoStage(int index)
	{
		if (s_CurrIndex != index)
		{
			s_CurrIndex = index;

			String title = s_Demos[index].title;
			Window::GetInstance()->SetTitle(L"Kiwanoʾ������ - " + title);

			StagePtr scene = s_Demos[index].Create();
			Director::GetInstance()->EnterStage(scene);

			// ��Ӱ�������
			scene->AddListener(Event::KeyUp, Closure(this, &DemoApp::KeyPressed));

			// ��ʾ��ʾ����
			String intro_str = String::format(L"���� 1~%d ���л�ʾ��\n", s_DemoNum);
			TextPtr intro = new Text(intro_str + title);
			intro->SetFontSize(16.f);
			intro->SetPosition(10, 10);
			scene->AddChild(intro);
		}
	}

	void KeyPressed(Event const& evt)
	{
		KGE_ASSERT(evt.type == Event::KeyUp);

		if (evt.key.code > KeyCode::Num0 &&
			evt.key.code <= (KeyCode::Num0 + s_DemoNum))
		{
			int index = evt.key.code - KeyCode::Num1;
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
	catch (std::exception& e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
