#include"critical_error.hpp"
#include<cstdlib>
#include<iostream>

[[noreturn]] void critical_error(std::string_view _message, std::source_location _source_location) {
    // TODO: use std::format and make everything a single string (including newline) to prevent race condition
    std::cout << "Critical error: " << _message << 
	"\nIn function: " << _source_location.function_name() <<
	"\nSource location: " << _source_location.file_name() << ":" << _source_location.line() <<
	"\nTerminating" << std::endl;
    
    std::abort();
}
