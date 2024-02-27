// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class PhysicContactDemo
	: public Stage
{
public:
	static inline RefPtr<Stage> Create() { return new PhysicContactDemo; }

	static inline String Name() { return "Physic Contact Demo"; }

	static inline String Label() { return "按任意键发射粒子！"; }

	PhysicContactDemo();

	// 接触开始
	void OnContactBegin(Event* evt);

	// 接触结束
	void OnContactEnd(Event* evt);

	// 设置物体颜色
	void SetBodyColor(physics::Body* body, const Color& color);

	void OnKeyDown(Event* evt);

	void OnUpdate(Duration dt) override;

private:
	RefPtr<physics::World> world_;
};

// 按键文本
class KeyText
	: public TextActor
{
public:
	KeyText(RefPtr<physics::World> world, const Point& pos, char ch);
};


PhysicContactDemo::PhysicContactDemo()
{
	// 创建物理世界
	world_ = new physics::World(b2Vec2(0, 10.f));
	AddComponent(world_);

	// 创建地面
	RefPtr<RectActor> ground = new RectActor(Size(GetWidth() / 2, 10));
	ground->SetFillColor(Color::Gray);

	// 设置木板的大小、位置和旋转角度
	ground->SetAnchor(0.5f, 0.5f);
	ground->SetPosition(Point(GetWidth() / 2, GetHeight()));
	AddChild(ground);

	// 创建地面物理身体
	b2BodyDef def;
	def.type = b2_staticBody;
	auto ground_body = world_->AddBody(&def);
	ground->AddComponent(ground_body);

	b2PolygonShape shape;
	b2Vec2 b2size = physics::LocalToWorld(ground->GetSize());
	shape.SetAsBox(b2size.x / 2, b2size.y / 2, b2Vec2_zero, math::Degree2Radian(0.f));

	b2FixtureDef fd;
	fd.friction = 0.2f;
	fd.shape = &shape;
	ground_body->GetB2Body()->CreateFixture(&fd);

	// 按键监听
	AddListener<KeyDownEvent>(Closure(this, &PhysicContactDemo::OnKeyDown));

	// 接触监听
	AddListener<physics::ContactBeginEvent>(Closure(this, &PhysicContactDemo::OnContactBegin));
	AddListener<physics::ContactEndEvent>(Closure(this, &PhysicContactDemo::OnContactEnd));
}

void PhysicContactDemo::OnKeyDown(Event* evt)
{
	KGE_ASSERT(evt->IsType<KeyDownEvent>());

	// 处理 A-Z 键
	auto key_evt = dynamic_cast<KeyDownEvent*>(evt);
	if (key_evt->code >= KeyCode::A && key_evt->code <= KeyCode::Z)
	{
		// 计算字母值
		char ch = char(key_evt->code) - char(KeyCode::A) + 'A';
		// 在屏幕底部创建一个 KeyText
		Point pos(GetWidth() / 2, GetHeight() - 20);
		RefPtr<KeyText> key = new KeyText(world_, pos, ch);
		AddChild(key);
	}
}

void PhysicContactDemo::OnContactBegin(Event* evt)
{
	KGE_ASSERT(evt->IsType<physics::ContactBeginEvent>());

	// 两物体碰撞后颜色变为橘红色
	auto contact_evt = dynamic_cast<physics::ContactBeginEvent*>(evt);
	SetBodyColor((physics::Body*)contact_evt->contact->GetFixtureA()->GetBody()->GetUserData(), Color::OrangeRed);
	SetBodyColor((physics::Body*)contact_evt->contact->GetFixtureB()->GetBody()->GetUserData(), Color::OrangeRed);
}

void PhysicContactDemo::OnContactEnd(Event* evt)
{
	KGE_ASSERT(evt->IsType<physics::ContactEndEvent>());

	// 两物体碰撞结束后颜色变回白色
	auto contact_evt = dynamic_cast<physics::ContactEndEvent*>(evt);
	SetBodyColor((physics::Body*)contact_evt->contact->GetFixtureA()->GetBody()->GetUserData(), Color::White);
	SetBodyColor((physics::Body*)contact_evt->contact->GetFixtureB()->GetBody()->GetUserData(), Color::White);
}

void PhysicContactDemo::SetBodyColor(physics::Body* body, const Color& color)
{
	// 只处理动态物体
	if (body->GetB2Body()->GetType() == b2_dynamicBody)
	{
		auto text = dynamic_cast<TextActor*>(body->GetBoundActor());
		text->SetFillColor(color);
	}
}

void PhysicContactDemo::OnUpdate(Duration dt)
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

KeyText::KeyText(RefPtr<physics::World> world, const Point& pos, char ch)
{
	// 设置文本内容、字号和颜色
	this->SetText(String(1, ch));
	this->SetFillColor(Color::White);
	this->SetFont(new Font("Arial", 35, FontWeight::ExtraBold));

	// 设置坐标和锚点
	this->SetAnchor(0.5f, 0.5f);
	this->SetPosition(pos);

	// 添加物理身体
	b2BodyDef def;
	def.type = b2_dynamicBody;
	auto body = world->AddBody(&def);
	this->AddComponent(body);

	b2PolygonShape shape;
	b2Vec2 b2size = physics::LocalToWorld(this->GetSize());
	shape.SetAsBox(b2size.x / 2, b2size.y / 2, b2Vec2_zero, math::Degree2Radian(0.f));

	b2FixtureDef fd;
	fd.density = 1.0f;
	fd.friction = 0.2f;
	fd.shape = &shape;
	body->GetB2Body()->CreateFixture(&fd);

	// 给物体一个随机横向受力
	float neg = (math::Random(0, 1) == 0 ? -1.0f : 1.0f);
	float h = neg * math::Random(1.f, 2.f) * 100.f;

	float ratio = body->GetB2Body()->GetMass();
	float v = -math::Random(4.f, 6.f) * 100.f;
	body->GetB2Body()->ApplyForceToCenter(b2Vec2(h * ratio, v * ratio), true);
}
