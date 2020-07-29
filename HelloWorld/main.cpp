// Copyright (C) 2019 Nomango

#include <kiwano/kiwano.h>

using namespace kiwano;

class MyRunner : public Runner
{
public:
	MyRunner()
	{
		// ��Ϸ����
		Settings settings;
		settings.window.title = "Hello World";
		settings.window.width = 640;
		settings.window.height = 480;
		settings.bg_color = Color::Rgb(0xE5E5E5);

		SetSettings(settings);
	}

	void OnReady() override
	{
		// ������̨
		StagePtr stage = new Stage;

		// ��������
		SpritePtr sprite = new Sprite("logo.png");

		// �޸ľ���λ��, ʹ��������Ļ�Ͼ���
		sprite->SetPosition(640 / 2, 480 / 2);

		// �޸ľ���ê��, ʹͼƬ���Ķ�����Ļ����
		sprite->SetAnchor(0.5, 0.5);

		// �޸�������, ͼƬ��С�� 0.5 ��
		sprite->SetScale(0.5f, 0.5f);

		// ��ӵ���̨��
		stage->AddChild(sprite);

		// ������̨
		Director::GetInstance().EnterStage(stage);
	}
};

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	// ����������
	RunnerPtr runner = new MyRunner();

	// ����
	Application::GetInstance().Run(runner);
	return 0;
}
