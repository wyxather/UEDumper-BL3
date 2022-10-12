#include "UEGame.h"
#include <array>
#include <winternl.h>

#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "version.lib")

UEGame::UEGame() noexcept : error{ false }, id{}, handle{}, version{}
{
	std::printf("UEGame...\n");

	window = FindWindowW(L"UnrealWindow", nullptr);
	if (!window) {
		error = true;
		return;
	}

	GetWindowThreadProcessId(window, id.getAddressOf<DWORD>());
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
		DWORD handle;
		DWORD size{ GetFileVersionInfoSizeW(path.c_str(), &handle) };
		if (handle || !size) {
			error = true;
			return;
		}

		std::unique_ptr<BYTE[]> data{ std::make_unique<decltype(data)::element_type[]>(size) };
		if (!GetFileVersionInfoW(path.c_str(), handle, size, data.get())) {
			error = true;
			return;
		}

		VS_FIXEDFILEINFO* info;
		UINT len;
		if (!VerQueryValueW(data.get(), L"\\", reinterpret_cast<LPVOID*>(&info), &len)) {
			error = true;
			return;
		}

		version[0] = HIWORD(info->dwFileVersionMS);
		version[1] = LOWORD(info->dwFileVersionMS);
		version[2] = HIWORD(info->dwFileVersionLS);
		version[3] = LOWORD(info->dwFileVersionLS);
	}

	{
		PROCESS_BASIC_INFORMATION pbi;
		if (!NT_SUCCESS(NtQueryInformationProcess(handle, ProcessBasicInformation, &pbi, sizeof(pbi), 0))) {
			error = true;
			return;
		}

		UEPointer base = read<std::uintptr_t>(reinterpret_cast<std::byte*>(pbi.PebBaseAddress) + 16);
		if (!base) {
			error = true;
			return;
		}

		std::array<std::byte, 1024> buffer;
		if (!read(base, buffer.data(), buffer.size())) {
			error = true;
			return;
		}

		const auto size = reinterpret_cast<PIMAGE_NT_HEADERS>(buffer.data() + (reinterpret_cast<PIMAGE_DOS_HEADER>(buffer.data()))->e_lfanew)->OptionalHeader.SizeOfImage;
		image = std::make_unique<decltype(image)::element_type[]>(size);
		if (!read(base, image.get(), size)) {
			error = true;
			return;
		}
	}
}

UEGame::~UEGame() noexcept
{
	std::printf("~UEGame...\n");

	if (handle)
		CloseHandle(handle);
}

auto UEGame::read(void* const address, void* const buffer, const SIZE_T size) const noexcept -> bool
{
	SIZE_T readBytes;
	return ReadProcessMemory(handle, address, buffer, size, &readBytes) && readBytes == size;
}