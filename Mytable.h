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
	vector<pair<CString, COLORREF>> category_color; //�з� �̸��� ������ �����ϴ� �迭

public:
	Mytable();
	int AnsiToUTF8(char* szSrc, char* strDest, int destSize);  //���ͳݿ��� ������ �Լ� //sqlite �ѱ��� �����ϱ� ���� ��ȯ �Լ� 
	int UTF8ToAnsi(char* szSrc, char* strDest, int destSize);  //���ͳݿ��� ������ �Լ� //sqlite �����͸� ���ο��� ó���ϱ� ���� ��ȯ �Լ� 
	void make_table(CWnd* pWnd, int l_t_x, int l_t_y, int r_b_x, int r_b_y);  //�ְ���ȹǥ�� Ʋ�� ����ϴ� �Լ�
	void make_timebox(CWnd* pWnd, int week_number, Myplan plan); //�ְ���ȹǥ ���ο� timebox�� ����ϴ� �Լ�
	void make_sqlite_category(); //plan.db�� CATEGORY_COLOR ���̺��� ����
	void insert_category_color(CString category, int red, int green, int blue); //CATEGORY_COLOR ���̺� ������ �߰�
	void take_category_db(); //plan.db�� CATEGORY_COLOR ���̺��� ������ ��������
	void change_category_color_db(CString category, int red, int green, int blue); //CATEGORY_COLOR ���̺��� ������ ���� ����
	void change_category_name_db(CString old_category, CString new_category); //CATEGORY_COLOR ���̺��� ������ �̸� ����
	void delete_category_db(CString category); //CATEGORY_COLOR ���̺��� ������ ����
	bool check_category_db(); //CATEGORY_COLOR ���̺� �������� ���� Ȯ��
	void make_chart(CWnd * pWnd, CRect rect, vector<Myplan> v_plan); //��Ʈ �׸��� ���
};