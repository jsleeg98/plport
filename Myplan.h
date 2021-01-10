//MYPLAN 클래스 나민수 구현

#pragma once
#include <iostream>
#include "pch.h"
#include <vector>

using namespace std;

class Myplan {
public:
	CString date; //날짜 - 형식 20201214
	CString s_time; //시작 시각 - 형식 1300
	CString e_time; //마침 시각 - 형식 1600
	CString content; //내용
	CString category; //분류
	CString plan_id; //sqlite 파일에 들어갈 기본키 - 형식 2020121413001600

	Myplan();
	int AnsiToUTF8(char* szSrc, char* strDest, int destSize); //인터넷에서 가져온 함수 //sqlite 한글을 저장하기 위한 변환 함수 
	int UTF8ToAnsi(char* szSrc, char* strDest, int destSize); //인터넷에서 가져온 함수 //sqlite 데이터를 내부에서 처리하기 위한 변환 함수 
	void insert(CString date, CString s_time, CString e_time, CString category, CString content); //객체 변수 저장
	void make_sqlite(); //plan.db 파일을 생성하며 PLANDATA 테이블을 생성
	void save_db(); //멤버 변수에 저장된 값들을 plan.db의 PLANDATA 테이블에 저장
	void delete_db(CString id); //id 값을 토대로 plan.db의 PLANDATA 테이블에서 데이터 삭제
	void get_plan(CString date, vector<Myplan>* v_plan); //plan.db의 PLANDATA 테이블에서 인자로 들어온 date에 해당하는 데이터를 받아 v_plan라는 백터 배열에 저장
	void get_week_array(CString week_array[], CTime time); //해당 날짜 기준 월요일부터 일요일까지 날짜를 배열에 저장
	void change_category_name_db(CString old_category, CString new_category); //plan.db의 PLANDATA 테이블에서 분류명 변경

};

