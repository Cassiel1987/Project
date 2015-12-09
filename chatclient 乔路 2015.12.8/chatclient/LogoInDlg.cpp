#include "stdafx.h"
#include "LogoInDlg.h"
#include "chatclient.h"
#include "Clientsocket.h"
#include "afxdialogex.h"

//IMPLEMENT_DYNAMIC(CLogoInDlg, CDialogEx)

CLogoInDlg::CLogoInDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogoInDlg::IDD, pParent), m_strUser(_T(""))
{
	//初始化IP地址
	//DWORD 就是unSigned long
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
	//登录	
	UpdateData();
	if (m_strUser.IsEmpty())
	{
		AfxMessageBox(_T("用户名不能为空！"));
		return;
	}

	if (m_dwIP == 0)
	{
		AfxMessageBox(_T("无效IP地址"));
		return;
	}

	Clientsocket* pSock = theApp.GetMainSocket();
	IN_ADDR addr;
	addr.S_un.S_addr = htonl(m_dwIP);
	//inet_ntoa返回一个char *,而这个char *的空间是在inet_ntoa里面静态分配
	CString strIP(inet_ntoa(addr));
	//开始只是创建了，并没有连接，这里连接socket，这个8989端口要和服务端监听的端口一直，否则监听不到的。
	if (!pSock->Connect(strIP.GetBuffer(0), 8989))
	{
		AfxMessageBox(_T("连接服务器失败！"));
		return;
	}

	CString Cm_strUser = m_strUser;
	char from_user[20];
	memset(from_user, 0, sizeof(from_user));
	WideCharToMultiByte(CP_OEMCP, 0, (LPCTSTR)m_strUser, -1, from_user, 260, 0, false);
	//发送
	pSock->m_strUserName = m_strUser;  //将用户名字传递过去
	char* pBuff = new char[m_strUser.GetLength() + 1];
	memset(pBuff, 0, m_strUser.GetLength());  //开辟一个，存储用户名的内存空间
	if (WChar2MByte(m_strUser.GetBuffer(0), pBuff, m_strUser.GetLength() + 1))
		pSock->LogoIn(pBuff, m_strUser.GetLength() + 1, from_user);  //头部空间，和头部长度
	delete pBuff;
	CDialogEx::OnCancel();
}


void CLogoInDlg::OnBnClickedOk()
{
	//退出
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