#pragma once

#include "pch.h"
//sqlite3 include----------------------------------------------------------------------------//
#include "sqlite3.h"
#include <assert.h>

#pragma comment(lib, "sqlite3.lib")
//--------------------------------------------------------------------------------------------//
#include <iostream>
#include "Myplan.h"
#include <map>
#include <vector>

using namespace std;

class Mytable {
public:
	vector<pair<CString, COLORREF>> category_color; //분류 이름과 색상을 저장하는 배열

public:
	Mytable();
	int AnsiToUTF8(char* szSrc, char* strDest, int destSize);  //인터넷에서 가져온 함수 //sqlite 한글을 저장하기 위한 변환 함수 
	int UTF8ToAnsi(char* szSrc, char* strDest, int destSize);  //인터넷에서 가져온 함수 //sqlite 데이터를 내부에서 처리하기 위한 변환 함수 
	void make_table(CWnd* pWnd, int l_t_x, int l_t_y, int r_b_x, int r_b_y);  //주간계획표의 틀을 출력하는 함수
	void make_timebox(CWnd* pWnd, int week_number, Myplan plan); //주간계획표 내부에 timebox를 출력하는 함수
	void make_sqlite_category(); //plan.db에 CATEGORY_COLOR 테이블을 생성
	void insert_category_color(CString category, int red, int green, int blue); //CATEGORY_COLOR 테이블에 데이터 추가
	void take_category_db(); //plan.db의 CATEGORY_COLOR 테이블의 데이터 가져오기
	void change_category_color_db(CString category, int red, int green, int blue); //CATEGORY_COLOR 테이블의 데이터 색상 변경
	void change_category_name_db(CString old_category, CString new_category); //CATEGORY_COLOR 테이블의 데이터 이름 변경
	void delete_category_db(CString category); //CATEGORY_COLOR 테이블의 데이터 삭제
	bool check_category_db(); //CATEGORY_COLOR 테이블에 데이터의 유무 확인
	void make_chart(CWnd * pWnd, CRect rect, vector<Myplan> v_plan); //차트 그리는 기능
};