// Copyright (C) 2019 Nomango

#include "kiwano/kiwano.h"

using namespace kiwano;

class MainStage
	: public Stage
{
public:
	MainStage()
	{
		String imageFile = L"../logo/logo_text.png";	// ͼƬ·��
		SpritePtr image = new Sprite(imageFile);		// ����ͼƬ����
		image->SetPosition(640 / 2, 480 / 2);			// �޸ľ���λ��, ʹ��������Ļ�Ͼ���
		image->SetScale(0.5f, 0.5f);					// �޸�������, ͼƬ��С�� 0.5 ��
		image->SetAnchor(0.5, 0.5);						// �޸ľ���ê��, ʹͼƬ���Ķ�����Ļ����
		this->AddChild(image);							// ��ӵ���̨��
	}
};

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	// ���� Kiwano ����ʵ��
	Application app;

	// �ڳ�ʼ��ѡ�������ô��ڿ��
	Options options;
	options.width = 640;
	options.height = 480;

	// ���ñ���ɫ
	options.clear_color = Color(0xE5E5E5);

	// ��ʼ�� app
	app.Init(options);

	// ������̨������
	StagePtr scene = new MainStage;
	Director::GetInstance()->EnterStage(scene);

	// ����
	app.Run();
	return 0;
}
