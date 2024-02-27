// Copyright (C) 2019 Nomango

#pragma once
#include "CircleBody.h"
#include "SquareBody.h"

class PhysicBodyDemo
	: public Stage
{
public:
	static inline RefPtr<Stage> Create() { return new PhysicBodyDemo; }

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
	RefPtr<physics::World> world_;
};

// 木板
class Board
	: public RectActor
{
public:
	Board(RefPtr<physics::World> world, const Size& size, const Point& pos);
};


PhysicBodyDemo::PhysicBodyDemo()
{
	// 创建物理世界
	world_ = new physics::World(b2Vec2(0, 10.f));
	AddComponent(world_);

	// 添加鼠标传感器，使舞台可以接收到鼠标点击消息
	RefPtr<Component> comp = new MouseSensor;
	AddComponent(comp);

	// 添加鼠标点击监听
	AddListener<MouseClickEvent>(Closure(this, &PhysicBodyDemo::OnClick));

	// 添加一块静态木板
	RefPtr<Board> board = new Board(world_, Size(GetWidth() - 100, 20), Point(GetWidth() / 2, GetHeight() - 110));
	AddChild(board);

	// 添加一个小球
	AddCircle(Point(GetWidth() / 2, 200));
}

void PhysicBodyDemo::OnUpdate(Duration dt)
{
	// 移除掉落到场景外的物体
	Vector<RefPtr<Actor>> outed;
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
	RefPtr<Square> rect = new Square(world_, pos, Size(80, 80));
	AddChild(rect);
}

void PhysicBodyDemo::AddCircle(const Point& pos)
{
	RefPtr<Circle> circle = new Circle(world_, pos, 50);
	AddChild(circle);
}

Board::Board(RefPtr<physics::World> world, const Size& size, const Point& pos)
{
	// 设置填充颜色
	this->SetFillColor(Color::Gray);

	// 设置木板的大小、位置和旋转角度
	this->SetRectSize(size);
	this->SetAnchor(0.5f, 0.5f);
	this->SetRotation(10);
	this->SetPosition(pos);

	b2BodyDef def;
	def.type = b2_staticBody;
	auto body = world->AddBody(&def);
	this->AddComponent(body);

	b2PolygonShape shape;
	b2Vec2 b2size = physics::LocalToWorld(this->GetSize());
	shape.SetAsBox(b2size.x / 2, b2size.y / 2, b2Vec2_zero, math::Degree2Radian(0.f));

	b2FixtureDef fd;
	fd.friction = 0.2f;
	fd.shape = &shape;
	body->GetB2Body()->CreateFixture(&fd);
}
