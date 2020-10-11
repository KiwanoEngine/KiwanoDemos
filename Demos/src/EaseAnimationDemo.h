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
		// 加载人物图片
		SpriteFrame man_image("res/images/man.png");

		// 创建缓动方程列表
		EaseFunc ease_functions[] = {
			Ease::Linear,		// 线性变化
			Ease::EaseInOut,	// 变化过程中有缓冲
			Ease::ExpoInOut,	// 在开始和结束阶段非常慢，但过程非常快
			Ease::BounceInOut,	// 在开始和结束阶段均赋予弹性
			Ease::BackInOut		// 开始和结束阶段均有一个短暂的反方向运动
		};

		String ease_names[] = {
			"Linear",
			"EaseInOut",
			"ExpoInOut",
			"BounceInOut",
			"BackInOut",
		};

		// 为每个人物使用不同的缓动方程执行动画
		float height = 80.f;
		for (size_t i = 0; i < std::size(ease_functions); ++i)
		{
			// 动画：4 秒内向右移动 350 像素，并设置缓动方程
			auto move = animation::MoveBy(4_sec, Point{ 300, 0 }).Ease(ease_functions[i]);
			// 动画：延迟 1 秒
			auto delay = animation::Delay(1_sec);
			// 动画：组合前两个动画，并循环执行
			auto group = animation::Group({ move, delay }).Loops(-1);
			// 动画结束后自动恢复人物位置
			auto handler = AnimationEventHandler::HandleLoopDone([](Animation*, Actor* target) { target->MoveBy(-300, 0); });
			group.Handler(handler);

			// 初始化人物
			SpritePtr man = new Sprite(man_image);
			man->SetPosition(200, height);
			man->SetAnchor(0.5f, 0.5f);
			// 执行动画
			man->StartAnimation(group);

			// 添加提示文字
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
