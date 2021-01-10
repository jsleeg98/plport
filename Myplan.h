//MYPLAN Ŭ���� ���μ� ����

#pragma once
#include <iostream>
#include "pch.h"
#include <vector>

using namespace std;

class Myplan {
public:
	CString date; //��¥ - ���� 20201214
	CString s_time; //���� �ð� - ���� 1300
	CString e_time; //��ħ �ð� - ���� 1600
	CString content; //����
	CString category; //�з�
	CString plan_id; //sqlite ���Ͽ� �� �⺻Ű - ���� 2020121413001600

	Myplan();
	int AnsiToUTF8(char* szSrc, char* strDest, int destSize); //���ͳݿ��� ������ �Լ� //sqlite �ѱ��� �����ϱ� ���� ��ȯ �Լ� 
	int UTF8ToAnsi(char* szSrc, char* strDest, int destSize); //���ͳݿ��� ������ �Լ� //sqlite �����͸� ���ο��� ó���ϱ� ���� ��ȯ �Լ� 
	void insert(CString date, CString s_time, CString e_time, CString category, CString content); //��ü ���� ����
	void make_sqlite(); //plan.db ������ �����ϸ� PLANDATA ���̺��� ����
	void save_db(); //��� ������ ����� ������ plan.db�� PLANDATA ���̺� ����
	void delete_db(CString id); //id ���� ���� plan.db�� PLANDATA ���̺��� ������ ����
	void get_plan(CString date, vector<Myplan>* v_plan); //plan.db�� PLANDATA ���̺��� ���ڷ� ���� date�� �ش��ϴ� �����͸� �޾� v_plan��� ���� �迭�� ����
	void get_week_array(CString week_array[], CTime time); //�ش� ��¥ ���� �����Ϻ��� �Ͽ��ϱ��� ��¥�� �迭�� ����
	void change_category_name_db(CString old_category, CString new_category); //plan.db�� PLANDATA ���̺��� �з��� ����

};

