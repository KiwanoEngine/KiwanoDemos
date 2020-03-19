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
		// ������ת����
		ActionRotateByPtr rotate_by = ActionRotateBy::Create(1_sec, 60.0f);
		// ��������ѭ��
		rotate_by->SetLoops(-1);

		// ����һ��С��ִ�иö���
		CreateManToRunAction("Rotating", rotate_by, position);
	}

	void ShowMove(const Point& position)
	{
		// ����λ�ƶ���
		// 0.5����x�����ƶ�20����
		ActionMoveByPtr move_by = ActionMoveBy::Create(0.5_sec, Vec2(20, 0));
		// ���ö����ӳ�
		move_by->SetDelay(0.5_sec);

		// �������򶯻�
		ActionPtr move_by_reverse = move_by->Reverse();

		// ������϶���
		ActionPtr group = ActionGroup::Create({ move_by, move_by_reverse });
		// ��������ѭ��
		group->SetLoops(-1);

		CreateManToRunAction("Move & Reverse", group, position);
	}

	void ShowFadeInAndFadeOut(const Point& position)
	{
		// ������������
		ActionFadeOutPtr fade_out = ActionFadeOut::Create(1.0_sec);

		// �������붯��
		ActionFadeInPtr fade_in = ActionFadeIn::Create(1.0_sec);

		// ������϶���
		ActionPtr group = ActionGroup::Create({ fade_out, fade_in });
		// ��������ѭ��
		group->SetLoops(-1);

		CreateManToRunAction("FadeIn & FadeOut", group, position);
	}

	void ShowWalk(const Point& position)
	{
		// ����·����״
		ShapePtr circle = Shape::CreateCircle(Point(10.0f, 0), 10.0f);

		// ����·���ƶ�����
		ActionWalkPtr walk = ActionWalk::Create(2.0_sec, circle);
		// ��������ѭ��
		walk->SetLoops(-1);

		CreateManToRunAction("Path Walk", walk, position);
	}

	void ShowGroup(const Point& position)
	{
		// ʹ��Tween�������ߴ�����϶���
		ActionPtr group = Tween::Group(
			{
				Tween::Group({ Tween::ScaleTo(0.3_sec, 0.5f, 0.5f), Tween::ScaleTo(0.7_sec, 1.0f, 1.0f) }),
				Tween::Group({ Tween::FadeTo(0.5_sec, 0.3f), Tween::FadeIn(0.5_sec) })
			},
			true /* ͬ��ִ�� */
		).SetLoops(-1);

		CreateManToRunAction("Group", group, position);
	}

	void ShowCustom(const Point& position)
	{
		auto custom = Tween::Custom(1_sec, [](Actor* target, float percent) {
			Sprite* sprite = (Sprite*)target;
			// ��ȡͼƬԭ��Ⱥ͸߶�
			float src_width = sprite->GetSourceWidth();
			float src_height = sprite->GetSourceHeight();
			// ���ݶ������ȼ����µĲü�����
			Rect crop_rect = Rect(0, percent * src_height / 2, src_width, (0.5f + (1.0f - percent) * 0.5f) * src_height);
			sprite->SetCropRect(crop_rect);
			// ���辫���СΪ�ü����δ�С
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

		// �����ʾ����
		TextActorPtr label = TextActor::Create(text);
		label->SetPosition(position.x, position.y + man->GetHeight() / 2 + 10.0f);
		label->SetAnchor(0.5f, 0);
		label->SetFontSize(16.0f);
		label->SetFillColor(Color::White);
		this->AddChild(label);
	}
};
