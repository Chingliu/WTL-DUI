
#include <tchar.h>
#include <windows.h>
#include <initguid.h>
#include <oleacc.h>

#include <atlbase.h>

#include "wanui_res/resource.h"

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/message_loop.h"
#include "base/path_service.h"
#include "base/process_util.h"

#include "ui/base/resource/resource_bundle.h"

#include "ui/views/focus/accelerator_handler.h"
#include "ui/views/widget/widget.h"

#include "desktop_view_delegate.h"
#include "desktop_window_view.h"

// �������.
int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HRESULT hRes = OleInitialize(NULL);

    // this resolves ATL window thunking problem when Microsoft Layer
    // for Unicode (MSLU) is used.
    ::DefWindowProc(NULL, 0, 0, 0L);

    base::EnableTerminationOnHeapCorruption();

    // ������õ����������������.
    base::AtExitManager exit_manager;

    CommandLine::Init(0, NULL);

    FilePath res_dll;
    PathService::Get(base::DIR_EXE, &res_dll);
    res_dll = res_dll.Append(L"wanui_res.dll");
	ui::ResourceBundle::InitSharedInstance(res_dll);

    MessageLoop main_message_loop(MessageLoop::TYPE_UI);

    views::desktop::DesktopViewDelegate view_delegate;

    // ֻ֧�ִ�view����.
    views::Widget::SetPureViews(true);

    views::desktop::DesktopWindowView::CreateDesktopWindow(
        view::desktop::DesktopWindowView::DESKTOP_DEFAULT);
    views::desktop::DesktopWindowView::desktop_window_view->CreateTestWindow(
        L"Sample Window 1", SK_ColorWHITE, gfx::Rect(500, 200, 400, 400), true);
    views::desktop::DesktopWindowView::desktop_window_view->CreateTestWindow(
        L"Sample Window 2", SK_ColorRED, gfx::Rect(600, 450, 450, 300), false);

    views::AcceleratorHandler accelerator_handler;
    MessageLoopForUI::current()->Run(&accelerator_handler);

    ui::ResourceBundle::CleanupSharedInstance();
    OleUninitialize();

    return 0;
}