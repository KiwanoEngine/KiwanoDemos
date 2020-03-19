// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class EaseActionDemo
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new EaseActionDemo;
	}

	static String DemoName()
	{
		return "Ease Action Demo";
	}

	EaseActionDemo()
	{
		// ����Դ�����л�ȡ����ͼƬ
		FramePtr man_image = Frame::Create("res/images/man.png");

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
			auto move = Tween::MoveBy(4_sec, Point{ 300, 0 }).SetEaseFunc(ease_functions[i]);
			// �������ӳ� 1 ��
			auto delay = Tween::Delay(1_sec);
			// ���������ǰ������������ѭ��ִ��
			auto group = Tween::Group({ move, delay }).SetLoops(-1);
			// �����������Զ��ָ�����λ��
			group.SetLoopDoneCallback([](Actor* target) { target->Move(-300, 0); });

			// ��ʼ������
			SpritePtr man = Sprite::Create(man_image);
			man->SetPosition(200, height);
			man->SetAnchor(0.5f, 0.5f);
			// ִ�ж���
			man->AddAction(group);

			// �����ʾ����
			TextActorPtr label = TextActor::Create(ease_names[i]);
			label->SetFillColor(Color::White);
			label->SetFontSize(16.0f);
			label->SetPosition(man->GetPositionX() - 150.0f, man->GetPositionY());
			label->SetAnchor(0, 0.5f);

			this->AddChild(label);
			this->AddChild(man);

			height += 80.f;
		}
	}
};
