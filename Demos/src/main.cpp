// Copyright (C) 2019 Nomango

#include "TextDemo.h"
#include "AnimationDemo.h"
#include "EaseAnimationDemo.h"
#include "ButtonDemo.h"
#include "InputDemo.h"
#include "ShapeDemo.h"
#include "AudioDemo.h"
#include "FrameAnimationDemo.h"
#include "resource.h"

struct Demo
{
	String title;
	Function<RefPtr<Stage>()> Create;
};

#define DECLARE_DEMO(DEMO_NAME) { DEMO_NAME::DemoName(), DEMO_NAME::Create }

Demo s_Demos[] = {
	DECLARE_DEMO(AnimationDemo),
	DECLARE_DEMO(EaseAnimationDemo),
	DECLARE_DEMO(TextDemo),
	DECLARE_DEMO(ButtonDemo),
	DECLARE_DEMO(InputDemo),
	DECLARE_DEMO(ShapeDemo),
	DECLARE_DEMO(AudioDemo),
	DECLARE_DEMO(FrameAnimationDemo),
};

class DemoRunner
	: public Runner
{
public:
	DemoRunner()
	{
		// 游戏设置
		Settings settings;
		settings.window.title = "Kiwano Demos";
		settings.window.width = WINDOW_WIDTH;
		settings.window.height = WINDOW_HEIGHT;
		settings.window.icon = Icon(IDI_ICON1);
		this->SetSettings(settings);

		// 使用 Audio 模块
		Application::GetInstance().Use(audio::Module::GetInstance());

		// 使用 ImGui 模块
		Application::GetInstance().Use(imgui::Module::GetInstance());
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
		Application::GetInstance().GetWindow()->SetTitle("Kiwano Demo - " + title);

		// 创建舞台
		RefPtr<Stage> scene = demo.Create();
		Director::GetInstance().EnterStage(scene, NewRandomTransition());

		// 创建GUI控制面板
		RefPtr<imgui::Layer> control_panel = new imgui::Layer("Control", Closure(this, &DemoRunner::ControlPanel));
		scene->AddChild(control_panel);
	}

	void ControlPanel()
	{
		ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(170, 200), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowCollapsed(true, ImGuiCond_FirstUseEver);

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

	RefPtr<Transition> NewRandomTransition()
	{
		// 生成随机的舞台过渡动画
		RefPtr<Transition> transition;
		switch (math::Random(0, 4))
		{
		case 0:
			transition = new FadeTransition(500_msec);
			break;
		case 1:
			transition = new FadeTransition(500_msec, true);
			break;
		case 2:
			transition = new MoveTransition(500_msec, MoveTransition::Type::Left);
			break;
		case 3:
			transition = new RotationTransition(500_msec);
			break;
		case 4:
			transition = new BoxTransition(500_msec);
			break;
		}
		return transition;
	}
};

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
#if defined(_WIN32)
	// Enable memory-leak reports
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif

	try
	{
		RefPtr<DemoRunner> runner = new DemoRunner;
		Application::GetInstance().Run(runner);
	}
	catch (std::exception& e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
