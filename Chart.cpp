//이동현 CHART 클래스 구현

#include "pch.h"
#include "plport_ver3.h"
#include "Chart.h"
#include "afxdialogex.h"

#include "Myplan.h"
#include "Mytable.h"

//sqlite3 include----------------------------------------------------------------------------//
#include "sqlite3.h"
#include <assert.h>


#pragma comment(lib, "sqlite3.lib")
//--------------------------------------------------------------------------------------------//

using namespace std;


// Chart 대화 상자

IMPLEMENT_DYNAMIC(Chart, CDialogEx)

Chart::Chart(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHART_DIALOG, pParent)
{

}

Chart::~Chart()
{
}

void Chart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THIS_WEEK_MONTHCALENDAR, m_this_week_monthcalendar);
}


BEGIN_MESSAGE_MAP(Chart, CDialogEx)

	ON_BN_CLICKED(IDC_TEST_BTN, &Chart::OnBnClickedTestBtn)
	ON_WM_PAINT()
	ON_NOTIFY(MCN_SELCHANGE, IDC_THIS_WEEK_MONTHCALENDAR, &Chart::OnMcnSelchangeThisWeekMonthcalendar)
	
	
	ON_BN_CLICKED(IDC_CHART1, &Chart::OnBnClickedChart1)
	ON_BN_CLICKED(IDC_CHART2, &Chart::OnBnClickedChart2)
	ON_BN_CLICKED(IDC_CHART_3, &Chart::OnBnClickedChart3)
	
END_MESSAGE_MAP()


// Chart 메시지 처리기



void Chart::OnBnClickedTestBtn()
{
	



}


BOOL Chart::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//달력 설정
	m_this_week_monthcalendar.SetFirstDayOfWeek(0);
	



	this_week = CTime::GetCurrentTime();
	before_1 = this_week - CTimeSpan(7, 0, 0, 0);
	before_2 = this_week - CTimeSpan(14, 0, 0, 0);
	

	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void Chart::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	
	


	//각 주의 week_array 얻기
	CString this_week_plan[7];
	CString before_1_plan[7];
	CString before_2_plan[7];
	Myplan temp_plan;
	temp_plan.get_week_array(this_week_plan, this_week);
	temp_plan.get_week_array(before_1_plan, before_1);
	temp_plan.get_week_array(before_2_plan, before_2);
	
	


	//선택된 주의 plan 데이터 가져오기
	vector<Myplan> this_week_v_plan;
	vector<Myplan> before_1_week_v_plan;
	vector<Myplan> before_2_week_v_plan;


	
	for (int i = 0; i < 7; i++) {
		temp_plan.get_plan(this_week_plan[i], &this_week_v_plan);
	}

	for (int i = 0; i < 7; i++) {
		temp_plan.get_plan(before_1_plan[i], &before_1_week_v_plan);
	}

	for (int i = 0; i < 7; i++) {
		temp_plan.get_plan(before_2_plan[i], &before_2_week_v_plan);
	}


	//현재 분류 정보 가져오기
	Mytable temp_table;
	temp_table.take_category_db();

	

	//각 PICTURE CONTROL dc 얻기
	CWnd* dc_this_week(GetDlgItem(IDC_THIS_WEEK_PIC));
	CWnd* dc_before_1_week(GetDlgItem(IDC_BEFORE_1_PIC));
	CWnd* dc_before_2_week(GetDlgItem(IDC_BEFORE_2_PIC));

	//각 PICTURE CONTROL rect 얻기
	CRect rect_this_week;
	CRect rect_before_1;
	CRect rect_before_2;
	


	//각 PICTURE CONTROL 차트 그리기
	GetDlgItem(IDC_THIS_WEEK_PIC)->GetClientRect(&rect_this_week); // IDC_TODAY_PIC의 크기 받기
	temp_table.make_chart(dc_this_week, rect_this_week, this_week_v_plan);

	GetDlgItem(IDC_BEFORE_1_PIC)->GetClientRect(&rect_before_1); // IDC_TODAY_PIC의 크기 받기
	temp_table.make_chart(dc_before_1_week, rect_before_1, before_1_week_v_plan);

	GetDlgItem(IDC_BEFORE_2_PIC)->GetClientRect(&rect_before_2); // IDC_TODAY_PIC의 크기 받기
	temp_table.make_chart(dc_before_2_week, rect_before_2, before_2_week_v_plan);

	
	//각 에디트 컨트롤에 그래프 기간 출력
	CString s_this_week = this_week_plan[0] + _T(" ~ ") + this_week_plan[6];
	SetDlgItemText(IDC_THIS_WEEK_EDIT, s_this_week);
	CString s_before_1_week = before_1_plan[0] + _T(" ~ ") + before_1_plan[6];
	SetDlgItemText(IDC_BEFORE_1_WEEK_EDIT, s_before_1_week);
	CString s_before_2_week = before_2_plan[0] + _T(" ~ ") + before_2_plan[6];
	SetDlgItemText(IDC_BEFORE_2_WEEK_EDIT, s_before_2_week);


}


void Chart::OnMcnSelchangeThisWeekMonthcalendar(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (chart_1_flag == TRUE) {
		m_this_week_monthcalendar.GetCurSel(before_2);

	}
	else if (chart_2_flag == TRUE) {
		m_this_week_monthcalendar.GetCurSel(before_1);
	}
	else if (chart_3_flag == TRUE) {
		m_this_week_monthcalendar.GetCurSel(this_week);
	}




	Invalidate();

	*pResult = 0;
}





void Chart::OnBnClickedChart1()
{
	chart_1_flag = TRUE;
	chart_2_flag = FALSE;
	chart_3_flag = FALSE;
}


void Chart::OnBnClickedChart2()
{
	chart_1_flag = FALSE;
	chart_2_flag = TRUE;
	chart_3_flag = FALSE;
}


void Chart::OnBnClickedChart3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	chart_1_flag = FALSE;
	chart_2_flag = FALSE;
	chart_3_flag = TRUE;
}


