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

	static inline String Label() { return "点击鼠标左右键创建物体"; }

	PhysicBodyDemo();

	void OnUpdate(Duration dt) override;

	// 鼠标点击事件
	void OnClick(Event* evt);

	// 添加方块
	void AddSquare(const Point& pos);

	// 添加小球
	void AddCircle(const Point& pos);

private:
	PhysicWorldPtr world_;
};

// 木板
class Board
	: public RectActor
{
public:
	static BoardPtr Create(PhysicWorldPtr world, const Size& size, const Point& pos);
};


PhysicBodyDemo::PhysicBodyDemo()
{
	// 创建物理世界
	world_ = PhysicWorld::Create();
	AddComponent(world_);

	// 设置可响应状态, 使舞台可以接收到鼠标点击消息
	SetResponsible(true);

	// 添加鼠标点击监听
	AddListener<MouseClickEvent>(Closure(this, &PhysicBodyDemo::OnClick));

	// 添加一块静态木板
	BoardPtr board = Board::Create(world_, Size(GetWidth() - 100, 20), Point(GetWidth() / 2, GetHeight() - 110));
	AddChild(board);

	// 添加一个小球
	AddCircle(Point(GetWidth() / 2, 200));
}

void PhysicBodyDemo::OnUpdate(Duration dt)
{
	// 移除掉落到场景外的物体
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

	// 左键添加一个小球, 右键添加一个盒子
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
	SquarePtr rect = Square::Create(world_, pos, Size(80, 80));
	AddChild(rect);
}

void PhysicBodyDemo::AddCircle(const Point& pos)
{
	CirclePtr circle = Circle::Create(world_, pos, 50);
	AddChild(circle);
}

BoardPtr Board::Create(PhysicWorldPtr world, const Size& size, const Point& pos)
{
	BoardPtr board = new Board;

	// 设置填充颜色
	board->SetFillColor(Color::Gray);

	// 设置木板的大小、位置和旋转角度
	board->SetRectSize(size);
	board->SetAnchor(0.5f, 0.5f);
	board->SetRotation(10);
	board->SetPosition(pos);

	PhysicBodyPtr body = PhysicBody::Create(world, PhysicBody::Type::Static);
	body->AddRectShape(board->GetSize(), 0.0f);
	board->AddComponent(body);
	return board;
}
