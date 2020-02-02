// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

// 方向
enum Direction
{
	Up,
	Down,
	Left,
	Right
};

// 老虎
KGE_DECLARE_SMART_PTR(Tiger);
class Tiger
	: public Sprite
{
	FrameSequencePtr run_frames;	// 跑步序列帧
	FrameSequencePtr stand_frames;	// 站立序列帧
	bool facing_left;				// 面朝左或面朝右
	bool running;					// 是否正在跑步
	Direction running_direction;	// 跑步方向

public:
	Tiger()
	{
		// 加载帧动画
		run_frames = ResourceCache::Instance().Get<FrameSequence>(L"tiger_running");
		stand_frames = ResourceCache::Instance().Get<FrameSequence>(L"tiger_standing");

		// 执行动画
		AddAction(
			Tween::Animation(1_sec, stand_frames).SetLoops(-1)
		);

		// 添加按键监听
		AddListener<KeyDownEvent>(Closure(this, &Tiger::OnKeyDown));
		AddListener<KeyUpEvent>(Closure(this, &Tiger::OnKeyUp));

		// 默认方向为 Left
		facing_left = true;
		running = false;

		// 设置锚点
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

			// 执行跑步动画
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

		// 缩放可以调整图片显示方向
		// 缩放至 -1 图片会反转
		SetScale(facing_left ? 1.0f : -1.0f, 1.0f);
	}

	void StopRun()
	{
		if (running)
		{
			running = false;
			StopAllActions();

			// 执行站立动画
			AddAction(
				Tween::Animation(1_sec, stand_frames).SetLoops(-1)
			);
		}
	}

	void OnUpdate(Duration dt)
	{
		if (running)
		{
			// 计算移动距离
			// OnUpdate 并不是一个稳定间隔执行的函数, 如果想实现稳定
			// 每秒移动 150 像素, 应根据 dt 参数计算移动距离
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
		// 创建背景
		SpritePtr bg = new Sprite;
		bg->Load(L"res/spring_forest.jpg");
		bg->SetSize(GetSize());

		// 创建老虎
		TigerPtr tiger = new Tiger;
		// 在屏幕上居中显示
		tiger->SetAnchor(0.5f, 0.5f);
		tiger->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// 创建说明文字
		TextActorPtr text = new TextActor(L"按上下左右键移动");
		// 设置文字位置
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 80);
		text->SetAlignment(TextAlign::Center);

		// 添加到舞台
		this->AddChild(bg);
		this->AddChild(tiger);
		this->AddChild(text);
	}
};
