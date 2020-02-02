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
		Use(&AudioEngine::Instance());

		// ʹ�� HttpClient ���
		Use(&HttpClient::Instance());

		// ��������
		Window::Instance().Create(L"Kiwano Demos", WINDOW_WIDTH, WINDOW_HEIGHT, IDI_ICON1);
	}

	void OnReady() override
	{
		// �� JSON �ļ��ж�ȡ��Դ��Ϣ
		//ResourceCache::Instance().LoadFromJsonFile(L"res/index.json");

		// �� XML �ļ��ж�ȡ��Դ��Ϣ
		ResourceCache::Instance().LoadFromXmlFile(L"res/index.xml");

		// �л�����һ����̨
		ChangeDemoStage(0);
	}

	void ChangeDemoStage(int index)
	{
		if (s_CurrIndex != index)
		{
			s_CurrIndex = index;

			String title = s_Demos[index].title;
			Window::Instance().SetTitle(L"Kiwanoʾ������ - " + title);

			StagePtr scene = s_Demos[index].Create();
			Director::Instance().EnterStage(scene);

			// ��Ӱ�������
			scene->AddListener<KeyUpEvent>(Closure(this, &DemoApp::KeyPressed));

			// ��ʾ��ʾ����
			String intro_str = String::format(L"���� 1~%d ���л�ʾ��\n", s_DemoNum);
			TextActorPtr intro = new TextActor(intro_str + title);
			intro->SetFontSize(16.f);
			intro->SetPosition(10, 10);
			scene->AddChild(intro);
		}
	}

	void KeyPressed(Event* evt)
	{
		KGE_ASSERT(evt->IsType<KeyUpEvent>());

		KGE_ASSERT(evt->IsType<KeyUpEvent>());

		auto key_evt = dynamic_cast<KeyUpEvent*>(evt);

		int index = int(key_evt->code) - int(KeyCode::Num1);
		if (index >= 0 && index < s_DemoNum)
		{
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
