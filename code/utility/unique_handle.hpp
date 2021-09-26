#pragma once

#include <optional>
#include <functional>
#include <utility>
#include "preprocessor/macros.hpp"
#include "utility/assert.hpp"

// TODO: make all ctors non-throwing, require that moving deleter does not throw!!
// TODO: make functions constexpr

namespace Utility {

    // RAII wrapper for an owning, non-nullable handle type using pass-by-copy semantics
    template<typename _Handle>
    class Unique_handle {
    public:
	typedef _Handle Handle;
	Unique_handle() = default;

	Unique_handle(_Handle _handle):
	    Unique_handle(_handle, +[](_Handle) constexpr noexcept { })
	{ }

	Unique_handle(_Handle _handle, auto&& _deleter):
	    handle(_handle),
	    deleter(std::forward<decltype(_deleter)>(_deleter))
	{ }
	
	// Non-const, because library functions may take handles by copy, in which case the pointed-to objects can still be mutated
	// Therefore, an object of type Unique_handle const must not be usable with library functions
	operator _Handle() noexcept { 
	    IF_DEBUG(ASSERT(handle.has_value()));
	    return *handle; 
	}

	_Handle get_handle() noexcept { 
	    IF_DEBUG(ASSERT(handle.has_value()));
	    return *handle; 
	}

	bool has_value() const noexcept { 
	    return handle.has_value(); 
	}

	virtual ~Unique_handle() {
	    if(handle.has_value()) deleter(*handle);
	}

	Unique_handle(Unique_handle const&) = delete;
	Unique_handle& operator=(Unique_handle const&) = delete;

	Unique_handle(Unique_handle&& _other):
	    handle(std::exchange(_other.handle, std::nullopt)),
	    deleter(std::exchange(_other.deleter, nullptr))
	{ }
	
	Unique_handle& operator=(Unique_handle&& _other) {
	    if(this != &_other) {
		if(handle.has_value()) deleter(*handle);
		handle = std::exchange(_other.handle, std::nullopt);
		deleter = std::exchange(_other.deleter, nullptr);
	    }
	    return *this;
	}
    
    private:
	std::optional<_Handle> handle;
	std::function<void(_Handle)> deleter;

    };
}
