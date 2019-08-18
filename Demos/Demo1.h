// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class Demo1
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new Demo1;
	}

	Demo1()
	{
		// ��ȡ����ͼƬ
		FramePtr man_image = ResourceCache::GetInstance()->GetFrame(L"man");

		// �������������б�
		auto ease_functions = {
			Ease::Linear,		// ���Ա仯
			Ease::EaseInOut,	// �仯�������л���
			Ease::ExpoInOut,	// �ڿ�ʼ�ͽ����׶ηǳ����������̷ǳ���
			Ease::BounceInOut,	// �ڿ�ʼ�ͽ����׶ξ����赯��
			Ease::BackInOut		// ��ʼ�ͽ����׶ξ���һ�����ݵķ������˶�
		};

		// Ϊÿ������ʹ�ò�ͬ�Ļ�������ִ�ж���
		float height = 100.f;
		for (auto& func : ease_functions)
		{
			// ��ʼ������
			SpritePtr man = new Sprite(man_image);
			man->SetPosition(100, height);
			man->SetScale(0.5f, 0.3f);
			this->AddChild(man);

			// ������4 ���������ƶ� 350 ���أ������û�������
			auto move = Tween::MoveBy(4_s, Point{ 350, 0 }).SetEaseFunc(func);
			// �������ӳ� 1 ��
			auto delay = Tween::Delay(1_s);
			// ���������ǰ������������ѭ��ִ��
			auto group = Tween::Group({ move, delay }).SetLoops(-1);
			// �����������Զ��ָ�����λ��
			group.SetLoopDoneCallback([ptr = man.get()]() { ptr->Move(-350, 0); });

			// ִ�ж���
			man->AddAction(group);

			height += 60.f;
		}
	}
};
