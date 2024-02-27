// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

// ��ɫ
class Hero
	: public GifSprite
{
public:
	Hero()
	{
		// ����ͼƬ
		if (this->Load("res/images/Kusanagi.gif"))
		{
			// ���� GIF ��ͼ����ѭ��
			this->SetLoopCount(-1);
		}
	}

	// ÿ֡��Ⱦǰִ�� OnUpdate
	void OnUpdate(Duration dt) override
	{
		// �˶��ٶȣ�����/�룩
		static const float speed_linear_per_sec = 100;
		// ��ת�ٶȣ���/�룩
		static const float speed_angular_per_sec = 60;

		// ʵ���ٶ� = �ٶ� x ʱ��
		float speed_linear = speed_linear_per_sec * dt.GetSeconds();
		float speed_angular = speed_angular_per_sec * dt.GetSeconds();

		// ��ȡ�����豸
		auto& input = Input::GetInstance();

		// �������Ҽ�
		if (input.IsDown(KeyCode::Left))
		{
			this->MoveBy(-speed_linear, 0);
		}
		else if (input.IsDown(KeyCode::Right))
		{
			this->MoveBy(speed_linear, 0);
		}

		// �������¼�
		if (input.IsDown(KeyCode::Up))
		{
			this->MoveBy(0, -speed_linear);
		}
		else if (input.IsDown(KeyCode::Down))
		{
			this->MoveBy(0, speed_linear);
		}

		// ������������˳ʱ����ת��ɫ
		if (input.IsDown(MouseButton::Left))
		{
			// ��ȡ��ǰ��ת�Ƕ�
			float rotation = this->GetRotation();
			// ���ýǶ�ֵ+2
			this->SetRotation(rotation + speed_angular);
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
	static RefPtr<Stage> Create()
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
		RefPtr<Hero> hero = new Hero();
		// ����Ļ�Ͼ�����ʾ
		hero->SetAnchor(0.5f, 0.5f);
		hero->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// ����˵������
		RefPtr<TextActor> intro = new TextActor("���������Ҽ��ƶ�\n����������ת\n�������Ҽ�����");
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
