// Copyright (C) 2019 Nomango

#include <kiwano/kiwano.h>

using namespace kiwano;

void Setup()
{
	// ������̨
	StagePtr stage = new Stage;

	// ��������
	SpritePtr sprite = new Sprite("../logo/logo_text.png");

	// �޸ľ���λ��, ʹ��������Ļ�Ͼ���
	sprite->SetPosition(640 / 2, 480 / 2);

	// �޸ľ���ê��, ʹͼƬ���Ķ�����Ļ����
	sprite->SetAnchor(0.5, 0.5);

	// �޸�������, ͼƬ��С�� 0.3 ��
	sprite->SetScale(0.3f, 0.3f);

	// ��ӵ���̨��
	stage->AddChild(sprite);

	// ������̨
	Director::GetInstance().EnterStage(stage);
}

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	// ��Ϸ����
	Settings settings;
	settings.window.title = "Hello World";
	settings.window.width = 640;
	settings.window.height = 480;
	settings.bg_color = Color::Rgb(0xE5E5E5);

	// ����
	Application::GetInstance().Run(settings, Setup);
	return 0;
}
