#include "MinHook.h"

typedef int (WINAPI* tIsWindowVisible)(HWND hWnd);
tIsWindowVisible pIsWindowVisible = NULL;

// This function retrieves the client rectangle of a window. 
// The client rectangle is the part of the window that can be 
// seen by the user and excludes the parts hidden by system decorations or other windows.
int WINAPI dIsWindowVisible(HWND hWnd)
{

	// The GetClientRect function is called on the window handle hWnd,
	// which retrieves the client rectangle data in the RECT structure named rect.

	RECT rect;
	GetClientRect(hWnd, &rect);

	// The POINT structure is used to store screen coordinates. 
	// The ul and lr variables are initialized with the upper-left 
	// and lower-right coordinates of the client rectangle respectively.

	POINT ul;
	ul.x = rect.left;
	ul.y = rect.top;

	POINT lr;
	lr.x = rect.right;
	lr.y = rect.bottom;

	// MapWindowPoints is a helper function that maps the client coordinates 
	// (ul and lr) to screen coordinates based on the window handle hWnd. 
	// The nullptr parameter is used to indicate that no conversion is needed for the calling window itself.

	MapWindowPoints(hWnd, nullptr, &ul, 1);
	MapWindowPoints(hWnd, nullptr, &lr, 1);

	// /The rect variable is updated with the mapped screen coordinates of ul and lr. 
	// This new rectangle defines the area of the client rectangle that will be visible after clipping.

	rect.left = ul.x;
	rect.top = ul.y;

	rect.right = lr.x;
	rect.bottom = lr.y;

	if (hWnd == GetForegroundWindow())
	{
		ClipCursor(&rect);
	}

	return pIsWindowVisible(hWnd);
}

UINT WINAPI cursorLock(VOID*) {

	HMODULE hModule;

	hModule = LoadLibraryW(L"user32");
	if (hModule != nullptr)
	{

		MH_CreateHook((DWORD_PTR*)IsWindowVisible, reinterpret_cast<LPVOID>(dIsWindowVisible), reinterpret_cast<LPVOID*>(&pIsWindowVisible));
		MH_EnableHook((DWORD_PTR*)IsWindowVisible);

	}

	return 0;

}

int main()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)cursorLock, 0, 0, 0);

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		if (MH_Initialize() != MH_OK)
		{
			MessageBox(NULL, (L"Error"), (L"CursorLock"), MB_OK);
			return 1;
		}

		main();

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

