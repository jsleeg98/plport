#pragma once
#include <iostream>
#include "Mytable.h"
#include "Myplan.h"
#include <vector>

// Week 대화 상자

class Week : public CDialogEx
{
	DECLARE_DYNAMIC(Week)

public:
	Week(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Week();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEEK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_plan_list; //리스트컨트롤 멤버 변수
	CEdit m_s_time; //시작 시각 에디트 컨트롤 멤버 변수
	CEdit m_e_time; //마침 시각 에디트 컨트롤 멤버 변수
	CEdit m_content; //내용 에디트 컨트롤 멤버 변수
	CComboBox m_category; //분류 콤보박스 멤버 변수
	
	
	CMonthCalCtrl m_monthcalendar; //달력 멤버 변수

	//Static text 폰트 설정
	CFont m_static_font;
	
	//일주일 날짜 저장
	CString week_array[7];

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedAddBtn();
	afx_msg void OnMcnSelchangeMonthcalendar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedDelBtn();
	afx_msg void OnNMClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedTestBtn();
};
