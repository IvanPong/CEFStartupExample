#include "BrowserHandler.h"
#include "EasyBMP.h"

CefString makeJunkScreen(std::string text, std::string color) {
	std::string ss = "<html><body bgcolor=\"" + color + "\"><h2>" + text + "</h2></body></html>";
	return CefString("data:text/html;base64,").ToString() + CefURIEncode(CefBase64Encode(ss.data(), ss.size()), false).ToString();
}

BrowserHandler::BrowserHandler() {
	std::experimental::filesystem::create_directory("output");
	m_ii = 0;
	m_height = 128;
	m_width = 128;

	m_browser = nullptr;

	bmp = new unsigned char[4 * m_height * m_width];

	CefWindowInfo window_info;
	window_info.SetAsWindowless(nullptr);

	CefBrowserSettings browser_settings = CefBrowserSettings();
	browser_settings.size = sizeof(CefBrowserSettings);
	browser_settings.webgl = STATE_DISABLED;
	browser_settings.windowless_frame_rate = 24;
	browser_settings.background_color = CefColorSetARGB(0, 0, 0, 0);

	CefBrowserHost::CreateBrowser(window_info, this, makeJunkScreen("Default page", "white"), browser_settings, NULL, NULL);
}

BrowserHandler::~BrowserHandler() {}

void BrowserHandler::Initialize(std::string path, int _width, int _height) {
	//the plan for this is to gate the initialization with the queue idea I mentioned.
	m_bufferHeight = _height;
	m_bufferWidth = _width;
	browserInitBeforeComplete.path = path;

	if (m_browser) {
		LoadPage();
	}
	else {
		browserInitBeforeComplete.needToCheckBrowser = true;
	}
}

void BrowserHandler::LoadPage() {
	m_browser->GetMainFrame()->LoadURL(browserInitBeforeComplete.path);
	m_browser->GetHost()->WasResized();
}

void BrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	if (m_browser == nullptr) {
		m_browser = browser;
	}
	if (browserInitBeforeComplete.needToCheckBrowser) {
		LoadPage();
		browserInitBeforeComplete.needToCheckBrowser = false;
	}
}


void BrowserHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect & rect)
{
	if (m_browser->GetIdentifier() == browser->GetIdentifier()) {
		rect = CefRect(0, 0, m_width, m_height);
	}
	else {
		rect = CefRect(0, 0, 128, 128);
	}
}


void BrowserHandler::OnPaint(
	CefRefPtr<CefBrowser> browser,
	PaintElementType type,
	const RectList & dirtyRects,
	const void* buffer,
	int _width,
	int _height)
{

	m_ii++;
	BMP bmp;
	unsigned char* b = (unsigned char*)buffer;
	bmp.SetSize(_width, _height);
	bmp.SetBitDepth(32);
	for (int jj = 0; jj < _height; jj++) {
		for (int ii = 0; ii < _width; ii++) {
			RGBApixel pixel;
			memcpy(&pixel, &b[4 * _width * jj + 4 * ii], 4);
			bmp.SetPixel(ii, jj, pixel);
		}
	}

	std::string filePath = "output\\BrowserOutput" + std::to_string(m_ii) + ".bmp";

	bmp.WriteToFile(filePath.c_str());
}