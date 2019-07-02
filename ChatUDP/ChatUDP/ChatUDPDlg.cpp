
// ChatUDPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatUDP.h"
#include "ChatUDPDlg.h"
#include "afxdialogex.h"
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UPDATEDLG WM_USER+101
unsigned int __stdcall  ServerRcvProc(PVOID pM);
unsigned int __stdcall  ClientSendProc(PVOID pM);

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatUDPDlg dialog




CChatUDPDlg::CChatUDPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatUDPDlg::IDD, pParent)
	, m_strRecv(_T(""))
	, m_strSendData(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_nSerPort = 0;
    m_nClientPort = 0;
	m_strRecvMSG = _T("");

    m_bRatioSer = FALSE;
    m_bRatioClient = FALSE;
	m_bCloseSockSrv = FALSE;
}

void CChatUDPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RECV, m_ERecv);
	DDX_Control(pDX, IDC_EDIT_SEND, m_ESend);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_nSerPort);
	DDV_MinMaxInt(pDX, m_nSerPort, 0, 65535);
	DDX_Control(pDX, IDC_IPADDRESS_CLIENT, m_IPClient);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, m_IPServer);
	DDX_Text(pDX, IDC_EDIT_CILENT_PORT, m_nClientPort);
	DDV_MinMaxInt(pDX, m_nClientPort, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_RECV, m_strRecv);
	DDX_Text(pDX, IDC_EDIT_SEND, m_strSendData);
}

BEGIN_MESSAGE_MAP(CChatUDPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CChatUDPDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_RADIO_SERVER_OPEN, &CChatUDPDlg::OnBnClickedRadioServerOpen)
    ON_BN_CLICKED(IDC_RADIO_CLIENT_OPEN, &CChatUDPDlg::OnBnClickedRadioClientOpen)
	ON_MESSAGE(UPDATEDLG, &CChatUDPDlg::UpdateDlg)
END_MESSAGE_MAP()


// CChatUDPDlg message handlers

BOOL CChatUDPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    //加载套接字
    WORD wVersionReq;
    WSADATA wsadata;
    int err;
    wVersionReq = MAKEWORD(1,1);

    err = WSAStartup(wVersionReq, &wsadata);
    if (err  != NULL)
    {
        return FALSE;
    }
    if (LOBYTE(wsadata.wVersion) != 1 || HIBYTE(wsadata.wVersion) != 1)
    {
        WSACleanup();
        return FALSE;
    }

     //((CButton *)GetDlgItem(IDC_RADIO_SERVER_OPEN))->SetCheck(FALSE);
     //((CButton *)GetDlgItem(IDC_RADIO_CLIENT_OPEN))->SetCheck(TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatUDPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatUDPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatUDPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatUDPDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here

	int len = sizeof(SOCKADDR);

	char recvBuf[1024] = { 0 };
	char sendBuf[1024] = { 0 };
	char tempBuf[1024] = { 0 };
	
	UpdateData(TRUE);

	//cChatUDPDlg->UpdateData(TRUE);//数据从编辑框取出来
	string strSendTemp = (CT2A)m_strSendData;
	//printf("Data: %s , sending... \n", strSendTemp);
	//printf("Data: %s , sending... \n", strSendTemp.c_str());

	sendto(m_tSockClient, strSendTemp.c_str(), strlen(strSendTemp.c_str())+1, 0, (SOCKADDR*)&m_tAddrSrc, len);

	printf("%s", tempBuf);
	//CString strTemp;
	//strTemp.Format(_T("%s"), tempBuf);
	//OutputDebugString(strTemp);
	//OutputDebugString((LPCWSTR)tempBuf);
	//更新到聊天框
	//
	UpdateData(FALSE);

    //CDialogEx::OnOK();
}

CChatUDPDlg::~CChatUDPDlg()
{
    WSACleanup();//关闭socket服务
}


void CChatUDPDlg::OnBnClickedRadioServerOpen()
{
    // TODO: Add your control notification handler code here
    m_bRatioSer = !m_bRatioSer;
    ((CButton *)GetDlgItem(IDC_RADIO_SERVER_OPEN))->SetCheck(m_bRatioSer);//选上

    if (m_bRatioSer) //开启 //TODO：
    {
        CreateServerProc();
    }else//关闭
    {
        CloseServerProc();
    }

    //UpdateData(FALSE);
}


void CChatUDPDlg::OnBnClickedRadioClientOpen()
{
    // TODO: Add your control notification handler code here
    m_bRatioClient = !m_bRatioClient;
    
    ((CButton *)GetDlgItem(IDC_RADIO_CLIENT_OPEN))->SetCheck(m_bRatioClient);//选上

     if (m_bRatioClient) //开启
     {
         CreateClientProc();
     }else//关闭
     {
         CloseClientProc();
     }

     //UpdateData(FALSE);
}

void CChatUDPDlg::CreateServerProc()
{
	m_tSockSrv = socket(AF_INET, SOCK_DGRAM, 0);
	
	UpdateData(TRUE);
	//port
	CString str;
	GetDlgItem(IDC_EDIT_SERVER_PORT)->GetWindowText(str);
	//MessageBox(str);
	int iPortNum = _ttoi(str);
	//ip
	unsigned long ip = 0;
	m_IPServer.GetAddress(ip);

	SOCKADDR_IN AddrSrc;
	AddrSrc.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	AddrSrc.sin_family = AF_INET;
	AddrSrc.sin_port = htons(iPortNum); //外部获取

	//绑定套接字
	int iRet = bind(m_tSockSrv, (SOCKADDR*)&AddrSrc, sizeof(SOCKADDR));

	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ServerRcvProc, (LPVOID)this, 0, NULL);
	DWORD dwThreadId = 0;

	return;

}

void CChatUDPDlg::CloseServerProc()
{
	m_bCloseSockSrv = TRUE;
	closesocket(m_tSockSrv);
	return;
}

void CChatUDPDlg::CreateClientProc()
{
	m_tSockClient = socket(AF_INET, SOCK_DGRAM, 0);

	UpdateData(TRUE);
	CString str;
	GetDlgItem(IDC_EDIT_CILENT_PORT)->GetWindowText(str);
	//MessageBox(str);
	//ip
	int iPortNum = _ttoi(str);
	//port
	unsigned long dIP = 0;
	m_IPClient.GetAddress(dIP);

	m_tAddrSrc.sin_addr.S_un.S_addr = htonl(dIP);
	m_tAddrSrc.sin_family = AF_INET;
	m_tAddrSrc.sin_port = htons(iPortNum); //外部获取

	return;
}

void CChatUDPDlg::CloseClientProc()
{
	closesocket(m_tSockClient);
	return;
}


//DWORD WINAPI  ServerRcvProc(LPVOID pM)
unsigned int __stdcall  ServerRcvProc(PVOID pM)
{
	printf("开始接收线程! \n");
	CChatUDPDlg *cChatUDPDlg = (CChatUDPDlg*)pM;
	//TODO:
	if (!cChatUDPDlg)
	{
		printf("cChatUDPDlg is null \n");
		return FALSE;
	}

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while (!cChatUDPDlg->m_bCloseSockSrv)
	{
		char recvBuf[1024] = { 0 };
		char sendBuf[1024] = { 0 };
		char tempBuf[1024] = { 0 };

		recvfrom(cChatUDPDlg->m_tSockSrv, recvBuf,100, 0, (SOCKADDR*)&addrClient, &len);

		sprintf_s(tempBuf, "%s say : %s\n", inet_ntoa(addrClient.sin_addr), recvBuf);

		//printf("%s", tempBuf);
		/*CString strTemp;
		strTemp.Format(_T("%s say : %s\n"), inet_ntoa(addrClient.sin_addr), recvBuf);*/
		//OutputDebugString((LPCWSTR)tempBuf);
		//更新到聊天框
		//UpdateData(TRUE);
		//cChatUDPDlg->UpdateData(TRUE);
		cChatUDPDlg->m_strRecvThread = tempBuf;

		::PostMessage(cChatUDPDlg->GetSafeHwnd(), UPDATEDLG, 0, 0);

		//cChatUDPDlg->UpdateData(FALSE);
	}
	printf("关闭接收线程! \n");
	return TRUE;
}

unsigned int __stdcall  ClientSendProc(PVOID pM)
{
	return TRUE;
}

afx_msg LRESULT CChatUDPDlg::UpdateDlg(WPARAM wParam, LPARAM lParam)
{
	UpdateData(TRUE);
	/*
	m_ERecv.SetSel(m_strRecv.GetLength());
	m_ERecv.ReplaceSel(m_strRecvThread);
	*/
	if(!m_strRecv.GetLength())
	{
		m_strRecv = m_strRecv + m_strRecvThread;
	}
	else
	{
		m_strRecv = m_strRecv + "\r\n" + m_strRecvThread;
	}
	//m_ERecv.SetSel(m_strRecv.GetLength(), m_strRecv.GetLength(),1);
	//m_ERecv.SetSel(0, -1);
	//m_ERecv.SetFocus();//设置光标位置
	/*int iLinecount = m_ERecv.GetLineCount();
	m_ERecv.LineScroll(iLinecount);*/
	/*
	m_strRecvMSG += m_strRecvThread;
	m_ERecv.SetWindowText(m_strRecvMSG);
	*/
	m_ERecv.SetScrollPos(SB_VERT, m_ERecv.GetLineCount(), TRUE);

	UpdateData(FALSE);
	return 0;
}