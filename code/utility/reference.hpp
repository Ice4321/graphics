#pragma once

#include "preprocessor/macros.hpp"
#include "utility/assert.hpp"
#include <memory>
#include <concepts>

namespace Utility {
    namespace Internal {
	template<typename _Type>
	concept Derived_from_unique_handle = std::derived_from<_Type, ::Utility::Unique_handle<typename _Type::Handle>>;
    }

    // Default-constructible, copyable and assignable reference (similar to std::reference_wrapper)
    template<typename _Object>
    class Reference {
    public:
	IF_DEBUG(
	    constexpr Reference() noexcept: object(nullptr) { }
	)

	IF_RELEASE(
	    Reference() = default;
	)
	
	// Only l-values are allowed
	constexpr Reference(_Object& _object) noexcept: object(std::addressof(_object)) { }
	
	constexpr operator _Object& () const & noexcept { 
	    IF_DEBUG(ASSERT(object)); 
	    return *object; 
	}

	constexpr operator _Object&& () const && noexcept { 
	    IF_DEBUG(ASSERT(object)); 
	    return std::move(*object); 
	}

	constexpr operator auto() noexcept requires Internal::Derived_from_unique_handle<_Object> {
	    IF_DEBUG(ASSERT(object)); 
	    return object->get_handle();
	}


	~Reference() = default;
	Reference(Reference const&) = default;
	Reference& operator=(Reference const&) = default;
	Reference(Reference&&) = default;
	Reference& operator=(Reference&&) = default;

    private:
	_Object* object;

    };
}
