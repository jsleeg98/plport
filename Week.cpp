// Week.cpp: 구현 파일
//

#include "pch.h"
#include "plport_ver3.h"
#include "Week.h"
#include "afxdialogex.h"
//sqlite3 include----------------------------------------------------------------------------//
#include "sqlite3.h"
#include <assert.h>


#pragma comment(lib, "sqlite3.lib")
//--------------------------------------------------------------------------------------------//

#include "libxl.h"

using namespace std;
using namespace libxl;

// Week 대화 상자

IMPLEMENT_DYNAMIC(Week, CDialogEx)

Week::Week(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WEEK_DIALOG, pParent)
{

}

Week::~Week()
{
}

void Week::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_plan_list);
	DDX_Control(pDX, IDC_S_TIME_EDIT, m_s_time);
	DDX_Control(pDX, IDC_E_TIME_EDIT, m_e_time);
	DDX_Control(pDX, IDC_CONTENT_EDIT, m_content);
	DDX_Control(pDX, IDC_CATEGORY_COMBO, m_category);
	DDX_Control(pDX, IDC_MONTHCALENDAR, m_monthcalendar);
}


BEGIN_MESSAGE_MAP(Week, CDialogEx)
	
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ADD_BTN, &Week::OnBnClickedAddBtn)
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR, &Week::OnMcnSelchangeMonthcalendar)
	ON_BN_CLICKED(IDC_DEL_BTN, &Week::OnBnClickedDelBtn)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &Week::OnNMClickList)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_TEST_BTN, &Week::OnBnClickedTestBtn)
END_MESSAGE_MAP()


// Week 메시지 처리기


int AnsiToUTF8(char* szSrc, char* strDest, int destSize)
{
	WCHAR 	szUnicode[255];
	char 	szUTF8code[255];

	int nUnicodeSize = MultiByteToWideChar(CP_ACP, 0, szSrc, (int)strlen(szSrc), szUnicode, sizeof(szUnicode));
	int nUTF8codeSize = WideCharToMultiByte(CP_UTF8, 0, szUnicode, nUnicodeSize, szUTF8code, sizeof(szUTF8code), NULL, NULL);
	assert(destSize > nUTF8codeSize);
	memcpy(strDest, szUTF8code, nUTF8codeSize);
	strDest[nUTF8codeSize] = 0;
	return nUTF8codeSize;
}

int UTF8ToAnsi(char* szSrc, char* strDest, int destSize)
{
	WCHAR 	szUnicode[255];
	char 	szAnsi[255];

	int nSize = MultiByteToWideChar(CP_UTF8, 0, szSrc, -1, 0, 0);
	int nUnicodeSize = MultiByteToWideChar(CP_UTF8, 0, szSrc, -1, szUnicode, nSize);
	int nAnsiSize = WideCharToMultiByte(CP_ACP, 0, szUnicode, nUnicodeSize, szAnsi, sizeof(szAnsi), NULL, NULL);
	assert(destSize > nAnsiSize);
	memcpy(strDest, szAnsi, nAnsiSize);
	strDest[nAnsiSize] = 0;
	return nAnsiSize;
}


BOOL Week::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

   

	//static text 폰트 설정
	m_static_font.CreateFont(15, 0, 0, 0,FW_HEAVY , 0, 0, 0, 0, 0, 0, 0, 0, _T("굴림"));
	GetDlgItem(IDC_S_TIME_STATIC)->SetFont(&m_static_font);
	GetDlgItem(IDC_E_TIME_STATIC)->SetFont(&m_static_font);
	GetDlgItem(IDC_CATEGORY_STATIC)->SetFont(&m_static_font);
	GetDlgItem(IDC_CONTENT_STATIC)->SetFont(&m_static_font);

	





	//달력 설정
	m_monthcalendar.SetFirstDayOfWeek(0);
	m_monthcalendar.SizeMinReq(FALSE);




	

	//리스트 컨트롤--------------------------------------------------------------------------------//
	// 한줄 선택
	m_plan_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 리스트 컨트롤에 컬럼 이름 입력
	m_plan_list.InsertColumn(0, L"일자");
	m_plan_list.SetColumnWidth(0, 70);
	m_plan_list.InsertColumn(1, L"시작시각");
	m_plan_list.SetColumnWidth(1, 60);
	m_plan_list.InsertColumn(2, L"마침시각");
	m_plan_list.SetColumnWidth(2, 60);
	m_plan_list.InsertColumn(3, L"분류");
	m_plan_list.SetColumnWidth(3, 50);
	m_plan_list.InsertColumn(4, L"내용");
    m_plan_list.SetColumnWidth(4, 100);

	//-------------------------------------------------------------------------------------------//

	//sqlite category_color table 생성
	Mytable temp_table;
	temp_table.make_sqlite_category();


	//sqlite 파일 만들기
	Myplan temp_plan;
	temp_plan.make_sqlite();

	//데이터 가져오기
	//week_array[] 초기화
	CTime today = CTime::GetCurrentTime();
	temp_plan.get_week_array(week_array, today);

	//선택된 날짜의 plan 데이터 가져오기
	vector<Myplan> temp_v_plan;
	for (int i = 0; i < 7; i++) {
		temp_plan.get_plan(week_array[i], &temp_v_plan);
	}

	//list 컨트롤 초기화
	if (temp_v_plan.size() > 0) {
		for (int i = 0; i < temp_v_plan.size(); i++) {
			int nitem = m_plan_list.InsertItem(0, temp_v_plan[i].date);
			m_plan_list.SetItemText(nitem, 1, temp_v_plan[i].s_time);
			m_plan_list.SetItemText(nitem, 2, temp_v_plan[i].e_time);
			m_plan_list.SetItemText(nitem, 3, temp_v_plan[i].category);
			m_plan_list.SetItemText(nitem, 4, temp_v_plan[i].content);
		}
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void Week::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	int left_top_x = 70;
	int left_top_y = 70;
	int right_btm_x = left_top_x + 700;
	int right_btm_y = left_top_y + 720;

	//dc를 얻기 위해 pWnd 가 필요함
	CWnd* pWnd = AfxGetMainWnd();

	//틀 그리기 함수
	Mytable temp_table;
	temp_table.make_table(pWnd, left_top_x, left_top_y, right_btm_x, right_btm_y);

	
	//vector 2차원 초기화 - 벡터는 생성되지 않은 곳은 참조할 수 없기 때문에 미리 7개를 기본값으로 생성한 후 그 자리에 요일별로 데이터를 추가시킨다.
	Myplan temp_plan_1;
	temp_plan_1.insert(_T("00000000"), _T("0000"), _T("0000"), _T("default"), _T("default"));
	vector<Myplan> temp_v_plan_1(1,temp_plan_1);
	vector<vector<Myplan>> temp_v_plan(7, temp_v_plan_1);
    //vector<vector<Myplan>> temp_v_plan;
	//일주일 데이터를 temp_v_plan 데이터에 받아오기 
	Myplan temp_plan;
	
	for (int i = 0; i < 7; i++) {
		temp_plan.get_plan(week_array[i], &temp_v_plan[i]);
	}

	//일주일 데이터를 timebox로 그리기
	temp_table.take_category_db(); // category 색상 값 받아오기
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < temp_v_plan[i].size(); j++) {
			temp_table.make_timebox(pWnd, i, temp_v_plan[i][j]);
		}
	}

	//콤보박스 리스트 설정하기
	m_category.ResetContent();
	for (int i = 0; i < temp_table.category_color.size(); i++) {
		m_category.AddString(temp_table.category_color[i].first);
	}


    m_plan_list.DeleteAllItems(); //리스트 컨트롤 내용을 전부 삭제

    //리스트 컨트롤에 일주일 데이터 입력
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < temp_v_plan[i].size(); j++) {
            if (temp_v_plan[i][j].date != _T("00000000")) { //기본값은 제외하고 출력한다.
                int nItem = m_plan_list.InsertItem(0, temp_v_plan[i][j].date);
                m_plan_list.SetItemText(nItem, 1, temp_v_plan[i][j].s_time);
                m_plan_list.SetItemText(nItem, 2, temp_v_plan[i][j].e_time);
                m_plan_list.SetItemText(nItem, 3, temp_v_plan[i][j].category);
                m_plan_list.SetItemText(nItem, 4, temp_v_plan[i][j].content);
            }
        }
    }
}
   
    



void Week::OnBnClickedAddBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//Date 업데이트
	UpdateData();

	//날짜 데이터 받아오기
	CTime date_picked;
	m_monthcalendar.GetCurSel(date_picked);
	CString date;
	CString year;
	CString month;
	CString day;

	year.Format(_T("%d"), date_picked.GetYear());

	if (date_picked.GetMonth() < 10) {
		month.Format(_T("0%d"), date_picked.GetMonth());
	}
	else {
		month.Format(_T("%d"), date_picked.GetMonth());
	}

	if (date_picked.GetDay() < 10) {
		day.Format(_T("0%d"), date_picked.GetDay());
	}
	else {
		day.Format(_T("%d"), date_picked.GetDay());
	}

	//날짜 데이터 생성
	date = year + month + day;

    //시작 시각 받아오기              
    CString t_s_time;
    CString s_time;
    int i_s_time; GetDlgItemText(IDC_S_TIME_EDIT, t_s_time);
    i_s_time = _ttoi(t_s_time);
    if (t_s_time.GetLength() == 4 && i_s_time >= 0 && i_s_time <= 2400) {
        if (t_s_time.Right(2) == _T("30") || t_s_time.Right(2) == _T("00")) {
            s_time = t_s_time;
        }
        else {
            AfxMessageBox(_T("시작시각을 30분 단위로 입력하세요"));
            return;
        }
    }
    else {
        AfxMessageBox(_T("시작시각을 0000 ~ 2400 사이 4자리로 입력하세요"));
        return;
    }

    //마침 시각 받아오기       
    CString t_e_time;
    CString e_time;
    int i_e_time;
    GetDlgItemText(IDC_E_TIME_EDIT, t_e_time);
    i_e_time = _ttoi(t_e_time);
    if (t_e_time.GetLength() == 4 && i_e_time >= 0 && i_e_time <= 2400) {
        if (t_e_time.Right(2) == _T("00") || t_e_time.Right(2) == _T("30")) {
            if (i_s_time < i_e_time) {
                e_time = t_e_time;
            }
            else {
                AfxMessageBox(_T("입력하려는 마침시각이 시작시각보다 작습니다"));
                return;
            }
        }
        else {
            AfxMessageBox(_T("마침시각을 30분 단위로 입력하세요"));
            return;
        }
    }
    else {
        AfxMessageBox(_T("마침시각을 0000 ~ 2400 사이 4자리로 입력하세요"));
        return;
    }


    //유효성 검사
    int count = m_plan_list.GetItemCount();
    for (int i = 0; i < count; i++)
    {
        if (date == m_plan_list.GetItemText(i, 0))
        {
            int S_time = _ttoi(m_plan_list.GetItemText(i, 1));          //리스트에 저장돼있는 시작시각
            int E_time = _ttoi(m_plan_list.GetItemText(i, 2));          //리스트에 저장돼있는 마침시각
            if (i_s_time < E_time && i_e_time > S_time)
            {
                AfxMessageBox(_T("같은 시간대에 계획이 있습니다"));
                return;
            }
        }
    }

	//분류 받아오기
	CString category;
	m_category.GetLBText(m_category.GetCurSel(), category);


	//내용 받아오기
	CString content;
	GetDlgItemText(IDC_CONTENT_EDIT, content);

	//Myplan 클래스 벡터
	Myplan c_plan;
	c_plan.insert(date, s_time, e_time, category, content);

    //plan 데이터베이스에 저장하기
    c_plan.save_db();



	//리스트 컨트롤에 추가하기
	int nItem = m_plan_list.InsertItem(0, date);
	m_plan_list.SetItemText(nItem, 1, s_time);
	m_plan_list.SetItemText(nItem, 2, e_time);
	m_plan_list.SetItemText(nItem, 3, category);
	m_plan_list.SetItemText(nItem, 4, content);

    //에디트 컨트롤 초기화
	m_s_time.SetWindowTextW(L"");
	m_e_time.SetWindowTextW(L"");
	m_content.SetWindowTextW(L"");
	m_category.SetWindowTextW(L"");

	
	
	Invalidate();

}


//달력 컨트롤 선택 변경 시 
void Week::OnMcnSelchangeMonthcalendar(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

      //데이터 업데이트
	UpdateData();
  

	//선택된 날짜가 속해 있는 일주일 날짜 배열 받아오기
	CTime date_picked;
	Myplan temp_plan;

	m_monthcalendar.GetCurSel(date_picked);
	temp_plan.get_week_array(week_array, date_picked);


   
    

	//일주일 날짜 Plan 데이터 받아오기
	vector<Myplan> temp_v_plan;
	for (int i = 0; i < 7; i++) {
		temp_plan.get_plan(week_array[i], &temp_v_plan);
	}
	

	Invalidate();

	*pResult = 0;
}


//삭제 버튼
void Week::OnBnClickedDelBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	 // list 컨트롤 구문
    int selectcheck = m_plan_list.GetSelectedCount(); //현재 선택된 행이 있는지 확인하기 위한 변수
    if (selectcheck > 0) { //현재 선택된 행이 있는 경우
        int row = m_plan_list.GetSelectionMark();
        CString id = m_plan_list.GetItemText(row, 0) + m_plan_list.GetItemText(row, 1) + m_plan_list.GetItemText(row, 2);
        m_plan_list.DeleteItem(row);

        m_s_time.SetWindowTextW(L"");
        m_e_time.SetWindowTextW(L"");
        m_content.SetWindowTextW(L"");
        m_category.SetWindowTextW(L"");

        //임시객체
        Myplan temp_plan;
        temp_plan.delete_db(id);

        Invalidate();
    }
	
	
}


void Week::OnNMClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int row = m_plan_list.GetSelectionMark();

	CString s_time = m_plan_list.GetItemText(row, 1);
	CString e_time = m_plan_list.GetItemText(row, 2);
	CString category = m_plan_list.GetItemText(row, 3);
	CString content = m_plan_list.GetItemText(row, 4);

	SetDlgItemText(IDC_S_TIME_EDIT, s_time);
	SetDlgItemText(IDC_E_TIME_EDIT, e_time);
	SetDlgItemText(IDC_CONTENT_EDIT, content);
	SetDlgItemText(IDC_CATEGORY_COMBO, category);







	*pResult = 0;
}


void Week::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_static_font.DeleteObject();
}




//엑셀로 출력 버튼 
void Week::OnBnClickedTestBtn()
{

    Book* book = xlCreateBook(); // xlCreateXMLBook() for xlsx

   //현재 시각 받아오기
    CTime today = CTime::GetCurrentTime();

    //int 형 변수를 CString 형으로 변환
    CString hour;
    if (today.GetHour() < 10) {
        hour.Format(_T("0%d"), today.GetHour());
    }
    else {
        hour.Format(_T("%d"), today.GetHour());
    }

    CString minute;
    if (today.GetMinute() < 10) {
        minute.Format(_T("0%d"), today.GetMinute());
    }
    else {
        minute.Format(_T("%d"), today.GetMinute());
    }

    CString second;
    if (today.GetSecond() < 10) {
        second.Format(_T("0%d"), today.GetSecond());
    }
    else {
        second.Format(_T("%d"), today.GetSecond());
    }

    //해당하는 주의 날짜
    CString period;
    period = week_array[0] + _T(" ~ ") + week_array[6];

    //엑셀 파일의 이름을 현재 날짜 + 시각으로 설정
    CString filename = period + _T("(") + hour + _T("시") + _T(" ") + minute + _T("분") _T(" ") + second + _T("초") + _T(")") + _T(".xls");




    //엑셀 테두리 그리기 + 가운데 정렬
    Format* format = book->addFormat();
    format->setAlignV(ALIGNV_CENTER);
    format->setAlignH(ALIGNH_CENTER);
    format->setBorder(BORDERSTYLE_MEDIUM);



    if (book)
    {
        Sheet* sheet = book->addSheet(L"계획");
        if (sheet)
        {
            Format* format1 = book->addFormat();
            format1->setBorder(BORDERSTYLE_DOUBLE);  //테두리 설정

            //1열 시간출력
            CString minute[2];
            minute[0] = _T("00");
            minute[1] = _T("30");
            int i_hour = 0;
            for (int i = 0; i < 48; i++) {           //24시간을 30분단위로 나눔
                CString s_hour1;
                CString s_hour2;
                CString time;
                if (i < 19) {
                    s_hour1.Format(_T("0%d"), i / 2);          //한 자리 수에 0을 붙여 두 자리수로 출력
                    s_hour2.Format(_T("0%d"), (i + 1) / 2);
                }
                else if (i == 19) {
                    s_hour1.Format(_T("0%d"), i / 2);
                    s_hour2.Format(_T("%d"), (i + 1) / 2);
                }
                else {
                    s_hour1.Format(_T("%d"), i / 2);
                    s_hour2.Format(_T("%d"), (i + 1) / 2);
                }


                time = s_hour1 + _T(":") + minute[i % 2] + _T("~") + s_hour2 + _T(":") + minute[(i + 1) % 2];     //엑셀의 시각부분을 출력하는 변수

                sheet->writeStr(i + 4, 0, time, format1);   //엑셀의 시각부분을 출력         
                sheet->setCol(0, 0, 11);        //셀의 열의 넓이를 11로 설정
            }

            //내용 출력
            Myplan My;
            vector <Myplan> MonDay;         //월요일의 계획을 저장하고 있는 myplan 객체의 vector 배열 선언
            vector <Myplan> TuesDay;
            vector <Myplan> WednesDay;
            vector <Myplan> ThursDay;
            vector <Myplan> FriDay;
            vector <Myplan> SaturDay;
            vector <Myplan> SunDay;

            //월요일 계획 출력
            My.get_plan(week_array[0], &MonDay);        //week 배열 0번째 정보를 MonDay에 저장

            for (int i = 0; i < MonDay.size(); i++) {                //월요일 계획의 갯수만큼 실행
                int hour1 = _ttoi(MonDay[i].s_time.Left(2));         //월요일 계획 시작 시각의 시간 부분을 저장
                int minute1 = _ttoi(MonDay[i].s_time.Right(2));      //월요일 계획 시작 시각의 분 부분을 저장
                int hour2 = _ttoi(MonDay[i].e_time.Left(2));         //월요일 계획 마침 시각의 시간 부분을 저장
                int minute2 = _ttoi(MonDay[i].e_time.Right(2));      //월요일 계획 마침 시각의 분 부분을 저장 

                int hour = (hour2 - hour1) * 60;                   //마침 시각의 시 부분에서 시작 시각의 시 부분을 뺀 후 X60 
                int Minute = minute2 - minute1;                    //마침 시각의 분 부분과 시작 시각의 분 부분의 차

                double Mon = 2 * MonDay[i].content.GetLength();            //월요일의 계획 중 가장 긴 글자 수의 X2

                for (int j = 0; j < (hour + Minute) / 30; j++)
                {
                    int num = sizeof(MonDay[i].content);
                    if (minute1 == 30) {                               //시작 시각의 분 부분이 30일 때
                       //셀 병합
                        sheet->setMerge(hour1 * 2 + 5, hour1 * 2 + 4 + (hour + Minute) / 30, 1, 1);
                        sheet->writeStr(hour1 * 2 + 5 + j, 1, MonDay[i].content, format);
                        if (Mon > 6) {
                            sheet->setCol(1, 1, Mon);
                        }
                        else {            //월요일 계획의 가장 긴 글자수가 6보다 작을 때
                            sheet->setCol(1, 1, 6);            //열의 너비를 6으로 설정
                        }
                    }
                    else {                                             //시작 시각의 분 부분이 00일 때
                        sheet->setMerge(hour1 * 2 + 4, hour1 * 2 + 3 + (hour + Minute) / 30, 1, 1);
                        sheet->writeStr(hour1 * 2 + 4 + j, 1, MonDay[i].content, format);
                        if (Mon > 6) {
                            sheet->setCol(1, 1, Mon);
                        }
                        else {
                            sheet->setCol(1, 1, 6);
                        }
                    }
                }
            }

            //화요일 계획 출력
            My.get_plan(week_array[1], &TuesDay);

            for (int i = 0; i < TuesDay.size(); i++) {
                int hour1 = _ttoi(TuesDay[i].s_time.Left(2));
                int minute1 = _ttoi(TuesDay[i].s_time.Right(2));
                int hour2 = _ttoi(TuesDay[i].e_time.Left(2));
                int minute2 = _ttoi(TuesDay[i].e_time.Right(2));

                int hour = (hour2 - hour1) * 60;
                int Minute = minute2 - minute1;

                double Tues = 2 * TuesDay[i].content.GetLength();

                for (int j = 0; j < (hour + Minute) / 30; j++)
                {
                    if (minute1 == 30) {
                        sheet->setMerge(hour1 * 2 + 5, hour1 * 2 + 4 + (hour + Minute) / 30, 2, 2);
                        sheet->writeStr(hour1 * 2 + 5 + j, 2, TuesDay[i].content, format);
                        if (Tues > 6) {
                            sheet->setCol(2, 2, Tues);
                        }
                        else {
                            sheet->setCol(2, 2, 6);
                        }
                    }
                    else {
                        sheet->setMerge(hour1 * 2 + 4, hour1 * 2 + 3 + (hour + Minute) / 30, 2, 2);
                        sheet->writeStr(hour1 * 2 + 4 + j, 2, TuesDay[i].content, format);
                        if (Tues > 6) {
                            sheet->setCol(2, 2, Tues);
                        }
                        else {
                            sheet->setCol(2, 2, 6);
                        }
                    }
                }
            }

            //수요일 계획 출력
            My.get_plan(week_array[2], &WednesDay);

            for (int i = 0; i < WednesDay.size(); i++) {
                int hour1 = _ttoi(WednesDay[i].s_time.Left(2));
                int minute1 = _ttoi(WednesDay[i].s_time.Right(2));
                int hour2 = _ttoi(WednesDay[i].e_time.Left(2));
                int minute2 = _ttoi(WednesDay[i].e_time.Right(2));

                int hour = (hour2 - hour1) * 60;
                int Minute = minute2 - minute1;

                double Wed = 2 * WednesDay[i].content.GetLength();

                for (int j = 0; j < (hour + Minute) / 30; j++)
                {
                    if (minute1 == 30) {
                        sheet->setMerge(hour1 * 2 + 5, hour1 * 2 + 4 + (hour + Minute) / 30, 3, 3);
                        sheet->writeStr(hour1 * 2 + 5 + j, 3, WednesDay[i].content, format);
                        if (Wed > 6) {
                            sheet->setCol(3, 3, Wed);
                        }
                        else {
                            sheet->setCol(3, 3, 6);
                        }
                    }
                    else {
                        sheet->setMerge(hour1 * 2 + 4, hour1 * 2 + 3 + (hour + Minute) / 30, 3, 3);
                        sheet->writeStr(hour1 * 2 + 4 + j, 3, WednesDay[i].content, format);
                        if (Wed > 6) {
                            sheet->setCol(3, 3, Wed);
                        }
                        else {
                            sheet->setCol(3, 3, 6);
                        }
                    }
                }
            }

            //목요일 계획 출력
            My.get_plan(week_array[3], &ThursDay);

            for (int i = 0; i < ThursDay.size(); i++) {
                int hour1 = _ttoi(ThursDay[i].s_time.Left(2));
                int minute1 = _ttoi(ThursDay[i].s_time.Right(2));
                int hour2 = _ttoi(ThursDay[i].e_time.Left(2));
                int minute2 = _ttoi(ThursDay[i].e_time.Right(2));

                int hour = (hour2 - hour1) * 60;
                int Minute = minute2 - minute1;

                double Thurs = 2 * ThursDay[i].content.GetLength();

                for (int j = 0; j < (hour + Minute) / 30; j++)
                {
                    if (minute1 == 30) {
                        sheet->setMerge(hour1 * 2 + 5, hour1 * 2 + 4 + (hour + Minute) / 30, 4, 4);
                        sheet->writeStr(hour1 * 2 + 5 + j, 4, ThursDay[i].content, format);
                        if (Thurs > 6) {
                            sheet->setCol(4, 4, Thurs);
                        }
                        else {
                            sheet->setCol(4, 4, 6);
                        }
                    }
                    else {
                        sheet->setMerge(hour1 * 2 + 4, hour1 * 2 + 3 + (hour + Minute) / 30, 4, 4);
                        sheet->writeStr(hour1 * 2 + 4 + j, 4, ThursDay[i].content, format);
                        if (Thurs > 6) {
                            sheet->setCol(4, 4, Thurs);
                        }
                        else {
                            sheet->setCol(4, 4, 6);
                        }
                    }
                }
            }

            //금요일 계획 출력
            My.get_plan(week_array[4], &FriDay);

            for (int i = 0; i < FriDay.size(); i++) {
                int hour1 = _ttoi(FriDay[i].s_time.Left(2));
                int minute1 = _ttoi(FriDay[i].s_time.Right(2));
                int hour2 = _ttoi(FriDay[i].e_time.Left(2));
                int minute2 = _ttoi(FriDay[i].e_time.Right(2));

                int hour = (hour2 - hour1) * 60;
                int Minute = minute2 - minute1;

                double Fri = 2 * FriDay[i].content.GetLength();

                for (int j = 0; j < (hour + Minute) / 30; j++)
                {
                    if (minute1 == 30) {
                        sheet->setMerge(hour1 * 2 + 5, hour1 * 2 + 4 + (hour + Minute) / 30, 5, 5);
                        sheet->writeStr(hour1 * 2 + 5 + j, 5, FriDay[i].content, format);
                        if (Fri > 6) {
                            sheet->setCol(5, 5, Fri);
                        }
                        else {
                            sheet->setCol(5, 5, 6);
                        }
                    }
                    else {
                        sheet->setMerge(hour1 * 2 + 4, hour1 * 2 + 3 + (hour + Minute) / 30, 5, 5);
                        sheet->writeStr(hour1 * 2 + 4 + j, 5, FriDay[i].content, format);
                        if (Fri > 6) {
                            sheet->setCol(5, 5, Fri);
                        }
                        else {
                            sheet->setCol(5, 5, 6);
                        }
                    }
                }
            }

            //토요일 계획 출력
            My.get_plan(week_array[5], &SaturDay);

            for (int i = 0; i < SaturDay.size(); i++) {
                int hour1 = _ttoi(SaturDay[i].s_time.Left(2));
                int minute1 = _ttoi(SaturDay[i].s_time.Right(2));
                int hour2 = _ttoi(SaturDay[i].e_time.Left(2));
                int minute2 = _ttoi(SaturDay[i].e_time.Right(2));

                int hour = (hour2 - hour1) * 60;
                int Minute = minute2 - minute1;

                double Satur = 2 * SaturDay[i].content.GetLength();

                for (int j = 0; j < (hour + Minute) / 30; j++)
                {
                    if (minute1 == 30) {
                        sheet->setMerge(hour1 * 2 + 5, hour1 * 2 + 4 + (hour + Minute) / 30, 6, 6);
                        sheet->writeStr(hour1 * 2 + 5 + j, 6, SaturDay[i].content, format);
                        if (Satur > 6) {
                            sheet->setCol(6, 6, Satur);
                        }
                        else {
                            sheet->setCol(6, 6, 6);
                        }
                    }
                    else {
                        sheet->setMerge(hour1 * 2 + 4, hour1 * 2 + 3 + (hour + Minute) / 30, 6, 6);
                        sheet->writeStr(hour1 * 2 + 4 + j, 6, SaturDay[i].content, format);
                        if (Satur > 6) {
                            sheet->setCol(6, 6, Satur);
                        }
                        else {
                            sheet->setCol(6, 6, 6);
                        }
                    }
                }
            }

            //일요일 계획 출력
            My.get_plan(week_array[6], &SunDay);

            for (int i = 0; i < SunDay.size(); i++) {
                int hour1 = _ttoi(SunDay[i].s_time.Left(2));
                int minute1 = _ttoi(SunDay[i].s_time.Right(2));
                int hour2 = _ttoi(SunDay[i].e_time.Left(2));
                int minute2 = _ttoi(SunDay[i].e_time.Right(2));

                int hour = (hour2 - hour1) * 60;
                int Minute = minute2 - minute1;

                double Sun = 2 * SunDay[i].content.GetLength();

                for (int j = 0; j < (hour + Minute) / 30; j++)
                {
                    if (minute1 == 30) {
                        sheet->setMerge(hour1 * 2 + 5, hour1 * 2 + 4 + (hour + Minute) / 30, 7, 7);
                        sheet->writeStr(hour1 * 2 + 5 + j, 7, SunDay[i].content, format);
                        if (Sun > 6) {
                            sheet->setCol(7, 7, Sun);
                        }
                        else {
                            sheet->setCol(7, 7, 6);
                        }
                    }
                    else {
                        sheet->setMerge(hour1 * 2 + 4, hour1 * 2 + 3 + (hour + Minute) / 30, 7, 7);
                        sheet->writeStr(hour1 * 2 + 4 + j, 7, SunDay[i].content, format);
                        if (Sun > 6) {
                            sheet->setCol(7, 7, Sun);
                        }
                        else {
                            sheet->setCol(7, 7, 6);
                        }
                    }
                }
            }
        }

        //제목 틀 만들기
        CString period;
        period = week_array[0] + _T(" ~ ") + week_array[6];            //해당하는 주의 날짜를 받는 변수
        Format* format3 = book->addFormat();
        format3->setBorder(BORDERSTYLE_THICK);                         //테두리 스타일
        format3->setAlignV(ALIGNV_CENTER);                             //폭의 중앙
        format3->setAlignH(ALIGNH_CENTER);                             //높이의 중앙
        sheet->setMerge(1, 2, 0, 7);                                   //1행부터 2행, 0열부터 7열까지 셀 병합
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 2; j++) {
                sheet->writeStr(1 + j, i, period, format3);
            }
        }



        //엑셀 3행에 각 요일 출력하기
        sheet->writeStr(3, 1, L"월요일", format);
        sheet->writeStr(3, 2, L"화요일", format);
        sheet->writeStr(3, 3, L"수요일", format);
        sheet->writeStr(3, 4, L"목요일", format);
        sheet->writeStr(3, 5, L"금요일", format);
        sheet->writeStr(3, 6, L"토요일", format);
        sheet->writeStr(3, 7, L"일요일", format);





        book->save(filename);
        book->release();
    }

}

