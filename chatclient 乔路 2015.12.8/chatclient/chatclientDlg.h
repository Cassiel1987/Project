
// chatclientDlg.h : 头文件
//

#pragma once


// CchatclientDlg 对话框
class CchatclientDlg : public CDialogEx
{
// 构造
public:
	CchatclientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATCLIENT_DIALOG };
#endif
	void UpdateUserInfo(CString strInfo);
	void UpdateText(CString& strText);  //跟新聊天记录
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	BOOL WChar2MByte(LPCTSTR lpSrc, LPSTR lpDest, int nlen);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	CString m_strSend;
	CString m_caption;//对话框标题设置
	
//	virtual void AssertValid() const;
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedCancel();
};
