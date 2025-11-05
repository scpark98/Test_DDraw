
// Test_DDrawDlg.h: 헤더 파일
//

#pragma once

#include "MyBitmap.h"

#include "Common/ResizeCtrl.h"
#include "Common/SCGdiplusBitmap.h"

#include "Common/directx/CSCD2Context/SCD2Context.h"
#include "Common/directx/CSCD2Image/SCD2Image.h"

// CTestDDrawDlg 대화 상자
class CTestDDrawDlg : public CDialogEx
{
// 생성입니다.
public:
	CTestDDrawDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CSCD2Context				m_d2dc;
	CSCD2Image					m_d2back;
	CSCD2Image					m_d2img2;
	CSCD2Image					m_d2gif;
	CSCD2Image					m_d2gif2;
	CSCD2Image					m_d2webp;
	CSCD2Image					m_d2raw;

	CSCD2Image					m_d2memDC;	//CMemoryDC에 그려진 결과를 CSCD2Image로 전환
	void						create_image_from_memory();


	CResizeCtrl					m_resize;
	CSCGdiplusBitmap			m_img;
	CString						m_filename;

	CRect						m_r_random;

	LRESULT						on_message_from_CSCD2Image(WPARAM wParam, LPARAM lParam);

	void Update();
	HRESULT OnRender();
	void OnResize(UINT width, UINT height);
	HRESULT test_render();

	enum TIMER_ID
	{
		timer_test = 0,
	};


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_DDRAW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic m_static_img;
	CStatic m_static_img2;
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonInvalidate();
	afx_msg void OnBnClickedButtonInvalidateStop();
	afx_msg void OnBnClickedButtonChangeBack();
	afx_msg void OnBnClickedCheckGif();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
