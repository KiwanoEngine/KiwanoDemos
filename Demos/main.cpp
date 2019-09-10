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
		{ L"动画示例", Demo1::Create },
		{ L"输入示例", Demo2::Create },
		{ L"音频播放示例", Demo3::Create },
		{ L"帧动画示例", Demo4::Create },
		{ L"网络请求示例", Demo5::Create },
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
		Use(Audio::GetInstance());

		// 使用 HttpClient 组件
		Use(HttpClient::GetInstance());

		Config config;
		config.window.title = L"Kiwano示例程序";
		config.window.width = WINDOW_WIDTH;
		config.window.height = WINDOW_HEIGHT;
		config.window.icon = IDI_ICON1;

		Init(config);
	}

	void OnReady() override
	{
		// 从 JSON 文件中读取资源信息
		//ResourceCache::GetInstance()->LoadFromJsonFile(L"res/index.json");

		// 从 XML 文件中读取资源信息
		ResourceCache::GetInstance()->LoadFromXmlFile(L"res/index.xml");

		// 切换到第一个舞台
		ChangeDemoStage(0);
	}

	void ChangeDemoStage(int index)
	{
		if (s_CurrIndex != index)
		{
			s_CurrIndex = index;

			String title = s_Demos[index].title;
			Window::GetInstance()->SetTitle(L"Kiwano示例程序 - " + title);

			StagePtr scene = s_Demos[index].Create();
			Director::GetInstance()->EnterStage(scene);

			// 添加按键监听
			scene->AddListener(Event::KeyUp, Closure(this, &DemoApp::KeyPressed));

			// 显示提示文字
			String intro_str = String::format(L"按键 1~%d 可切换示例\n", s_DemoNum);
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
