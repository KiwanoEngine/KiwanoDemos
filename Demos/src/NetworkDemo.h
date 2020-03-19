// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"
#include <iostream>

class NetworkDemo
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new NetworkDemo;
	}

	static String DemoName()
	{
		return "Network Demo";
	}

	NetworkDemo()
	{
		// ��Ӱ�������
		AddListener<KeyDownEvent>(Closure(this, &NetworkDemo::OnKeyDown));

		// ����˵������
		TextActorPtr intro = TextActor::Create("��G����GET����\n��P����POST����\n��U����PUT����\n��D����DELETE����");
		// ��������λ��
		intro->SetAnchor(0.5f, 0.5f);
		intro->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		intro->SetFillColor(Color::White);

		// ��ӵ���̨
		this->AddChild(intro);
	}

	void OnEnter() override
	{
		// ������̨ʱ�򿪿���̨
		Logger::GetInstance().ShowConsole(true);
	}

	void OnExit() override
	{
		// �˳���̨ʱ�رտ���̨
		Logger::GetInstance().ShowConsole(false);
	}

	void OnKeyDown(Event* evt)
	{
		KGE_ASSERT(evt->IsType<KeyDownEvent>());

		// ����ͬ�����Ͳ�ͬ����
		auto key_evt = dynamic_cast<KeyDownEvent*>(evt);
		if (key_evt->code == KeyCode::G)
		{
			SendGetRequest();
		}
		else if (key_evt->code == KeyCode::P)
		{
			SendPostRequest();
		}
		else if (key_evt->code == KeyCode::U)
		{
			SendPutRequest();
		}
		else if (key_evt->code == KeyCode::D)
		{
			SendDeleteRequest();
		}
	}

	void SendGetRequest()
	{
		// ���� GET ����
		KGE_LOG("Start to send GET request...");

		// ����HTTP����
		HttpRequestPtr request = HttpRequest::Create(
			"http://httpbin.org/get",
			HttpType::Get,
			Closure(this, &NetworkDemo::Complete)
		);

		// ���� HTTP ����
		HttpModule::GetInstance().Send(request);
	}

	void SendPostRequest()
	{
		// ���� POST ����
		KGE_LOG("Start to send POST request...");

		// ���� JSON ��ʽ�� POST ����
		Json request_data = {
			{ "string", "a simple string" },
			{ "boolean", true },
			{ "integer", 12 },
			{ "float", 3.125 },
			{ "array", { 1, 2, 3, 4, 4.5 } },
			{ "object", { "key", "value" } },
		};

		HttpRequestPtr request = HttpRequest::Create(
			"http://httpbin.org/post",
			HttpType::Post,
			request_data,
			Closure(this, &NetworkDemo::Complete)
		);

		HttpModule::GetInstance().Send(request);
	}

	void SendPutRequest()
	{
		// ���� PUT ����
		KGE_LOG("Start to send PUT request...");

		// ���� JSON ��ʽ�� PUT ����
		Json request_data = Json::array({ 1, 2, 3 });

		HttpRequestPtr request = HttpRequest::Create(
			"http://httpbin.org/put",
			HttpType::Put,
			request_data,
			Closure(this, &NetworkDemo::Complete)
		);

		HttpModule::GetInstance().Send(request);
	}

	void SendDeleteRequest()
	{
		// ���� DELETE ����
		KGE_LOG("Start to send DELETE request...");

		HttpRequestPtr request = HttpRequest::Create(
			"http://httpbin.org/delete",
			HttpType::Delete,
			Closure(this, &NetworkDemo::Complete)
		);

		HttpModule::GetInstance().Send(request);
	}

	void Complete(HttpRequestPtr request, HttpResponsePtr response)
	{
		// �ж������Ƿ�ɹ�
		if (response->IsSucceed())
		{
			try
			{
				// ����ȡ�������ݽ����� JSON ��ʽ
				Json response_data = Json::parse(response->GetData());
				Json result = {
					{"HttpCode", response->GetResponseCode()},
					{"Data", response_data},
				};

				KGE_LOG("Response:\n", result.dump(4));
			}
			catch (std::exception&)
			{
				KGE_ERROR("Parse JSON failed!");
			}
		}
		else
		{
			// ����ʧ��ʱ��ӡ������Ϣ
			OutputError(response);
		}
	}

	void OutputError(HttpResponsePtr response)
	{
		// ��ӡ HTTP ��Ӧ�����״̬��ʹ�����Ϣ
		Json result = {
				{"HttpCode", response->GetResponseCode()},
				{"Error", response->GetError()},
		};

		String result_str = result.dump(4);
		KGE_ERROR("Response: %s\n", result_str.c_str());
	}
};
