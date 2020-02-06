// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"
#include <iostream>

class Demo5
	: public Stage
{
public:
	static StagePtr Create()
	{
		return new Demo5;
	}

	Demo5()
	{
		// ��Ӱ�������
		AddListener<KeyDownEvent>(Closure(this, &Demo5::OnKeyDown));

		// ����˵������
		TextActorPtr intro = new TextActor(L"��G����GET����\n��P����POST����\n��U����PUT����\n��D����DELETE����");
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
		Logger::Instance().ShowConsole(true);
	}

	void OnExit() override
	{
		// �˳���̨ʱ�رտ���̨
		Logger::Instance().ShowConsole(false);
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
		KGE_LOG(L"Start to send GET request...");

		HttpRequestPtr request = new HttpRequest;
		// �������� URL
		request->SetUrl(L"http://httpbin.org/get");
		// ������������Ϊ GET
		request->SetType(HttpRequest::Type::Get);
		// ����������ɺ�Ļص�����
		request->SetResponseCallback(Closure(this, &Demo5::Complete));

		// ���� HTTP ����
		HttpClient::Instance().Send(request);
	}

	void SendPostRequest()
	{
		// ���� POST ����
		KGE_LOG(L"Start to send POST request...");

		// ���� JSON ��ʽ�� POST ����
		Json request_data = {
			{ L"string",	L"test����" },
			{ L"boolean",	true },
			{ L"integer",	12 },
			{ L"float",		3.125 },
			{ L"array",		{ 1, 2, 3, 4, 4.5 } },
			{ L"object",	{ L"key", L"value" } },
		};

		HttpRequestPtr request = new HttpRequest;
		request->SetUrl(L"http://httpbin.org/post");
		request->SetType(HttpRequest::Type::Post);
		// ���� POST ���������
		request->SetJsonData(request_data);
		request->SetResponseCallback(Closure(this, &Demo5::Complete));

		HttpClient::Instance().Send(request);
	}

	void SendPutRequest()
	{
		// ���� PUT ����
		KGE_LOG(L"Start to send PUT request...");

		// ���� JSON ��ʽ�� PUT ����
		Json request_data = Json::array({ 1, 2, 3 });

		HttpRequestPtr request = new HttpRequest;
		request->SetUrl(L"http://httpbin.org/put");
		request->SetType(HttpRequest::Type::Put);
		// ���� PUT ���������
		request->SetJsonData(request_data);
		request->SetResponseCallback(Closure(this, &Demo5::Complete));

		HttpClient::Instance().Send(request);
	}

	void SendDeleteRequest()
	{
		// ���� DELETE ����
		KGE_LOG(L"Start to send DELETE request...");

		HttpRequestPtr request = new HttpRequest;
		request->SetUrl(L"http://httpbin.org/delete");
		request->SetType(HttpRequest::Type::Delete);
		request->SetResponseCallback(Closure(this, &Demo5::Complete));

		HttpClient::Instance().Send(request);
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
					{L"HttpCode", response->GetResponseCode()},
					{L"Data", response_data},
				};

				KGE_LOG(L"Response:\n", result.dump(4));
			}
			catch (std::exception&)
			{
				KGE_ERROR(L"Parse JSON failed!");
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
				{L"HttpCode", response->GetResponseCode()},
				{L"Error", response->GetError()},
		};

		String result_str = result.dump(4);
		KGE_ERROR(L"Response: %s\n", result_str.c_str());
	}
};
