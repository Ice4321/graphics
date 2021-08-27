#ifndef INCLUDED_TESTS_ARRAY_HPP
#define INCLUDED_TESTS_ARRAY_HPP

#include"preprocessor/build.hpp"

#if BUILD_TESTS
    #include<iostream>
    #include"utility/array.hpp"

    namespace Tests {
	namespace Array {

	    constexpr int f() {
		{
		    ::Array<int> a(5);
		    a.construct(1, 1234);
		    a.construct(1, 1234);
		    a.destroy(1);
		    a.destroy(1);
		}

		{
		    ::Array<int> a(5);
		    ::Array<int> const ac(5);
		    a.construct(3, 4321);
		    int x1 = a[3];
		    int x2 = std::move(a)[3];
		    int x3 = ac[3];
		    //int x4 = std::move(ac)[3];
		    
		}

		return 5;

	    }

	    constexpr int a = f();
	}
    }

#endif

#endif
