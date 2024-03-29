// Copyright (C) 2019 Nomango

#include "PhysicBodyDemo.h"
#include "PhysicJointDemo.h"
#include "PhysicContactDemo.h"

struct Demo
{
	String title;
	String label;
	Function<RefPtr<Stage>()> Create;
};

#define DECLARE_DEMO(DEMO_NAME) { DEMO_NAME::Name(), DEMO_NAME::Label(), DEMO_NAME::Create }

Demo s_Demos[] = {
	DECLARE_DEMO(PhysicBodyDemo),
	DECLARE_DEMO(PhysicJointDemo),
	DECLARE_DEMO(PhysicContactDemo),
};

class DemoRunner
	: public Runner
{
public:
	DemoRunner()
	{
		// 游戏设置
		Settings settings;
		settings.window.title = "Kiwano Physic Demos";
		settings.window.width = 800;
		settings.window.height = 600;
		this->SetSettings(settings);

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
		Application::GetInstance().GetWindow()->SetTitle("Kiwano Physic Demos - " + title);

		// 创建舞台
		RefPtr<Stage> scene = demo.Create();
		Director::GetInstance().EnterStage(scene);

		// 添加文本说明
		RefPtr<TextActor> intro = new TextActor(demo.label);
		intro->SetFillColor(Color::White);
		intro->SetAnchor(0.5f, 0.5f);
		intro->SetPosition(scene->GetWidth() / 2, 50);
		scene->AddChild(intro);

		// 创建GUI控制面板
		RefPtr<imgui::Layer> control_panel = new imgui::Layer("Control", Closure(this, &DemoRunner::ControlPanel));
		scene->AddChild(control_panel);
	}

	void ControlPanel()
	{
		ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(170, 160), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowCollapsed(false, ImGuiCond_FirstUseEver);

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
#if defined(_WIN32)
	// Enable memory-leak reports
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif

	try
	{
		RefPtr<DemoRunner> runner = new DemoRunner;
		Application::GetInstance().Run(runner);
	}
	catch (std::exception & e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
