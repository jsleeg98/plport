
// plport_ver3Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "plport_ver3.h"
#include "plport_ver3Dlg.h"
#include "afxdialogex.h"
#include "Mytable.h"
#include "Setup.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cplportver3Dlg 대화 상자



Cplportver3Dlg::Cplportver3Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLPORT_VER3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//--------------------------------------------------------------------------------------//
	//CWnd 변수 초기화
	m_pwndshow = NULL;

	//--------------------------------------------------------------------------------------//
}

void Cplportver3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(Cplportver3Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &Cplportver3Dlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// Cplportver3Dlg 메시지 처리기

BOOL Cplportver3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//창 위치, 크기 설정
	MoveWindow(100, 100, 1600, 870);


	//---------------------------------------------------------------------------------------------------------------------------//
	// CString 클래스 활용 탭 이름 설정
	CString tab1 = _T("주간계획");
	CString tab2 = _T("주간통계");
	CString tab3 = _T("분류설정");

	// Tap control 변수에 탭 이름 삽입
	m_tab.InsertItem(0, tab1);
	m_tab.InsertItem(1, tab2);
	m_tab.InsertItem(2, tab3);

	// Tap control 클라이언트 사각형 크기 가져오기
	CRect rect;
	m_tab.GetClientRect(&rect);

	//주간통계 대화상자 추가하기
	m_week.Create(IDD_WEEK_DIALOG, &m_tab);
	m_week.SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);

	
	

	//차트 대화상자 추가하기
	m_chart.Create(IDD_CHART_DIALOG, &m_tab);
	m_chart.SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_NOZORDER);

	//설정 대화상자 추가하기
	m_setup.Create(IDD_SETUP_DIALOG, &m_tab);
	m_setup.SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_NOZORDER);

	
	//초기 데이터 확인
	bool check;
	Mytable temp_table;
	check = temp_table.check_category_db();

	//category 테이블에 값이 없을 경우 분류를 추가해주기 위해 분류설정 화면을 설정 화면으로 한다.
	if (check == FALSE) {
		m_week.ShowWindow(SW_HIDE);
		m_setup.ShowWindow(SW_SHOW);
		m_tab.SetCurSel(2);
		MessageBox(_T("플포트의 시작을 환영합니다.\n분류 추가를 통해 플포트의 모든 기능을 사용할 수 있습니다."));
	}
	//category 테이블에 값이 있을 경우 주간계획표가 나오도록 한다.
	else {
		m_week.ShowWindow(SW_SHOW);
		m_setup.ShowWindow(SW_HIDE);
		m_tab.SetCurSel(0);
	}
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Cplportver3Dlg::OnPaint()
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
//  이 함수를 호출합니다.
HCURSOR Cplportver3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cplportver3Dlg::OnTcnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//---------------------------------------------------------------------------------------------------//
	//m_pwndshow 값이 없는 경우
	if (m_pwndshow != NULL) {
		m_pwndshow->ShowWindow(SW_HIDE);
		m_pwndshow = NULL;
	}

	//현재 탭의 위치 변수 설정
	int nIndex = m_tab.GetCurSel();

	//각 경우 스위치문
	switch (nIndex) {
	case 0:
		
		m_week.ShowWindow(SW_SHOW);
		m_setup.ShowWindow(SW_HIDE);
		m_chart.ShowWindow(SW_HIDE);
		m_pwndshow = &m_week;
		
		break;
	
	case 1:
		m_week.ShowWindow(SW_HIDE);
		m_chart.ShowWindow(SW_SHOW);
		m_setup.ShowWindow(SW_HIDE);
		m_pwndshow = &m_chart;
		break;

	case 2:
	m_chart.ShowWindow(SW_HIDE);
	m_setup.ShowWindow(SW_SHOW);
	m_week.ShowWindow(SW_HIDE);
	m_pwndshow = &m_setup;
	break;
	}


	//---------------------------------------------------------------------------------------------------//


	*pResult = 0;
}
