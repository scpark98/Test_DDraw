// Test_DDrawDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Test_DDraw.h"
#include "Test_DDrawDlg.h"
#include "afxdialogex.h"

#include "Common/Functions.h"
#include "Common/MemoryDC.h"

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestDDrawDlg 대화 상자



CTestDDrawDlg::CTestDDrawDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_DDRAW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMG, m_static_img);
	DDX_Control(pDX, IDC_STATIC_IMG2, m_static_img2);
}

BEGIN_MESSAGE_MAP(CTestDDrawDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDOK, &CTestDDrawDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestDDrawDlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_INVALIDATE, &CTestDDrawDlg::OnBnClickedButtonInvalidate)
	ON_BN_CLICKED(IDC_BUTTON_INVALIDATE_STOP, &CTestDDrawDlg::OnBnClickedButtonInvalidateStop)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_BACK, &CTestDDrawDlg::OnBnClickedButtonChangeBack)
	ON_BN_CLICKED(IDC_CHECK_GIF, &CTestDDrawDlg::OnBnClickedCheckGif)
	ON_REGISTERED_MESSAGE(Message_CSCD2Image, &CTestDDrawDlg::on_message_from_CSCD2Image)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CTestDDrawDlg 메시지 처리기

BOOL CTestDDrawDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_resize.Create(this);
	m_resize.Add(IDC_STATIC_IMG, 0, 0, 100, 100);
	m_resize.Add(IDC_STATIC_IMG2, 0, 0, 0, 0);


	//m_filename = _T("D:\\ink-and-wash.png");
	//m_filename = _T("D:\\bmp_256color.bmp");
	//m_img.load(IDB_PNG_INK_AND_WASH);
	//m_img.set_alpha(128);
	
	m_d2dc.init(m_hWnd);

	int w = 700;// 1920;// 700;
	int h = 600;// 1080;
	int c = 4;// 3;
	uint8_t* data = new uint8_t[w * h * c];
	read_raw(_T("d:\\snail_700_600_32.raw"), data, w * h * c);
	//read_raw(_T("d:\\mountain_1920_1080_24_BGR.raw"), data, w * h * c);
	m_d2raw.load(m_d2dc.get_WICFactory(), m_d2dc.get_d2dc(), data, w, h, c);

	delete[] data;

	m_d2back.load(m_d2dc.get_WICFactory(), m_d2dc.get_d2dc(), IDR_JPG_LOBBY, _T("JPG"));
	m_d2img2.load(m_d2dc.get_WICFactory(), m_d2dc.get_d2dc(), IDB_PNG_SNAIL_SMALL);

	m_d2gif.load(m_d2dc.get_WICFactory(), m_d2dc.get_d2dc(), IDR_GIF_NHQV06, _T("GIF"));
	m_d2gif.set_parent(m_hWnd);

	m_d2gif2.load(m_d2dc.get_WICFactory(), m_d2dc.get_d2dc(), IDR_GIF_CALLING, _T("GIF"));
	m_d2gif2.set_parent(m_hWnd);

	m_d2webp.load(m_d2dc.get_WICFactory(), m_d2dc.get_d2dc(), IDR_WEBP_XMAS, _T("WEBP"));
	m_d2webp.set_parent(m_hWnd);

	RestoreWindowPosition(&theApp, this);

	DragAcceptFiles();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTestDDrawDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestDDrawDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//이 문장을 주석처리하면 render가 안됨.
		//ID2D1DeviceContext 방식으로 변경 후 주석처리해도 정상동작함.
		//CDialogEx::OnPaint();

		//여기서 dlg의 배경을 그리려했으나 위의 CDialogEx::OnPaint(); 때문인지 배경 적용 안됨.
		//OnEraseBkgnd();에서 배경 처리함.
		//ID2D1DeviceContext 방식으로 변경 후 dc에도 잘 그려짐

		CPaintDC dc(this);
		//long t0 = clock();
		//CRect rc;

		//GetClientRect(rc);
		//CMemoryDC dc(&dc1, &rc);

		//dc.FillSolidRect(rc, red);

		//CRect r(50, 50, 250, 250);
		//dc.FillSolidRect(m_r, get_random_color());

		//OnRender();
		//test_render();

		ID2D1DeviceContext* d2dc = m_d2dc.get_d2dc();
		D2D1_SIZE_F dc_size = m_d2dc.get_size();

		d2dc->BeginDraw();
		d2dc->SetTransform(D2D1::Matrix3x2F::Identity());

		//black으로 칠한 후
		d2dc->Clear(D2D1::ColorF(D2D1::ColorF::Black));

		//배경그림을 그리고 (비율을 유지한 채 가로 또는 세로에 맞게 확대/축소하여 그림)
		m_d2back.draw(d2dc, eSCD2Image_DRAW_MODE::draw_mode_zoom);

		//x, y에 그림
		m_d2img2.draw(d2dc, m_r.left, m_r.top);

		int cx = 400;
		int x = 10;
		int y = dc_size.height / 2.0f - cx / 2.0f;
		m_d2gif.draw(d2dc, x, y, cx);

		x += 10 + cx;
		m_d2gif2.draw(d2dc, x, y, cx);

		x += 10 + cx;
		m_d2webp.draw(d2dc, x, y, cx);

		x += 10 + cx;
		m_d2raw.draw(d2dc, x, y, cx);

		HRESULT hr = d2dc->EndDraw();

		if (SUCCEEDED(hr))
			hr = m_d2dc.get_swapchain()->Present(0, 0);

		//m_d2img.render();
		//m_d2img2.render();

		//Gdiplus::Graphics g(dc.m_hDC);
		//m_img.draw(&dc, 0, 0);

		//TRACE(_T("elapsed = %d\n"), clock() - t0);
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTestDDrawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
D2D1_SIZE_U CTestDDrawDlg::CalculateD2DWindowSize()
{
	//RECT rc;
	//::GetClientRect(m_static_img.m_hWnd, &rc);
	CRect rc;
	GetClientRect(rc);

	D2D1_SIZE_U d2dWindowSize = { 0 };
	d2dWindowSize.width = rc.right;
	d2dWindowSize.height = rc.bottom;

	return d2dWindowSize;
}

HRESULT CTestDDrawDlg::CreateDeviceContext()
{
	HRESULT hr = S_OK;

	D2D1_SIZE_U size = CalculateD2DWindowSize();

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
	};
	UINT countOfDriverTypes = ARRAYSIZE(driverTypes);

	DXGI_SWAP_CHAIN_DESC swapDescription;
	ZeroMemory(&swapDescription, sizeof(swapDescription));
	swapDescription.BufferDesc.Width = size.width;
	swapDescription.BufferDesc.Height = size.height;
	swapDescription.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapDescription.BufferDesc.RefreshRate.Numerator = 60;
	swapDescription.BufferDesc.RefreshRate.Denominator = 1;
	swapDescription.SampleDesc.Count = 1;
	swapDescription.SampleDesc.Quality = 0;
	swapDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDescription.BufferCount = 1;
	swapDescription.OutputWindow = m_hWnd;
	swapDescription.Windowed = TRUE;

	ComPtr<ID3D11Device> d3dDevice;
	for (UINT driverTypeIndex = 0; driverTypeIndex < countOfDriverTypes; driverTypeIndex++) {
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			driverTypes[driverTypeIndex],
			nullptr,
			createDeviceFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&swapDescription,
			&m_swapchain,
			&d3dDevice,
			nullptr,
			nullptr
		);

		if (SUCCEEDED(hr)) {
			break;
		}
	}

	ComPtr<IDXGIDevice> dxgiDevice;
	if (SUCCEEDED(hr)) {
		hr = d3dDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
	}

	if (SUCCEEDED(hr)) {
		hr = m_d2factory->CreateDevice(
			dxgiDevice.Get(),
			&m_d2device
		);
	}
	if (SUCCEEDED(hr)) {
		hr = m_d2device->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&m_d2context
		);
	}

	return hr;
}

HRESULT CTestDDrawDlg::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, IID_PPV_ARGS(&m_d2factory));

	if (SUCCEEDED(hr))
	{
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_WICFactory));
	}

	m_img_gif = std::make_shared<MyBitmap>();
	myCharacterBitmap = std::make_shared<MyBitmap>();

	return hr;
}

HRESULT CTestDDrawDlg::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_d2context) {
		hr = CreateDeviceContext();

		ComPtr<IDXGISurface> surface = nullptr;
		if (SUCCEEDED(hr)) {
			hr = m_swapchain->GetBuffer(
				0,
				IID_PPV_ARGS(&surface)
			);
		}
		ComPtr<ID2D1Bitmap1> bitmap = nullptr;
		if (SUCCEEDED(hr)) {
			FLOAT dpiX, dpiY;
			dpiX = (FLOAT)GetDpiForWindow(::GetDesktopWindow());
			dpiY = dpiX;

			D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE), dpiX, dpiY);

			hr = m_d2context->CreateBitmapFromDxgiSurface(
				surface.Get(),
				&properties,
				&bitmap
			);
		}
		if (SUCCEEDED(hr)) {
			m_d2context->SetTarget(bitmap.Get());
		}

		if (SUCCEEDED(hr)) {
			hr = m_d2context->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&myLightSlateGrayBrush
			);
		}
		if (SUCCEEDED(hr)) {
			hr = m_d2context->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&myCornflowerBlueBrush
			);
		}
	}

	return hr;
}

HRESULT CTestDDrawDlg::LoadBitmapFromResource(CString type, UINT resourceID, ID2D1Bitmap** ppBitmap)
{
	if (!m_WICFactory)
		return S_FALSE;

	if (!m_d2context)
		return S_FALSE;

	IWICStream* pStream = NULL;
	IWICBitmapDecoder* pDecoder = NULL;
	//IWICBitmapFrameDecode* pIDecoderFrame = NULL;
	ComPtr<IWICBitmapFrameDecode> pSource;
	ComPtr<IWICFormatConverter> pConverter;
	ComPtr<IWICBitmapScaler> pScaler;

	// Resource management.
	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void* pImageFile = NULL;
	DWORD imageFileSize = 0;
	HRESULT hr = S_OK;

	// Locate the resource in the application's executable.
	imageResHandle = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceID), type);

	if (SUCCEEDED(hr)) {
		imageResDataHandle = LoadResource(GetModuleHandle(NULL), imageResHandle);
		hr = (imageResDataHandle ? S_OK : E_FAIL);
	}

	if (SUCCEEDED(hr)) {
		pImageFile = LockResource(imageResDataHandle);
		hr = (pImageFile ? S_OK : E_FAIL);
	}

	if (SUCCEEDED(hr)) {
		imageFileSize = SizeofResource(GetModuleHandle(NULL), imageResHandle);
		hr = (imageFileSize ? S_OK : E_FAIL);
	}

	// Create a WIC stream to map onto the memory.
	if (SUCCEEDED(hr)) {
		hr = m_WICFactory->CreateStream(&pStream);
	}

	// Initialize the stream with the memory pointer and size.
	if (SUCCEEDED(hr)) {
		hr = pStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile),
			imageFileSize);
	}

	// Create a decoder for the stream.
	if (SUCCEEDED(hr)) {
		hr = m_WICFactory->CreateDecoderFromStream(
			pStream,                   // The stream to use to create the decoder
			NULL,                          // Do not prefer a particular vendor
			WICDecodeMetadataCacheOnLoad,  // Cache metadata when needed
			&pDecoder);                   // Pointer to the decoder
	}

	// Retrieve the first bitmap frame.
	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr)) {
		hr = m_WICFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pSource.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}
	if (SUCCEEDED(hr)) {
		hr = m_d2context->CreateBitmapFromWicBitmap(
			pConverter.Get(),
			NULL,
			ppBitmap
		);
	}

	return hr;	
}

HRESULT CTestDDrawDlg::LoadBitmapFromFile(PCWSTR uri, ID2D1Bitmap** ppBitmap)
{
	ComPtr<IWICBitmapDecoder> pDecoder;
	ComPtr<IWICBitmapFrameDecode> pSource;
	ComPtr<IWICStream> pStream;
	ComPtr<IWICFormatConverter> pConverter;
	ComPtr<IWICBitmapScaler> pScaler;

	if (!m_WICFactory)
		return S_FALSE;// CreateDeviceIndependentResources();

	if (!m_d2context)
		return S_FALSE; //CreateDeviceContext();

	HRESULT hr = m_WICFactory->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr)) {
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr)) {
		hr = m_WICFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr)) {
		hr = pConverter->Initialize(
			pSource.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}
	if (SUCCEEDED(hr)) {
		hr = m_d2context->CreateBitmapFromWicBitmap(
			pConverter.Get(),
			NULL,
			ppBitmap
		);
	}

	return hr;
}

HRESULT CTestDDrawDlg::LoadBitmapFromFile2(PCWSTR uri, MyBitmap* img)
{
	ComPtr<IWICBitmapDecoder> pDecoder;
	std::vector<ComPtr<ID2D1Bitmap>> bitmapArr;

	if (!m_WICFactory) CreateDeviceIndependentResources();
	if (!m_d2context) CreateDeviceContext();

	HRESULT hr = m_WICFactory->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	UINT frameCount = 0;
	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrameCount(&frameCount);
	}

	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < frameCount; i++)
		{
			ComPtr<IWICFormatConverter> pConverter;
			ComPtr<IWICBitmapFrameDecode> tmpSource;
			ComPtr<ID2D1Bitmap> tmpBitmap;
			if (SUCCEEDED(hr))
			{
				hr = m_WICFactory->CreateFormatConverter(&pConverter);
			}

			if (SUCCEEDED(hr))
			{
				hr = pDecoder->GetFrame(i, &tmpSource);
			}

			if (SUCCEEDED(hr))
			{
				hr = pConverter->Initialize(
					tmpSource.Get(),
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
			}
			if (SUCCEEDED(hr)) {
				hr = m_d2context->CreateBitmapFromWicBitmap(
					pConverter.Get(),
					NULL,
					&tmpBitmap
				);
			}
			if (SUCCEEDED(hr)) {
				bitmapArr.push_back(std::move(tmpBitmap));
			}
		}
	}
	if (img)
	{
		img->Initialize(frameCount, bitmapArr);
	}


	return hr;
}

void CTestDDrawDlg::DiscardDeviceResources()
{
}

void CTestDDrawDlg::OnResize(UINT width, UINT height)
{
	if (m_d2context) {
		HRESULT hr = S_OK;

		m_d2context->SetTarget(nullptr);

		if (SUCCEEDED(hr)) {
			hr = m_swapchain->ResizeBuffers(
				0,
				width,
				height,
				DXGI_FORMAT_B8G8R8A8_UNORM,
				0
			);
		}

		ComPtr<IDXGISurface> surface = nullptr;
		if (SUCCEEDED(hr)) {
			hr = m_swapchain->GetBuffer(
				0,
				IID_PPV_ARGS(&surface)
			);
		}

		ComPtr<ID2D1Bitmap1> bitmap = nullptr;
		if (SUCCEEDED(hr)) {
			FLOAT dpiX, dpiY;
			dpiX = (FLOAT)GetDpiForWindow(::GetDesktopWindow());
			dpiY = dpiX;
			D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(
					DXGI_FORMAT_B8G8R8A8_UNORM,
					D2D1_ALPHA_MODE_IGNORE
				),
				dpiX,
				dpiY
			);
			hr = m_d2context->CreateBitmapFromDxgiSurface(
				surface.Get(),
				&properties,
				&bitmap
			);
		}

		if (SUCCEEDED(hr)) {
			m_d2context->SetTarget(bitmap.Get());
		}

		InvalidateRect(FALSE);
	}
}
*/
BOOL CTestDDrawDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return FALSE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CTestDDrawDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	m_d2back.on_resize(m_d2dc.get_d2dc(), m_d2dc.get_swapchain(), cx, cy);
	Invalidate();
	//OnResize(cx, cy);
	// 
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//Invalidate();
	//if (m_pRenderTarget)
	//{
	//	// 렌더타겟 크기 변경에 대응하여 리소스를 재생성
	//	DiscardDeviceResources();
	//	CreateDeviceResources();
	//	Invalidate();
	//}
}

void CTestDDrawDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//m_pD2DFactory->Release();
	//m_pWICFactory->Release();
	//DiscardDeviceResources();
	CoUninitialize();
}

void CTestDDrawDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CTestDDrawDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	AfxMessageBox(_T("lbutton up"));
	CDialogEx::OnLButtonUp(nFlags, point);
}

/*
HRESULT CTestDDrawDlg::test_render()
{
	HRESULT hr = S_OK;
	hr = CreateDeviceResources();
	if (!SUCCEEDED(hr))
		return S_FALSE;

	m_d2context->BeginDraw();
	m_d2context->SetTransform(D2D1::Matrix3x2F::Identity());
	//m_d2context->Clear(D2D1::ColorF(D2D1::ColorF::DimGray));

	float r = random19937(0.0f, 1.0f);
	float g = random19937(0.0f, 1.0f);
	float b = random19937(0.0f, 1.0f);
	float a = random19937(0.1f, 1.0f);
	TRACE(_T("%.1f, %.1f, %.1f, %.1f\n"), r, g, b, a);
	myCornflowerBlueBrush->SetColor(D2D1::ColorF(r, g, b, a));

	m_d2context->FillRectangle(D2D1::RectF(m_r.left, m_r.top, m_r.right, m_r.bottom), myCornflowerBlueBrush.Get());

	hr = m_d2context->EndDraw();

	if (SUCCEEDED(hr))
	{
		hr = m_swapchain->Present(0, 0);
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

HRESULT CTestDDrawDlg::OnRender()
{
	if (!m_d2context)
		return S_FALSE;

	HRESULT hr = S_OK;
	//hr = CreateDeviceResources();
	if (!SUCCEEDED(hr))
		return S_FALSE;

	m_d2context->BeginDraw();
	m_d2context->SetTransform(D2D1::Matrix3x2F::Identity());
	//m_d2context->Clear(D2D1::ColorF(D2D1::ColorF::DimGray));

	D2D1_SIZE_F rtSize = m_d2context->GetSize();

	int width = static_cast<int>(rtSize.width);
	int height = static_cast<int>(rtSize.height);

	//for (auto& i : ground) {
	//	m_d2context->FillRectangle(&i, myCornflowerBlueBrush.Get());
	//}

	//ComPtr<ID2D1Effect> affineTransformEffect;
	//m_d2context->CreateEffect(
	//	CLSID_D2D12DAffineTransform,
	//	&affineTransformEffect
	//);

	//affineTransformEffect->SetInput(0, m_img.Get());
	//affineTransformEffect->SetValue(
	//	D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX,
	//	D2D1::Matrix3x2F::Scale(D2D1::SizeF(0.5f, 0.5f), D2D1::Point2F(0.f, 0.f))
	//);

	if (m_img_back)
	{
		//m_d2context->DrawImage(affineTransformEffect.Get());
		D2D1_SIZE_F sz_img = m_img_back->GetSize();
		CRect r = get_ratio_rect(CRect(0, 0, width * 2, height * 2), (int)(sz_img.width), (int)(sz_img.height));
		TRACE(_T("rc = %dx%d, sz_img = %.0fx%.0f, r = %s\n"), width, height, sz_img.width, sz_img.height, get_rect_info_string(r));
		//m_d2context->DrawBitmap(m_img.Get(), D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height));
		m_d2context->DrawBitmap(m_img_back.Get(), D2D1::RectF(r.left, r.top, r.right, r.bottom));
	}

	if (m_img_ink_and_wash)
	{
		m_d2context->DrawBitmap(m_img_ink_and_wash.Get(), D2D1::RectF(0.0f, 0.0f, rtSize.width / 1.0f, rtSize.height / 1.0f), 0.5f);
	}

	if (m_img_arrow_left)
	{
		m_d2context->DrawBitmap(m_img_arrow_left.Get(),
			D2D1::RectF(
				200.0f, 200.0f,
				500.0f, 500.0f
			));
	}

	if (m_img_gif)
	{
		ComPtr<ID2D1Bitmap> tmp = m_img_gif->GetBitmap();
		if (tmp)
		{
			m_d2context->DrawImage(tmp.Get());// ,
				//m_img_gif->GetBitmapPosition());
		}
	}

	if (myCharacterBitmap)
	{
		ComPtr<ID2D1Bitmap> tmp = myCharacterBitmap->GetBitmap();
		if (tmp)
		{
			// Affine transform init
			ComPtr<ID2D1Effect> affineTransformEffect;
			m_d2context->CreateEffect(
				CLSID_D2D12DAffineTransform,
				&affineTransformEffect
			);
			// Color matrix init
			ComPtr<ID2D1Effect> colorMatrixEffect;
			ComPtr<ID2D1Effect> colorMatrixEffect2;
			m_d2context->CreateEffect(
				CLSID_D2D1ColorMatrix,
				&colorMatrixEffect
			);
			m_d2context->CreateEffect(
				CLSID_D2D1ColorMatrix,
				&colorMatrixEffect2
			);
			// Edge detection init
			ComPtr<ID2D1Effect> edgeDetectionEffect;
			ComPtr<ID2D1Effect> edgeDetectionEffect2;
			ComPtr<ID2D1Effect> edgeDetectionEffect3;
			m_d2context->CreateEffect(CLSID_D2D1EdgeDetection, &edgeDetectionEffect);
			m_d2context->CreateEffect(CLSID_D2D1EdgeDetection, &edgeDetectionEffect2);
			m_d2context->CreateEffect(CLSID_D2D1EdgeDetection, &edgeDetectionEffect3);

			// Chromakey init
			ComPtr<ID2D1Effect> chromakeyEffect;
			ComPtr<ID2D1Effect> chromakeyEffect2;
			ComPtr<ID2D1Effect> chromakeyEffect3;
			m_d2context->CreateEffect(CLSID_D2D1ChromaKey, &chromakeyEffect);
			m_d2context->CreateEffect(CLSID_D2D1ChromaKey, &chromakeyEffect2);
			m_d2context->CreateEffect(CLSID_D2D1ChromaKey, &chromakeyEffect3);

			// Affine transform input
			affineTransformEffect->SetInput(0, tmp.Get());

			// Color matrix input
			colorMatrixEffect->SetInputEffect(0, affineTransformEffect.Get());
			colorMatrixEffect2->SetInputEffect(0, colorMatrixEffect.Get());

			// Edge detection input
			edgeDetectionEffect->SetInputEffect(0, affineTransformEffect.Get());
			edgeDetectionEffect2->SetInputEffect(0, colorMatrixEffect.Get());
			edgeDetectionEffect3->SetInputEffect(0, colorMatrixEffect2.Get());

			// Chromakey input
			chromakeyEffect->SetInputEffect(0, edgeDetectionEffect.Get());
			chromakeyEffect2->SetInputEffect(0, edgeDetectionEffect2.Get());
			chromakeyEffect3->SetInputEffect(0, edgeDetectionEffect3.Get());

			auto size = tmp->GetPixelSize();
			D2D1_POINT_2F ps = myCharacterBitmap->GetBitmapDrawOffset();
			//D2D1_POINT_2F center = D2D1::Point2F(ps.right - ((ps.right - ps.left) / 2), ps.bottom - ((ps.bottom - ps.top) / 2));

			// Affine transform
			//affineTransformEffect->SetValue(
			//	D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX,
			//	D2D1::Matrix3x2F::Scale(D2D1::SizeF((isLeft ? -1 : 1) * 1.f, 1.f), D2D1::Point2F(size.width / 2, size.height / 2))
			//);

			// Color matrix 1
			D2D1_MATRIX_5X4_F matrix = D2D1::Matrix5x4F(
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 100,
				0, 1, 0, 0);
			colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, matrix);
			colorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_CLAMP_OUTPUT, TRUE);

			// Color matrix 2
			D2D1_MATRIX_5X4_F matrix2 = D2D1::Matrix5x4F(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 0.5,
				0, 0, 0, 0);
			colorMatrixEffect2->SetValue(
				D2D1_COLORMATRIX_PROP_COLOR_MATRIX,
				matrix2
			);

			D2D1_RECT_F rect;
			UINT rectC;
			ComPtr<ID2D1Image> tmpImg;
			affineTransformEffect->QueryInterface(IID_PPV_ARGS(&tmpImg));
			m_d2context->GetImageLocalBounds(tmpImg.Get(), &rect);
			//ComPtr<ID2D1Image> affineImage(ConvertEffectToImage(affineTransformEffect.Get()));
			//m_d2context->GetImageLocalBounds(affineImage.Get(), &rect);
			//std::cout << rect.bottom << " " << rect.left << " " << rect.right << " " << rect.top << " : " << "\n";
			//m_d2context->FillRectangle(myCharacterBitmap->GetBitmapRect(), myLightSlateGrayBrush.Get());
			//m_d2context->SetTransform(D2D1::Matrix3x2F::Scale((isLeft ? -1 : 1), 1.f, center));
			// Print image (affine, color matrix)
			m_d2context->DrawImage(tmpImg.Get(), D2D1::Point2F(ps.x - 0.f, ps.y - 0.f));
			//m_d2context->DrawImage(colorMatrixEffect.Get(), D2D1::Point2F(ps.x - 175.f, ps.y - 70.f));
			//m_d2context->DrawImage(colorMatrixEffect2.Get(), D2D1::Point2F(ps.x - 175.f, ps.y + 60.f));
			//// Print image (edge detection)
			//m_d2context->DrawImage(edgeDetectionEffect.Get(), D2D1::Point2F(ps.x - 25.f, ps.y - 200.f));
			//m_d2context->DrawImage(edgeDetectionEffect2.Get(), D2D1::Point2F(ps.x - 25.f, ps.y - 70.f));
			//m_d2context->DrawImage(edgeDetectionEffect3.Get(), D2D1::Point2F(ps.x - 25.f, ps.y + 60.f));
			//// Print image (chromakey)
			//m_d2context->DrawImage(chromakeyEffect.Get(), D2D1::Point2F(ps.x + 125.f, ps.y - 200.f));
			//m_d2context->DrawImage(chromakeyEffect2.Get(), D2D1::Point2F(ps.x + 125.f, ps.y - 70.f));
			//m_d2context->DrawImage(chromakeyEffect3.Get(), D2D1::Point2F(ps.x + 125.f, ps.y + 60.f));


			//m_d2context->SetTransform(D2D1::Matrix3x2F::Scale(1.f, 1.f));
		}
	}


	D2D1_RECT_F rectangle1 = D2D1::RectF(
		rtSize.width / 2 - 50.0f, rtSize.height / 2 - 50.0f,
		rtSize.width / 2 + 50.0f, rtSize.height / 2 + 50.0f
	);
	D2D1_RECT_F rectangle2 = D2D1::RectF(
		rtSize.width / 2 - 100.0f, rtSize.height / 2 - 100.0f,
		rtSize.width / 2 + 100.0f, rtSize.height / 2 + 100.0f
	);
	//m_d2context->FillRectangle(&rectangle1, myLightSlateGrayBrush.Get());
	//m_d2context->DrawRectangle(&rectangle2, myCornflowerBlueBrush.Get());

	hr = m_d2context->EndDraw();

	if (SUCCEEDED(hr)) {
		hr = m_swapchain->Present(0, 0);
	}

	if (hr == D2DERR_RECREATE_TARGET) {
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}
*/
void CTestDDrawDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(&theApp, this);
}

void CTestDDrawDlg::OnBnClickedOk()
{
}

void CTestDDrawDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

void CTestDDrawDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == timer_test)
	{
		m_r.left = random19937(0, 1900);
		m_r.top = random19937(0, 1900);
		m_r.right = m_r.left + 400;
		m_r.bottom = m_r.top + 400;
		Invalidate();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CTestDDrawDlg::OnBnClickedButtonInvalidate()
{
	SetTimer(timer_test, 10, NULL);
}

void CTestDDrawDlg::OnBnClickedButtonInvalidateStop()
{
	KillTimer(timer_test);
}

void CTestDDrawDlg::OnBnClickedButtonChangeBack()
{
	//m_d2img.load(m_d2dc.get_WICFactory(), m_d2dc.get_d2dc(), L"D:\\ironman.jfif");
	m_d2back.load(m_d2dc.get_WICFactory(), m_d2dc.get_d2dc(), IDR_JFIF_IRONMAN, _T("JFIF"));
	Invalidate();
}

void CTestDDrawDlg::OnBnClickedCheckGif()
{
	m_d2gif.play();
	m_d2gif2.play();
	m_d2webp.play();
}

LRESULT CTestDDrawDlg::on_message_from_CSCD2Image(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("index = %d\n"), (int)lParam);
	Invalidate();

	return 0;
}

void CTestDDrawDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TCHAR sfile[MAX_PATH];

	DragQueryFile(hDropInfo, 0, sfile, MAX_PATH);

	if (is_exist_keyword(get_part(sfile, fn_ext), FILE_EXTENSION_IMAGE, false, true))
	{
		m_img.load(sfile);
		//m_d2img2.load(m_d2dc.get_WICFactory(), m_d2dc.get_d2dc(), sfile);

		if (m_img.is_empty())
			return;

		int w = m_img.width;
		int h = m_img.height;
		int c = m_img.channel;
		m_img.get_raw_data();
		//uint8_t* data = new uint8_t[w * h * c];
		//read_raw(sfile, data, w * h * c);
		//read_raw(_T("d:\\mountain_1920_1080_24_BGR.raw"), data, w * h * c);
		m_d2raw.load(m_d2dc.get_WICFactory(), m_d2dc.get_d2dc(), m_img.data, w, h, c);

		//delete[] data;



		Invalidate();

		//m_img.save_raw_data();
	}

	CDialogEx::OnDropFiles(hDropInfo);
}
