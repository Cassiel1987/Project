
// chatclient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "LogoInDlg.h"
#include "Clientsocket.h"
// CchatclientApp: 
// �йش����ʵ�֣������ chatclient.cpp
//

class CchatclientApp : public CWinApp
{
public:
	CchatclientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	Clientsocket* GetMainSocket() const;
private:
	Clientsocket* m_pSocket;


public:
	virtual int ExitInstance();
};

extern CchatclientApp theApp;