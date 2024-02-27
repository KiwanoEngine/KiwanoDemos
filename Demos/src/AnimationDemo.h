// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class AnimationDemo
	: public Stage
{
public:
	static RefPtr<Stage> Create()
	{
		return new AnimationDemo;
	}

	static String DemoName()
	{
		return "Animation Demo";
	}

	AnimationDemo()
	{
		const int max_rows = 2;
		const int max_cols = 3;
		float width = this->GetWidth();
		float height = this->GetHeight();
		float x = width / max_cols;
		float y = height / max_rows;

		Vector<Function<void(const Point&)>> functions;
		functions = {
			Closure(this, &AnimationDemo::ShowRotate),
			Closure(this, &AnimationDemo::ShowMove),
			Closure(this, &AnimationDemo::ShowFadeInAndFadeOut),
			Closure(this, &AnimationDemo::ShowWalk),
			Closure(this, &AnimationDemo::ShowGroup),
			Closure(this, &AnimationDemo::ShowCustom),
		};

		for (size_t i = 0; i < functions.size(); ++i)
		{
			Point position = Point((i % max_cols + 0.5f) * x, (i / max_cols + 0.5f) * y);
			functions[i](position);
		}
	}

	void ShowRotate(const Point& position)
	{
		// 创建旋转动画
		auto rotate_by = animation::RotateBy(1_sec, 60.0f);
		// 设置无限循环
		rotate_by.Loops(-1);

		// 创建一个小人执行该动画
		CreateManToRunAction("Rotating", rotate_by, position);
	}

	void ShowMove(const Point& position)
	{
		// 创建位移动画
		// 0.5秒向x方向移动20像素
		auto move_by = animation::MoveBy(0.5_sec, Vec2(20, 0));
		// 设置动画延迟
		move_by.Delay(0.5_sec);

		// 创建反向动画
		auto move_by_reverse = move_by.Reverse();

		// 创建组合动画
		auto group = animation::Group({ move_by, move_by_reverse });
		// 设置无限循环
		group.Loops(-1);

		CreateManToRunAction("Move & Reverse", group, position);
	}

	void ShowFadeInAndFadeOut(const Point& position)
	{
		// 创建淡出动画
		auto fade_out = animation::FadeOut(1.0_sec);

		// 创建淡入动画
		auto fade_in = animation::FadeIn(1.0_sec);

		// 创建组合动画
		auto group = animation::Group({ fade_out, fade_in });
		// 设置无限循环
		group.Loops(-1);

		CreateManToRunAction("FadeIn & FadeOut", group, position);
	}

	void ShowWalk(const Point& position)
	{
		// 创建路径形状
		RefPtr<Shape> circle = Shape::CreateCircle(Point(10.0f, 0), 10.0f);

		// 创建路径移动动画
		auto walk = animation::Path(2.0_sec, circle);
		// 设置无限循环
		walk.Loops(-1);

		CreateManToRunAction("Path Walk", walk, position);
	}

	void ShowGroup(const Point& position)
	{
		// 使用Tween辅助工具创建组合动画
		auto group = animation::Group(
			{
				animation::Group({ animation::ScaleTo(0.3_sec, Vec2(0.5f, 0.5f)), animation::ScaleTo(0.7_sec, Vec2(1.0f, 1.0f)) }),
				animation::Group({ animation::FadeTo(0.5_sec, 0.3f), animation::FadeIn(0.5_sec) })
			},
			true /* 同步执行 */
		).Loops(-1);

		CreateManToRunAction("Group", group, position);
	}

	void ShowCustom(const Point& position)
	{
		auto custom = animation::Custom(1_sec, [](Actor* target, float percent) {
			Sprite* sprite = (Sprite*)target;
			// 获取图片原宽度和高度
			float src_width = sprite->GetTexture()->GetWidth();
			float src_height = sprite->GetTexture()->GetHeight();
			// 根据动画进度计算新的裁剪矩形
			Rect crop_rect = Rect(0, percent * src_height / 2, src_width, (0.5f + (1.0f - percent) * 0.5f) * src_height);
			sprite->SetCropRect(crop_rect);
			// 重设精灵大小为裁剪矩形大小
			sprite->SetSize(crop_rect.GetSize());
		}).Loops(-1);

		CreateManToRunAction("Custom", custom, position);
	}

	void CreateManToRunAction(const String& text, RefPtr<Animation> animation, const Point& position)
	{
		RefPtr<Sprite> man = new Sprite("res/images/man.png");
		man->StartAnimation(animation);
		man->SetPosition(position);
		man->SetAnchor(0.5f, 0.5f);
		this->AddChild(man);

		// 添加提示文字
		RefPtr<TextActor> label = new TextActor(text);
		label->SetPosition(position.x, position.y + man->GetHeight() / 2 + 10.0f);
		label->SetAnchor(0.5f, 0);
		label->SetFillColor(Color::White);
		this->AddChild(label);
	}
};
