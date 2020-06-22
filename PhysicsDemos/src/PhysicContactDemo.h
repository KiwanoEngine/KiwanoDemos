// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(PhysicContactDemo);
KGE_DECLARE_SMART_PTR(KeyText);

namespace
{
	const float min_h_force = 5.0f;		// ��С��������
	const float max_h_force = 20.0f;	// ����������
	const float min_v_force = 50.0f;	// ��С��������
	const float max_v_force = 80.0f;	// �����������
}

class PhysicContactDemo
	: public Stage
{
public:
	static inline StagePtr Create() { return new PhysicContactDemo; }

	static inline String Name() { return "Physic Contact Demo"; }

	static inline String Label() { return "��������������ӣ�"; }

	PhysicContactDemo();

	// �Ӵ���ʼ
	void OnContactBegin(Event* evt);

	// �Ӵ�����
	void OnContactEnd(Event* evt);

	// ����������ɫ
	void SetBodyColor(PhysicBody* body, const Color& color);

	void OnKeyDown(Event* evt);

	void OnUpdate(Duration dt) override;

private:
	PhysicWorldPtr world_;
};

// �����ı�
class KeyText
	: public TextActor
{
public:
	static KeyTextPtr Create(PhysicWorldPtr world, const Point& pos, char ch);
};


PhysicContactDemo::PhysicContactDemo()
{
	// ������������
	world_ = PhysicWorld::Create();
	AddComponent(world_);

	// ��������
	RectActorPtr ground = RectActor::Create(Size(GetWidth(), 10));
	ground->SetFillColor(Color::Gray);

	// ����ľ��Ĵ�С��λ�ú���ת�Ƕ�
	ground->SetAnchor(0.5f, 0.5f);
	ground->SetPosition(Point(GetWidth() / 2, GetHeight()));
	AddChild(ground);

	// ����������������
	PhysicBodyPtr ground_body = PhysicBody::Create(world_, PhysicBody::Type::Static);
	ground_body->AddRectShape(ground->GetSize(), 0.0f);
	ground->AddComponent(ground_body);

	// ��������
	AddListener<KeyDownEvent>(Closure(this, &PhysicContactDemo::OnKeyDown));

	// �Ӵ�����
	AddListener<ContactBeginEvent>(Closure(this, &PhysicContactDemo::OnContactBegin));
	AddListener<ContactEndEvent>(Closure(this, &PhysicContactDemo::OnContactEnd));
}

void PhysicContactDemo::OnKeyDown(Event* evt)
{
	KGE_ASSERT(evt->IsType<KeyDownEvent>());

	// ���� A-Z ��
	auto key_evt = dynamic_cast<KeyDownEvent*>(evt);
	if (key_evt->code >= KeyCode::A && key_evt->code <= KeyCode::Z)
	{
		// ������ĸֵ
		char ch = char(key_evt->code) - char(KeyCode::A) + 'A';
		// ����Ļ�ײ�����һ�� KeyText
		Point pos(GetWidth() / 2, GetHeight() - 20);
		KeyTextPtr key = KeyText::Create(world_, pos, ch);
		AddChild(key);
	}
}

void PhysicContactDemo::OnContactBegin(Event* evt)
{
	KGE_ASSERT(evt->IsType<ContactBeginEvent>());

	// ��������ײ����ɫ��Ϊ�ٺ�ɫ
	auto contact_evt = dynamic_cast<ContactBeginEvent*>(evt);
	SetBodyColor(contact_evt->contact.GetBodyA(), Color::OrangeRed);
	SetBodyColor(contact_evt->contact.GetBodyB(), Color::OrangeRed);
}

void PhysicContactDemo::OnContactEnd(Event* evt)
{
	KGE_ASSERT(evt->IsType<ContactEndEvent>());

	// ��������ײ��������ɫ��ذ�ɫ
	auto contact_evt = dynamic_cast<ContactEndEvent*>(evt);
	SetBodyColor(contact_evt->contact.GetBodyA(), Color::White);
	SetBodyColor(contact_evt->contact.GetBodyB(), Color::White);
}

void PhysicContactDemo::SetBodyColor(PhysicBody* body, const Color& color)
{
	// ֻ����̬����
	if (body->GetType() == PhysicBody::Type::Dynamic)
	{
		auto text = static_cast<TextActor*>(body->GetBoundActor());
		text->SetFillColor(color);
	}
}

void PhysicContactDemo::OnUpdate(Duration dt)
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

KeyTextPtr KeyText::Create(PhysicWorldPtr world, const Point& pos, char ch)
{
	KeyTextPtr key = new KeyText;

	// �����ı����ݡ��ֺź���ɫ
	key->SetText(String(1, ch));
	key->SetFillColor(Color::White);
	key->SetFontFamily("Arial");
	key->SetFontSize(35);
	key->SetFontWeight(FontWeight::ExtraBold);

	// ���������ê��
	key->SetAnchor(0.5f, 0.5f);
	key->SetPosition(pos);

	// �����������
	PhysicBodyPtr body = PhysicBody::Create(world, PhysicBody::Type::Dynamic);
	body->AddRectShape(key->GetSize(), 1.0f);
	key->AddComponent(body);

	// ������һ���������
	float ratio = body->GetMass() * 10.0f;
	float neg = (math::Random(0, 1) == 0 ? -1.0f : 1.0f);
	float h = neg * math::Random(min_h_force, max_h_force);
	float v = -math::Random(min_v_force, max_v_force);
	body->ApplyForceToCenter(Vec2(h, v) * ratio);
	return key;
}
