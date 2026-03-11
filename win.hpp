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
}
