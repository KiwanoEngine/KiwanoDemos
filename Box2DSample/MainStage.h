// Copyright (C) 2019 Nomango

#pragma once

#include "Ball.h"
#include "Box.h"
#include "Board.h"

KGE_DECLARE_SMART_PTR(MainStage);
class MainStage
	: public Stage
{
	b2World* world_ = nullptr;

public:
	MainStage()
	{
		// 设置可响应状态, 使舞台可以接收到鼠标点击消息
		SetResponsible(true);

		// 添加鼠标点击监听
		AddListener(Event::Click, Closure(this, &MainStage::Click));

		// 创建物理世界
		world_ = new b2World(b2Vec2(0, 10));

		// 添加一块静态木板
		BoardPtr board = new Board(world_, Size(GetWidth() - 100, 20), Point(GetWidth() / 2, GetHeight() - 50));
		AddChild(board);

		// 添加一个小球
		BallPtr ball = new Ball(world_, Point(320, 240));
		AddChild(ball);
	}

	~MainStage()
	{
		if (world_)
		{
			delete world_;
			world_ = nullptr;
		}
	}

	void OnUpdate(Duration dt) override
	{
		// 更新 Box2D 世界
		world_->Step(dt.Seconds(), 6, 2);

		// 更新每一个物理对象的位置和旋转角度
		b2Body* body = world_->GetBodyList();
		while (body)
		{
			Actor* actor = (Actor*)body->GetUserData();
			b2Body* next = body->GetNext();
			if (actor)
			{
				const b2Vec2& pos = body->GetPosition();
				actor->SetPosition(World2Stage(pos));
				actor->SetRotation(Radian2Angle(body->GetAngle()));

				// 移除掉落到舞台外的物体
				if (actor->GetPosition().y > GetHeight() + 50)
				{
					body->SetUserData(nullptr);
					world_->DestroyBody(body);

					actor->RemoveFromParent();
				}
			}

			body = next;
		}
	}

	void Click(Event const& evt)
	{
		// 左键添加一个小球, 右键添加一个盒子
		if (evt.mouse.button == MouseButton::Left)
		{
			BallPtr ball = new Ball(world_, Point{ evt.mouse.x, evt.mouse.y });
			AddChild(ball);
		}
		else if (evt.mouse.button == MouseButton::Right)
		{
			BoxPtr box = new Box(world_, Point{ evt.mouse.x, evt.mouse.y });
			AddChild(box);
		}
	}
};
