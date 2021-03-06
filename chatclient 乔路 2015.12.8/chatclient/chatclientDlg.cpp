
// chatclientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "chatclient.h"
#include "chatclientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CchatclientDlg 对话框

CchatclientDlg::CchatclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHATCLIENT_DIALOG, pParent)
	, m_strSend(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CchatclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CchatclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEND, &CchatclientDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDCANCEL, &CchatclientDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CchatclientDlg 消息处理程序

BOOL CchatclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowText(m_caption);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CchatclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CchatclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CchatclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CchatclientDlg::OnBnClickedBtnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	//发送消息
	UpdateData();
	if (m_strSend.IsEmpty())
	{
		AfxMessageBox(_T("发送内容不能为空！"));
		return;
	}

	//获取选中内容
	CListBox* pList = (CListBox*)GetDlgItem(IDC_LB_ONLINE);
	CString tep(_T(""));
	INT nIndex = 0;
	nIndex = pList->GetCurSel();
	if (LB_ERR == nIndex)
	{
		AfxMessageBox(_T("请选择聊天对象！"));
		return;
	}
	pList->GetText(nIndex, tep);
	char* to_user = new char[tep.GetLength() * 2 + 1];
	memset(to_user, 0, tep.GetLength() * 2 + 1);
	WChar2MByte(tep.GetBuffer(0), to_user, tep.GetLength() * 2 + 1);


	CString m_strUserName = theApp.GetMainSocket()->m_strUserName;
	char from_user[20];
	memset(from_user, 0, sizeof(from_user));

	WChar2MByte(m_strUserName.GetBuffer(0), from_user, m_strUserName.GetLength() * 2);


	CString temp;
	CTime time = CTime::GetCurrentTime();
	temp = time.Format("%H:%M:%S");
	//姓名 +_T("\n\t") 时间

	m_strSend = theApp.GetMainSocket()->m_strUserName + _T(" 发送给  ") + to_user + _T("  ") + temp + _T("\r\n   ") + m_strSend + _T("\r\n");
	char* pBuff = new char[m_strSend.GetLength() * 2];
	memset(pBuff, 0, m_strSend.GetLength() * 2);

	//转换为多字节
	WChar2MByte(m_strSend.GetBuffer(0), pBuff, m_strSend.GetLength() * 2);
	theApp.GetMainSocket()->SendMSG(pBuff, m_strSend.GetLength() * 2, to_user, from_user);

	delete pBuff;

	m_strSend.Empty();
	UpdateData(0);

}


void CchatclientDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
	//退出
}

BOOL CchatclientDlg::WChar2MByte(LPCWSTR lpSrc, LPSTR lpDest, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, 0, 0, FALSE);  //返回缓冲区大小
	if (nlen<n)
		return FALSE;
	WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, nlen, 0, FALSE);   //转换
	return TRUE;
}

void CchatclientDlg::UpdateUserInfo(CString strInfo)  //显示所有用户
{
	CString strTmp;
	CListBox* pBox = (CListBox*)GetDlgItem(IDC_LB_ONLINE);
	pBox->ResetContent();

	CString m_strUserName = theApp.GetMainSocket()->m_strUserName;
	strInfo = _T("群聊#") + strInfo;

	while (!strInfo.IsEmpty())
	{
		int n = strInfo.Find('#');
		if (n == -1)
			break;
		strTmp = strInfo.Left(n);
		if (strTmp != m_strUserName)
		{
			pBox->AddString(strTmp);
		}
		strInfo = strInfo.Right(strInfo.GetLength() - n - 1);
	}
}

void CchatclientDlg::UpdateText(CString &strText)
{
	((CEdit*)GetDlgItem(IDC_ET_TEXT))->ReplaceSel(strText);
}
