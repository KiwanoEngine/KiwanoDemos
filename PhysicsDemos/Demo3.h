// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Demo3);
KGE_DECLARE_SMART_PTR(KeyText);

namespace
{
	const float min_h_force = 5.0f;
	const float max_h_force = 20.0f;
	const float min_v_force = 50.0f;
	const float max_v_force = 80.0f;
}


class Demo3
	: public physics::World
{
	physics::BodyPtr ground_;

public:
	Demo3();

	// 接触开始
	void OnContactBegin(Event* evt);

	// 接触结束
	void OnContactEnd(Event* evt);

	// 设置物体颜色
	void SetBodyColor(physics::Body* body, const Color& color);

	void OnKeyDown(Event* evt);

	void OnUpdate(Duration dt) override;

	static inline StagePtr Create()
	{
		return new Demo3;
	}
};


// 按键文本
class KeyText
	: public TextActor
{
	physics::BodyPtr body_;

public:
	KeyText(physics::World* world, const Point& pos, WCHAR ch)
	{
		// 设置文本内容、字号和颜色
		SetText(String(1, ch));
		SetFillColor(Color::White);
		SetFontFamily(L"Arial");
		SetFontSize(35);
		SetFontWeight(FontWeight::ExtraBold);

		// 设置坐标和锚点
		SetAnchor(0.5f, 0.5f);
		SetPosition(pos);

		// 更新文本布局
		UpdateLayout();

		// 添加物理身体
		body_ = new physics::Body;
		body_->InitBody(world, this);
		body_->SetType(physics::Body::Type::Dynamic);
		body_->AddBoxShape(GetSize(), 1.f);

		// 给身体一个随机受力
		float neg = (math::Random(0, 1) == 0 ? -1.f : 1.f);
		float h = neg * math::Random(min_h_force, max_h_force);
		float v = -math::Random(min_v_force, max_v_force);
		body_->ApplyForceToCenter(Vec2(h, v));
	}
};


Demo3::Demo3()
{
	ground_ = new physics::Body;
	ground_->InitBody(this, nullptr);
	ground_->SetType(physics::Body::Type::Static);
	ground_->SetBodyPosition(Point(GetWidth() / 2, GetHeight()));
	ground_->AddBoxShape(Size(GetWidth(), 10));

	// 按键监听
	AddListener<KeyDownEvent>(Closure(this, &Demo3::OnKeyDown));

	// 接触监听
	AddListener<physics::ContactBeginEvent>(Closure(this, &Demo3::OnContactBegin));
	AddListener<physics::ContactEndEvent>(Closure(this, &Demo3::OnContactEnd));

	// 添加文本说明
	TextActorPtr intro = new TextActor(L"按任意键发射粒子！");
	intro->SetFillColor(Color::White);
	intro->SetAnchor(0.5f, 0.5f);
	intro->SetPosition(GetWidth() / 2, GetHeight() - 100);
	AddChild(intro);
}

void Demo3::OnContactBegin(Event* evt)
{
	// 两物体碰撞后颜色变为橘红色
	KGE_ASSERT(evt->IsType<physics::ContactBeginEvent>());
	auto contact_evt = dynamic_cast<physics::ContactBeginEvent*>(evt);
	SetBodyColor(contact_evt->contact.GetBodyA(), Color::OrangeRed);
	SetBodyColor(contact_evt->contact.GetBodyB(), Color::OrangeRed);
}

void Demo3::OnContactEnd(Event* evt)
{
	// 两物体碰撞结束后颜色变回白色
	KGE_ASSERT(evt->IsType<physics::ContactEndEvent>());
	auto contact_evt = dynamic_cast<physics::ContactEndEvent*>(evt);
	SetBodyColor(contact_evt->contact.GetBodyA(), Color::White);
	SetBodyColor(contact_evt->contact.GetBodyB(), Color::White);
}

void Demo3::SetBodyColor(physics::Body* body, const Color& color)
{
	// 只处理动态物体
	if (body->GetType() == physics::Body::Type::Dynamic)
	{
		auto text = static_cast<TextActor*>(body->GetActor());
		text->SetFillColor(color);
	}
}

void Demo3::OnKeyDown(Event* evt)
{
	KGE_ASSERT(evt->IsType<KeyDownEvent>());

	// 处理 A-Z 键
	auto key_evt = dynamic_cast<KeyDownEvent*>(evt);
	if (key_evt->code >= KeyCode::A && key_evt->code <= KeyCode::Z)
	{
		// 计算字母值
		wchar_t ch = wchar_t(key_evt->code) - wchar_t(KeyCode::A) + L'A';
		// 在屏幕底部创建一个 KeyText
		Point pos(GetWidth() / 2, GetHeight() - 20);
		KeyTextPtr key = new KeyText(this, pos, ch);
		AddChild(key);
	}
}

void Demo3::OnUpdate(Duration dt)
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
