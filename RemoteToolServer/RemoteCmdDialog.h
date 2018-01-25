#pragma once


// RemoteCmdDialog �Ի���

class RemoteCmdDialog : public CDialogEx
{
	DECLARE_DYNAMIC(RemoteCmdDialog)

	public:
	RemoteCmdDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~RemoteCmdDialog();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CMD };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	public:
	CEdit m_editInput;
	CEdit m_editOutput;
	CString m_strOutput;
	TcpClient* m_socketClient;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void PostMessageToClient();

	public:
	afx_msg void OnClickedButtonConfirm();
	afx_msg LRESULT OnUpdateOutput(WPARAM wParam, LPARAM lParam);
};
