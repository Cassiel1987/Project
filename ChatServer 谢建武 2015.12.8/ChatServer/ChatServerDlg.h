
// ChatServerDlg.h : 头文件
//

#pragma once


// CChatServerDlg 对话框
class CChatServerDlg : public CDialogEx
{
// 构造
public:
	CChatServerDlg(CWnd* pParent = NULL);	// 标准构造函数
	void DisplayLog(CString &strInfo) const;
	void UpdateUserInfo(CString strUserInfo) const;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
