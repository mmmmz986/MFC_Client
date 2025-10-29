
// MFC_ClientDlg.h: 헤더 파일
//

#pragma once
#include "Packet.h"

// CMFCClientDlg 대화 상자
class CMFCClientDlg : public CDialogEx
{
	// 생성입니다.
public:
	CMFCClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	// 대화 상자 초기 화면입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_CLIENT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


	// 구현입니다.
private:
	Packet m_net;
	CWinThread* m_pRecvThread = nullptr; // 수신 스레드 포인터
	bool m_bStopRecvThread = false;      // 스레드 종료 플래그
	static UINT RecvThreadFunc(LPVOID pParam);
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnConnect();
	virtual BOOL DestroyWindow();
};
