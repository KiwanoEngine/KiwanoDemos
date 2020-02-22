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

	static inline String Label() { return "按←→↓键控制小车"; }

	PhysicJointDemo();

	void OnUpdate(Duration dt) override;

private:
	PhysicWorldPtr world_;
	ActorPtr map_;
	CarPtr car_;
};

// 地面
class Ground
	: public ShapeActor
{
public:
	static GroundPtr Create(PhysicWorldPtr world, Point const& pos);

	// 生成地面路径点
	Vector<Point> GeneratePathPoints();
};

// 小车
class Car
	: public Actor
{
	ShapeActorPtr chassis_;
	WheelJointPtr wheel_;

public:
	static CarPtr Create(PhysicWorldPtr world, Point const& pos);

	// 给小车后轮提供动力
	void SetSpeed(float speed);

	// 获取小车躯干的位置
	Point GetPosition() const;
};


PhysicJointDemo::PhysicJointDemo()
{
	// 创建物理世界
	world_ = PhysicWorld::Create();
	AddComponent(world_);

	// 创建地图
	map_ = Actor::Create();
	AddChild(map_);

	// 创建地面
	ActorPtr ground = Ground::Create(world_, Point(0, GetHeight() - 200));
	map_->AddChild(ground);

	// 创建小车
	car_ = Car::Create(world_, Point(190, 240));
	map_->AddChild(car_);
}

void PhysicJointDemo::OnUpdate(Duration dt)
{
	// 获取小车位置
	Point pos = car_->GetPosition();
	if (pos.x > GetWidth() / 2)
	{
		// 小车位置超过屏幕一半大小时, 移动整个地图, 让视角跟随小车
		map_->SetPositionX(GetWidth() / 2 - pos.x);
	}
	else
	{
		map_->SetPositionX(0.f);
	}

	// 按键处理
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

GroundPtr Ground::Create(PhysicWorldPtr world, Point const& pos)
{
	GroundPtr ground = new Ground;

	// 设置形状颜色和位置
	ground->SetStrokeColor(Color::White);
	ground->SetPosition(pos);

	// 生成路径点
	Vector<Point> path_points = ground->GeneratePathPoints();

	// 根据路径点绘制路径
	ShapeMakerPtr maker = ShapeMaker::Create();
	maker->BeginPath(path_points[0]);
	for (size_t i = 1; i < path_points.size(); ++i)
	{
		maker->AddLine(path_points[i]);
	}
	maker->EndPath();
	ground->SetShape(maker->GetShape());

	// 根据路径点生成物理边
	PhysicBodyPtr body = PhysicBody::Create(world, PhysicBody::Type::Static);
	for (size_t i = 1; i < path_points.size(); ++i)
	{
		body->AddEdgeShape(path_points[i - 1], path_points[i], 0.0f, 0.6f);
	}
	ground->AddComponent(body);
	return ground;
}

Vector<Point> Ground::GeneratePathPoints()
{
	Vector<Point> path_points;	// 路径点

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

CarPtr Car::Create(PhysicWorldPtr world, Point const& pos)
{
	CarPtr car = new Car;

	// 小车躯干形状顶点
	Vector<Point> vertices = { Point(-150, 50), Point(150, 50), Point(150, 0), Point(0, -90), Point(-115, -90), Point(-150, -20), };

	// 生成小车形状
	ShapeMakerPtr maker = ShapeMaker::Create();
	maker->BeginPath(vertices[0]);
	maker->AddLines(vertices);
	maker->EndPath(true);

	// 创建小车躯干
	ShapeActorPtr chassis = ShapeActor::Create(maker->GetShape());
	chassis->SetPosition(pos + Point(0, -100));
	chassis->SetStrokeColor(Color::White);
	car->AddChild(chassis);

	// 创建小车躯干的物理身体
	PhysicBodyPtr chassis_body = PhysicBody::Create(world, PhysicBody::Type::Dynamic);
	chassis_body->AddPolygonShape(vertices, 1.0f);
	chassis->AddComponent(chassis_body);

	// 创建左右轮子
	CirclePtr lwheel = Circle::Create(world, pos + Point(-100, -35), 40.0f);
	CirclePtr rwheel = Circle::Create(world, pos + Point(100, -40), 40.0f);
	car->AddChild(lwheel);
	car->AddChild(rwheel);

	auto lwheel_body = lwheel->GetPhysicBody();
	auto rwheel_body = rwheel->GetPhysicBody();

	// 设置摩擦力
	lwheel_body->GetFixtureList().begin()->SetFriction(0.9f);
	rwheel_body->GetFixtureList().begin()->SetFriction(0.9f);

	// 创建左轮子关节
	WheelJoint::Param param1(chassis_body, lwheel_body, lwheel_body->GetPosition(), Vec2(0, 1));
	param1.frequency_hz = 4.0f;			// 频率和阻尼率赋予关节弹性
	param1.damping_ratio = 0.7f;
	param1.enable_motor = true;			// 启用马达
	param1.motor_speed = 0.0f;			// 初始马达速度为零
	param1.max_motor_torque = 2000;		// 最大马达转矩

	WheelJointPtr left_joint = WheelJoint::Create(param1);
	world->AddJoint(left_joint);

	// 创建右轮子关节
	WheelJoint::Param param2(chassis_body, rwheel_body, rwheel_body->GetPosition(), Vec2(0, 1));
	param2.frequency_hz = 4.0f;
	param2.damping_ratio = 0.7f;
	param2.enable_motor = false;
	param2.motor_speed = 0.0f;
	param2.max_motor_torque = 1000;

	WheelJointPtr right_joint = WheelJoint::Create(param2);
	world->AddJoint(right_joint);

	car->chassis_ = chassis;
	car->wheel_ = left_joint;
	return car;
}

void Car::SetSpeed(float speed)
{
	wheel_->SetMotorSpeed(speed);
}

Point Car::GetPosition() const
{
	return chassis_->GetPosition();
}
