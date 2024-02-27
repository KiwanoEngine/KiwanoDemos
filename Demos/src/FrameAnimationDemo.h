// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

// ����
enum class Direction
{
	Up,
	Down,
	Left,
	Right
};

// �ϻ�
class Tiger
	: public Sprite
{
	RefPtr<FrameSequence> run_frames;	// �ܲ�����֡
	RefPtr<FrameSequence> stand_frames;	// վ������֡
	bool facing_left;				// �泯����泯��
	bool running;					// �Ƿ������ܲ�
	Direction running_direction;	// �ܲ�����

public:
	static RefPtr<Tiger> Create()
	{
		RefPtr<Tiger> tiger = new Tiger;
		return tiger;
	}

	Tiger()
	{
		// ����֡����
		run_frames = new FrameSequence({
			SpriteFrame("res/images/tiger/run/run01.png"),
			SpriteFrame("res/images/tiger/run/run02.png"),
			SpriteFrame("res/images/tiger/run/run03.png"),
			SpriteFrame("res/images/tiger/run/run04.png"),
			SpriteFrame("res/images/tiger/run/run05.png"),
			SpriteFrame("res/images/tiger/run/run06.png"),
		});

		SpriteFrame stand_frame("res/images/tiger/stand.png");
		stand_frames = new FrameSequence(stand_frame.Split(3, 2));

		// ִ�ж���
		StartStandAnimation();

		// ��Ӱ�������
		AddListener<KeyDownEvent>(Closure(this, &Tiger::OnKeyDown));
		AddListener<KeyUpEvent>(Closure(this, &Tiger::OnKeyUp));

		// Ĭ�Ϸ���Ϊ Left
		facing_left = true;
		running = false;

		// ����ê��
		SetAnchor(0.5f, 0.5f);
	}

	void OnKeyDown(Event* evt)
	{
		KGE_ASSERT(evt->IsType<KeyDownEvent>());

		auto key_evt = dynamic_cast<KeyDownEvent*>(evt);
		if (key_evt->code == KeyCode::Left)
			Run(Direction::Left);
		else if (key_evt->code == KeyCode::Right)
			Run(Direction::Right);
		else if (key_evt->code == KeyCode::Up)
			Run(Direction::Up);
		else if (key_evt->code == KeyCode::Down)
			Run(Direction::Down);
	}

	void OnKeyUp(Event* evt)
	{
		KGE_ASSERT(evt->IsType<KeyUpEvent>());

		auto key_evt = dynamic_cast<KeyUpEvent*>(evt);
		switch (key_evt->code)
		{
		case KeyCode::Left:
		case KeyCode::Right:
		case KeyCode::Up:
		case KeyCode::Down:
			StopRun();
			break;
		}
	}

	void Run(Direction d)
	{
		if (!running)
		{
			running = true;

			// ִ���ܲ�����
			StartRunAnimation();
		}

		running_direction = d;
		if (running_direction == Direction::Left)
		{
			facing_left = true;
		}
		else if (running_direction == Direction::Right)
		{
			facing_left = false;
		}

		// ���ſ��Ե���ͼƬ��ʾ����
		// ������ -1 ͼƬ�ᷴת
		SetScale(facing_left ? 1.0f : -1.0f, 1.0f);
	}

	void StopRun()
	{
		if (running)
		{
			running = false;

			// ִ��վ������
			StartStandAnimation();
		}
	}

	void OnUpdate(Duration dt)
	{
		if (running)
		{
			// �����ƶ�����
			// OnUpdate ������һ���ȶ����ִ�еĺ���, �����ʵ���ȶ�
			// ÿ���ƶ� 150 ����, Ӧ���� dt ���������ƶ�����
			const float moving_per_sec = 150;
			const float distance = moving_per_sec * dt.GetSeconds();

			switch (running_direction)
			{
			case Direction::Up:
				MoveBy(0, -distance);
				break;
			case Direction::Down:
				MoveBy(0, distance);
				break;
			case Direction::Left:
				MoveBy(-distance, 0);
				break;
			case Direction::Right:
				MoveBy(distance, 0);
				break;
			}
		}
	}

	// ִ���ܲ�����
	void StartRunAnimation()
	{
		StopAllAnimations();
		StartAnimation(animation::Frames(0.5_sec, run_frames).Loops(-1));
	}

	// ִ��վ������
	void StartStandAnimation()
	{
		StopAllAnimations();
		StartAnimation(animation::Frames(1_sec, stand_frames).Loops(-1));
	}
};

class FrameAnimationDemo
	: public Stage
{
public:
	static RefPtr<Stage> Create()
	{
		return new FrameAnimationDemo;
	}

	static String DemoName()
	{
		return "Frame Animation Demo";
	}

	FrameAnimationDemo()
	{
		// ��������
		RefPtr<Sprite> bg = new Sprite("res/images/spring_forest.jpg");
		bg->SetSize(GetSize());

		// �����ϻ�
		RefPtr<Tiger> tiger = new Tiger();
		// ����Ļ�Ͼ�����ʾ
		tiger->SetAnchor(0.5f, 0.5f);
		tiger->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// ����˵������
		RefPtr<TextActor> intro = new TextActor("���������Ҽ��ƶ�");
		// ��������λ��
		intro->SetAnchor(0.5f, 0.5f);
		intro->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 80);
		intro->SetFillColor(Color::White);
		intro->SetAlignment(TextAlign::Center);

		// ��ӵ���̨
		this->AddChild(bg);
		this->AddChild(tiger);
		this->AddChild(intro);
	}
};
