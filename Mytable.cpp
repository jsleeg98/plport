//이동현 MYTABLE 클래스 구현

#include "pch.h"
#include <iostream>
#include "Mytable.h"

Mytable::Mytable() {
	
}

//인터넷에서 가져온 함수 //sqlite 한글을 저장하기 위한 변환 함수 
int Mytable::AnsiToUTF8(char* szSrc, char* strDest, int destSize)
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

//인터넷에서 가져온 함수 //sqlite 데이터를 내부에서 처리하기 위한 변환 함수 
int Mytable::UTF8ToAnsi(char* szSrc, char* strDest, int destSize)
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

//주간계획표의 틀을 출력하는 함수
void Mytable::make_table(CWnd* pWnd, int l_t_x, int l_t_y, int r_b_x, int r_b_y) {

	CClientDC dc(pWnd);

	//외각 틀 그리기
	dc.Rectangle(l_t_x, l_t_y, r_b_x, r_b_y);


	//요일 칸 나누기
	for (int i = 1; i < 7; i++) { // 요일 당 너비 100으로 설정
		dc.MoveTo(l_t_x + 100 * i, l_t_y);
		dc.LineTo(l_t_x + 100 * i, r_b_y);
	}

	//시간 칸 나누기
	CPen pen;
	pen.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	dc.SelectObject(&pen);
	for (int i = 1; i < 24; i++) { //1시간 당 높이 30으로 설정
		dc.MoveTo(l_t_x, l_t_y + 30 * i);
		dc.LineTo(r_b_x, l_t_y + 30 * i);
	}

	//요일 텍스트 출력
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(90 , 50, _T("월요일"));
	dc.TextOut(90 + 100, 50, _T("화요일"));
	dc.TextOut(90 + 200, 50, _T("수요일"));
	dc.TextOut(90 + 300, 50, _T("목요일"));
	dc.TextOut(90 + 400, 50, _T("금요일"));
	dc.TextOut(90 + 500, 50, _T("토요일"));
	dc.TextOut(90 + 600, 50, _T("일요일"));

	//시간 텍스트 출력
	for (int i = 0; i < 25; i++) {
		CString time;
		if (i < 10) {
			time.Format(_T("0%d:00"), i);
		}
		else {
			time.Format(_T("%d:00"), i);
		}
		
		dc.TextOut(25, 60 + 30 * i, time);
	}


}

//주간계획표 내부에 timebox를 출력하는 함수
void Mytable::make_timebox(CWnd* pWnd, int week_number, Myplan plan) {

	CClientDC dc(pWnd);

	CString s_time = plan.s_time; //시작시각 받아오기
	CString e_time = plan.e_time; //마침시각 받아오기

	int top_left_x;
	double top_left_y;
	int btm_right_x;
	double btm_right_y;

	//시간 데이터를 정수형으로 시, 분으로 나누어 받기
	int s_time_h = _ttoi(s_time.Left(2)); //시작시각 시
	int s_time_m = _ttoi(s_time.Right(2)); //시작시각 분
	int e_time_h = _ttoi(e_time.Left(2)); //마침시각 시
	int e_time_m = _ttoi(e_time.Right(2)); //마침시각 분

	//사각형을 그릴 좌표 설정// 하루 x = 100, 1시간 = 30 
	top_left_x = 70 + (100 * week_number); //요일에 따라 x축 위치 설정
	top_left_y = double(70.0 + (30.0 * s_time_h) + (30.0 * double(s_time_m) / 60.0)); //시작시각에 따라 왼쪽 위 y축 설정 한시간당 30픽셀, 30분은 15픽셀
	btm_right_x = 70 + (100 * (1 + week_number)); 
	btm_right_y = double(70.0 + (30.0 * e_time_h) + (30.0 * double(e_time_m) / 60.0)); //마침시각에 따라 오른쪽 아래 y축 설정, 한시간당 30픽셀, 30분은 15픽셀


	CBrush brush;
	CBrush* poldbrush = NULL;
	for (int i = 0; i < category_color.size();i++) {
		if (plan.category == category_color[i].first) { //분류명이 같은 것 확인
			brush.CreateSolidBrush(category_color[i].second); //cateogory_color 색상 정보를 활용하여 브러쉬 생성
			poldbrush= dc.SelectObject(&brush); //브러쉬 선택
		}
	}

	//폰트 설정
	CFont font;
	font.CreateFont(15,0, 0,0, FW_EXTRABOLD,  FALSE, FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH, _T("굴림체"));

	//timebox 그리기
	CRect timebox(top_left_x, top_left_y, btm_right_x, btm_right_y);
	dc.Rectangle(timebox); //timebox 그리기
	dc.SelectObject(poldbrush); //브러쉬 색상 원래대로 되돌려주기
	dc.SelectObject(font); //폰트 설정
	dc.SetBkMode(TRANSPARENT); // 텍스트 배경 투명화
	dc.DrawText(plan.content, timebox, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //content 쓰기
	
}

//plan.db에 CATEGORY_COLOR 테이블을 생성
void Mytable::make_sqlite_category() {

	sqlite3* db;
	sqlite3_stmt* stmt = NULL;
	char* errmsg = NULL;

	int rc = sqlite3_open("plan.db", &db); //plan.db 파일 열기

	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}

	//plan.db에 CATEGORY_COLOR 테이블 생성
	char* sql;
	sql = "CREATE TABLE IF NOT EXISTS CATEGORY_COLOR("
		"CATEGORY		TEXT	 PRIMARY KEY ,"
		"RED	INT		NOT NULL,"
		"GREEN	INT		NOT NULL,"
		"BLUE	INT		NOT NULL);";

	rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);

	if (rc != SQLITE_OK) {
		printf("create table");
		sqlite3_free(errmsg);
		sqlite3_close(db);
		exit(1);
	}

	sqlite3_finalize(stmt);

	sqlite3_close(db);

}



//CATEGORY_COLOR 테이블에 데이터 추가
void Mytable::insert_category_color(CString category, int red, int green, int blue) {

	sqlite3* db;
	int rc = sqlite3_open("plan.db", &db); //plan.db 열기


	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}

	
	//category sql 데이터로 바꿔주기
	char* s_category;
	int sLen = WideCharToMultiByte(CP_ACP, 0, category, -1, NULL, 0, NULL, NULL);
	s_category = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, category, -1, s_category, sLen, NULL, NULL);

	char sz_category[100];
	AnsiToUTF8(s_category, sz_category, 100);

	delete[]s_category;

	char* errmsg = NULL;
	char sql[3000] = { 0 };
	sprintf(sql, "insert into CATEGORY_COLOR(CATEGORY, RED, GREEN, BLUE) values('%s',%d,%d,%d);",sz_category, red, green, blue);
	//sqlite insert 명령어를 사용하여 CATEGORY_COLOR 테이블에 데이터 삽입


	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg))
	{
		printf("insert");
	}



	sqlite3_close(db); //plan.db 파일 닫기

}

//plan.db의 CATEGORY_COLOR 테이블의 데이터 가져오기
void Mytable::take_category_db() {

	sqlite3* db;
	sqlite3_stmt* stmt = NULL;
	char* errmsg = NULL;

	int rc = sqlite3_open("plan.db", &db); //plan.db 파일 열기

	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}



	sqlite3_prepare_v2(db, "select * from CATEGORY_COLOR", -1, &stmt, NULL);
	//sqlite select 구문을 사용하여 CATEGORY_COLOR 테이블에서 데이터 모두 가져오기

	while (sqlite3_step(stmt) != SQLITE_DONE) {
		int red;
		int green;
		int blue;

		char szAnsi[300];
		UTF8ToAnsi((char*)sqlite3_column_text(stmt, 0), szAnsi, 300);
		CString category(szAnsi);

		UTF8ToAnsi((char*)sqlite3_column_text(stmt, 1), szAnsi, 300);
		CString s_red(szAnsi);
		red = _ttoi(s_red);

		UTF8ToAnsi((char*)sqlite3_column_text(stmt, 2), szAnsi, 300);
		CString s_green(szAnsi);
		green = _ttoi(s_green);

		UTF8ToAnsi((char*)sqlite3_column_text(stmt, 3), szAnsi, 300);
		CString s_blue(szAnsi);
		blue = _ttoi(s_blue);

		COLORREF rgb = RGB(red, green, blue);

		category_color.push_back(make_pair(category, rgb)); //category_color 백터 배열에 추가
	}


	sqlite3_finalize(stmt);

	sqlite3_close(db);


}

//CATEGORY_COLOR 테이블에 데이터의 유무 확인
bool Mytable::check_category_db() {
	sqlite3* db;
	sqlite3_stmt* stmt = NULL;
	char* errmsg = NULL;

	int rc = sqlite3_open("plan.db", &db); //plan.db 열기

	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}



	sqlite3_prepare_v2(db, "select * from CATEGORY_COLOR", -1, &stmt, NULL);
	//sqlite select 구문을 사용하여 CATEGORY_COLOR 테이블에서 데이터 모두 가져오기


	//category_color 테이블에 값이 들어 있는지 확인하는 변수 
	CString category = _T("");

	while (sqlite3_step(stmt) != SQLITE_DONE) {		

		char szAnsi[300];
		UTF8ToAnsi((char*)sqlite3_column_text(stmt, 0), szAnsi, 300);
		category = szAnsi;
		

	}

	sqlite3_finalize(stmt);

	sqlite3_close(db); //plan.db 열기


	//값을 없을 경우 FALSE 반환
	if (category == _T("")) {
		return FALSE;
	}
	//값이 있을 경우 TRUE 반환
	else {
		return TRUE;
	}

}

//CATEGORY_COLOR 테이블의 데이터 이름 변경
void Mytable::change_category_name_db(CString old_category, CString new_category) {

	sqlite3* db;
	int rc = sqlite3_open("plan.db", &db); //plan.db 열기


	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}


	//old_category을 sql 명령문으로 바꾸기
	char* s_old_category;
	int sLen = WideCharToMultiByte(CP_ACP, 0, old_category, -1, NULL, 0, NULL, NULL);
	s_old_category = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, old_category, -1, s_old_category, sLen, NULL, NULL);

	char sz_old_category[100];
	AnsiToUTF8(s_old_category, sz_old_category, 100);

	delete[]s_old_category;

	//new_category를 sql 명령문으로 바꾸기
	char* s_new_category;
	sLen = WideCharToMultiByte(CP_ACP, 0, new_category, -1, NULL, 0, NULL, NULL);
	s_new_category = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, new_category, -1, s_new_category, sLen, NULL, NULL);

	char sz_new_category[100];
	AnsiToUTF8(s_new_category, sz_new_category, 100);

	delete[]s_new_category;







	char* errmsg = NULL;
	char sql[3000] = { 0 };
	sprintf(sql, "UPDATE CATEGORY_COLOR SET CATEGORY = '%s' where CATEGORY = '%s'", sz_new_category, sz_old_category); //새로운 분류명으로 변경
	//sqlite update 구분을 이용하여 CATEGORY_COLOR 테이블에서 분류명 바꾸기


	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg))
	{
		printf("update");
	}



	sqlite3_close(db); //plan.db 파일 닫기

}

//CATEGORY_COLOR 테이블의 데이터 색상 변경
void Mytable::change_category_color_db(CString category, int red, int green, int blue) {

	sqlite3* db;
	int rc = sqlite3_open("plan.db", &db); //plan.db 열기


	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}


	//old_category을 sql 명령문으로 바꾸기
	char* s_category;
	int sLen = WideCharToMultiByte(CP_ACP, 0, category, -1, NULL, 0, NULL, NULL);
	s_category = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, category, -1, s_category, sLen, NULL, NULL);

	char sz_category[100];
	AnsiToUTF8(s_category, sz_category, 100);

	delete[]s_category;






	char* errmsg = NULL;
	char sql[3000] = { 0 };
	sprintf(sql, "update CATEGORY_COLOR set RED = %d, GREEN = %d, BLUE = %d where CATEGORY = '%s'", red, green, blue, sz_category); //분류의 색생 r, g, b 값 변경
	//sqlite update 구분을 이용하여 CATEGORY_COLOR 테이블에서 색상 rgb 값 바꾸기


	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg))
	{
		printf("update");
	}



	sqlite3_close(db); //plan.db 파일 닫기

}

//CATEGORY_COLOR 테이블의 데이터 삭제
void Mytable::delete_category_db(CString category) {

	sqlite3* db;
	int rc = sqlite3_open("plan.db", &db); //plan.db 파일 열기


	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}


	//old_category을 sql 명령문으로 바꾸기
	char* s_category;
	int sLen = WideCharToMultiByte(CP_ACP, 0, category, -1, NULL, 0, NULL, NULL);
	s_category = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, category, -1, s_category, sLen, NULL, NULL);

	char sz_category[100];
	AnsiToUTF8(s_category, sz_category, 100);

	delete[]s_category;






	char* errmsg = NULL;
	char sql[3000] = { 0 };
	sprintf(sql, "delete from CATEGORY_COLOR where category = '%s'", sz_category); //분류 삭제하기
	//sqlite delete 구문을 사용하여 CATEORY_COLOR 테이블에서 분류 삭제


	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg))
	{
		printf("update");
	}



	sqlite3_close(db); //plan.db 파일 닫기
}

//차트 그리는 기능
void Mytable::make_chart(CWnd*pWnd, CRect rect, vector<Myplan> v_plan) {

	//분류명과 시간을 저장할 새로운 벡터배열 생성
	vector<pair<CString, int>> chart_data_today;

	//분류명 입력
	for (int i = 0; i < category_color.size(); i++) {
		chart_data_today.push_back(make_pair(category_color[i].first, 0)); //분류명과 시간은 0으로 초기화하여 배열 생성
	}

	//분류별 총 시간을 분 단위로 입력
	for (int i = 0; i < v_plan.size(); i++) {
		for (int j = 0; j < chart_data_today.size(); j++) {
			if (v_plan[i].category == chart_data_today[j].first) {
				int total_min;
				int s_hour_i;
				int s_min_i;
				int e_hour_i;
				int e_min_i;
				s_hour_i = _ttoi(v_plan[i].s_time.Left(2));
				s_min_i = _ttoi(v_plan[i].s_time.Right(2));
				e_hour_i = _ttoi(v_plan[i].e_time.Left(2));
				e_min_i = _ttoi(v_plan[i].e_time.Right(2));
				total_min = ((e_hour_i - s_hour_i) * 60) + e_min_i - s_min_i; //계획의 시간을 분단위로 계산

				chart_data_today[j].second += total_min; //chart_data_today 배열에 분류의 총 시간 저장
			}
		}

	}

	//분류의 시간 값이 0이면 차트 출력하지 않기 위해 배열에서 삭제
	for (int i = 0; i < chart_data_today.size(); i++) {
		if (chart_data_today[i].second == 0) {
			chart_data_today.erase(std::remove(chart_data_today.begin(), chart_data_today.end(), make_pair(chart_data_today[i].first, chart_data_today[i].second)));
			i--;
		}
	}

	//차트에 시간을 비율로 출력하기위해 총시간을 얻는다.
	int category_total_min = 0; //모든 계획의 총 시간을 0으로 초기화
	for (int i = 0; i < chart_data_today.size(); i++) {
		category_total_min += chart_data_today[i].second; //총 시간을 분단위도 저장
	}

	CClientDC dc(pWnd);



   //막대 그래프 그리기
	if (category_total_min > 0) {


		//차트 양 옆 간격
		int w_side = 40;

		//차트 위 아래 간격
		int h_side = 40;

		int chart_width = rect.Width() - (w_side * 2);
		int chart_height = rect.Height() - (h_side * 2);

		int gap = 3; //그래프의 막대 사이 간격을 3으로 설정

		//막대의 폭을 차트를 그릴 영역에서 간격을 빼준 값을 전체 막대의 개수로 나누어 설정
		int bar_width = (chart_width - (gap * chart_data_today.size())) / chart_data_today.size();

		//세로축 그리기
		dc.MoveTo(w_side, h_side - 10);
		dc.LineTo(w_side, rect.Height() - h_side);
		//가로축 그리기
		dc.MoveTo(w_side, rect.Height() - h_side);
		dc.LineTo(rect.Width() - w_side, rect.Height() - h_side);

		//y축 단위 출력
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(0, 0, _T("(분)"));

		//y축 선, 내용 출력
		for (int i = 0; i < 5; i++) {
			//y축 선 그리기
			dc.MoveTo(w_side, h_side + (chart_height / 4) * i);
			dc.LineTo(w_side + chart_width, h_side + (chart_height / 4) * i);

			//y축 값 계산
			int i_y_value = category_total_min * double((4.0 - i) / 4.0);
			CString y_value;
			y_value.Format(_T("%d"), i_y_value);

			//y축 값 출력
			dc.TextOut(0, h_side + (chart_height / 4) * i, y_value);
		}


		//막대 시작 x 위치 설정
		int bar_x = w_side + gap;


		//막데 색상 선택
		for (int i = 0; i < chart_data_today.size(); i++) {
			for (int j = 0; j < category_color.size(); j++) {
				if (chart_data_today[i].first == category_color[j].first) {
					CBrush brush;
					brush.CreateSolidBrush(category_color[j].second);
					dc.SelectObject(&brush);

					//막대 그리기
					dc.Rectangle(bar_x, h_side + chart_height * (category_total_min - chart_data_today[i].second) / category_total_min, bar_x + bar_width, h_side + chart_height);

				}
			}
			

			
			//글씨 설정
			CFont font;
			font.CreatePointFont(75, _T("굴림"));
			dc.SelectObject(&font);

			//y축 값 출력
			CString value;
			value.Format(_T("%d"), chart_data_today[i].second);
			dc.TextOut(bar_x, h_side + chart_height * (category_total_min - chart_data_today[i].second) / category_total_min - 10, value);


			//x축 분류명 출력
			dc.TextOut(bar_x, (rect.Height() - h_side), chart_data_today[i].first);

			//다음 막대 출력을 위한 x좌표 설정
			bar_x += gap + bar_width;
		}
	}
	
}