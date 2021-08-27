#ifndef INCLUDED_UTILITY_ARRAY_HPP
#define INCLUDED_UTILITY_ARRAY_HPP



/* Old version:

#include<type_traits>
#include<memory>
#include<ranges>

template<typename _Element>
class Array {
private:
    // Unique_ptr equivalent, but constexpr. Only for arrays
    template<typename _Type>
    struct Raii_array_wrapper {
        _Type* array;

        constexpr decltype(auto) operator[](std::size_t _index) noexcept { return array[_index]; }

        constexpr Raii_array_wrapper(_Type* _array) noexcept: array(_array) { }
        constexpr ~Raii_array_wrapper() noexcept { delete[] array; }
        constexpr Raii_array_wrapper(Raii_array_wrapper&& _other) noexcept {
            array = std::exchange(_other.array, nullptr);
        }
        constexpr Raii_array_wrapper& operator=(Raii_array_wrapper&& _other) noexcept {
            delete[] array;
            array = std::exchange(_other.array, nullptr);
        }

    };
    
    union Memory {
    public:
        _Element element;

        constexpr Memory() noexcept { }
        constexpr ~Memory() noexcept { }
    };

    std::size_t size;
    Raii_array_wrapper<bool> constructed;
    Raii_array_wrapper<Memory> memory;

public:
    constexpr Array(std::size_t _size):
        size(_size),
        constructed(::new bool[_size]()), // value-init
        memory(::new Memory[_size]) // default-init
    { }

    constexpr void construct(std::size_t _index, auto&&... _arguments) 
        noexcept(
            std::is_nothrow_constructible<_Element, decltype(_arguments)...>::value &&
            noexcept(destroy(0))
        )
    {
        if(constructed[_index]) destroy(_index);
        std::construct_at<_Element>(&memory[_index].element, std::forward<decltype(_arguments)>(_arguments)...);
        constructed[_index] = true;
    }

    constexpr void destroy(std::size_t _index)
        noexcept(std::is_nothrow_destructible<_Element>::value) 
    {
        if(constructed[_index]) memory[_index].element.~_Element();
        constructed[_index] = false;
    }

    constexpr _Element& operator[](std::size_t _index) noexcept { return memory[_index].element; }
    constexpr _Element const& operator[](std::size_t _index) const noexcept { return memory[_index].element; }

    constexpr ~Array() 
        noexcept(noexcept(destroy(0)))
    {
        // Check if this has been moved from
        if(memory.array) for(std::size_t index = 0; index < size; ++index) destroy(index);
    }

    constexpr Array(const Array& _other):
        Array(_other.size)
    {
        for(std::size_t index = 0; index < size; ++index) {
            if(_other.constructed[index]) construct(_other[index]);
        }
    }

    constexpr Array& operator=(const Array& _other) {
        if(size != _other.size) {
            size = _other.size;
            constructed = ::new bool[size]();
            memory = ::new Memory[size];
        }

        for(std::size_t index = 0; index < size; ++index) {
            if(_other.constructed[index]) construct(_other[index]);
        }
    }

    constexpr Array(Array&& _other) noexcept = default;
    constexpr Array& operator=(Array&& _other) noexcept = default;

};


 */

#endif
