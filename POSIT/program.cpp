#define _USE_MATH_DEFINES 
#include <cmath>
#include <complex>

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <fstream>

#include "posit/posit.h"

using namespace std;

double stdev(double a, double b)
{
	return sqrt(a*a + b*b);
}

void complex_mul_test()
{
	cout << "********************" << endl;
	cout << "* complex mul test *" << endl;
	cout << "********************" << endl;
	cout << endl;

	for (double length : {1., 1e3, 1e6, 1e10})
	{
		for (long count : {4, 100, 1000, 10000, 100000, 1000000})
		{
			cout << std::setprecision(0) << "> length: " << length << endl;
			cout << ">  count: " << count << endl;
			cout << "---------------------------" << endl;

			std::cout << std::setprecision(8) << std::fixed;

			double phi = 2 * M_PI / double(count);
			double cc = cos(phi);
			double ss = sin(phi);

			double err, err_re, err_im;

			//--- double ---
			complex<double> drot(cc, ss);
			complex<double> dvec(length, 0);
			for (long i = 0; i < count; i++)
			{
				dvec *= drot;
			}
			cout << " double error: " << stdev(dvec.real() - length, dvec.imag()) << endl;

			//--- float ---
			complex<float> frot(cc, ss);
			complex<float> fvec(length, 0);
			for (long i = 0; i < count; i++)
			{
				fvec *= frot;
			}
			cout << "  float error: " << stdev(fvec.real() - length, fvec.imag()) << endl;

			//--- posit ---
			complex<Posit32> prot(cc, ss);
			complex<Posit32> pvec(length, 0.);
			for (long i = 0; i < count; i++)
			{
				pvec *= prot;
			}
			cout << "Posit32 error: " << stdev(pvec.real().getDouble() - length, pvec.imag().getDouble()) << endl;

			cout << endl;
		}
	}
}

template< typename T >
T padesin(T x)
{
	T xx = x*x;
	return
		(x*(T(363275871831577908403200.) +
			xx*(-T(54839355237791393068800.) +
				xx*(T(2120649063015013090560.) +
					xx*(-T(31712777908498486800.) +
						xx*(T(203385425766914820.) -
							T(481959816488503.) * xx)))))) /
							(T(363275871831577908403200.) +
								xx*(T(5706623400804924998400.) +
									xx*(T(44454031219351353600.) + xx*
									(T(219578286347980560.) +
										xx*(T(707177798947620.) +
											T(1230626892363.) * xx)))));
}

void rational_poly_test()
{
	cout << "**********************" << endl;
	cout << "* rational poly test *" << endl;
	cout << "**********************" << endl;
	cout << endl;

	cout << std::setprecision(16) << std::defaultfloat;
	for (double x : {0.5, 1., 2., 3., 4., 5.})
	{
		cout << "    x = " << x << endl;
		cout << " -------------------------" << endl;

		cout << "sin(x): " << sin(x) << endl;
		cout << "double: " << padesin<double>(x) << endl;
		cout << " float: " << padesin<float>(x) << endl;
		cout << " posit: " << (padesin<Posit32>(x)).getDouble() << endl;
		cout << endl;
	}
	cout << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	complex_mul_test();
	rational_poly_test();
	return 0;
}
