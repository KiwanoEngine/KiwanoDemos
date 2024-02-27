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
		// ������ת����
		auto rotate_by = animation::RotateBy(1_sec, 60.0f);
		// ��������ѭ��
		rotate_by.Loops(-1);

		// ����һ��С��ִ�иö���
		CreateManToRunAction("Rotating", rotate_by, position);
	}

	void ShowMove(const Point& position)
	{
		// ����λ�ƶ���
		// 0.5����x�����ƶ�20����
		auto move_by = animation::MoveBy(0.5_sec, Vec2(20, 0));
		// ���ö����ӳ�
		move_by.Delay(0.5_sec);

		// �������򶯻�
		auto move_by_reverse = move_by.Reverse();

		// ������϶���
		auto group = animation::Group({ move_by, move_by_reverse });
		// ��������ѭ��
		group.Loops(-1);

		CreateManToRunAction("Move & Reverse", group, position);
	}

	void ShowFadeInAndFadeOut(const Point& position)
	{
		// ������������
		auto fade_out = animation::FadeOut(1.0_sec);

		// �������붯��
		auto fade_in = animation::FadeIn(1.0_sec);

		// ������϶���
		auto group = animation::Group({ fade_out, fade_in });
		// ��������ѭ��
		group.Loops(-1);

		CreateManToRunAction("FadeIn & FadeOut", group, position);
	}

	void ShowWalk(const Point& position)
	{
		// ����·����״
		RefPtr<Shape> circle = Shape::CreateCircle(Point(10.0f, 0), 10.0f);

		// ����·���ƶ�����
		auto walk = animation::Path(2.0_sec, circle);
		// ��������ѭ��
		walk.Loops(-1);

		CreateManToRunAction("Path Walk", walk, position);
	}

	void ShowGroup(const Point& position)
	{
		// ʹ��Tween�������ߴ�����϶���
		auto group = animation::Group(
			{
				animation::Group({ animation::ScaleTo(0.3_sec, Vec2(0.5f, 0.5f)), animation::ScaleTo(0.7_sec, Vec2(1.0f, 1.0f)) }),
				animation::Group({ animation::FadeTo(0.5_sec, 0.3f), animation::FadeIn(0.5_sec) })
			},
			true /* ͬ��ִ�� */
		).Loops(-1);

		CreateManToRunAction("Group", group, position);
	}

	void ShowCustom(const Point& position)
	{
		auto custom = animation::Custom(1_sec, [](Actor* target, float percent) {
			Sprite* sprite = (Sprite*)target;
			// ��ȡͼƬԭ��Ⱥ͸߶�
			float src_width = sprite->GetTexture()->GetWidth();
			float src_height = sprite->GetTexture()->GetHeight();
			// ���ݶ������ȼ����µĲü�����
			Rect crop_rect = Rect(0, percent * src_height / 2, src_width, (0.5f + (1.0f - percent) * 0.5f) * src_height);
			sprite->SetCropRect(crop_rect);
			// ���辫���СΪ�ü����δ�С
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

		// �����ʾ����
		RefPtr<TextActor> label = new TextActor(text);
		label->SetPosition(position.x, position.y + man->GetHeight() / 2 + 10.0f);
		label->SetAnchor(0.5f, 0);
		label->SetFillColor(Color::White);
		this->AddChild(label);
	}
};
