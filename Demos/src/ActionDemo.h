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
		Action rotate_by = ActionRotateBy(1_sec, 60.0f);
		// ��������ѭ��
		rotate_by.Loops(-1);

		// ����һ��С��ִ�иö���
		CreateManToRunAction("Rotating", rotate_by, position);
	}

	void ShowMove(const Point& position)
	{
		// ����λ�ƶ���
		// 0.5����x�����ƶ�20����
		Action move_by = ActionMoveBy(0.5_sec, Vec2(20, 0));
		// ���ö����ӳ�
		move_by.Delay(0.5_sec);

		// �������򶯻�
		Action move_by_reverse = move_by.Reverse();

		// ������϶���
		Action group = ActionGroup({ move_by, move_by_reverse });
		// ��������ѭ��
		group.Loops(-1);

		CreateManToRunAction("Move & Reverse", group, position);
	}

	void ShowFadeInAndFadeOut(const Point& position)
	{
		// ������������
		Action fade_out = ActionFadeOut(1.0_sec);

		// �������붯��
		Action fade_in = ActionFadeIn(1.0_sec);

		// ������϶���
		Action group = ActionGroup({ fade_out, fade_in });
		// ��������ѭ��
		group.Loops(-1);

		CreateManToRunAction("FadeIn & FadeOut", group, position);
	}

	void ShowWalk(const Point& position)
	{
		// ����·����״
		ShapePtr circle = Shape::CreateCircle(Point(10.0f, 0), 10.0f);

		// ����·���ƶ�����
		Action walk = ActionWalk(2.0_sec, circle);
		// ��������ѭ��
		walk.Loops(-1);

		CreateManToRunAction("Path Walk", walk, position);
	}

	void ShowGroup(const Point& position)
	{
		// ʹ��Tween�������ߴ�����϶���
		Action group = ActionGroup(
			{
				ActionGroup({ ActionScaleTo(0.3_sec, 0.5f, 0.5f), ActionScaleTo(0.7_sec, 1.0f, 1.0f) }),
				ActionGroup({ ActionFadeTo(0.5_sec, 0.3f), ActionFadeIn(0.5_sec) })
			},
			true /* ͬ��ִ�� */
		).Loops(-1);

		CreateManToRunAction("Group", group, position);
	}

	void ShowCustom(const Point& position)
	{
		Action custom = ActionCustom(1_sec, [](Actor* target, float percent) {
			Sprite* sprite = (Sprite*)target;
			// ��ȡͼƬԭ��Ⱥ͸߶�
			float src_width = sprite->GetSourceWidth();
			float src_height = sprite->GetSourceHeight();
			// ���ݶ������ȼ����µĲü�����
			Rect crop_rect = Rect(0, percent * src_height / 2, src_width, (0.5f + (1.0f - percent) * 0.5f) * src_height);
			sprite->SetCropRect(crop_rect);
			// ���辫���СΪ�ü����δ�С
			sprite->SetSize(crop_rect.GetSize());
		}).Loops(-1);

		CreateManToRunAction("Custom", custom, position);
	}

	void CreateManToRunAction(const String& text, Action action, const Point& position)
	{
		SpritePtr man = new Sprite("res/images/man.png");
		man->AddAction(action);
		man->SetPosition(position);
		man->SetAnchor(0.5f, 0.5f);
		this->AddChild(man);

		// �����ʾ����
		TextActorPtr label = new TextActor(text);
		label->SetPosition(position.x, position.y + man->GetHeight() / 2 + 10.0f);
		label->SetAnchor(0.5f, 0);
		label->SetFillColor(Color::White);
		this->AddChild(label);
	}
};
