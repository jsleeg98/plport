#pragma once
#include "Mytable.h"
#include "Myplan.h"
#include <vector>


// Setup 대화 상자

class Setup : public CDialogEx
{
	DECLARE_DYNAMIC(Setup)

public:
	Setup(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Setup(); 

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETUP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedColorSelBtn();
	CListCtrl m_category_list;
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();

	COLORREF color; //색상선택에서 선택한 색상

	CButton m_add_btn;
	CButton m_del_btn;
	CButton m_category_btn;
	CButton m_color_btn;

	int i_red; //선택된 분류 색상 R 값
	int i_green; //선택된 분류 색상 G 값
	int i_blue; //선택된 분류 색상 B 값

	bool check_color; //색상 선택 여부 확인 



	afx_msg void OnNMClickCategoryColorList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCategoryAddBtn(); 
	afx_msg void OnBnClickedAddRadio();
	afx_msg void OnBnClickedDelRadio();
	afx_msg void OnBnClickedModifyColorBtn();
	afx_msg void OnBnClickedModifyCategoryBtn();
	afx_msg void OnBnClickedCategoryDelBtn();
	afx_msg void OnBnClickedCategoryChangeRadio();
	afx_msg void OnBnClickedColorChangeRadio();
	
};
