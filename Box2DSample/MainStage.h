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
		// ���ÿ���Ӧ״̬, ʹ��̨���Խ��յ��������Ϣ
		SetResponsible(true);

		// ������������
		AddListener(Event::Click, Closure(this, &MainStage::Click));

		// ������������
		world_ = new b2World(b2Vec2(0, 10));

		// ���һ�龲̬ľ��
		BoardPtr board = new Board(world_, Size(GetWidth() - 100, 20), Point(GetWidth() / 2, GetHeight() - 50));
		AddChild(board);

		// ���һ��С��
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
		// ���� Box2D ����
		world_->Step(dt.Seconds(), 6, 2);

		// ����ÿһ����������λ�ú���ת�Ƕ�
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

				// �Ƴ����䵽��̨�������
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
		// ������һ��С��, �Ҽ����һ������
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
