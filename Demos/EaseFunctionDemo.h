// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class EaseFunctionDemo
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new EaseFunctionDemo;
	}

	static String DemoName()
	{
		return "Ease Function Demo";
	}

	EaseFunctionDemo()
	{
		// 从资源缓存中获取人物图片
		FramePtr man_image = Frame::Create("res/man.png");

		// 创建缓动方程列表
		auto ease_functions = {
			Ease::Linear,		// 线性变化
			Ease::EaseInOut,	// 变化过程中有缓冲
			Ease::ExpoInOut,	// 在开始和结束阶段非常慢，但过程非常快
			Ease::BounceInOut,	// 在开始和结束阶段均赋予弹性
			Ease::BackInOut		// 开始和结束阶段均有一个短暂的反方向运动
		};

		// 为每个人物使用不同的缓动方程执行动画
		float height = 100.f;
		for (auto& func : ease_functions)
		{
			// 初始化人物
			SpritePtr man = Sprite::Create(man_image);
			man->SetPosition(100, height);
			man->SetScale(0.5f, 0.3f);
			this->AddChild(man);

			// 动画：4 秒内向右移动 350 像素，并设置缓动方程
			auto move = Tween::MoveBy(4_sec, Point{ 350, 0 }).SetEaseFunc(func);
			// 动画：延迟 1 秒
			auto delay = Tween::Delay(1_sec);
			// 动画：组合前两个动画，并循环执行
			auto group = Tween::Group({ move, delay }).SetLoops(-1);
			// 动画结束后自动恢复人物位置
			group.SetLoopDoneCallback([](Actor* target) { target->Move(-350, 0); });

			// 执行动画
			man->AddAction(group);

			height += 60.f;
		}
	}
};
