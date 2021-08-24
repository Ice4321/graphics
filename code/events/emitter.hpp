#ifndef INCLUDED_EVENTS_EMITTER_HPP
#define INCLUDED_EVENTS_EMITTER_HPP

#include<concepts>
#include<stdexcept>
#include<type_traits>
#include<memory>

namespace Internal {
    namespace Event_emitter_impl {
	template<typename _Type>
	concept Remove_cvref = std::same_as<_Type, typename std::remove_cvref<_Type>::type>;


	template<Remove_cvref _Argument> 
	class Callable_wrapper_base {
	public:
	    virtual void invoke(_Argument& _argument) = 0;
	    virtual void invoke(_Argument&& _argument) = 0;
	    virtual void invoke(_Argument const& _argument) = 0;
	    virtual void invoke(_Argument const&& _argument) = 0;

	    virtual ~Callable_wrapper_base() = default;
	};


	template<Remove_cvref _Argument, typename _Callable>
	class Callable_wrapper: public Callable_wrapper_base<_Argument> {
	public:
	    Callable_wrapper(auto&& _callable): callable(std::forward<decltype(_callable)>(_callable)) { }
	    
	    virtual void invoke(_Argument& _argument) override {
		if constexpr(std::invocable<_Callable&, decltype(_argument)>) callable(std::forward<decltype(_argument)>(_argument));
		else throw std::logic_error("Internal::Event_emitter_impl::Callable_wrapper::invoke(): invalid value category");
	    }

	    virtual void invoke(_Argument&& _argument) override {
		if constexpr(std::invocable<_Callable&, decltype(_argument)>) callable(std::forward<decltype(_argument)>(_argument));
		else throw std::logic_error("Internal::Event_emitter_impl::Callable_wrapper::invoke(): invalid value category");
	    }

	    virtual void invoke(_Argument const& _argument) override {
		if constexpr(std::invocable<_Callable&, decltype(_argument)>) callable(std::forward<decltype(_argument)>(_argument));
		else throw std::logic_error("Internal::Event_emitter_impl::Callable_wrapper::invoke(): invalid value category");
	    }

	    virtual void invoke(_Argument const&& _argument) override {
		if constexpr(std::invocable<_Callable&, decltype(_argument)>) callable(std::forward<decltype(_argument)>(_argument));
		else throw std::logic_error("Internal::Event_emitter_impl::Callable_wrapper::invoke(): invalid value category");
	    }

	    virtual ~Callable_wrapper() override = default;

	    Callable_wrapper(Callable_wrapper const&) = delete;
	    Callable_wrapper& operator=(Callable_wrapper const&) = delete;

	private:
	    _Callable callable;

	};

	
	template<Remove_cvref _Argument>
	class Callable {
	public:
	    Callable(auto&& _callable): 
		callable(
		    std::make_unique<Callable_wrapper<_Argument, typename std::decay<decltype(_callable)>::type>>(
			std::forward<decltype(_callable)>(_callable)
		    )
		)
	    { }

	    void invoke(auto&& _argument) {
		callable->invoke(std::forward<decltype(_argument)>(_argument));
	    }

	private:
	    std::unique_ptr<Callable_wrapper_base<_Argument>> callable;

	};



	template<typename> struct Callable_ctad_helper;
	template<typename _C, typename _A, typename _R, bool _e> struct Callable_ctad_helper<_R(_C::*)(_A) noexcept(_e)> { typedef std::remove_cvref_t<_A> Argument; };
	template<typename _C, typename _A, typename _R, bool _e> struct Callable_ctad_helper<_R(_C::*)(_A) const noexcept(_e)> { typedef std::remove_cvref_t<_A> Argument; };
	template<typename _C, typename _A, typename _R, bool _e> struct Callable_ctad_helper<_R(_C::*)(_A) & noexcept(_e)> { typedef std::remove_cvref_t<_A> Argument; };
	template<typename _C, typename _A, typename _R, bool _e> struct Callable_ctad_helper<_R(_C::*)(_A) const & noexcept(_e)> { typedef std::remove_cvref_t<_A> Argument; };
	template<typename _C, typename _A, typename _R, bool _e> struct Callable_ctad_helper<_R(_C::*)(_A) && noexcept(_e)> { typedef std::remove_cvref_t<_A> Argument; };
	template<typename _C, typename _A, typename _R, bool _e> struct Callable_ctad_helper<_R(_C::*)(_A) const && noexcept(_e)> { typedef std::remove_cvref_t<_A> Argument; };
	
	template<typename _R, typename _A> Callable(_R(*)(_A)) -> Callable<typename std::remove_cvref<_A>::type>;
	template<typename _C> Callable(_C) -> Callable<typename Callable_ctad_helper<decltype(&_C::operator())>::Argument>;

    }
}



#endif
