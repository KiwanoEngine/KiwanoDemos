// Copyright (C) 2019 Nomango

#include "TextDemo.h"
#include "ActionDemo.h"
#include "EaseActionDemo.h"
#include "ButtonDemo.h"
#include "InputDemo.h"
#include "ShapeDemo.h"
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
	DECLARE_DEMO(ActionDemo),
	DECLARE_DEMO(EaseActionDemo),
	DECLARE_DEMO(TextDemo),
	DECLARE_DEMO(ButtonDemo),
	DECLARE_DEMO(InputDemo),
	DECLARE_DEMO(ShapeDemo),
	DECLARE_DEMO(AudioDemo),
	DECLARE_DEMO(AnimationDemo),
	DECLARE_DEMO(NetworkDemo),
};

KGE_DECLARE_SMART_PTR(DemoRunner);
class DemoRunner
	: public Runner
{
public:
	DemoRunner()
	{
		// ʹ�� Audio ģ��
		Application::GetInstance().Use(AudioModule::GetInstance());

		// ʹ�� HttpClient ģ��
		Application::GetInstance().Use(HttpModule::GetInstance());

		// ʹ�� ImGui ģ��
		Application::GetInstance().Use(ImGuiModule::GetInstance());

		// ��Ϸ����
		Settings settings;
		settings.title = "Kiwano Demos";
		settings.width = WINDOW_WIDTH;
		settings.height = WINDOW_HEIGHT;
		settings.icon = Icon(IDI_ICON1);
		SetSettings(settings);
	}

	void OnReady() override
	{
		// �л�����һ����̨
		EnterDemo(s_Demos[0]);
	}

	void EnterDemo(Demo& demo)
	{
		// �޸Ĵ��ڱ���
		String title = demo.title;
		Application::GetInstance().GetWindow()->SetTitle("Kiwano Demo - " + title);

		// ������̨
		StagePtr scene = demo.Create();
		Director::GetInstance().EnterStage(scene);

		// ����GUI�������
		ImGuiLayerPtr control_panel = ImGuiLayer::Create("Control", Closure(this, &DemoRunner::ControlPanel));
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
};

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
#if defined(_WIN32)
	// Enable memory-leak reports
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif

	try
	{
		DemoRunnerPtr runner = new DemoRunner;
		Application::GetInstance().Run(runner);
	}
	catch (std::exception& e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
