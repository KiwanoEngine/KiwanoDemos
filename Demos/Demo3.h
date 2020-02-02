// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class Demo3
	: public Stage
{
	SoundPtr bgmusic;		// ���ֶ���
	TextActorPtr volume_text;	// ��������
	TextActorPtr state_text;		// ����״̬����

public:
	static StagePtr Create()
	{
		return new Demo3;
	}

	Demo3()
	{
		// ��������
		bgmusic = new Sound;
		if (!bgmusic->Load(L"res/splash.mp3"))
		{
			bgmusic = nullptr;

			TextActorPtr err = new TextActor(L"��Ƶ�ļ�����ʧ��");
			err->SetAnchor(0.5f, 0.5f);
			err->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
			this->AddChild(err);
			return;
		}

		// �������֣������������ò���ѭ��������-1 ��ʾѭ�����ţ�
		bgmusic->Play(-1);

		// ����˵������
		TextActorPtr intro_text = new TextActor(L"�����¼���������\n���ո����ͣ�����");
		intro_text->SetAlignment(TextAlign::Center);
		intro_text->SetAnchor(0.5f, 0.5f);
		intro_text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50);

		// ������������
		volume_text = new TextActor(L"��ǰ������");
		volume_text->SetAnchor(0.5f, 0.5f);
		volume_text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 30);

		// ����״̬����
		state_text = new TextActor(L"��ǰ״̬��");
		state_text->SetAnchor(0.5f, 0.5f);
		state_text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 60);

		// ��ӵ���̨
		this->AddChild(intro_text);
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
		volume_text->SetText(L"��ǰ������" + String::parse(volume));
		state_text->SetText(playing ? L"��ǰ״̬�����ڲ���" : L"��ǰ״̬��ֹͣ����");

		// ��ȡ�����豸
		auto& input = Input::Instance();

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
