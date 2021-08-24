#ifndef INCLUDED_TESTS_CALLBACK_HPP
#define INCLUDED_TESTS_CALLBACK_HPP

#include"preprocessor/build.hpp"

#if BUILD_TESTS
    #include"events/emitter.hpp"
    #include<concepts>
    #include<functional>

    namespace Tests {
	namespace Callback {
	    struct X {}; struct Y {};
	    struct Callable1 { void operator()(int) {} };
	    struct Callable2 { void operator()(int&) const & {} };
	    struct Callable3 { void operator()(int const&&) const && noexcept {} };
	    struct Callable4 { 
		void operator()(X) noexcept {}
		void operator()(Y) noexcept {} 
	    };
	    double callable5(double) {}
	    double callable6(double) noexcept {}

	    void run() {
		Callable1 ca1{};
		Internal::Event_emitter_impl::Callable c1(ca1);
		static_assert(std::same_as<decltype(c1), Internal::Event_emitter_impl::Callable<int>>);
		Internal::Event_emitter_impl::Callable c2(std::move(ca1));
		static_assert(std::same_as<decltype(c2), Internal::Event_emitter_impl::Callable<int>>);

		Callable2 const ca2{};
		Internal::Event_emitter_impl::Callable c3(ca2);
		static_assert(std::same_as<decltype(c3), Internal::Event_emitter_impl::Callable<int>>);
		Internal::Event_emitter_impl::Callable c4(std::move(ca2));
		static_assert(std::same_as<decltype(c4), Internal::Event_emitter_impl::Callable<int>>);
		Internal::Event_emitter_impl::Callable c5(std::move(ca2));
		static_assert(std::same_as<decltype(c5), Internal::Event_emitter_impl::Callable<int>>);

		Callable2 const ca3{};
		Internal::Event_emitter_impl::Callable c6(ca3);
		static_assert(std::same_as<decltype(c6), Internal::Event_emitter_impl::Callable<int>>);
		Internal::Event_emitter_impl::Callable c7(Callable3{});
		static_assert(std::same_as<decltype(c7), Internal::Event_emitter_impl::Callable<int>>);

		Internal::Event_emitter_impl::Callable c8(callable5);
		static_assert(std::same_as<decltype(c8), Internal::Event_emitter_impl::Callable<double>>);
		Internal::Event_emitter_impl::Callable c9(callable6);
		static_assert(std::same_as<decltype(c9), Internal::Event_emitter_impl::Callable<double>>);

		Internal::Event_emitter_impl::Callable c10([](float){});
		static_assert(std::same_as<decltype(c10), Internal::Event_emitter_impl::Callable<float>>);
		Internal::Event_emitter_impl::Callable c11([](float) mutable noexcept -> void {});
		static_assert(std::same_as<decltype(c11), Internal::Event_emitter_impl::Callable<float>>);
		
		auto b = std::bind_front([](double, double){}, 1.0);
		Internal::Event_emitter_impl::Callable<double> c13(b);
	    }
	}
    }
#endif

#endif
