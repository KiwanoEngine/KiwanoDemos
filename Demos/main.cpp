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
		{ "动画示例", Demo1::Create },
		{ "输入示例", Demo2::Create },
		{ "音频播放示例", Demo3::Create },
		{ "帧动画示例", Demo4::Create },
		{ "网络请求示例", Demo5::Create },
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
		// 使用 Audio 组件
		Use(&AudioEngine::GetInstance());

		// 使用 HttpClient 组件
		Use(&HttpClient::GetInstance());

		// 创建窗口
		Window::GetInstance().Create("Kiwano Demos", WINDOW_WIDTH, WINDOW_HEIGHT, IDI_ICON1);
	}

	void OnReady() override
	{
		// 从 JSON 文件中读取资源信息
		//ResourceCache::GetInstance().LoadFromJsonFile("res/index.json");

		// 从 XML 文件中读取资源信息
		ResourceCache::GetInstance().LoadFromXmlFile("res/index.xml");

		// 切换到第一个舞台
		ChangeDemoStage(0);
	}

	void ChangeDemoStage(int index)
	{
		if (s_CurrIndex != index)
		{
			s_CurrIndex = index;

			String title = s_Demos[index].title;
			Window::GetInstance().SetTitle("Kiwano示例程序 - " + title);

			StagePtr scene = s_Demos[index].Create();
			Director::GetInstance().EnterStage(scene);

			// 添加按键监听
			scene->AddListener<KeyUpEvent>(Closure(this, &DemoApp::KeyPressed));

			// 显示提示文字
			String intro_str = String::format("按键 1~%d 可切换示例\n", s_DemoNum);
			TextActorPtr intro = TextActor::Create(intro_str + title);
			intro->SetFillColor(Color::White);
			intro->SetFontSize(16.f);
			intro->SetPosition(10, 10);
			scene->AddChild(intro);
		}
	}

	void KeyPressed(Event* evt)
	{
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
