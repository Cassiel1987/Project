
// ChatServerDlg.h : ͷ�ļ�
//

#pragma once


// CChatServerDlg �Ի���
class CChatServerDlg : public CDialogEx
{
// ����
public:
	CChatServerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	void DisplayLog(CString &strInfo) const;
	void UpdateUserInfo(CString strUserInfo) const;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
