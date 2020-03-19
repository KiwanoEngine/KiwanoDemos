// Copyright (C) 2019 Nomango

#include "PhysicBodyDemo.h"
#include "PhysicJointDemo.h"
#include "PhysicContactDemo.h"

struct Demo
{
	String title;
	String label;
	Function<StagePtr()> Create;
};

#define DECLARE_DEMO(DEMO_NAME) { DEMO_NAME::Name(), DEMO_NAME::Label(), DEMO_NAME::Create }

Demo s_Demos[] = {
	DECLARE_DEMO(PhysicBodyDemo),
	DECLARE_DEMO(PhysicJointDemo),
	DECLARE_DEMO(PhysicContactDemo),
};

KGE_DECLARE_SMART_PTR(DemoRunner);
class DemoRunner
	: public Runner
{
public:
	DemoRunner()
	{
		// ʹ�� ImGui ģ��
		Application::GetInstance().Use(ImGuiModule::GetInstance());

		// ��������
		WindowPtr window = Window::Create("Kiwano Physic Demos", 800, 600);
		SetMainWindow(window);
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
		Application::GetInstance().GetMainWindow()->SetTitle("Kiwano Physic Demos - " + title);

		// ������̨
		StagePtr scene = demo.Create();
		Director::GetInstance().EnterStage(scene);

		// ����ı�˵��
		TextActorPtr intro = TextActor::Create(demo.label);
		intro->SetFillColor(Color::White);
		intro->SetAnchor(0.5f, 0.5f);
		intro->SetPosition(scene->GetWidth() / 2, 50);
		scene->AddChild(intro);

		// ����GUI�������
		ImGuiLayerPtr control_panel = ImGuiLayer::Create("Control", Closure(this, &DemoRunner::ControlPanel));
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
		DemoRunnerPtr runner = new DemoRunner;
		Application::GetInstance().Run(runner);
	}
	catch (std::exception & e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
