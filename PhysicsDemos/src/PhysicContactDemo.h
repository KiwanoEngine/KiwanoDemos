// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(PhysicContactDemo);
KGE_DECLARE_SMART_PTR(KeyText);

namespace
{
	const float min_h_force = 5.0f;		// 最小横向受力
	const float max_h_force = 20.0f;	// 最大横向受力
	const float min_v_force = 50.0f;	// 最小纵向受力
	const float max_v_force = 80.0f;	// 最大纵向受力
}

class PhysicContactDemo
	: public Stage
{
public:
	static inline StagePtr Create() { return new PhysicContactDemo; }

	static inline String Name() { return "Physic Contact Demo"; }

	static inline String Label() { return "按任意键发射粒子！"; }

	PhysicContactDemo();

	// 接触开始
	void OnContactBegin(Event* evt);

	// 接触结束
	void OnContactEnd(Event* evt);

	// 设置物体颜色
	void SetBodyColor(PhysicBody* body, const Color& color);

	void OnKeyDown(Event* evt);

	void OnUpdate(Duration dt) override;

private:
	PhysicWorldPtr world_;
};

// 按键文本
class KeyText
	: public TextActor
{
public:
	static KeyTextPtr Create(PhysicWorldPtr world, const Point& pos, char ch);
};


PhysicContactDemo::PhysicContactDemo()
{
	// 创建物理世界
	world_ = PhysicWorld::Create();
	AddComponent(world_);

	// 创建地面
	RectActorPtr ground = RectActor::Create(Size(GetWidth(), 10));
	ground->SetFillColor(Color::Gray);

	// 设置木板的大小、位置和旋转角度
	ground->SetAnchor(0.5f, 0.5f);
	ground->SetPosition(Point(GetWidth() / 2, GetHeight()));
	AddChild(ground);

	// 创建地面物理身体
	PhysicBodyPtr ground_body = PhysicBody::Create(world_, PhysicBody::Type::Static);
	ground_body->AddRectShape(ground->GetSize(), 0.0f);
	ground->AddComponent(ground_body);

	// 按键监听
	AddListener<KeyDownEvent>(Closure(this, &PhysicContactDemo::OnKeyDown));

	// 接触监听
	AddListener<ContactBeginEvent>(Closure(this, &PhysicContactDemo::OnContactBegin));
	AddListener<ContactEndEvent>(Closure(this, &PhysicContactDemo::OnContactEnd));
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
		KeyTextPtr key = KeyText::Create(world_, pos, ch);
		AddChild(key);
	}
}

void PhysicContactDemo::OnContactBegin(Event* evt)
{
	KGE_ASSERT(evt->IsType<ContactBeginEvent>());

	// 两物体碰撞后颜色变为橘红色
	auto contact_evt = dynamic_cast<ContactBeginEvent*>(evt);
	SetBodyColor(contact_evt->contact.GetBodyA(), Color::OrangeRed);
	SetBodyColor(contact_evt->contact.GetBodyB(), Color::OrangeRed);
}

void PhysicContactDemo::OnContactEnd(Event* evt)
{
	KGE_ASSERT(evt->IsType<ContactEndEvent>());

	// 两物体碰撞结束后颜色变回白色
	auto contact_evt = dynamic_cast<ContactEndEvent*>(evt);
	SetBodyColor(contact_evt->contact.GetBodyA(), Color::White);
	SetBodyColor(contact_evt->contact.GetBodyB(), Color::White);
}

void PhysicContactDemo::SetBodyColor(PhysicBody* body, const Color& color)
{
	// 只处理动态物体
	if (body->GetType() == PhysicBody::Type::Dynamic)
	{
		auto text = static_cast<TextActor*>(body->GetBoundActor());
		text->SetFillColor(color);
	}
}

void PhysicContactDemo::OnUpdate(Duration dt)
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

KeyTextPtr KeyText::Create(PhysicWorldPtr world, const Point& pos, char ch)
{
	KeyTextPtr key = new KeyText;

	// 设置文本内容、字号和颜色
	key->SetText(String(1, ch));
	key->SetFillColor(Color::White);
	key->SetFontFamily("Arial");
	key->SetFontSize(35);
	key->SetFontWeight(FontWeight::ExtraBold);

	// 设置坐标和锚点
	key->SetAnchor(0.5f, 0.5f);
	key->SetPosition(pos);

	// 添加物理身体
	PhysicBodyPtr body = PhysicBody::Create(world, PhysicBody::Type::Dynamic);
	body->AddRectShape(key->GetSize(), 1.0f);
	key->AddComponent(body);

	// 给身体一个随机受力
	float ratio = body->GetMass() * 10.0f;
	float neg = (math::Random(0, 1) == 0 ? -1.0f : 1.0f);
	float h = neg * math::Random(min_h_force, max_h_force);
	float v = -math::Random(min_v_force, max_v_force);
	body->ApplyForceToCenter(Vec2(h, v) * ratio);
	return key;
}
