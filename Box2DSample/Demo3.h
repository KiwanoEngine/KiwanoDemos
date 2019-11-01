// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Demo3);
KGE_DECLARE_SMART_PTR(KeyText);

class Demo3
	: public PhysicWorld
{
	PhysicBodyPtr ground_;
	PhysicContactCallbackListenerPtr listener_;

public:
	Demo3();

	// �Ӵ���ʼ
	void OnContactBegin(PhysicContact contact);

	// �Ӵ�����
	void OnContactEnd(PhysicContact contact);

	// ����������ɫ
	void SetBodyColor(PhysicBodyPtr body, const Color& color);

	void OnKeyDown(Event const& evt);

	void OnUpdate(Duration dt) override;

	static inline StagePtr Create()
	{
		return new Demo3;
	}
};


// �����ı�
class KeyText
	: public Text
{
	PhysicBodyPtr body_;

public:
	KeyText(PhysicWorld* world, const Point& pos, WCHAR ch)
	{
		// �����ı����ݡ��ֺź���ɫ
		SetText(String(1, ch));
		SetColor(Color::White);
		SetFontSize(35);
		SetFontWeight(FontWeight::ExtraBold);

		// ���������ê��
		SetAnchor(0.5f, 0.5f);
		SetPosition(pos);

		// �����ı�����
		UpdateLayout();

		// �����������
		body_ = new PhysicBody(world, this);
		body_->SetType(PhysicBody::Type::Dynamic);
		body_->AddBoxShape(GetSize(), 1.f);

		// ������һ���������
		float neg = (math::Random(0, 1) == 0 ? -1.f : 1.f);
		float h = neg * math::Random(5.f, 20.f);
		float v = -math::Random(60.f, 80.f);
		body_->ApplyForceToCenter(Vec2(h, v));
	}
};


Demo3::Demo3()
{
	ground_ = new PhysicBody(this, nullptr);
	ground_->SetType(PhysicBody::Type::Static);
	ground_->SetBodyPosition(Point(GetWidth() / 2, GetHeight()));
	ground_->AddBoxShape(Size(GetWidth(), 10));

	// ��������
	AddListener(Event::KeyDown, Closure(this, &Demo3::OnKeyDown));

	// �Ӵ�����
	listener_ = new PhysicContactCallbackListener;
	listener_->SetCallbackOnContactBegin(Closure(this, &Demo3::OnContactBegin));
	listener_->SetCallbackOnContactEnd(Closure(this, &Demo3::OnContactEnd));
	AddContactListener(listener_);
}

inline void Demo3::OnContactBegin(PhysicContact contact)
{
	auto body_a = contact.GetFixtureA().GetBody();
	auto body_b = contact.GetFixtureB().GetBody();
	SetBodyColor(body_a, Color::OrangeRed);
	SetBodyColor(body_b, Color::OrangeRed);
}

inline void Demo3::OnContactEnd(PhysicContact contact)
{
	auto body_a = contact.GetFixtureA().GetBody();
	auto body_b = contact.GetFixtureB().GetBody();
	SetBodyColor(body_a, Color::White);
	SetBodyColor(body_b, Color::White);
}

void Demo3::SetBodyColor(PhysicBodyPtr body, const Color& color)
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
