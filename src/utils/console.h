#pragma once

#include <string_view>

void push_console_locale_utf8(std::string_view locale_no_encode);
void pop_console_locale();
