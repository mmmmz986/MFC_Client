// MFC_ClientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC_Client.h"
#include "MFC_ClientDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <iomanip>
#include <vector> // MsgType과 vector<uint8_t>를 위해 필요

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 우클릭 후 정보 출력에 사용되는 CAboutDlg 대화 상자
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
// CMFCClientDlg 대화 상자

//추가: 수신 스레드 함수
UINT CMFCClientDlg::RecvThreadFunc(LPVOID pParam)
{
	CMFCClientDlg* pDlg = reinterpret_cast<CMFCClientDlg*>(pParam);
	if (!pDlg) return 1;

	MsgType type;
	uint32_t imgId;
	std::vector<uint8_t> recvBuf;

	std::cout << "[INFO] 수신 스레드 시작됨. 데이터 대기 중...\n";

	while (!pDlg->m_bStopRecvThread)
	{
		if (pDlg->m_net.Receive(type, imgId, recvBuf))
		{
			// 수신 성공: 서버 패킷 처리 로직
			std::cout << "[RECV] 패킷 수신 성공! Type=" << (int)type << ", ID=" << imgId << ", BodySize=" << recvBuf.size() << "\n";
		}
		else
		{
			if (!pDlg->m_bStopRecvThread)
			{
				std::cout << "[ERROR] 수신 실패 또는 연결 종료됨. 스레드 종료.\n";
			}
			break;
		}
	}
	std::cout << "[INFO] 수신 스레드 종료됨.\n";
	return 0;
}


CMFCClientDlg::CMFCClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CMFCClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CMFCClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CMFCClientDlg::OnBnClickedBtnConnect)
END_MESSAGE_MAP()
// CMFCClientDlg(메인화면) 메시지 처리기

BOOL CMFCClientDlg::OnInitDialog()
{
#ifdef _DEBUG
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	std::cout << "[DEBUG] 콘솔창 활성화됨!" << std::endl;
#endif

	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;
}

void CMFCClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCClientDlg::OnBnClickedBtnConnect()
{
	// 추가: 실행 중이면 중복 실행 방지
	if (m_pRecvThread != nullptr) {
		AfxMessageBox(L"이미 서버에 연결되어 있습니다.");
		return;
	}

	// 서버 연결 시도
	if (!m_net.Connect("10.10.21.101", 7000))
	{
		AfxMessageBox(L"서버 연결 실패!");
		return;
	}

	// 추가: 연결 성공 시 스레드 시작
	m_bStopRecvThread = false;
	m_pRecvThread = AfxBeginThread(RecvThreadFunc, this);
	if (m_pRecvThread == nullptr)
	{
		AfxMessageBox(L"수신 스레드 시작 실패!");
		m_net.Disconnect();
		return;
	}

	// 간단한 문자열 전송
	std::string msg = "123";
	std::vector<uint8_t> body(msg.begin(), msg.end());
	m_net.Send(MsgType::IMG_REQ, body);

	// 로그 출력
	std::cout << "[SEND] body(string): " << msg << std::endl;
	std::cout << "[SEND] body(hex): ";
	for (auto b : body)
		std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b << " ";
	std::cout << std::dec << std::endl;

}

BOOL CMFCClientDlg::DestroyWindow()
{
	// 대화 상자가 닫힐 때 수신 스레드 종료 요청
	if (m_pRecvThread != nullptr)
	{
		std::cout << "[INFO] DestroyWindow 호출됨. 수신 스레드 종료 요청 시작.\n";

		m_bStopRecvThread = true; // 스레드 루프 종료 플래그 설정

		// Blocking 중인 recv() 함수를 깨우기 위해 Disconnect() 호출
		m_net.Disconnect();

		// 스레드가 완전히 종료될 때까지 대기
		::WaitForSingleObject(m_pRecvThread->m_hThread, INFINITE);

		// 메모리 해제
		delete m_pRecvThread;
		m_pRecvThread = nullptr;
		std::cout << "[INFO] 수신 스레드 안전하게 종료 및 해제 완료.\n";
	}

	return CDialogEx::DestroyWindow();
}