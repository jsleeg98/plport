//�̵��� MYTABLE Ŭ���� ����

#include "pch.h"
#include <iostream>
#include "Mytable.h"

Mytable::Mytable() {
	
}

//���ͳݿ��� ������ �Լ� //sqlite �ѱ��� �����ϱ� ���� ��ȯ �Լ� 
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

//���ͳݿ��� ������ �Լ� //sqlite �����͸� ���ο��� ó���ϱ� ���� ��ȯ �Լ� 
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

//�ְ���ȹǥ�� Ʋ�� ����ϴ� �Լ�
void Mytable::make_table(CWnd* pWnd, int l_t_x, int l_t_y, int r_b_x, int r_b_y) {

	CClientDC dc(pWnd);

	//�ܰ� Ʋ �׸���
	dc.Rectangle(l_t_x, l_t_y, r_b_x, r_b_y);


	//���� ĭ ������
	for (int i = 1; i < 7; i++) { // ���� �� �ʺ� 100���� ����
		dc.MoveTo(l_t_x + 100 * i, l_t_y);
		dc.LineTo(l_t_x + 100 * i, r_b_y);
	}

	//�ð� ĭ ������
	CPen pen;
	pen.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	dc.SelectObject(&pen);
	for (int i = 1; i < 24; i++) { //1�ð� �� ���� 30���� ����
		dc.MoveTo(l_t_x, l_t_y + 30 * i);
		dc.LineTo(r_b_x, l_t_y + 30 * i);
	}

	//���� �ؽ�Ʈ ���
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(90 , 50, _T("������"));
	dc.TextOut(90 + 100, 50, _T("ȭ����"));
	dc.TextOut(90 + 200, 50, _T("������"));
	dc.TextOut(90 + 300, 50, _T("�����"));
	dc.TextOut(90 + 400, 50, _T("�ݿ���"));
	dc.TextOut(90 + 500, 50, _T("�����"));
	dc.TextOut(90 + 600, 50, _T("�Ͽ���"));

	//�ð� �ؽ�Ʈ ���
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

//�ְ���ȹǥ ���ο� timebox�� ����ϴ� �Լ�
void Mytable::make_timebox(CWnd* pWnd, int week_number, Myplan plan) {

	CClientDC dc(pWnd);

	CString s_time = plan.s_time; //���۽ð� �޾ƿ���
	CString e_time = plan.e_time; //��ħ�ð� �޾ƿ���

	int top_left_x;
	double top_left_y;
	int btm_right_x;
	double btm_right_y;

	//�ð� �����͸� ���������� ��, ������ ������ �ޱ�
	int s_time_h = _ttoi(s_time.Left(2)); //���۽ð� ��
	int s_time_m = _ttoi(s_time.Right(2)); //���۽ð� ��
	int e_time_h = _ttoi(e_time.Left(2)); //��ħ�ð� ��
	int e_time_m = _ttoi(e_time.Right(2)); //��ħ�ð� ��

	//�簢���� �׸� ��ǥ ����// �Ϸ� x = 100, 1�ð� = 30 
	top_left_x = 70 + (100 * week_number); //���Ͽ� ���� x�� ��ġ ����
	top_left_y = double(70.0 + (30.0 * s_time_h) + (30.0 * double(s_time_m) / 60.0)); //���۽ð��� ���� ���� �� y�� ���� �ѽð��� 30�ȼ�, 30���� 15�ȼ�
	btm_right_x = 70 + (100 * (1 + week_number)); 
	btm_right_y = double(70.0 + (30.0 * e_time_h) + (30.0 * double(e_time_m) / 60.0)); //��ħ�ð��� ���� ������ �Ʒ� y�� ����, �ѽð��� 30�ȼ�, 30���� 15�ȼ�


	CBrush brush;
	CBrush* poldbrush = NULL;
	for (int i = 0; i < category_color.size();i++) {
		if (plan.category == category_color[i].first) { //�з����� ���� �� Ȯ��
			brush.CreateSolidBrush(category_color[i].second); //cateogory_color ���� ������ Ȱ���Ͽ� �귯�� ����
			poldbrush= dc.SelectObject(&brush); //�귯�� ����
		}
	}

	//��Ʈ ����
	CFont font;
	font.CreateFont(15,0, 0,0, FW_EXTRABOLD,  FALSE, FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH, _T("����ü"));

	//timebox �׸���
	CRect timebox(top_left_x, top_left_y, btm_right_x, btm_right_y);
	dc.Rectangle(timebox); //timebox �׸���
	dc.SelectObject(poldbrush); //�귯�� ���� ������� �ǵ����ֱ�
	dc.SelectObject(font); //��Ʈ ����
	dc.SetBkMode(TRANSPARENT); // �ؽ�Ʈ ��� ����ȭ
	dc.DrawText(plan.content, timebox, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //content ����
	
}

//plan.db�� CATEGORY_COLOR ���̺��� ����
void Mytable::make_sqlite_category() {

	sqlite3* db;
	sqlite3_stmt* stmt = NULL;
	char* errmsg = NULL;

	int rc = sqlite3_open("plan.db", &db); //plan.db ���� ����

	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}

	//plan.db�� CATEGORY_COLOR ���̺� ����
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



//CATEGORY_COLOR ���̺� ������ �߰�
void Mytable::insert_category_color(CString category, int red, int green, int blue) {

	sqlite3* db;
	int rc = sqlite3_open("plan.db", &db); //plan.db ����


	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}

	
	//category sql �����ͷ� �ٲ��ֱ�
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
	//sqlite insert ��ɾ ����Ͽ� CATEGORY_COLOR ���̺� ������ ����


	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg))
	{
		printf("insert");
	}



	sqlite3_close(db); //plan.db ���� �ݱ�

}

//plan.db�� CATEGORY_COLOR ���̺��� ������ ��������
void Mytable::take_category_db() {

	sqlite3* db;
	sqlite3_stmt* stmt = NULL;
	char* errmsg = NULL;

	int rc = sqlite3_open("plan.db", &db); //plan.db ���� ����

	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}



	sqlite3_prepare_v2(db, "select * from CATEGORY_COLOR", -1, &stmt, NULL);
	//sqlite select ������ ����Ͽ� CATEGORY_COLOR ���̺��� ������ ��� ��������

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

		category_color.push_back(make_pair(category, rgb)); //category_color ���� �迭�� �߰�
	}


	sqlite3_finalize(stmt);

	sqlite3_close(db);


}

//CATEGORY_COLOR ���̺� �������� ���� Ȯ��
bool Mytable::check_category_db() {
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



	sqlite3_prepare_v2(db, "select * from CATEGORY_COLOR", -1, &stmt, NULL);
	//sqlite select ������ ����Ͽ� CATEGORY_COLOR ���̺��� ������ ��� ��������


	//category_color ���̺� ���� ��� �ִ��� Ȯ���ϴ� ���� 
	CString category = _T("");

	while (sqlite3_step(stmt) != SQLITE_DONE) {		

		char szAnsi[300];
		UTF8ToAnsi((char*)sqlite3_column_text(stmt, 0), szAnsi, 300);
		category = szAnsi;
		

	}

	sqlite3_finalize(stmt);

	sqlite3_close(db); //plan.db ����


	//���� ���� ��� FALSE ��ȯ
	if (category == _T("")) {
		return FALSE;
	}
	//���� ���� ��� TRUE ��ȯ
	else {
		return TRUE;
	}

}

//CATEGORY_COLOR ���̺��� ������ �̸� ����
void Mytable::change_category_name_db(CString old_category, CString new_category) {

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
	sprintf(sql, "UPDATE CATEGORY_COLOR SET CATEGORY = '%s' where CATEGORY = '%s'", sz_new_category, sz_old_category); //���ο� �з������� ����
	//sqlite update ������ �̿��Ͽ� CATEGORY_COLOR ���̺��� �з��� �ٲٱ�


	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg))
	{
		printf("update");
	}



	sqlite3_close(db); //plan.db ���� �ݱ�

}

//CATEGORY_COLOR ���̺��� ������ ���� ����
void Mytable::change_category_color_db(CString category, int red, int green, int blue) {

	sqlite3* db;
	int rc = sqlite3_open("plan.db", &db); //plan.db ����


	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}


	//old_category�� sql ��ɹ����� �ٲٱ�
	char* s_category;
	int sLen = WideCharToMultiByte(CP_ACP, 0, category, -1, NULL, 0, NULL, NULL);
	s_category = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, category, -1, s_category, sLen, NULL, NULL);

	char sz_category[100];
	AnsiToUTF8(s_category, sz_category, 100);

	delete[]s_category;






	char* errmsg = NULL;
	char sql[3000] = { 0 };
	sprintf(sql, "update CATEGORY_COLOR set RED = %d, GREEN = %d, BLUE = %d where CATEGORY = '%s'", red, green, blue, sz_category); //�з��� ���� r, g, b �� ����
	//sqlite update ������ �̿��Ͽ� CATEGORY_COLOR ���̺��� ���� rgb �� �ٲٱ�


	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg))
	{
		printf("update");
	}



	sqlite3_close(db); //plan.db ���� �ݱ�

}

//CATEGORY_COLOR ���̺��� ������ ����
void Mytable::delete_category_db(CString category) {

	sqlite3* db;
	int rc = sqlite3_open("plan.db", &db); //plan.db ���� ����


	if (rc != SQLITE_OK)
	{
		printf("Failed to open DB\n");
		sqlite3_close(db);
		exit(1);
	}


	//old_category�� sql ��ɹ����� �ٲٱ�
	char* s_category;
	int sLen = WideCharToMultiByte(CP_ACP, 0, category, -1, NULL, 0, NULL, NULL);
	s_category = new char[sLen + 1];
	WideCharToMultiByte(CP_ACP, 0, category, -1, s_category, sLen, NULL, NULL);

	char sz_category[100];
	AnsiToUTF8(s_category, sz_category, 100);

	delete[]s_category;






	char* errmsg = NULL;
	char sql[3000] = { 0 };
	sprintf(sql, "delete from CATEGORY_COLOR where category = '%s'", sz_category); //�з� �����ϱ�
	//sqlite delete ������ ����Ͽ� CATEORY_COLOR ���̺��� �з� ����


	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg))
	{
		printf("update");
	}



	sqlite3_close(db); //plan.db ���� �ݱ�
}

//��Ʈ �׸��� ���
void Mytable::make_chart(CWnd*pWnd, CRect rect, vector<Myplan> v_plan) {

	//�з���� �ð��� ������ ���ο� ���͹迭 ����
	vector<pair<CString, int>> chart_data_today;

	//�з��� �Է�
	for (int i = 0; i < category_color.size(); i++) {
		chart_data_today.push_back(make_pair(category_color[i].first, 0)); //�з���� �ð��� 0���� �ʱ�ȭ�Ͽ� �迭 ����
	}

	//�з��� �� �ð��� �� ������ �Է�
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
				total_min = ((e_hour_i - s_hour_i) * 60) + e_min_i - s_min_i; //��ȹ�� �ð��� �д����� ���

				chart_data_today[j].second += total_min; //chart_data_today �迭�� �з��� �� �ð� ����
			}
		}

	}

	//�з��� �ð� ���� 0�̸� ��Ʈ ������� �ʱ� ���� �迭���� ����
	for (int i = 0; i < chart_data_today.size(); i++) {
		if (chart_data_today[i].second == 0) {
			chart_data_today.erase(std::remove(chart_data_today.begin(), chart_data_today.end(), make_pair(chart_data_today[i].first, chart_data_today[i].second)));
			i--;
		}
	}

	//��Ʈ�� �ð��� ������ ����ϱ����� �ѽð��� ��´�.
	int category_total_min = 0; //��� ��ȹ�� �� �ð��� 0���� �ʱ�ȭ
	for (int i = 0; i < chart_data_today.size(); i++) {
		category_total_min += chart_data_today[i].second; //�� �ð��� �д����� ����
	}

	CClientDC dc(pWnd);



   //���� �׷��� �׸���
	if (category_total_min > 0) {


		//��Ʈ �� �� ����
		int w_side = 40;

		//��Ʈ �� �Ʒ� ����
		int h_side = 40;

		int chart_width = rect.Width() - (w_side * 2);
		int chart_height = rect.Height() - (h_side * 2);

		int gap = 3; //�׷����� ���� ���� ������ 3���� ����

		//������ ���� ��Ʈ�� �׸� �������� ������ ���� ���� ��ü ������ ������ ������ ����
		int bar_width = (chart_width - (gap * chart_data_today.size())) / chart_data_today.size();

		//������ �׸���
		dc.MoveTo(w_side, h_side - 10);
		dc.LineTo(w_side, rect.Height() - h_side);
		//������ �׸���
		dc.MoveTo(w_side, rect.Height() - h_side);
		dc.LineTo(rect.Width() - w_side, rect.Height() - h_side);

		//y�� ���� ���
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(0, 0, _T("(��)"));

		//y�� ��, ���� ���
		for (int i = 0; i < 5; i++) {
			//y�� �� �׸���
			dc.MoveTo(w_side, h_side + (chart_height / 4) * i);
			dc.LineTo(w_side + chart_width, h_side + (chart_height / 4) * i);

			//y�� �� ���
			int i_y_value = category_total_min * double((4.0 - i) / 4.0);
			CString y_value;
			y_value.Format(_T("%d"), i_y_value);

			//y�� �� ���
			dc.TextOut(0, h_side + (chart_height / 4) * i, y_value);
		}


		//���� ���� x ��ġ ����
		int bar_x = w_side + gap;


		//���� ���� ����
		for (int i = 0; i < chart_data_today.size(); i++) {
			for (int j = 0; j < category_color.size(); j++) {
				if (chart_data_today[i].first == category_color[j].first) {
					CBrush brush;
					brush.CreateSolidBrush(category_color[j].second);
					dc.SelectObject(&brush);

					//���� �׸���
					dc.Rectangle(bar_x, h_side + chart_height * (category_total_min - chart_data_today[i].second) / category_total_min, bar_x + bar_width, h_side + chart_height);

				}
			}
			

			
			//�۾� ����
			CFont font;
			font.CreatePointFont(75, _T("����"));
			dc.SelectObject(&font);

			//y�� �� ���
			CString value;
			value.Format(_T("%d"), chart_data_today[i].second);
			dc.TextOut(bar_x, h_side + chart_height * (category_total_min - chart_data_today[i].second) / category_total_min - 10, value);


			//x�� �з��� ���
			dc.TextOut(bar_x, (rect.Height() - h_side), chart_data_today[i].first);

			//���� ���� ����� ���� x��ǥ ����
			bar_x += gap + bar_width;
		}
	}
	
}