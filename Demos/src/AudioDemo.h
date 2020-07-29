// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class AudioDemo
	: public Stage
{
	SoundPtr bgmusic;		// ���ֶ���
	TextActorPtr volume_text;	// ��������
	TextActorPtr state_text;		// ����״̬����

public:
	static StagePtr Create()
	{
		return new AudioDemo;
	}

	static String DemoName()
	{
		return "Audio Demo";
	}

	AudioDemo()
	{
		// ��������
		bgmusic = new Sound("res/sounds/splash.mp3");

		// �������֣������������ò���ѭ��������-1 ��ʾѭ�����ţ�
		bgmusic->Play(-1);

		// ����˵������
		TextActorPtr intro = new TextActor("�����¼���������\n���ո����ͣ�����");
		intro->SetFillColor(Color::White);
		intro->SetAlignment(TextAlign::Center);
		intro->SetAnchor(0.5f, 0.5f);
		intro->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50);

		// ������������
		volume_text = new TextActor("��ǰ������");
		volume_text->SetFillColor(Color::White);
		volume_text->SetAnchor(0.5f, 0.5f);
		volume_text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 30);

		// ����״̬����
		state_text = new TextActor("��ǰ״̬��");
		state_text->SetFillColor(Color::White);
		state_text->SetAnchor(0.5f, 0.5f);
		state_text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 60);

		// ��ӵ���̨
		this->AddChild(intro);
		this->AddChild(volume_text);
		this->AddChild(state_text);
	}

	void OnUpdate(Duration dt) override
	{
		if (bgmusic == nullptr)
			return;

		// ��ȡ�����Ͳ���״̬
		float volume = bgmusic->GetVolume();
		bool playing = bgmusic->IsPlaying();

		// �޸��ı�
		volume_text->SetText("��ǰ������" + std::to_string(volume));
		state_text->SetText(playing ? "��ǰ״̬�����ڲ���" : "��ǰ״̬��ֹͣ����");

		// ��ȡ�����豸
		auto& input = Input::GetInstance();

		// ���ո����ͣ�����
		if (input.WasPressed(KeyCode::Space))
		{
			bgmusic->IsPlaying() ? bgmusic->Pause() : bgmusic->Resume();
		}

		// �����¼���������
		if (input.WasPressed(KeyCode::Up))
		{
			bgmusic->SetVolume(volume + 0.1f);
		}
		else if (input.WasPressed(KeyCode::Down))
		{
			bgmusic->SetVolume(volume - 0.1f);
		}
	}
};
