#include "stdafx.h"
#include "LogoInDlg.h"
#include "chatclient.h"
#include "Clientsocket.h"
#include "afxdialogex.h"

//IMPLEMENT_DYNAMIC(CLogoInDlg, CDialogEx)

CLogoInDlg::CLogoInDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogoInDlg::IDD, pParent), m_strUser(_T(""))
{
	//��ʼ��IP��ַ
	//DWORD ����unSigned long
	m_dwIP = ntohl(inet_addr("192.168.1.254"));
}

CLogoInDlg::~CLogoInDlg()
{
}

void CLogoInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IP_ADDR, m_dwIP);
	DDX_Text(pDX, IDC_ET_NAME, m_strUser);
}


BEGIN_MESSAGE_MAP(CLogoInDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGOIN, &CLogoInDlg::OnBnClickedBtnLogoin)
	ON_BN_CLICKED(IDOK, &CLogoInDlg::OnBnClickedOk)
END_MESSAGE_MAP()



void CLogoInDlg::OnBnClickedBtnLogoin()
{
	//��¼	
	UpdateData();
	if (m_strUser.IsEmpty())
	{
		AfxMessageBox(_T("�û�������Ϊ�գ�"));
		return;
	}

	if (m_dwIP == 0)
	{
		AfxMessageBox(_T("��ЧIP��ַ"));
		return;
	}

	Clientsocket* pSock = theApp.GetMainSocket();
	IN_ADDR addr;
	addr.S_un.S_addr = htonl(m_dwIP);
	//inet_ntoa����һ��char *,�����char *�Ŀռ�����inet_ntoa���澲̬����
	CString strIP(inet_ntoa(addr));
	//��ʼֻ�Ǵ����ˣ���û�����ӣ���������socket�����8989�˿�Ҫ�ͷ���˼����Ķ˿�һֱ��������������ġ�
	if (!pSock->Connect(strIP.GetBuffer(0), 8989))
	{
		AfxMessageBox(_T("���ӷ�����ʧ�ܣ�"));
		return;
	}

	CString Cm_strUser = m_strUser;
	char from_user[20];
	memset(from_user, 0, sizeof(from_user));
	WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)m_strUser, -1, from_user, 260, 0, false);
	//����
	pSock->m_strUserName = m_strUser;  //���û����ִ��ݹ�ȥ
	char* pBuff = new char[m_strUser.GetLength() + 1];
	memset(pBuff, 0, m_strUser.GetLength());  //����һ�����洢�û������ڴ�ռ�
	if (WChar2MByte(m_strUser.GetBuffer(0), pBuff, m_strUser.GetLength() + 1))
		pSock->LogoIn(pBuff, m_strUser.GetLength() + 1, from_user);  //ͷ���ռ䣬��ͷ������
	delete pBuff;
	CDialogEx::OnCancel();
}


void CLogoInDlg::OnBnClickedOk()
{
	//�˳�
	Clientsocket* pSock = theApp.GetMainSocket();
	pSock->Close();
	CDialogEx::OnOK();
}


BOOL CLogoInDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(0);
	((CEdit*)GetDlgItem(IDC_ET_PSW))->SetWindowTextW(_T("123123"));
	return TRUE;
}

BOOL CLogoInDlg::WChar2MByte(LPCWSTR lpSrc, LPSTR lpDest, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, 0, 0, FALSE);
	if (n<nlen)
		return FALSE;
	WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, nlen, 0, FALSE);
	return TRUE;
}