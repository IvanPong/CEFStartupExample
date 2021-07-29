#ifndef _BROWSERHANDLER_
#define _BROWSERHANDLER_
#include <memory>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <functional>
#include <exception>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <list>
#include <queue>
#include <fstream>
#include <filesystem>
#include <windows.h>
#include <stringapiset.h>

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "include/cef_scheme.h"
#include "include/cef_resource_handler.h"
#include "include/cef_origin_whitelist.h"
#include "include/cef_base.h"
#include "include/base/cef_bind.h"
#include "include/cef_parser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

class BrowserHandler :
	public CefClient,
	public CefLifeSpanHandler,
	public CefRenderHandler {
private:
	CefRefPtr<CefBrowser> m_browser;
	int m_bufferHeight;
	int m_bufferWidth;
	int m_height;
	int m_width;
	int m_ii;
	bool bmpPainted;
	unsigned char* bmp;
	std::mutex mtx;

	struct {
		bool needToCheckBrowser = false;
		std::string path = "";
	} browserInitBeforeComplete;

public:
	//generic initialization
	BrowserHandler();
	~BrowserHandler();
	//web page initialization
	void Initialize(std::string path, int _width, int _height);

	// CefClient methods:
	CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
	void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;

	CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }
	void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
	void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int _width, int _height) override;

private:
	void LoadPage();

	IMPLEMENT_REFCOUNTING(BrowserHandler);
};

#endif