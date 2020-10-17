// Copyright (C) 2019 Nomango

#pragma once
#include "CircleBody.h"
#include "SquareBody.h"

KGE_DECLARE_SMART_PTR(PhysicBodyDemo);
KGE_DECLARE_SMART_PTR(Board);

class PhysicBodyDemo
	: public Stage
{
public:
	static inline StagePtr Create() { return new PhysicBodyDemo; }

	static inline String Name() { return "Physic Body Demo"; }

	static inline String Label() { return "���������Ҽ���������"; }

	PhysicBodyDemo();

	void OnUpdate(Duration dt) override;

	// ������¼�
	void OnClick(Event* evt);

	// ��ӷ���
	void AddSquare(const Point& pos);

	// ���С��
	void AddCircle(const Point& pos);

private:
	PhysicWorldPtr world_;
};

// ľ��
class Board
	: public RectActor
{
public:
	Board(PhysicWorldPtr world, const Size& size, const Point& pos);
};


PhysicBodyDemo::PhysicBodyDemo()
{
	// ������������
	world_ = new PhysicWorld();
	AddComponent(world_);

	// �����괫������ʹ��̨���Խ��յ��������Ϣ
	ComponentPtr comp = new MouseSensor;
	AddComponent(comp);

	// ������������
	AddListener<MouseClickEvent>(Closure(this, &PhysicBodyDemo::OnClick));

	// ���һ�龲̬ľ��
	BoardPtr board = new Board(world_, Size(GetWidth() - 100, 20), Point(GetWidth() / 2, GetHeight() - 110));
	AddChild(board);

	// ���һ��С��
	AddCircle(Point(GetWidth() / 2, 200));
}

void PhysicBodyDemo::OnUpdate(Duration dt)
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

void PhysicBodyDemo::OnClick(Event* evt)
{
	KGE_ASSERT(evt->IsType<MouseClickEvent>());

	// ������һ��С��, �Ҽ����һ������
	auto mouse_evt = dynamic_cast<MouseClickEvent*>(evt);
	if (mouse_evt->button == MouseButton::Left)
	{
		AddCircle(mouse_evt->pos);
	}
	else if (mouse_evt->button == MouseButton::Right)
	{
		AddSquare(mouse_evt->pos);
	}
}

void PhysicBodyDemo::AddSquare(const Point& pos)
{
	SquarePtr rect = new Square(world_, pos, Size(80, 80));
	AddChild(rect);
}

void PhysicBodyDemo::AddCircle(const Point& pos)
{
	CirclePtr circle = new Circle(world_, pos, 50);
	AddChild(circle);
}

Board::Board(PhysicWorldPtr world, const Size& size, const Point& pos)
{
	// ���������ɫ
	this->SetFillColor(Color::Gray);

	// ����ľ��Ĵ�С��λ�ú���ת�Ƕ�
	this->SetRectSize(size);
	this->SetAnchor(0.5f, 0.5f);
	this->SetRotation(10);
	this->SetPosition(pos);

	PhysicBodyPtr body = new PhysicBody(world, PhysicBody::Type::Static);
	body->AddRectShape(this->GetSize(), 0.0f);
	this->AddComponent(body);
}
