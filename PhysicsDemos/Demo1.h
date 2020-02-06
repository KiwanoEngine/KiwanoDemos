// Copyright (C) 2019 Nomango

#pragma once
#include "Circle.h"
#include "Square.h"

KGE_DECLARE_SMART_PTR(Demo1);
KGE_DECLARE_SMART_PTR(Board);

class Demo1
	: public physics::World
{
public:
	Demo1();

	void OnUpdate(Duration dt) override;

	// 鼠标点击事件
	void OnClick(Event* evt);

	// 添加方块
	void AddSquare(const Point& pos);

	// 添加小球
	void AddCircle(const Point& pos);

	static inline StagePtr Create() { return new Demo1; }
};


// 木板
class Board
	: public RectActor
{
	physics::BodyPtr body_;

public:
	Board(physics::World* world, const Size& size, const Point& pos)
	{
		// 设置线条和填充颜色
		Color color = 0x868686;
		SetStrokeColor(color);
		SetFillColor(color);

		// 设置木板的大小、位置和旋转角度
		SetRectSize(size);
		SetAnchor(0.5f, 0.5f);
		SetRotation(10);
		SetPosition(pos);

		body_ = new physics::Body;
		body_->InitBody(world, this);
		body_->SetType(physics::Body::Type::Static);
		body_->AddBoxShape(GetSize());
	}
};


Demo1::Demo1()
{
	// 设置可响应状态, 使舞台可以接收到鼠标点击消息
	SetResponsible(true);

	// 添加鼠标点击监听
	AddListener<MouseClickEvent>(Closure(this, &Demo1::OnClick));

	// 添加一块静态木板
	BoardPtr board = new Board(this, Size(GetWidth() - 100, 20), Point(GetWidth() / 2, GetHeight() - 110));
	AddChild(board);

	// 添加一个小球
	AddCircle(Point(GetWidth() / 2, 200));

	// 添加文本说明
	TextActorPtr intro = new TextActor(L"点击鼠标左右键创建物体");
	intro->SetFillColor(Color::White);
	intro->SetAnchor(0.5f, 0.5f);
	intro->SetPosition(GetWidth() / 2, GetHeight() / 2);
	AddChild(intro);
}

void Demo1::OnUpdate(Duration dt)
{
	// 移除掉落到场景外的物体
	Vector<ActorPtr> outed;
	for (auto& child : GetAllChildren())
	{
		if (child.GetPosition().y > GetHeight() + 50)
		{
			outed.push_back(&child);
		}
	}

	for (auto child : outed)
	{
		child->RemoveFromParent();
	}
}

void Demo1::OnClick(Event* evt)
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
