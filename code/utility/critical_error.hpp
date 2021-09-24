#pragma once

#include <string_view>
#include <source_location>

// TODO: Put ths in namespace Utility
[[noreturn]] void critical_error(std::string_view _message, std::source_location _source_location = std::source_location::current());
