
// ChatUDPDlg.h : header file
//

#pragma once


// CChatUDPDlg dialog
class CChatUDPDlg : public CDialogEx
{
// Construction
public:
	CChatUDPDlg(CWnd* pParent = NULL);	// standard constructor
    ~CChatUDPDlg();
// Dialog Data
	enum { IDD = IDD_CHATUDP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg LRESULT UpdateDlg(WPARAM wParam, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedRadioServerOpen();
    CEdit m_ERecv;
    CEdit m_ESend;
    int m_nSerPort;
    CIPAddressCtrl m_IPClient;
    CIPAddressCtrl m_IPServer;
    int m_nClientPort;

    BOOL m_bRatioSer;
    BOOL m_bRatioClient;
    afx_msg void OnBnClickedRadioClientOpen();

    void CreateServerProc();
    void CloseServerProc();
    void CreateClientProc();
    void CloseClientProc();

    SOCKET	m_tSockSrv;
	BOOL	m_bCloseSockSrv;
    SOCKET	m_tSockClient;
	CString m_strRecv;
	CString m_strRecvThread;
	CString m_strRecvMSG;

	CString m_strSendData;
	SOCKADDR_IN m_tAddrSrc;
};
