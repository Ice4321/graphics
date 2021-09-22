#ifndef INCLUDED_UTILITY_UNIQUE_HANDLE_HPP
#define INCLUDED_UTILITY_UNIQUE_HANDLE_HPP

#include<optional>
#include<functional>
#include<utility>
#include"preprocessor/macros.hpp"
#include "utility/assert.hpp"

namespace Utility {

    // RAII wrapper for a generic, non-nullable handle type
    
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
	operator _Handle& () & noexcept { 
	    IF_DEBUG(ASSERT(handle.has_value()));
	    return *handle; 
	}

	operator _Handle&& () && noexcept { 
	    IF_DEBUG(ASSERT(handle.has_value()));
	    return std::move(*handle);
	}

	bool has_value() const noexcept { return handle.has_value(); }

	virtual ~Unique_handle() {
	    if(handle.has_value()) deleter(*handle);
	}

	Unique_handle(Unique_handle const&) = delete;
	Unique_handle& operator=(Unique_handle const&) = delete;

	Unique_handle(Unique_handle&& _other):
	    handle(std::exchange(_other.handle, std::nullopt)),
	    deleter(std::exchange(_other.deleter, nullptr))
	{ }
	
	// See example under [basic.life#8]
	//
	// Is the this pointer valid after calling dtor in a member function?
	// Suppress dynamic dispatch of the destructor
	//static_cast<Unique_handle*>(this)->Unique_handle::~Unique_handle();
	//this->Unique_handle::~Unique_handle();
	// TODO: Not transparently-replceable, potentially-overlapping subobject?
	//new (this) Unique_handle(std::move(_other));
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

#endif
