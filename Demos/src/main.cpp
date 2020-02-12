// Copyright (C) 2019 Nomango

#include "TextDemo.h"
#include "EaseActionDemo.h"
#include "ButtonDemo.h"
#include "InputDemo.h"
#include "AudioDemo.h"
#include "AnimationDemo.h"
#include "NetworkDemo.h"
#include "resource.h"

struct Demo
{
	String title;
	Function<StagePtr()> Create;
};

#define DECLARE_DEMO(DEMO_NAME) { DEMO_NAME::DemoName(), DEMO_NAME::Create }

Demo s_Demos[] = {
	DECLARE_DEMO(EaseActionDemo),
	DECLARE_DEMO(TextDemo),
	DECLARE_DEMO(ButtonDemo),
	DECLARE_DEMO(InputDemo),
	DECLARE_DEMO(AudioDemo),
	DECLARE_DEMO(AnimationDemo),
	DECLARE_DEMO(NetworkDemo),
};

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

		// 使用 ImGui 组件
		Use(&ImGuiModule::GetInstance());

		// 创建窗口
		Window::GetInstance().Create("Kiwano Demos", WINDOW_WIDTH, WINDOW_HEIGHT, IDI_ICON1);
	}

	void OnReady() override
	{
		// 切换到第一个舞台
		EnterDemo(s_Demos[0]);
	}

	void EnterDemo(Demo& demo)
	{
		// 修改窗口标题
		String title = demo.title;
		Window::GetInstance().SetTitle("Kiwano Demo - " + title);

		// 创建舞台
		StagePtr scene = demo.Create();
		Director::GetInstance().EnterStage(scene);

		// 创建GUI控制面板
		ImGuiLayerPtr control_panel = ImGuiLayer::Create("Control", Closure(this, &DemoApp::ControlPanel));
		scene->AddChild(control_panel);
	}

	void ControlPanel()
	{
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(170, 200), ImGuiCond_FirstUseEver);

		ImGui::Begin("Select demos", nullptr, ImGuiWindowFlags_NoSavedSettings);

		for (auto& demo : s_Demos)
		{
			if (ImGui::Button(demo.title.c_str()))
			{
				EnterDemo(demo);
			}
		}

		ImGui::End();
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
