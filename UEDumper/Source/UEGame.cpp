#include "UEGame.h"
#pragma comment(lib, "version.lib")

UEGame::UEGame() noexcept : error{ false }, id{ 0 }, handle{ nullptr }
{
	std::printf("UEGame...\n");

	window = FindWindowW(L"UnrealWindow", nullptr);
	if (!window) {
		error = true;
		return;
	}

	GetWindowThreadProcessId(window, &id);
	if (!id) {
		error = true;
		return;
	}

	handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, id);
	if (!handle) {
		error = true;
		return;
	}
	
	{
		WCHAR name[MAX_PATH];
		DWORD size{ MAX_PATH };
		if (!QueryFullProcessImageNameW(handle, 0, name, &size)) {
			error = true;
			return;
		}
		path = name;
	}

	{
		DWORD handle{};
		DWORD size{ GetFileVersionInfoSizeW(path.c_str(), &handle) };

		std::unique_ptr<BYTE[]> data{ std::make_unique<decltype(data)::element_type[]>(size) };
		GetFileVersionInfoW(path.c_str(), handle, size, data.get());

		VS_FIXEDFILEINFO* info;
		UINT len;
		VerQueryValueW(data.get(), L"\\", reinterpret_cast<LPVOID*>(&info), &len);

		version[0] = HIWORD(info->dwFileVersionMS);
		version[1] = LOWORD(info->dwFileVersionMS);
		version[2] = HIWORD(info->dwFileVersionLS);
		version[3] = LOWORD(info->dwFileVersionLS);
	}
}

UEGame::~UEGame() noexcept
{
	std::printf("~UEGame...\n");

	if (handle)
		CloseHandle(handle);
}