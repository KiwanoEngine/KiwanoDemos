// Copyright (C) 2019 Nomango

#pragma once
#include "Circle.h"
#include "Square.h"

KGE_DECLARE_SMART_PTR(Demo1);
KGE_DECLARE_SMART_PTR(Board);

class Demo1
	: public PhysicWorld
{
public:
	Demo1();

	void OnUpdate(Duration dt) override;

	// ������¼�
	void OnClick(Event const& evt);

	// ��ӷ���
	void AddSquare(const Point& pos);

	// ���С��
	void AddCircle(const Point& pos);

	static inline StagePtr Create() { return new Demo1; }
};


// ľ��
class Board
	: public RectActor
{
	PhysicBodyPtr body_;

public:
	Board(PhysicWorld* world, const Size& size, const Point& pos)
	{
		// ���������������ɫ
		Color color = 0x868686;
		SetStrokeColor(color);
		SetFillColor(color);

		// ����ľ��Ĵ�С��λ�ú���ת�Ƕ�
		SetRectSize(size);
		SetAnchor(0.5f, 0.5f);
		SetRotation(10);
		SetPosition(pos);

		body_ = new PhysicBody(world, this);
		body_->SetType(PhysicBody::Type::Static);
		body_->AddBoxShape(GetSize());
	}
};


Demo1::Demo1()
{
	// ���ÿ���Ӧ״̬, ʹ��̨���Խ��յ��������Ϣ
	SetResponsible(true);

	// ������������
	AddListener(Event::Click, Closure(this, &Demo1::OnClick));

	// ���һ�龲̬ľ��
	BoardPtr board = new Board(this, Size(GetWidth() - 100, 20), Point(GetWidth() / 2, GetHeight() - 110));
	AddChild(board);

	// ���һ��С��
	AddCircle(Point(GetWidth() / 2, 200));

	// ����ı�˵��
	TextPtr intro = new Text(L"���������Ҽ���������");
	intro->SetAnchor(0.5f, 0.5f);
	intro->SetPosition(GetWidth() / 2, GetHeight() / 2);
	AddChild(intro);
}

void Demo1::OnUpdate(Duration dt)
{
	// �Ƴ����䵽�����������
	Vector<ActorPtr> outed;
	for (auto child : GetAllChildren())
	{
		if (child->GetPosition().y > GetHeight() + 50)
		{
			outed.push_back(child);
		}
	}

	for (auto child : outed)
	{
		child->RemoveFromParent();
	}
}

void Demo1::OnClick(Event const& evt)
{
	// ������һ��С��, �Ҽ����һ������
	if (evt.mouse.button == MouseButton::Left)
	{
		AddCircle(Point{ evt.mouse.x, evt.mouse.y });
	}
	else if (evt.mouse.button == MouseButton::Right)
	{
		AddSquare(Point{ evt.mouse.x, evt.mouse.y });
	}
}

void Demo1::AddSquare(const Point& pos)
{
	SquarePtr rect = new Square(this, pos, Size(80, 80));
	AddChild(rect);
}

void Demo1::AddCircle(const Point& pos)
{
	CirclePtr circle = new Circle(this, pos, 50);
	AddChild(circle);
}
