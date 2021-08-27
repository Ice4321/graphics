#ifndef INCLUDED_TESTS_ARRAY_HPP
#define INCLUDED_TESTS_ARRAY_HPP

#include"preprocessor/build.hpp"

#if BUILD_TESTS
    #include<iostream>
    #include"utility/array.hpp"

    namespace Tests {
	namespace Array {
	    struct Thr {
		Thr() { throw 12; }
	    };

	    void run() {
		// TODO: test exception handling
	    }

	    constexpr int f() {
		{
		    ::Array<int> a(5);
		    a.construct(1, 1234);
		    a.construct(1, 1234);
		    int x;
		    x = a[1];
		    x = std::move(a)[1];
		    x = std::as_const(a)[1];
		    x = std::move(std::as_const(a))[1];
		    (void)x;
		    a.destroy(1);
		    a.destroy(1);
		}

		{
		    ::Array<int> a1(5), a2(3);
		    a1 = a2;
		}
		{
		    ::Array<int> a1(5), a2(3);
		    a1 = std::move(a2);
		}
		{
		    ::Array<int> a1(5);
		    ::Array<int> a2(a1);
		}
		{
		    ::Array<int> a1(5);
		    ::Array<int> a2(std::move(a1));
		}

		return 5;

	    }

	    constexpr int a = f();
	}
    }

#endif

#endif
