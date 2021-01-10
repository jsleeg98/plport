#include "pch.h"
#include "plport_ver3.h"
#include "Setup.h"
#include "afxdialogex.h"

#include "Myplan.h"
#include "Mytable.h"

//sqlite3 include----------------------------------------------------------------------------//
#include "sqlite3.h"
#include <assert.h>


#pragma comment(lib, "sqlite3.lib")
//--------------------------------------------------------------------------------------------//

using namespace std;

// Setup 대화 상자

IMPLEMENT_DYNAMIC(Setup, CDialogEx)

Setup::Setup(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETUP_DIALOG, pParent)
{
	
}

Setup::~Setup()
{
}

void Setup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CATEGORY_COLOR_LIST, m_category_list);
	
}


BEGIN_MESSAGE_MAP(Setup, CDialogEx)
	ON_BN_CLICKED(IDC_COLOR_SEL_BTN, &Setup::OnBnClickedColorSelBtn)
	ON_WM_PAINT()
	
	ON_NOTIFY(NM_CLICK, IDC_CATEGORY_COLOR_LIST, &Setup::OnNMClickCategoryColorList)
	ON_BN_CLICKED(IDC_CATEGORY_ADD_BTN, &Setup::OnBnClickedCategoryAddBtn)
	ON_BN_CLICKED(IDC_ADD_RADIO, &Setup::OnBnClickedAddRadio)
	ON_BN_CLICKED(IDC_DEL_RADIO, &Setup::OnBnClickedDelRadio)
	ON_BN_CLICKED(IDC_MODIFY_COLOR_BTN, &Setup::OnBnClickedModifyColorBtn)
	ON_BN_CLICKED(IDC_MODIFY_CATEGORY_BTN, &Setup::OnBnClickedModifyCategoryBtn)
	ON_BN_CLICKED(IDC_CATEGORY_DEL_BTN, &Setup::OnBnClickedCategoryDelBtn)
	ON_BN_CLICKED(IDC_CATEGORY_CHANGE_RADIO, &Setup::OnBnClickedCategoryChangeRadio)
	ON_BN_CLICKED(IDC_COLOR_CHANGE_RADIO, &Setup::OnBnClickedColorChangeRadio)
END_MESSAGE_MAP()


// Setup 메시지 처리기





void Setup::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					  
	m_category_list.DeleteAllItems(); //리스트컨트롤 리셋
	

	Mytable temp_table; //Mytable 사용을 위한 객체 생성
	temp_table.take_category_db(); //데이터베이스에서 CATEGORY_COLOR 데이터 받아오기

	//리스트 컨트롤에 데이터베이스에 있는 분류, R, G, B 색상값을 출력
	for (int i = 0; i < temp_table.category_color.size(); i++) {
		CString red;
		CString blue;
		CString green;
		red.Format(_T("%d"), GetRValue(temp_table.category_color[i].second));
		green.Format(_T("%d"), GetGValue(temp_table.category_color[i].second));
		blue.Format(_T("%d"), GetBValue(temp_table.category_color[i].second));

		int nitem = m_category_list.InsertItem(0, temp_table.category_color[i].first);
		m_category_list.SetItemText(nitem, 1, red);
		m_category_list.SetItemText(nitem, 2, green);
		m_category_list.SetItemText(nitem, 3, blue);
	}

	//분류 선택 여부 확인
	int row = -1;
	row = m_category_list.GetSelectionMark();
	if (row == -1) { //선택되지 않았다면 흰색 박스로 선택된 불류 색상 상자 그리기
		i_red = i_green = i_blue = 255; //i_red, i_green, i_blue 초기화
	}
	
	//색상 선택 여부 확인 //항상 FALSE로 탭이 바뀌거나 메뉴 선택을 했을때 흰색으로 초기화 되도록 한다.
	if (check_color == FALSE) {
		color = RGB(255, 255, 255);
	}
	
	//선택된 분류 색상 상자 칠하기
	CClientDC dc_1(GetDlgItem(IDC_SEL_COLOR));
	CRect rect_1;
	GetDlgItem(IDC_SEL_COLOR)->GetClientRect(&rect_1);
	dc_1.FillSolidRect(rect_1, RGB(i_red, i_green, i_blue));



	//추가할 색상 상자 칠하기
	CClientDC dc_2(GetDlgItem(IDC_ADD_COLOR));
	CRect rect_2;
	GetDlgItem(IDC_ADD_COLOR)->GetClientRect(&rect_2);
	dc_2.FillSolidRect(rect_2, color);
	




}


BOOL Setup::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	color = RGB(255, 255, 255); //color 초기값 설정
	i_red = i_green = i_blue = 255; //i_red, i_green, i_blue 초기값 설정
	check_color = FALSE;

	//리스트 컨트롤 전체 셀선택과 그리드 라인 옵션 설정
	m_category_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 리스트 컨트롤에 컬럼 이름 입력
	m_category_list.InsertColumn(0, L"분류");
	m_category_list.SetColumnWidth(0, 80);
	m_category_list.InsertColumn(1, L"R");
	m_category_list.SetColumnWidth(1, 40);
	m_category_list.InsertColumn(2, L"G");
	m_category_list.SetColumnWidth(2, 40);
	m_category_list.InsertColumn(3, L"B");
	m_category_list.SetColumnWidth(3, 40);

	//초기 버튼 모두 비활성화
	GetDlgItem(IDC_CATEGORY_ADD_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CATEGORY_DEL_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MODIFY_CATEGORY_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MODIFY_COLOR_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_COLOR_SEL_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CATEGORY_EDIT)->EnableWindow(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void Setup::OnBnClickedColorSelBtn() //색상선택 버튼이 눌렸을 때
{

	Mytable temp_table; //Mytable 사용을 위한 객체 생성
	temp_table.take_category_db(); //데이터베이스 파일에서 CATEGORY_COLOR 데이터 가져오기
	COLORREF colors[16]; //사용자 지정색 표시를 위한 색상 배열
	for (int i = 0; i < temp_table.category_color.size(); i++) {
		if (i < 16) {
			colors[i] = temp_table.category_color[i].second; //현재 데이터베이스에 있는 색상을 사용자 지정색 배열에 입력
		}
	}

	//색상 선택 대화상자 생성
	CColorDialog dlg(RGB(255, 0, 0), CC_FULLOPEN);
	dlg.m_cc.lpCustColors = colors; //사용자 지정색 위치에 현재 데이터베이스에 있는 색상을 입력
	dlg.DoModal(); //모달창으로 대화상자 출력
	color = dlg.GetColor(); //선택한 색상을 받음


	
	//색상 상자 그리기
	CClientDC dc_2(GetDlgItem(IDC_ADD_COLOR));
	CRect rect_2;
	GetDlgItem(IDC_ADD_COLOR)->GetClientRect(&rect_2);
	dc_2.FillSolidRect(rect_2, color);

}




void Setup::OnNMClickCategoryColorList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//선택된 리스트 정보 가져오기
	int row = m_category_list.GetSelectionMark();

	CString category = m_category_list.GetItemText(row, 0);
	CString red = m_category_list.GetItemText(row, 1);
	CString green = m_category_list.GetItemText(row, 2);
	CString blue = m_category_list.GetItemText(row, 3);
	i_red = _ttoi(red);
	i_green = _ttoi(green);
	i_blue = _ttoi(blue);



	//색상 상자 그리기
	//선택된 분류 색상 상자 칠하기
	CClientDC dc_1(GetDlgItem(IDC_SEL_COLOR));
	CRect rect_1;
	GetDlgItem(IDC_SEL_COLOR)->GetClientRect(&rect_1);
	dc_1.FillSolidRect(rect_1, RGB(i_red, i_green, i_blue));
	

	*pResult = 0;
}



void Setup::OnBnClickedCategoryAddBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	//추가/수정할 분류 이름 가져오기
	CString category;
	GetDlgItemText(IDC_CATEGORY_EDIT,category);

	Mytable temp_table; //Mytable 사용을 위한 객체 생성
	temp_table.take_category_db(); //데이터베이스 파일에서 CATEGORY_COLOR 데이터 가져오기

	//중복되는 분류명이 있는지 확인
	for (int i = 0; i < temp_table.category_color.size(); i++) {
		if (category == temp_table.category_color[i].first) {
			MessageBox(_T("현재 존재하는 분류명입니다\n다른 분류명을 입력해주세요"));
			return;
		}
	}

	//분류명 입력 여부 확인
	if (category == _T("")) {
		MessageBox(_T("분류명을 입력해주세요"), 0, MB_OK | MB_ICONERROR);
		return;
	}

	//색상선택여부 확인
	if (color == RGB(255, 255, 255)) {
		MessageBox(_T("색상을 선택해주세요"), 0, MB_OK | MB_ICONERROR);
		return;
	}

	//plan.db 파일에 저장하기
	temp_table.insert_category_color(category, GetRValue(color), GetGValue(color), GetBValue(color));

	//확인메세지
	MessageBox(_T("분류가 추가되었습니다"), 0, MB_OK|MB_ICONINFORMATION);
	SetDlgItemText(IDC_CATEGORY_EDIT, _T(""));

	//리스트 컨트롤 갱신하기
	i_red = i_green = i_blue = 255; //i_red, i_green, i_blue 초기화
	Invalidate();
}



void Setup::OnBnClickedModifyColorBtn()
{
	//선택된 분류명 가져오기
	int row = -1;
	row = m_category_list.GetSelectionMark();
	CString category = m_category_list.GetItemText(row, 0);

	//분류 선택여부 확인
	if (row == -1) {
		MessageBox(_T("선택된 분류가 없습니다\n분류를 선택해주세요"), 0, MB_OK | MB_ICONERROR);
		return;
	}


	//색상선택여부 확인
	if (color == RGB(255, 255, 255)) {
		MessageBox(_T("색상을 선택해주세요"), 0, MB_OK | MB_ICONERROR);
		return;
	}

	Mytable temp_table; //Mytable 사용을 위한 객체 생성
	temp_table.change_category_color_db(category, GetRValue(color), GetGValue(color), GetBValue(color)); //분류 색상 변경

	//확인메세지
	MessageBox(_T("분류 색상이 변경되었습니다"), 0, MB_OK | MB_ICONINFORMATION);
	color = RGB(255, 255, 255); //색상 초기화
	i_red = i_green = i_blue = 255; //i_red, i_green, i_blue 초기화

	Invalidate(); //리스트 컨트롤 갱신

}


void Setup::OnBnClickedModifyCategoryBtn()
{
	CString old_category;
	CString new_category;

	GetDlgItemText(IDC_CATEGORY_EDIT, new_category); //새로 입력한 분류명 가져오기

	//선택된 분류명 가져오기
	int row = -1;
	row = m_category_list.GetSelectionMark();
	old_category = m_category_list.GetItemText(row, 0);

	//분류 선택여부 확인
	if (row == -1) {
		MessageBox(_T("선택된 분류가 없습니다\n분류를 선택해주세요"), 0, MB_OK | MB_ICONERROR);
		return;
	}

	Mytable temp_table; //Mytable 사용을 위한 객체 생성
	temp_table.take_category_db(); //데이터베이스 파일에서 CATEGORY_COLOR 데이터 가져오기

	//분류명 입력 여부 확인
	if (new_category == _T("")) {
		MessageBox(_T("분류명을 입력해주세요"), 0, MB_OK | MB_ICONERROR);
		return;
	}
	//중복되는 분류명이 있는지 확인
	for (int i = 0; i < temp_table.category_color.size(); i++) {
		if (new_category == temp_table.category_color[i].first) {
			MessageBox(_T("현재 존재하는 분류명입니다\n 다른 분류명을 입력해주세요"));
			return;
		}
	}


	temp_table.change_category_name_db(old_category, new_category); //CATEGORY_COLOR 테이블에서 분류명 변경하기

	Myplan temp_plan;
	temp_plan.change_category_name_db(old_category, new_category); //PLANDATA_COLOR 테이블에서 분류명 변경하기


	//확인메세지
	MessageBox(_T("분류명이 수정되었습니다"), 0, MB_OK | MB_ICONINFORMATION);
	SetDlgItemText(IDC_CATEGORY_EDIT, _T(""));

	i_red = i_green = i_blue = 255; //i_red, i_green, i_blue 초기화

	//리스트컨트롤 갱신
	Invalidate(); 



}


void Setup::OnBnClickedCategoryDelBtn()
{
	//선택된 분류명 가져오기
	int row = -1;
	row = m_category_list.GetSelectionMark();
	CString category = m_category_list.GetItemText(row, 0);

	//분류 선택여부 확인
	if (row == -1) {
		MessageBox(_T("선택된 분류가 없습니다\n분류를 선택해주세요"), 0, MB_OK | MB_ICONERROR);
		return;
	}

	//삭제 경고 메세지
	if (IDYES == MessageBox(_T("삭제하려는 분류로 이전에 계획을 추가했다면\n 분류 삭제 시 계획표에 색상이 지워질 수 있습니다\n그래도 삭제하시겠습니까?"), 0, MB_YESNO | MB_ICONWARNING)) {
		Mytable temp_table; //Mytable 사용을 위한 객체 생성
		temp_table.delete_category_db(category); //데이터베이스 파일에서 선택된 분류 삭제
		MessageBox(_T("분류가 삭제되었습니다"), 0, MB_OK | MB_ICONINFORMATION); //삭제 확인 메세지
		
	}
	else {
		MessageBox(_T("삭제를 취소했습니다"), 0, MB_OK | MB_ICONINFORMATION);
		return;
	}

	i_red = i_green = i_blue = 255; //i_red, i_green, i_blue 초기화

	Invalidate(); //리스트 컨트롤 갱신
	
}


void Setup::OnBnClickedAddRadio()
{
	GetDlgItem(IDC_CATEGORY_ADD_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_CATEGORY_DEL_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MODIFY_CATEGORY_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MODIFY_COLOR_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_COLOR_SEL_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_CATEGORY_EDIT)->EnableWindow(TRUE);
	i_red = i_green = i_blue = 255; //i_red, i_green, i_blue 초기화
	Invalidate(); //화면 갱신
}


void Setup::OnBnClickedDelRadio()
{
	GetDlgItem(IDC_CATEGORY_ADD_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CATEGORY_DEL_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_MODIFY_CATEGORY_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MODIFY_COLOR_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_COLOR_SEL_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CATEGORY_EDIT)->EnableWindow(FALSE);
	i_red = i_green = i_blue = 255; //i_red, i_green, i_blue 초기화
	Invalidate(); //화면 갱신
}

void Setup::OnBnClickedCategoryChangeRadio()
{
	GetDlgItem(IDC_CATEGORY_ADD_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CATEGORY_DEL_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MODIFY_CATEGORY_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_MODIFY_COLOR_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_COLOR_SEL_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CATEGORY_EDIT)->EnableWindow(TRUE);
	i_red = i_green = i_blue = 255; //i_red, i_green, i_blue 초기화
	Invalidate(); //화면 갱신
}


void Setup::OnBnClickedColorChangeRadio()
{
	GetDlgItem(IDC_CATEGORY_ADD_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_CATEGORY_DEL_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MODIFY_CATEGORY_BTN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MODIFY_COLOR_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_COLOR_SEL_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_CATEGORY_EDIT)->EnableWindow(FALSE);
	i_red = i_green = i_blue = 255; //i_red, i_green, i_blue 초기화
	Invalidate(); //화면 갱신
}

