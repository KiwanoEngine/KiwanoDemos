// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class ActionDemo
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new ActionDemo;
	}

	static String DemoName()
	{
		return "Action Demo";
	}

	ActionDemo()
	{
		const int max_rows = 2;
		const int max_cols = 3;
		float width = this->GetWidth();
		float height = this->GetHeight();
		float x = width / max_cols;
		float y = height / max_rows;

		Vector<Function<void(const Point&)>> functions;
		functions = {
			Closure(this, &ActionDemo::ShowRotate),
			Closure(this, &ActionDemo::ShowMove),
			Closure(this, &ActionDemo::ShowFadeInAndFadeOut),
			Closure(this, &ActionDemo::ShowWalk),
			Closure(this, &ActionDemo::ShowGroup),
			Closure(this, &ActionDemo::ShowCustom),
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
		ActionRotateByPtr rotate_by = ActionRotateBy::Create(1_sec, 60.0f);
		// 设置无限循环
		rotate_by->SetLoops(-1);

		// 创建一个小人执行该动画
		CreateManToRunAction("Rotating", rotate_by, position);
	}

	void ShowMove(const Point& position)
	{
		// 创建位移动画
		// 0.5秒向x方向移动20像素
		ActionMoveByPtr move_by = ActionMoveBy::Create(0.5_sec, Vec2(20, 0));
		// 设置动画延迟
		move_by->SetDelay(0.5_sec);

		// 创建反向动画
		ActionPtr move_by_reverse = move_by->Reverse();

		// 创建组合动画
		ActionPtr group = ActionGroup::Create({ move_by, move_by_reverse });
		// 设置无限循环
		group->SetLoops(-1);

		CreateManToRunAction("Move & Reverse", group, position);
	}

	void ShowFadeInAndFadeOut(const Point& position)
	{
		// 创建淡出动画
		ActionFadeOutPtr fade_out = ActionFadeOut::Create(1.0_sec);

		// 创建淡入动画
		ActionFadeInPtr fade_in = ActionFadeIn::Create(1.0_sec);

		// 创建组合动画
		ActionPtr group = ActionGroup::Create({ fade_out, fade_in });
		// 设置无限循环
		group->SetLoops(-1);

		CreateManToRunAction("FadeIn & FadeOut", group, position);
	}

	void ShowWalk(const Point& position)
	{
		// 创建路径形状
		ShapePtr circle = Shape::CreateCircle(Point(10.0f, 0), 10.0f);

		// 创建路径移动动画
		ActionWalkPtr walk = ActionWalk::Create(2.0_sec, circle);
		// 设置无限循环
		walk->SetLoops(-1);

		CreateManToRunAction("Path Walk", walk, position);
	}

	void ShowGroup(const Point& position)
	{
		// 使用Tween辅助工具创建组合动画
		auto group = Tween::Group(
			{
				Tween::Group({ Tween::ScaleTo(0.3_sec, 0.5f, 0.5f), Tween::ScaleTo(0.7_sec, 1.0f, 1.0f) }),
				Tween::Group({ Tween::FadeTo(0.5_sec, 0.3f), Tween::FadeIn(0.5_sec) })
			},
			true /* 同步执行 */
		).SetLoops(-1);

		CreateManToRunAction("Group", group, position);
	}

	void ShowCustom(const Point& position)
	{
		auto custom = Tween::Custom(1_sec, [](Actor* target, float percent) {
			Sprite* sprite = (Sprite*)target;
			// 获取图片原宽度和高度
			float src_width = sprite->GetSourceWidth();
			float src_height = sprite->GetSourceHeight();
			// 根据动画进度计算新的裁剪矩形
			Rect crop_rect = Rect(0, percent * src_height / 2, src_width, (0.5f + (1.0f - percent) * 0.5f) * src_height);
			sprite->SetCropRect(crop_rect);
			// 重设精灵大小为裁剪矩形大小
			sprite->SetSize(crop_rect.GetSize());
		}).SetLoops(-1);

		CreateManToRunAction("Custom", custom, position);
	}

	void CreateManToRunAction(const String& text, ActionPtr action, const Point& position)
	{
		SpritePtr man = Sprite::Create("res/images/man.png");
		man->AddAction(action);
		man->SetPosition(position);
		man->SetAnchor(0.5f, 0.5f);
		this->AddChild(man);

		// 添加提示文字
		TextActorPtr intro = TextActor::Create(text);
		intro->SetPosition(position.x, position.y + man->GetHeight() / 2 + 10.0f);
		intro->SetAnchor(0.5f, 0);
		intro->SetFontSize(16.0f);
		intro->SetFillColor(Color::White);
		this->AddChild(intro);
	}
};
