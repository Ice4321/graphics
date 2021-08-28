#ifndef INCLUDED_UTILITY_ARRAY_HPP
#define INCLUDED_UTILITY_ARRAY_HPP

#include<memory>
#include<type_traits>
#include<utility>

// TODO: More comments!
// TODO: Add iterators

// Exception specification is given assuming valid parameters are passed
template<typename _Element>
class Array {
private:
    union Element_union {
	_Element element;

	constexpr Element_union() noexcept { }
	constexpr ~Element_union() noexcept { }
    };

    std::size_t size;
    Element_union* data;
    bool* constructed;


public:
    constexpr Array(std::size_t _size):
	size(_size)
    { 
	// Default init
	data = ::new Element_union[size];
	try {
	    // Value init
	    constructed = ::new bool[size]();
	} catch(...) {
	    delete[] data;
	    throw;
	}
    }

    constexpr void construct(std::size_t _index, auto&&... _arguments) 
        noexcept(
            std::is_nothrow_constructible<_Element, decltype(_arguments)...>::value &&
	    std::is_nothrow_destructible<_Element>::value
	)
    {
        destroy(_index);
        std::construct_at(&data[_index].element, std::forward<decltype(_arguments)>(_arguments)...);
        constructed[_index] = true;
    }

    constexpr void destroy(std::size_t _index)
        noexcept(std::is_nothrow_destructible<_Element>::value) 
    {
        if(constructed[_index]) {
	    // Lifetime ends when the destructor call starts [basic.life]
	    constructed[_index] = false;
	    data[_index].element.~_Element();
	}
    }

    constexpr bool is_constructed(std::size_t _index) const noexcept { return constructed[_index]; }

    constexpr _Element& operator[](std::size_t _index) & noexcept { return data[_index].element; }
    constexpr _Element const& operator[](std::size_t _index) const & noexcept { return data[_index].element; }
    constexpr _Element&& operator[](std::size_t _index) && noexcept { return std::move(data[_index].element); }
    constexpr _Element const&& operator[](std::size_t _index) const && noexcept { return std::move(data[_index].element); }

    constexpr void swap(Array& _other) noexcept {
	std::swap(size, _other.size);
	std::swap(data, _other.data);
	std::swap(constructed, _other.constructed);
    }
    
    constexpr ~Array() noexcept requires std::is_nothrow_destructible<_Element>::value {
	// If this has been moved from, size is 0
        for(std::size_t index = 0; index < size; ++index) destroy(index);
	// Safe to delete nullptr if this has been moved from
	delete[] constructed;
	delete[] data;
    }

    constexpr Array(const Array& _other):
        Array(_other.size)
    {
        for(std::size_t index = 0; index < size; ++index) {
	    // If .construct() throws, ~Array is called here because this is a delegating ctor
            if(_other.constructed[index]) construct(_other[index]);
        }
    }

    
    constexpr Array& operator=(const Array& _other) {
	if(this != &_other) {
	    Array temp(_other);
	    swap(temp);
	}

	return *this;
    }
    
    constexpr Array(Array&& _other) noexcept:
	size(std::exchange(_other.size, 0)),
	data(std::exchange(_other.data, nullptr)),
	constructed(std::exchange(_other.constructed, nullptr))
    { }
    
    constexpr Array& operator=(Array&& _other) noexcept {
	if(this != &_other) {
	    Array temp(std::move(_other));
	    swap(temp);
	}

	return *this;
    }

};

template<typename _Element>
constexpr void swap(Array<_Element>& _first, Array<_Element> _second) noexcept {
    _first.swap(_second);
}


#endif
