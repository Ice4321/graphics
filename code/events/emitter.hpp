#ifndef INCLUDED_EVENTS_EMITTER_HPP
#define INCLUDED_EVENTS_EMITTER_HPP

#include<concepts>
#include<stdexcept>

namespace Internal {
    namespace Event_emitter_impl {
	template<typename _Argument>
	class Callable_wrapper_base {
	public:
	    virtual void invoke(_Argument& _argument) = 0;
	    virtual void invoke(_Argument&& _argument) = 0;
	    virtual void invoke(_Argument const& _argument) = 0;
	    virtual void invoke(_Argument const&& _argument) = 0;

	    virtual ~Callable_wrapper_base() = default;
	};


	template<typename _Argument, typename _Callable>
	class Callable_wrapper {
	public:
	    Callable_wrapper(auto&& _callable): callable(std::forward<decltype(_callable)>(_callable)) { }
	    
	    //TODO: compile-time error if wrong value category
	    //
	    virtual void invoke(_Argument& _argument) override {
		typedef decltype(_argument) Argument_type;
		if constexpr(std::invocable<_Callable&, Argument_type>) callable(std::forward<Argument_type>(_argument));
		else throw std::logic_error("Internal::Event_emitter_impl::Callable_wrapper::invoke(): invalid value category");
	    }

	    virtual void invoke(_Argument&& _argument) override {
		typedef decltype(_argument) Argument_type;
		if constexpr(std::invocable<_Callable&, Argument_type>) callable(std::forward<Argument_type>(_argument));
		else throw std::logic_error("Internal::Event_emitter_impl::Callable_wrapper::invoke(): invalid value category");
	    }

	    virtual void invoke(_Argument const& _argument) override {
		typedef decltype(_argument) Argument_type;
		if constexpr(std::invocable<_Callable&, Argument_type>) callable(std::forward<Argument_type>(_argument));
		else throw std::logic_error("Internal::Event_emitter_impl::Callable_wrapper::invoke(): invalid value category");
	    }

	    virtual void invoke(_Argument const&& _argument) override {
		typedef decltype(_argument) Argument_type;
		if constexpr(std::invocable<_Callable&, Argument_type>) callable(std::forward<Argument_type>(_argument));
		else throw std::logic_error("Internal::Event_emitter_impl::Callable_wrapper::invoke(): invalid value category");
	    }

	    //TODO: Destructor necessary?

	private:
	    _Callable callable;
	

	};
    }
}



#endif
