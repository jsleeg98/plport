#pragma once


// Chart 대화 상자

class Chart : public CDialogEx
{
	DECLARE_DYNAMIC(Chart)

public:
	Chart(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Chart();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHART_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	CString week_array_today[7];

	afx_msg void OnBnClickedTestBtn();
	virtual BOOL OnInitDialog();
	CMonthCalCtrl m_this_week_monthcalendar;
	afx_msg void OnPaint();
	CMonthCalCtrl m_before_2_week_monthcalendar;
	CMonthCalCtrl m_before_1_week_monthcalendar;
	afx_msg void OnMcnSelchangeThisWeekMonthcalendar(NMHDR* pNMHDR, LRESULT* pResult);
	CTime this_week;
	CTime before_1;
	CTime before_2;
	afx_msg void OnBnClickedChart1();
	afx_msg void OnBnClickedChart2();
	afx_msg void OnBnClickedChart3();


	//어떤 차트 라디오 버튼이 선택되어있는지 확인하기 위한 변수
	bool chart_1_flag; 
	bool chart_2_flag;
	bool chart_3_flag;
	
};
