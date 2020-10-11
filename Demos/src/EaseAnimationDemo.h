// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class EaseAnimationDemo
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new EaseAnimationDemo;
	}

	static String DemoName()
	{
		return "Ease Animation Demo";
	}

	EaseAnimationDemo()
	{
		// ��������ͼƬ
		SpriteFrame man_image("res/images/man.png");

		// �������������б�
		EaseFunc ease_functions[] = {
			Ease::Linear,		// ���Ա仯
			Ease::EaseInOut,	// �仯�������л���
			Ease::ExpoInOut,	// �ڿ�ʼ�ͽ����׶ηǳ����������̷ǳ���
			Ease::BounceInOut,	// �ڿ�ʼ�ͽ����׶ξ����赯��
			Ease::BackInOut		// ��ʼ�ͽ����׶ξ���һ�����ݵķ������˶�
		};

		String ease_names[] = {
			"Linear",
			"EaseInOut",
			"ExpoInOut",
			"BounceInOut",
			"BackInOut",
		};

		// Ϊÿ������ʹ�ò�ͬ�Ļ�������ִ�ж���
		float height = 80.f;
		for (size_t i = 0; i < std::size(ease_functions); ++i)
		{
			// ������4 ���������ƶ� 350 ���أ������û�������
			auto move = animation::MoveBy(4_sec, Point{ 300, 0 }).Ease(ease_functions[i]);
			// �������ӳ� 1 ��
			auto delay = animation::Delay(1_sec);
			// ���������ǰ������������ѭ��ִ��
			auto group = animation::Group({ move, delay }).Loops(-1);
			// �����������Զ��ָ�����λ��
			auto handler = AnimationEventHandler::HandleLoopDone([](Animation*, Actor* target) { target->MoveBy(-300, 0); });
			group.Handler(handler);

			// ��ʼ������
			SpritePtr man = new Sprite(man_image);
			man->SetPosition(200, height);
			man->SetAnchor(0.5f, 0.5f);
			// ִ�ж���
			man->StartAnimation(group);

			// �����ʾ����
			TextActorPtr label = new TextActor(ease_names[i]);
			label->SetFillColor(Color::White);
			label->SetPosition(man->GetPositionX() - 150.0f, man->GetPositionY());
			label->SetAnchor(0, 0.5f);

			this->AddChild(label);
			this->AddChild(man);

			height += 80.f;
		}
	}
};
