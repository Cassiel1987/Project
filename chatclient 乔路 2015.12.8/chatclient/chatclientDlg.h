
// chatclientDlg.h : ͷ�ļ�
//

#pragma once


// CchatclientDlg �Ի���
class CchatclientDlg : public CDialogEx
{
// ����
public:
	CchatclientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATCLIENT_DIALOG };
#endif
	void UpdateUserInfo(CString strInfo);
	void UpdateText(CString& strText);  //���������¼
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	BOOL WChar2MByte(LPCTSTR lpSrc, LPSTR lpDest, int nlen);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	CString m_strSend;
	CString m_caption;//�Ի����������
	
//	virtual void AssertValid() const;
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedCancel();
};
