#pragma once


// RemoteDialog �Ի���

class RemoteDialog : public CDialogEx
{
	DECLARE_DYNAMIC(RemoteDialog)

public:
	RemoteDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~RemoteDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DESKTOP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	public:
	afx_msg void OnAcceleratorSaveBMP();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	HACCEL m_hAccel;
	CRITICAL_SECTION m_cs;
	bool m_isSaveBMP = false;
};
