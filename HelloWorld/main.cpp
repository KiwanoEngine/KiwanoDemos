// Copyright (C) 2019 Nomango

#include <kiwano/kiwano.h>

using namespace kiwano;

void Startup()
{
	// ������̨
	StagePtr stage = new Stage;

	// ��������
	SpritePtr sprite = Sprite::Create("logo.png");

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

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	// ���ñ���ɫ
	Renderer::GetInstance().SetClearColor(Color(0xE5E5E5));

	// ��������
	WindowPtr window = Window::Create("Hello World", 640, 480);

	// ����������������������ִ�� Startup ����
	RunnerPtr runner = Runner::Create(window, Startup);

	// ����
	Application::GetInstance().Run(runner);
	return 0;
}
