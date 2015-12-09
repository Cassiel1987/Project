
// chatclient.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "chatclient.h"
#include "chatclientDlg.h"
#include "Clientsocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CchatclientApp

BEGIN_MESSAGE_MAP(CchatclientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CchatclientApp ����

CchatclientApp::CchatclientApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CchatclientApp ����

CchatclientApp theApp;


// CchatclientApp ��ʼ��

BOOL CchatclientApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Chatclient"));

	m_pSocket = new Clientsocket();
	if (!m_pSocket)
	{
		AfxMessageBox(_T("�ڴ治�㣡"));
		return false;
	}

	if (!m_pSocket->Create())
	{
		AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
		return false;
	}

	CLogoInDlg* pLogoinDlg;
	pLogoinDlg = new CLogoInDlg();
	CString m_strUser;
	if (pLogoinDlg->DoModal() == IDOK)
	{
		//����¼
		delete pLogoinDlg;
		m_pSocket->Close();
		return false;
	}
	else
	{
		m_strUser = pLogoinDlg->m_strUser;
		delete pLogoinDlg;
	}
	CchatclientDlg dlg;
	dlg.m_caption = m_strUser + _T(" ������Ի���");
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CchatclientApp::ExitInstance()
{
	if (m_pSocket)
	{
		delete m_pSocket;
		m_pSocket = NULL;
	}

	return CWinApp::ExitInstance();
}

Clientsocket* CchatclientApp::GetMainSocket() const
{
	return m_pSocket;
}