// Copyright (C) 2019 Nomango

#pragma once
#include "CircleBody.h"

KGE_DECLARE_SMART_PTR(PhysicJointDemo);
KGE_DECLARE_SMART_PTR(Car);
KGE_DECLARE_SMART_PTR(Ground);

class PhysicJointDemo
	: public Stage
{
public:
	static inline StagePtr Create() { return new PhysicJointDemo; }

	static inline String Name() { return "Physic Joint Demo"; }

	static inline String Label() { return "��������������С��"; }

	PhysicJointDemo();

	void OnUpdate(Duration dt) override;

private:
	PhysicWorldPtr world_;
	ActorPtr map_;
	CarPtr car_;
};

// ����
class Ground
	: public ShapeActor
{
public:
	Ground(PhysicWorldPtr world, Point const& pos);

	// ���ɵ���·����
	Vector<Point> GeneratePathPoints();
};

// С��
class Car
	: public Actor
{
	ShapeActorPtr chassis_;
	WheelJointPtr wheel_;

public:
	Car(PhysicWorldPtr world, Point const& pos);

	// ��С�������ṩ����
	void SetSpeed(float speed);

	// ��ȡС�����ɵ�λ��
	Point GetPosition() const;
};


PhysicJointDemo::PhysicJointDemo()
{
	// ������������
	world_ = new PhysicWorld();
	AddComponent(world_);

	// ������ͼ
	map_ = new Actor();
	AddChild(map_);

	// ��������
	ActorPtr ground = new Ground(world_, Point(0, GetHeight() - 200));
	map_->AddChild(ground);

	// ����С��
	car_ = new Car(world_, Point(190, 240));
	map_->AddChild(car_);
}

void PhysicJointDemo::OnUpdate(Duration dt)
{
	// ��ȡС��λ��
	Point pos = car_->GetPosition();
	if (pos.x > GetWidth() / 2)
	{
		// С��λ�ó�����Ļһ���Сʱ, �ƶ�������ͼ, ���ӽǸ���С��
		map_->SetPositionX(GetWidth() / 2 - pos.x);
	}
	else
	{
		map_->SetPositionX(0.f);
	}

	// ��������
	auto& input = Input::GetInstance();
	if (input.IsDown(KeyCode::Right))
	{
		car_->SetSpeed(360.0f * 3);
	}
	else if (input.IsDown(KeyCode::Left))
	{
		car_->SetSpeed(-360.0f * 3);
	}
	else if (input.IsDown(KeyCode::Down))
	{
		car_->SetSpeed(0);
	}
}

Ground::Ground(PhysicWorldPtr world, Point const& pos)
{
	// ������״��ɫ��λ��
	this->SetStrokeColor(Color::White);
	this->SetPosition(pos);

	// ����·����
	Vector<Point> path_points = this->GeneratePathPoints();

	// ����·�������·��
	ShapeMaker maker;
	maker.BeginPath(path_points[0]);
	for (size_t i = 1; i < path_points.size(); ++i)
	{
		maker.AddLine(path_points[i]);
	}
	maker.EndPath();
	this->SetShape(maker.GetShape());

	// ����·�������������
	PhysicBodyPtr body = new PhysicBody(world, PhysicBody::Type::Static);
	for (size_t i = 1; i < path_points.size(); ++i)
	{
		body->AddEdgeShape(path_points[i - 1], path_points[i], 0.0f, 0.6f);
	}
	this->AddComponent(body);
}

Vector<Point> Ground::GeneratePathPoints()
{
	Vector<Point> path_points;	// ·����

	path_points.push_back(Point(5, -400));
	path_points.push_back(Point(5, 0));
	path_points.push_back(Point(200, 0));

	float hs[] = { -25, -100, -250, 0, 0, 100, 200, 200, 125, 0 };
	float x = 200, dx = 500;

	for (int i = 0; i < 10; ++i)
	{
		float y = hs[i];
		path_points.push_back(Point(x + dx, y));
		x += dx;
	}

	for (int i = 0; i < 10; ++i)
	{
		float y = hs[i];
		path_points.push_back(Point(x + dx, y));
		x += dx;
	}

	path_points.push_back(Point(x, -400));
	return path_points;
}

Car::Car(PhysicWorldPtr world, Point const& pos)
{
	// С��������״����
	Vector<Point> vertices = { Point(-150, 50), Point(150, 50), Point(150, 0), Point(0, -90), Point(-115, -90), Point(-150, -20), };

	// ����С����״
	ShapeMaker maker;
	maker.BeginPath(vertices[0]);
	maker.AddLines(vertices);
	maker.EndPath(true);

	// ����С������
	ShapeActorPtr chassis = new ShapeActor(maker.GetShape());
	chassis->SetPosition(pos + Point(0, -100));
	chassis->SetStrokeColor(Color::White);
	this->AddChild(chassis);

	// ����С�����ɵ���������
	PhysicBodyPtr chassis_body = new PhysicBody(world, PhysicBody::Type::Dynamic);
	chassis_body->AddPolygonShape(vertices, 1.0f);
	chassis->AddComponent(chassis_body);

	// ������������
	CirclePtr lwheel = new Circle(world, pos + Point(-100, -35), 40.0f);
	CirclePtr rwheel = new Circle(world, pos + Point(100, -40), 40.0f);
	this->AddChild(lwheel);
	this->AddChild(rwheel);

	auto lwheel_body = lwheel->GetPhysicBody();
	auto rwheel_body = rwheel->GetPhysicBody();

	// ����Ħ����
	lwheel_body->GetFixtureList().begin()->SetFriction(0.9f);
	rwheel_body->GetFixtureList().begin()->SetFriction(0.9f);

	// ���������ӹؽ�
	WheelJoint::Param param1(chassis_body, lwheel_body, lwheel_body->GetPosition(), Vec2(0, 1));
	param1.frequency_hz = 4.0f;			// Ƶ�ʺ������ʸ���ؽڵ���
	param1.damping_ratio = 0.7f;
	param1.enable_motor = true;			// �������
	param1.motor_speed = 0.0f;			// ��ʼ����ٶ�Ϊ��
	param1.max_motor_torque = 2000;		// ������ת��

	WheelJointPtr left_joint = new WheelJoint(param1);
	world->AddJoint(left_joint);

	// ���������ӹؽ�
	WheelJoint::Param param2(chassis_body, rwheel_body, rwheel_body->GetPosition(), Vec2(0, 1));
	param2.frequency_hz = 4.0f;
	param2.damping_ratio = 0.7f;
	param2.enable_motor = false;
	param2.motor_speed = 0.0f;
	param2.max_motor_torque = 1000;

	WheelJointPtr right_joint = new WheelJoint(param2);
	world->AddJoint(right_joint);

	this->chassis_ = chassis;
	this->wheel_ = left_joint;
}

void Car::SetSpeed(float speed)
{
	wheel_->SetMotorSpeed(speed);
}

Point Car::GetPosition() const
{
	return chassis_->GetPosition();
}
