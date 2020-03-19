// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

// ��ɫ
KGE_DECLARE_SMART_PTR(Hero);
class Hero
	: public GifSprite
{
public:
	static HeroPtr Create()
	{
		HeroPtr hero = new Hero;

		// ����Դ�����л�ȡGIFͼ��
		GifImagePtr img = GifImage::Create("res/images/Kusanagi.gif");

		// ����ͼƬ
		if (hero->Load(img))
		{
			// ���� GIF ��ͼ����ѭ��
			hero->SetLoopCount(-1);
			return hero;
		}
		return nullptr;
	}

	// ÿ֡��Ⱦǰִ�� OnUpdate
	void OnUpdate(Duration dt) override
	{
		// ��ȡ�����豸
		auto& input = Input::GetInstance();

		// �������Ҽ�
		if (input.IsDown(KeyCode::Left))
		{
			this->Move(-2, 0);
		}
		else if (input.IsDown(KeyCode::Right))
		{
			this->Move(2, 0);
		}

		// �������¼�
		if (input.IsDown(KeyCode::Up))
		{
			this->Move(0, -2);
		}
		else if (input.IsDown(KeyCode::Down))
		{
			this->Move(0, 2);
		}

		// ������������˳ʱ����ת��ɫ
		if (input.IsDown(MouseButton::Left))
		{
			// ��ȡ��ǰ��ת�Ƕ�
			float rotation = this->GetRotation();
			// ���ýǶ�ֵ+2
			this->SetRotation(rotation + 2);
		}

		// �������Ҽ������ػ���ʾ��ɫ
		if (input.WasPressed(MouseButton::Right))
		{
			// ��ȡ��ǰ��ʾ״̬
			bool visible = this->IsVisible();
			// �����෴����ʾ״̬
			this->SetVisible(!visible);
		}
	}
};

class InputDemo
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new InputDemo;
	}

	static String DemoName()
	{
		return "Input Demo";
	}

	InputDemo()
	{
		// ������ɫ
		HeroPtr hero = Hero::Create();
		// ����Ļ�Ͼ�����ʾ
		hero->SetAnchor(0.5f, 0.5f);
		hero->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// ����˵������
		TextActorPtr intro = TextActor::Create("���������Ҽ��ƶ�\n����������ת\n�������Ҽ�����");
		// ��������λ��
		intro->SetAnchor(0.5f, 0.5f);
		intro->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);
		intro->SetFillColor(Color::White);
		intro->SetAlignment(TextAlign::Center);

		// ��ӵ���̨
		this->AddChild(hero);
		this->AddChild(intro);
	}
};
