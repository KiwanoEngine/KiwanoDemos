// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Demo3);
KGE_DECLARE_SMART_PTR(KeyText);

namespace
{
	const float min_h_force = 5.0f;
	const float max_h_force = 20.0f;
	const float min_v_force = 40.0f;
	const float max_v_force = 60.0f;
}


class Demo3
	: public PhysicWorld
{
	PhysicBodyPtr ground_;

public:
	Demo3();

	// 接触开始
	void OnContactBegin(PhysicContact contact);

	// 接触结束
	void OnContactEnd(PhysicContact contact);

	// 设置物体颜色
	void SetBodyColor(PhysicBody* body, const Color& color);

	void OnKeyDown(Event const& evt);

	void OnUpdate(Duration dt) override;

	static inline StagePtr Create()
	{
		return new Demo3;
	}
};


// 按键文本
class KeyText
	: public Text
{
	PhysicBodyPtr body_;

public:
	KeyText(PhysicWorld* world, const Point& pos, WCHAR ch)
	{
		// 设置文本内容、字号和颜色
		SetText(String(1, ch));
		SetColor(Color::White);
		SetFontSize(35);
		SetFontWeight(FontWeight::ExtraBold);

		// 设置坐标和锚点
		SetAnchor(0.5f, 0.5f);
		SetPosition(pos);

		// 更新文本布局
		UpdateLayout();

		// 添加物理身体
		body_ = new PhysicBody(world, this);
		body_->SetType(PhysicBody::Type::Dynamic);
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
	ground_ = new PhysicBody(this, nullptr);
	ground_->SetType(PhysicBody::Type::Static);
	ground_->SetBodyPosition(Point(GetWidth() / 2, GetHeight()));
	ground_->AddBoxShape(Size(GetWidth(), 10));

	// 按键监听
	AddListener(Event::KeyDown, Closure(this, &Demo3::OnKeyDown));

	// 接触监听
	auto listener = new PhysicContactCallbackListener;
	listener->SetCallbackOnContactBegin(Closure(this, &Demo3::OnContactBegin));
	listener->SetCallbackOnContactEnd(Closure(this, &Demo3::OnContactEnd));
	AddContactListener(listener);

	// 添加文本说明
	TextPtr intro = new Text(L"按任意键发射粒子！");
	intro->SetAnchor(0.5f, 0.5f);
	intro->SetPosition(GetWidth() / 2, GetHeight() - 100);
	AddChild(intro);
}

void Demo3::OnContactBegin(PhysicContact contact)
{
	SetBodyColor(contact.GetFixtureA().GetBody(), Color::OrangeRed);
	SetBodyColor(contact.GetFixtureB().GetBody(), Color::OrangeRed);
}

void Demo3::OnContactEnd(PhysicContact contact)
{
	SetBodyColor(contact.GetFixtureA().GetBody(), Color::White);
	SetBodyColor(contact.GetFixtureB().GetBody(), Color::White);
}

void Demo3::SetBodyColor(PhysicBody* body, const Color& color)
{
	if (body->GetType() == PhysicBody::Type::Dynamic)
	{
		auto text = static_cast<Text*>(body->GetActor());
		text->SetColor(color);
	}
}

void Demo3::OnKeyDown(Event const& evt)
{
	if (evt.key.code >= KeyCode::A && evt.key.code <= KeyCode::Z)
	{
		Point pos(GetWidth() / 2, GetHeight() - 20);
		KeyTextPtr key = new KeyText(this, pos, evt.key.code - KeyCode::A + L'A');
		AddChild(key);
	}
}

void Demo3::OnUpdate(Duration dt)
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
