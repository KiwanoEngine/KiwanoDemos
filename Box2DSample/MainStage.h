// Copyright (C) 2019 Nomango

#pragma once

#include "Circle.h"
#include "Rect.h"
#include "Board.h"

KGE_DECLARE_SMART_PTR(MainStage);
class MainStage
	: public Stage
{
	b2World* world_;

public:
	MainStage()
	{
		// 设置可响应状态, 使舞台可以接收到鼠标 Click 消息
		SetResponsible(true);

		// 添加消息监听
		AddListener(Event::Click, MakeClosure(this, &MainStage::Click));

		// 创建物理世界
		world_ = new b2World(b2Vec2(0, 10));

		BoardPtr board = new Board(world_, Size(GetWidth() - 100, 20), Point(GetWidth() / 2, GetHeight() - 50));
		AddChild(board);

		CirclePtr circle = new Circle(world_, Point(320, 240));
		AddChild(circle);
	}

	~MainStage()
	{
		if (world_)
			delete world_;
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
				actor->SetPosition(Vec2Convert(pos));
				actor->SetRotation(body->GetAngle() * 180.f / math::constants::PI_F);

				// 移除掉落到舞台外的物体
				if (actor->GetPosition().y > GetHeight() + 50)
				{
					body->SetUserData(0);
					world_->DestroyBody(body);

					actor->RemoveFromParent();
				}
			}

			body = next;
		}
	}

	void Click(Event const& evt)
	{
		if (evt.mouse.button == MouseButton::Left)
		{
			CirclePtr circle = new Circle(world_, Point{ evt.mouse.x, evt.mouse.y });
			AddChild(circle);
		}
		else if (evt.mouse.button == MouseButton::Right)
		{
			SquarePtr rect = new Square(world_, Point{ evt.mouse.x, evt.mouse.y });
			AddChild(rect);
		}
	}
};
