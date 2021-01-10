#include "pch.h"
#include "Myplan.h"
#include <iostream>
//sqlite3 include----------------------------------------------------------------------------//
#include "sqlite3.h"
#include <assert.h>

#pragma comment(lib, "sqlite3.lib")
//--------------------------------------------------------------------------------------------//

using namespace std;

Myplan::Myplan() {

}

//sqlite������ UTF-8�� text ���������� ����Ǳ� ������ ������ �� ansi���� UTF-8�� ��ȯ�� �ʿ�
int Myplan::AnsiToUTF8(char* szSrc, char* strDest, int destSize)
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

//UTF-8 ������ ���� ansi ������������ �޾ƿͼ� ���α׷� ������ ó���� �����ϵ��� ��ȯ�� �ʿ�
int Myplan::UTF8ToAnsi(char* szSrc, char* strDest, int destSize)
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


//Ŭ���� ���� ����
void Myplan::insert(CString date, CString s_time, CString e_time, CString category, CString content) {
	this->date = date;
	this->s_time = s_time;
	this->e_time = e_time;
	this->category = category;
	this->content = content;
}


//plan.db ������ �����ϸ� PLANDATA ���̺��� ����
void Myplan::make_sqlite() {

	sqlite3* db;
	sqlite3_stmt* stmt = NULL;
	char* errmsg = NULL;

	int rc = sqlite3_open("plan.db", &db);

	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}

	//sql ���̺� ����
	char* sql;
	sql = "CREATE TABLE IF NOT EXISTS PLANDATA("
		"ID		TEXT	 PRIMARY KEY ,"
		"DATE		TEXT	NOT NULL,"
		"S_TIME		TEXT	NOT NULL,"
		"E_TIME		TEXT	NOT NULL,"
		"CATEGORY	TEXT	NOT NULL,"
		"CONTENT	TEXT	NOT NULL);";

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

//��� ������ ����� ������ plan.db�� PLANDATA ���̺� ����
void Myplan::save_db() {

	sqlite3* db;
	int rc = sqlite3_open("plan.db", &db); //plan.db ����


	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}



	//date sql �����ͷ� �ٲ��ֱ�
	char* s_date;
	int sLen = WideCharToMultiByte(CP_ACP, 0, date, -1, NULL, 0, NULL, NULL);
	s_date = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, date, -1, s_date, sLen, NULL, NULL);

	char sz_date[100];
	AnsiToUTF8(s_date, sz_date, 100);

	delete[]s_date;
	
	//s_time sql �����ͷ� �ٲ��ֱ�
	char* t_s_time;
	sLen = WideCharToMultiByte(CP_ACP, 0, s_time, -1, NULL, 0, NULL, NULL);
	t_s_time = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, s_time, -1, t_s_time, sLen, NULL, NULL);

	char sz_s_time[100];
	AnsiToUTF8(t_s_time, sz_s_time, 100);

	delete[]t_s_time;

	//e_time sql �����ͷ� �ٲ��ֱ�
	char* t_e_time;
	sLen = WideCharToMultiByte(CP_ACP, 0, e_time, -1, NULL, 0, NULL, NULL);
	t_e_time = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, e_time, -1, t_e_time, sLen, NULL, NULL);

	char sz_e_time[100];
	AnsiToUTF8(t_e_time, sz_e_time, 100);

	delete[]t_e_time;

	//category sql �����ͷ� �ٲ��ֱ�
	char* s_category;
	sLen = WideCharToMultiByte(CP_ACP, 0, category, -1, NULL, 0, NULL, NULL);
	s_category = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, category, -1, s_category, sLen, NULL, NULL);

	char sz_category[100];
	AnsiToUTF8(s_category, sz_category, 100);

	delete[]s_category;

	//content sql �����ͷ� �ٲ��ֱ�
	char* s_content;
	sLen = WideCharToMultiByte(CP_ACP, 0, content, -1, NULL, 0, NULL, NULL);
	s_content = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, content, -1, s_content, sLen, NULL, NULL);

	char sz_content[100];
	AnsiToUTF8(s_content, sz_content, 100);

	delete[]s_content;

	
	//plan_id sql �����ͷ� �ٲ��ֱ�
	CString plan_id = date + s_time + e_time;
	char* s_plan_id;
	sLen = WideCharToMultiByte(CP_ACP, 0, plan_id, -1, NULL, 0, NULL, NULL);
	s_plan_id = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, plan_id, -1, s_plan_id, sLen, NULL, NULL);

	char sz_plan_id[100];
	AnsiToUTF8(s_plan_id, sz_plan_id, 100);

	delete[]s_plan_id;
	

	char* errmsg = NULL;
	char sql[3000] = { 0 };
	sprintf(sql, "insert into PLANDATA(ID, DATE, S_TIME, E_TIME, CATEGORY, CONTENT) values('%s','%s','%s','%s','%s','%s');", sz_plan_id, sz_date, sz_s_time, sz_e_time, sz_category, sz_content);
	


	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg))
	{
		printf("insert");
	}



	sqlite3_close(db);

}

//id ���� ���� plan.db�� PLANDATA ���̺��� ������ ����
void Myplan::delete_db(CString id) {

	//sqlite3 ����
	char* s_id;

	int sLen = WideCharToMultiByte(CP_ACP, 0, id, -1, NULL, 0, NULL, NULL);
	s_id = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, id, -1, s_id, sLen, NULL, NULL);

	char sz_id[100];
	AnsiToUTF8(s_id, sz_id, 100);

	delete[]s_id;

	sqlite3* db;
	int rc = sqlite3_open("plan.db", &db);

	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}


	char* errmsg = NULL;
	char sql[255] = { 0 };
	sprintf(sql, "delete from plandata where id = '%s';", sz_id);

	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg))
	{
		printf("delete");
	}



	sqlite3_close(db);


}


//plan.db�� PLANDATA ���̺��� ���ڷ� ���� date�� �ش��ϴ� �����͸� �޾� v_plan��� ���� �迭�� ����
void Myplan::get_plan(CString date, vector<Myplan>* v_plan) {

	CString cmd = _T("select * from PLANDATA where date = ");
	cmd += date;

	
	//sqlite ��ɹ� ����� ���� ��ȯ
	char* s_cmd;
	int sLen = WideCharToMultiByte(CP_ACP, 0, cmd, -1, NULL, 0, NULL, NULL);
	s_cmd = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, cmd, -1, s_cmd, sLen, NULL, NULL);

	char sz_cmd[100];
	AnsiToUTF8(s_cmd, sz_cmd, 100);

	delete[]s_cmd;

	

	sqlite3* db;
	sqlite3_stmt* stmt = NULL;
	char* errmsg = NULL;

	int rc = sqlite3_open("plan.db", &db); //plan.db ����

	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}

	
	
	
	sqlite3_prepare_v2(db, sz_cmd, -1, &stmt, NULL); //sqlite ��ɹ� ���

	


	while (sqlite3_step(stmt) != SQLITE_DONE) {

		char szAnsi[300];
		UTF8ToAnsi((char*)sqlite3_column_text(stmt, 1), szAnsi, 300);
		CString date(szAnsi);

		UTF8ToAnsi((char*)sqlite3_column_text(stmt, 2), szAnsi, 300);
		CString s_time(szAnsi);

		UTF8ToAnsi((char*)sqlite3_column_text(stmt, 3), szAnsi, 300);
		CString e_time(szAnsi);

		UTF8ToAnsi((char*)sqlite3_column_text(stmt, 4), szAnsi, 300);
		CString category(szAnsi);

		UTF8ToAnsi((char*)sqlite3_column_text(stmt, 5), szAnsi, 300);
		CString content(szAnsi);

		Myplan temp;
		temp.insert(date, s_time, e_time, category, content); //�ӽ� ��ü�� ������ ����

		v_plan->push_back(temp); //���� �迭�� ����


	}

	

	sqlite3_finalize(stmt);

	sqlite3_close(db);


}

//�ش� ��¥ ���� �����Ϻ��� �Ͽ��ϱ��� ��¥�� �迭�� ����
void Myplan::get_week_array(CString week_array[], CTime time) {

	// 1 = �Ͽ���, 2 = ��
	if (time.GetDayOfWeek() == 2) { //������ ���
		for (int i = 0; i < 7; i++) {
			CTime monday_date = time + CTimeSpan(i, 0, 0, 0);

			CString date_return; //���Ŀ� �´� ���� ������ ���� ���� // ���� 20201214
			CString year; //�⵵ ���� ����
			CString month; //�� ���� ����
			CString day; //�� ���� ����

			year.Format(_T("%d"), monday_date.GetYear());

			if (monday_date.GetMonth() < 10) { //���� 10���� ���� ��� ������ �����ֱ� ���ؼ� �տ� 0�� �ٿ� ���ڸ� �������� �����.
				month.Format(_T("0%d"), monday_date.GetMonth()); 
			}
			else {
				month.Format(_T("%d"), monday_date.GetMonth());
			}

			if (monday_date.GetDay() < 10) { //���� 10���� ���� ��� ������ �����ֱ� ���ؼ� �տ� 0�� �ٿ� ���ڸ� �������� �����.
				day.Format(_T("0%d"), monday_date.GetDay());
			}
			else {
				day.Format(_T("%d"), monday_date.GetDay());
			}

			//��¥ ������ ����
			date_return = year + month + day; 
			week_array[i] = date_return;
		}
	}
	else if (time.GetDayOfWeek() == 1) { //�Ͽ��� ���
		for (int i = 0; i < 7; i++) {
			CTime monday_date = time - CTimeSpan(6-i, 0, 0, 0); //6�� ���� ������
			

			CString date_return;
			CString year;
			CString month;
			CString day;

			year.Format(_T("%d"), monday_date.GetYear());

			if (monday_date.GetMonth() < 10) {
				month.Format(_T("0%d"), monday_date.GetMonth());
			}
			else {
				month.Format(_T("%d"), monday_date.GetMonth());
			}

			if (monday_date.GetDay() < 10) {
				day.Format(_T("0%d"), monday_date.GetDay());
			}
			else {
				day.Format(_T("%d"), monday_date.GetDay());
			}

			//��¥ ������ ����
			date_return = year + month + day;
			week_array[i] = date_return;
		}
	}
	else {
		for (int i = 0; i < 7; i++) { //ȭ~��
			CTime monday_date = time - CTimeSpan(time.GetDayOfWeek() - 2 - i, 0, 0, 0); 
			

			CString date_return;
			CString year;
			CString month;
			CString day;

			year.Format(_T("%d"), monday_date.GetYear());

			if (monday_date.GetMonth() < 10) {
				month.Format(_T("0%d"), monday_date.GetMonth());
			}
			else {
				month.Format(_T("%d"), monday_date.GetMonth());
			}

			if (monday_date.GetDay() < 10) {
				day.Format(_T("0%d"), monday_date.GetDay());
			}
			else {
				day.Format(_T("%d"), monday_date.GetDay());
			}

			//��¥ ������ ����
			date_return = year + month + day;
			week_array[i] = date_return;
		}
	}

}

void Myplan::change_category_name_db(CString old_category, CString new_category) {

	sqlite3* db;
	int rc = sqlite3_open("plan.db", &db); //plan.db ����


	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}


	//old_category�� sql ��ɹ����� �ٲٱ�
	char* s_old_category;
	int sLen = WideCharToMultiByte(CP_ACP, 0, old_category, -1, NULL, 0, NULL, NULL);
	s_old_category = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, old_category, -1, s_old_category, sLen, NULL, NULL);

	char sz_old_category[100];
	AnsiToUTF8(s_old_category, sz_old_category, 100);

	delete[]s_old_category;

	//new_category�� sql ��ɹ����� �ٲٱ�
	char* s_new_category;
	sLen = WideCharToMultiByte(CP_ACP, 0, new_category, -1, NULL, 0, NULL, NULL);
	s_new_category = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, new_category, -1, s_new_category, sLen, NULL, NULL);

	char sz_new_category[100];
	AnsiToUTF8(s_new_category, sz_new_category, 100);

	delete[]s_new_category;







	char* errmsg = NULL;
	char sql[3000] = { 0 };
	sprintf(sql, "UPDATE PLANDATA SET CATEGORY = '%s' where CATEGORY = '%s'", sz_new_category, sz_old_category); //���ο� �з������� ����



	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg))
	{
		printf("update");
	}



	sqlite3_close(db); //plan.db ���� �ݱ�

}