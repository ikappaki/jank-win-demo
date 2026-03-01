#include <windows.h>

namespace w32 {

  // -----------------------------------
  // Generic jank window handler support
  // -----------------------------------

  struct JankHandler
  {
    jank::runtime::oref<jank::runtime::object> fn;
  };

  LRESULT CallJankHandler(jank::runtime::oref<jank::runtime::object>* o,
                          HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    auto h = (*o);
    auto a = jank::runtime::make_box<jank::runtime::obj::opaque_box>(hwnd, "HWND__*");
    auto b = jank::runtime::make_box<jank::runtime::obj::integer>(msg);
    auto c = jank::runtime::make_box<jank::runtime::obj::integer>(wParam);
    auto d = jank::runtime::make_box<jank::runtime::obj::integer>(lParam);
    auto args = jank::runtime::make_box<jank::runtime::obj::persistent_list>(std::in_place, a, b, c, d);
    auto x = h->call(args);
    auto res = try_object<jank::runtime::obj::integer>(x);
    return res->to_integer();
  }

  LRESULT CALLBACK JankWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    auto handler = (jank::runtime::oref<jank::runtime::object>*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (handler)
      return CallJankHandler(handler, hwnd, msg, wParam, lParam);
    else
      return DefWindowProc(hwnd, msg, wParam, lParam);
  }

  // -------------------
  // Convenience helpers
  // -------------------

  COLORREF RGB_(BYTE r, BYTE g, BYTE b)
  {
    return RGB(r, g, b);
  }

  LONG_PTR SetWindowLongPtrA(HWND hwnd, int nIndex, jank::runtime::oref<jank::runtime::object> & dwNewLong)
  {
    return ::SetWindowLongPtr(hwnd, nIndex, (LONG_PTR)&dwNewLong);
  }

  ::WNDCLASS WNDCLASS(HMODULE hInstance, LPCSTR className, LRESULT (CALLBACK *handler)(HWND, UINT, WPARAM, LPARAM))
  {
    ::WNDCLASS wc = {};
    wc.lpfnWndProc = handler;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    return wc;
  }


  // -------------------------------------------------------------------
  // Thin Win32 wrappers: needed because the API uses stdcall, which the
  // JIT does not yet support.
  // -------------------------------------------------------------------


  HDC BeginPaint(HWND hwnd, PAINTSTRUCT* ps)
  {
    return ::BeginPaint(hwnd, ps);
  }

  HWND CreateWindowExA(DWORD exStyle, LPCSTR className, LPCSTR windowName,
                       DWORD style, int x, int y, int width, int height,
                       HWND hwndParent, HMENU hMenu, HINSTANCE hInstance,
                       LPVOID lpParam)
  {
    return ::CreateWindowExA(exStyle, className, windowName, style,
                             x, y, width, height, hwndParent, hMenu,
                             hInstance, lpParam);
  }

  LRESULT DefWindowProcA(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    return ::DefWindowProcA(hwnd, msg, wParam, lParam);
  }

  LRESULT DispatchMessageA(MSG* msg)
  {
    return ::DispatchMessageA(msg);
  }

  BOOL EndPaint(HWND hwnd, PAINTSTRUCT* ps)
  {
    return ::EndPaint(hwnd, ps);
  }

  BOOL GetMessageA(MSG* msg, HWND hwnd, UINT wMin, UINT wMax)
  {
    return ::GetMessageA(msg, hwnd, wMin, wMax);
  }

  HMODULE GetModuleHandleA(LPCSTR lpModuleName)
  {
    return ::GetModuleHandleA(lpModuleName);
  }

  BOOL GetTextExtentPoint32A(HDC hdc, LPCSTR text, int len, SIZE* size)
  {
    return ::GetTextExtentPoint32A(hdc, text, len, size);
  }

  BOOL InvalidateRect(HWND hwnd, RECT const* rect, BOOL erase)
  {
    return ::InvalidateRect(hwnd, rect, erase);
  }

  COLORREF SetTextColor(HDC hdc, COLORREF color)
  {
    return ::SetTextColor(hdc, color);
  }

  ATOM RegisterClassA(::WNDCLASS* wc)
  {
    return ::RegisterClassA(wc);
  }

  BOOL SetWindowTextA(HWND hwnd, LPCSTR lpString)
  {
    return ::SetWindowTextA(hwnd, lpString);
  }

  BOOL ShowWindow(HWND hwnd, int nCmdShow)
  {
    return ::ShowWindow(hwnd, nCmdShow);
  }


  BOOL TextOutA(HDC hdc, int x, int y, LPCSTR text, int c)
  {
    return ::TextOutA(hdc, x, y, text, c);
  }

  BOOL TranslateMessage(MSG* msg)
  {
    return ::TranslateMessage(msg);
  }

  BOOL UpdateWindow(HWND hwnd)
  {
    return ::UpdateWindow(hwnd);
  }

}
