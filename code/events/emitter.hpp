#pragma once

#include <concepts>
#include <stdexcept>
#include <type_traits>
#include <memory>
#include <list>
#include <vector>
#include <ranges>
#include "utility/assert.hpp"

namespace Internal {
    namespace Event_emitter_impl {
	template<typename _Type>
	concept No_cvref = std::same_as<_Type, typename std::remove_cvref<_Type>::type>;

	template<typename _Type>
	concept Decayed = std::same_as<_Type, typename std::decay<_Type>::type>;

	template<typename _Argument> requires No_cvref<_Argument>
	class Callable_wrapper_base {
	public:
	    virtual void invoke(_Argument& _argument) = 0;
	    virtual void invoke(_Argument&& _argument) = 0;
	    virtual void invoke(_Argument const& _argument) = 0;
	    virtual void invoke(_Argument const&& _argument) = 0;

	    virtual ~Callable_wrapper_base() = default;
	};


	template<typename _Argument, typename _Callable> requires No_cvref<_Argument> && Decayed<_Callable>
	class Callable_wrapper: public Callable_wrapper_base<_Argument> {
	public:
	    Callable_wrapper(auto&& _callable): callable(std::forward<decltype(_callable)>(_callable)) { }
	    
	    virtual void invoke(_Argument& _argument) override {
		if constexpr(std::invocable<_Callable&, decltype(_argument)>) callable(std::forward<decltype(_argument)>(_argument));
		else ASSERT(false && "Incorrect value category");
	    }

	    virtual void invoke(_Argument&& _argument) override {
		if constexpr(std::invocable<_Callable&, decltype(_argument)>) callable(std::forward<decltype(_argument)>(_argument));
		else ASSERT(false && "Incorrect value category");
	    }

	    virtual void invoke(_Argument const& _argument) override {
		if constexpr(std::invocable<_Callable&, decltype(_argument)>) callable(std::forward<decltype(_argument)>(_argument));
		else ASSERT(false && "Incorrect value category");
	    }

	    virtual void invoke(_Argument const&& _argument) override {
		if constexpr(std::invocable<_Callable&, decltype(_argument)>) callable(std::forward<decltype(_argument)>(_argument));
		else ASSERT(false && "Incorrect value category");
	    }

	    virtual ~Callable_wrapper() override = default;

	    Callable_wrapper(Callable_wrapper const&) = delete;
	    Callable_wrapper& operator=(Callable_wrapper const&) = delete;

	private:
	    _Callable callable;

	};

	
	template<typename _Argument> requires No_cvref<_Argument>
	class Callable final {
	public:
	    typedef _Argument Argument_type;

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
	    
	    // This is needed to avoid recursion if the main constructor is selected
	    Callable(Callable&&) = default;
	    Callable& operator=(Callable&&) = default;

	private:
	    std::unique_ptr<Callable_wrapper_base<_Argument>> callable;

	};
	
	// &&-qualified operator()s need not be considered, because the invoked callable will be an l-value
	// Exception specification is part of the function type
	template<typename> struct Callable_ctad_helper;
	template<typename _C, typename _A, typename _R> struct Callable_ctad_helper<_R(_C::*)(_A) > { typedef std::remove_cvref_t<_A> Argument; };
	template<typename _C, typename _A, typename _R> struct Callable_ctad_helper<_R(_C::*)(_A) & > { typedef std::remove_cvref_t<_A> Argument; };
	template<typename _C, typename _A, typename _R> struct Callable_ctad_helper<_R(_C::*)(_A) const > { typedef std::remove_cvref_t<_A> Argument; };
	template<typename _C, typename _A, typename _R> struct Callable_ctad_helper<_R(_C::*)(_A) const & > { typedef std::remove_cvref_t<_A> Argument; };
	template<typename _C, typename _A, typename _R> struct Callable_ctad_helper<_R(_C::*)(_A) noexcept> { typedef std::remove_cvref_t<_A> Argument; };
	template<typename _C, typename _A, typename _R> struct Callable_ctad_helper<_R(_C::*)(_A) & noexcept> { typedef std::remove_cvref_t<_A> Argument; };
	template<typename _C, typename _A, typename _R> struct Callable_ctad_helper<_R(_C::*)(_A) const noexcept> { typedef std::remove_cvref_t<_A> Argument; };
	template<typename _C, typename _A, typename _R> struct Callable_ctad_helper<_R(_C::*)(_A) const & noexcept> { typedef std::remove_cvref_t<_A> Argument; };
	
	// noexcept function pointers are convertible to throwing function pointers
	template<typename _R, typename _A> Callable(_R(*)(_A)) -> Callable<typename std::remove_cvref<_A>::type>;
	template<typename _C> Callable(_C) -> Callable<typename Callable_ctad_helper<decltype(&_C::operator())>::Argument>;



	template<typename _Event> requires No_cvref<_Event>
	class Event_emitter_base {
	private:
	    // std::list never invalidates iterators
	    std::list<Callable<_Event>> callbacks;

	    typedef typename decltype(callbacks)::const_iterator Callback_handle;

	    std::vector<Callback_handle> callbacks_for_removal;

	protected:
	    Event_emitter_base() = default;

	    Callback_handle add_callback(Callable<_Event>&& _callback) {
		return callbacks.emplace(std::cend(callbacks), std::move(_callback));
	    }

	    void remove_callback(Callback_handle const& _handle) {
		callbacks_for_removal.emplace_back(_handle);
	    }

	    void post_event(auto&& _event) {
		// This allows callbacks to register and deregister other callbacks or themselves
		// If that is done, changes are only visible in future calls to post_event()

		for(auto const& callback : callbacks_for_removal) callbacks.erase(callback);
		callbacks_for_removal.clear();

		// This will only iterate over the original range of callbacks, whether or not this range changes during iteration
		for(auto& callback : callbacks | std::views::take(std::ssize(callbacks))) callback.invoke(std::forward<decltype(_event)>(_event));
	    }

	    ~Event_emitter_base() = default;

	    Event_emitter_base(Event_emitter_base const&) = delete;
	    Event_emitter_base& operator= (Event_emitter_base const&) = delete;

	};
    }
}


template<typename... _Events> requires (... && Internal::Event_emitter_impl::No_cvref<_Events>)
class Event_emitter: private Internal::Event_emitter_impl::Event_emitter_base<_Events>... {
public:
    // Argument type is deduced in Callable's constructor
    template<typename _Callback>
    decltype(auto) add_event_callback(_Callback&& _callback) {
	return add_callback(Internal::Event_emitter_impl::Callable(std::forward<_Callback>(_callback)));
    }

    // Argument type provided explicitly
    template<typename _Event, typename _Callback> requires Internal::Event_emitter_impl::No_cvref<_Event>
    decltype(auto) add_event_callback(_Callback&& _callback) {
	return add_callback(Internal::Event_emitter_impl::Callable<_Event>(std::forward<_Callback>(_callback)));
    }

    void remove_event_callback(auto const& _handle) {
	remove_callback(_handle);
    }

protected:
    Event_emitter() = default;

    void post_event(auto&& _event) {
	Internal::Event_emitter_impl::Event_emitter_base<typename std::remove_cvref<decltype(_event)>::type>::post_event(std::forward<decltype(_event)>(_event));
    }

    Event_emitter(Event_emitter const&) = delete;
    Event_emitter& operator= (Event_emitter const&) = delete;

private:
    using Internal::Event_emitter_impl::Event_emitter_base<_Events>::add_callback...;
    using Internal::Event_emitter_impl::Event_emitter_base<_Events>::remove_callback...;

};


