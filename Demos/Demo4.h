// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

// ����
enum Direction
{
	Up,
	Down,
	Left,
	Right
};

// �ϻ�
KGE_DECLARE_SMART_PTR(Tiger);
class Tiger
	: public Sprite
{
	FrameSequencePtr run_frames;	// �ܲ�����֡
	FrameSequencePtr stand_frames;	// վ������֡
	bool facing_left;				// �泯����泯��
	bool running;					// �Ƿ������ܲ�
	Direction running_direction;	// �ܲ�����

public:
	Tiger()
	{
		// ����֡����
		run_frames = ResourceCache::Instance().Get<FrameSequence>(L"tiger_running");
		stand_frames = ResourceCache::Instance().Get<FrameSequence>(L"tiger_standing");

		// ִ�ж���
		AddAction(
			Tween::Animation(1_sec, stand_frames).SetLoops(-1)
		);

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
			StopAllActions();

			// ִ���ܲ�����
			AddAction(
				Tween::Animation(0.5_sec, run_frames).SetLoops(-1)
			);
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
			StopAllActions();

			// ִ��վ������
			AddAction(
				Tween::Animation(1_sec, stand_frames).SetLoops(-1)
			);
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
			const float distance = moving_per_sec * dt.Seconds();

			switch (running_direction)
			{
			case Direction::Up:
				Move(0, -distance);
				break;
			case Direction::Down:
				Move(0, distance);
				break;
			case Direction::Left:
				Move(-distance, 0);
				break;
			case Direction::Right:
				Move(distance, 0);
				break;
			}
		}
	}
};

class Demo4
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new Demo4;
	}

	Demo4()
	{
		// ��������
		SpritePtr bg = new Sprite;
		bg->Load(L"res/spring_forest.jpg");
		bg->SetSize(GetSize());

		// �����ϻ�
		TigerPtr tiger = new Tiger;
		// ����Ļ�Ͼ�����ʾ
		tiger->SetAnchor(0.5f, 0.5f);
		tiger->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// ����˵������
		TextActorPtr text = new TextActor(L"���������Ҽ��ƶ�");
		// ��������λ��
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 80);
		text->SetAlignment(TextAlign::Center);

		// ��ӵ���̨
		this->AddChild(bg);
		this->AddChild(tiger);
		this->AddChild(text);
	}
};
