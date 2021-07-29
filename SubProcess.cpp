// SubProcess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_base.h"
#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/cef_parser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"


int WINAPI WinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
{
	CefMainArgs main_args(hInstance);
	return CefExecuteProcess(main_args, NULL, NULL);
}