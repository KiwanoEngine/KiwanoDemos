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
		{ "静态与动态物体", Demo1::Create },
		{ "关节与运动", Demo2::Create },
		{ "接触监听", Demo3::Create },
	};
	int s_CurrIndex = -1;
	int s_DemoNum = sizeof(s_Demos) / sizeof(Demo);
}

KGE_DECLARE_SMART_PTR(DemoRunner);
class DemoRunner
	: public Runner
{
public:
	DemoRunner()
	{
		// 创建窗口
		WindowPtr window = Window::Create("Physics World", 1200, 900);
		SetMainWindow(window);
	}

	void OnReady() override
	{
		// 切换到第一个舞台
		ChangeDemoStage(0);
	}

	void ChangeDemoStage(int index)
	{
		if (s_CurrIndex != index)
		{
			s_CurrIndex = index;

			String title = s_Demos[index].title;
			Application::GetInstance().GetMainWindow()->SetTitle("物理引擎示例 - " + title);

			StagePtr scene = s_Demos[index].Create();
			Director::GetInstance().EnterStage(scene);

			// 添加按键监听
			scene->AddListener<KeyUpEvent>(Closure(this, &DemoRunner::KeyPressed));

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
		DemoRunnerPtr runner = new DemoRunner;
		Application::GetInstance().Run(runner);
	}
	catch (std::exception & e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
