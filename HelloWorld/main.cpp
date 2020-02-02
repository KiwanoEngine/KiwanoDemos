// Copyright (C) 2019 Nomango

#include <kiwano/kiwano.h>

using namespace kiwano;

class HelloWorld
	: public Stage
{
public:
	HelloWorld()
	{
		// ����ͼƬ����
		SpritePtr sprite = new Sprite;

		// ����ͼƬ
		sprite->Load(L"../logo/logo_text.png");

		// �޸ľ���λ��, ʹ��������Ļ�Ͼ���
		sprite->SetPosition(640 / 2, 480 / 2);

		// �޸ľ���ê��, ʹͼƬ���Ķ�����Ļ����
		sprite->SetAnchor(0.5, 0.5);

		// �޸�������, ͼƬ��С�� 0.5 ��
		sprite->SetScale(0.5f, 0.5f);

		// ��ӵ���̨��
		this->AddChild(sprite);
	}
};

class HelloWorldApp
	: public Application
{
public:
	void OnReady() override
	{
		// ������̨
		StagePtr scene = new HelloWorld;

		// ������̨
		Director::Instance().EnterStage(scene);
	}
};

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	// ��������
	Window::Instance().Create(L"Hello World", 640, 480);

	// ���ñ���ɫ
	Renderer::Instance().SetClearColor(Color(0xE5E5E5));

	// ��������ʵ��
	HelloWorldApp app;

	// ����
	app.Run();
	return 0;
}
