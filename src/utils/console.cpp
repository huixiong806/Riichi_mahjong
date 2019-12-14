#include "console.h"
#include <string>
#include <mutex>
#include <vector>
#include <clocale>

#if __has_include(<Windows.h>)
#define WIN32_LEAN_AND_MEAN
#define NOIME
#define NOMCX
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <sstream>
#include <iostream>
#define WIN32_CP
#endif

namespace {
	std::mutex locale_lock{};
	std::vector<const char*> last{};
	std::vector<std::string> pushed{};
#ifdef WIN32_CP
	UINT default_codepage;

	class MBuf final : public std::stringbuf {
	public:
		int sync() override {
			const auto multi_byte = str();
			str("");
			std::wstring wide;
			wide.resize(multi_byte.size());
			MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, multi_byte.c_str(), -1, wide.data(), wide.size());
			std::fputws(wide.c_str(), stdout);
			return 0;
		}
	} buf;
#endif
}

void push_console_locale_utf8(std::string_view locale_no_encode) {
	auto new_locale = std::string(locale_no_encode) + ".UTF-8";
	{
		std::lock_guard<std::mutex> lk{locale_lock};
#ifdef WIN32_CP
		if (last.empty()) {
			default_codepage = GetConsoleOutputCP();
			if (!SetConsoleOutputCP(CP_UTF8)) {
				std::terminate(); // TODO: NOT GOOD
			}
			_setmode(_fileno(stdout), _O_U8TEXT);
			std::cout.rdbuf(&buf);
			std::cerr.rdbuf(&buf);
		}
#endif
		last.push_back(std::setlocale(LC_ALL, new_locale.c_str()));
		pushed.push_back(std::move(new_locale));
	}
}

void pop_console_locale() {
	{
		std::lock_guard<std::mutex> lk{locale_lock};
		if (!last.empty()) {
			std::setlocale(LC_ALL, last.back());
			last.pop_back();
			pushed.pop_back();
		}
#ifdef WIN32_CP
		else { SetConsoleOutputCP(default_codepage); }
#endif
	}
}
