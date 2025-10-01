
// Test_DDrawDlg.h: 헤더 파일
//

#pragma once

#include <d2d1_1.h>
#include <d2d1helper.h>
#include <d2d1effects_2.h>

#include <dwrite.h>
#include <wincodec.h>
#include <d3d11.h>
#include <wrl/client.h>

#include "MyBitmap.h"

using namespace Microsoft::WRL;

#include "Common/ResizeCtrl.h"
#include "Common/SCGdiplusBitmap.h"

// CTestDDrawDlg 대화 상자
class CTestDDrawDlg : public CDialogEx
{
// 생성입니다.
public:
	CTestDDrawDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CResizeCtrl					m_resize;
	CSCGdiplusBitmap			m_img;
	CString						m_filename;

	ComPtr<ID2D1Factory1>        m_Direct2dFactory;
	ComPtr<IWICImagingFactory2>  m_WICFactory;
	ComPtr<ID2D1Device>          m_Direct2dDevice;
	ComPtr<ID2D1DeviceContext>   m_Direct2dContext;
	ComPtr<IDXGISwapChain>       m_SwapChain;

	ComPtr<ID2D1Bitmap>          myBitmap;
	ComPtr<ID2D1Bitmap>          m_img_arrow_left;
	ComPtr<ID2D1Bitmap>          m_img_ink_and_wash;
	std::shared_ptr<MyBitmap>    m_img_gif;
	std::shared_ptr<MyBitmap>    myCharacterBitmap;

	ComPtr<ID2D1SolidColorBrush> myLightSlateGrayBrush;
	ComPtr<ID2D1SolidColorBrush> myCornflowerBlueBrush;


	HRESULT CreateDeviceContext();
	D2D1_SIZE_U CalculateD2DWindowSize();

	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	HRESULT LoadBitmapFromFile(PCWSTR uri, ID2D1Bitmap** ppBitmap);
	HRESULT LoadBitmapFromFile2(PCWSTR uri, MyBitmap* myBitmap);
	HRESULT LoadBitmapFromResource(CString type, UINT resourceID, ID2D1Bitmap** ppBitmap);
	void DiscardDeviceResources();
	void Update();
	HRESULT OnRender();
	void OnResize(UINT width, UINT height);


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
};
