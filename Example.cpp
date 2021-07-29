#include "BrowserHandler.h"

#include <memory>
#include <string>
#include <sstream>
#include <filesystem>
#include <iostream>

#include <windows.h>
#include <stringapiset.h>

#include "include/cef_app.h"


#define ERRORREP(stri) \
{ std::stringstream sss; sss << "Error! File:" << __FILE__ << " Line: " << __LINE__ << " " << stri << "\n"; \
MessageBox(NULL, sss.str().c_str() , NULL, MB_OK); }


inline bool exists_file(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

class ManagerApp :
	public CefApp,
	public CefRenderProcessHandler,
	public CefBrowserProcessHandler
{
public:
	ManagerApp() {};
	~ManagerApp() {};

	CefRefPtr< CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

	void OnBeforeCommandLineProcessing(
		const CefString& process_type,
		CefRefPtr<CefCommandLine> command_line) {
		command_line->AppendArgument("disable-gpu");
		command_line->AppendArgument("disable-gpu-compositing");
		command_line->AppendArgument("no-sandbox");
	}

	void OnContextInitialized() {
		//browser creation should be safe here, right?
		auto memUnsafeBrowser = new BrowserHandler();
		memUnsafeBrowser->Initialize("https://www.bing.com/", 500, 500);
	}

	IMPLEMENT_REFCOUNTING(ManagerApp);
};

int main(int argc, char** argv) {
	CefMainArgs mainargs;
	memset(&mainargs, 0, sizeof(CefMainArgs));
	mainargs.instance = GetModuleHandle(NULL);

	CefSettings settings = CefSettings();
	settings.multi_threaded_message_loop = true;
	settings.windowless_rendering_enabled = true;
	settings.no_sandbox = true;


	LPSTR wPath = new char[MAX_PATH + 1];
	if (GetModuleFileName(GetModuleHandle(NULL), wPath, MAX_PATH + 1)) {
		//currently this looks like the full path of this exe, we need to change it to the other one.
		std::string wP(wPath);
		auto point = wP.find_last_of("\\");
		std::string realExePath = wP.substr(0, point + 1) + "SubProcess.exe";
		std::wstring actualExeString = s2ws(realExePath);
		cef_string_set(actualExeString.c_str(), std::wcslen(actualExeString.c_str()), &(settings.browser_subprocess_path), true);
	}
	else ERRORREP("SubProcess path failed to resolve!")
		delete[] wPath;

	// Initialize CEF - other modules will conflict
	if (!CefInitialize(mainargs, settings, new ManagerApp, NULL)) ERRORREP("Cef initialization error!");

	std::cout << "At this point, we just wait for bitmaps to appear. When you type, the program will end\n";
	std::cout << "What ought to happen, is we get a bunch of bitmaps to appear in the folder labelled ouptut as cef renders bing\n";
	std::string str;
	std::cin >> str;

	return 1;
}