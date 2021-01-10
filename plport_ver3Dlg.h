
// plport_ver3Dlg.h: 헤더 파일
//

#pragma once
//-----------------------------------------------------------------------------------//
//대화상자 헤더파일 include
#include "Week.h"
#include "Chart.h"
#include "Setup.h"
//-----------------------------------------------------------------------------------//


// Cplportver3Dlg 대화 상자
class Cplportver3Dlg : public CDialogEx
{
// 생성입니다.
public:
	Cplportver3Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	//---------------------------------------------------------------------------------------//
	//각 대화상자 멤버 변수 선언 
	Week m_week;
	Chart m_chart;
	Setup m_setup;
	CWnd* m_pwndshow;


	//---------------------------------------------------------------------------------------//
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLPORT_VER3_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
};
