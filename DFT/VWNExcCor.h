#pragma once


#define _USE_MATH_DEFINES
#include <math.h>

namespace DFT {

	// Vosko-Wilk-Nusair 

	// see: http://muchomas.lassp.cornell.edu/P480/Psets/ps5/node37.html
	// but be aware that the derivative from there is wrong

	class VWNExchCor
	{
	public:
		static Eigen::MatrixXcd exc(const Eigen::MatrixXcd& n)
		{
			assert(n.cols() == 1);
			static const double
				X1 = 0.75*pow(3. / (2.*M_PI), 2. / 3.),  /* Exchange energy coeff */
				A = 0.0310907,
				x0 = -0.10498,
				b = 3.72744,
				c = 12.9352,
				Q = sqrt(4 * c - b * b),
				X0 = x0 * x0 + b * x0 + c;

			Eigen::MatrixXcd res(n.rows(), 1);


			for (int i = 0; i < n.rows(); ++i)
			{
				const double ro = n(i, 0).real();

				const double rs = pow(4.*M_PI / 3.*ro, -1. / 3.);

				const double x = sqrt(rs);
				const double X = x * x + b * x + c;

				res(i, 0) = -X1 / rs + A * (log(x*x / X) + 2.*b / Q * atan(Q / (2.*x + b)) - (b*x0) / X0 * (log((x - x0)*(x - x0) / X) + 2.*(2.*x0 + b) / Q * atan(Q / (2.*x + b))));
			}

			return res;
		}

		static Eigen::MatrixXcd excDeriv(const Eigen::MatrixXcd& n)
		{
			assert(n.cols() == 1);
			static const double
				X1 = 0.75*pow(3. / (2.*M_PI), 2. / 3.),  /* Exchange energy coeff */
				A = 0.0310907,
				x0 = -0.10498,
				b = 3.72744,
				c = 12.9352,
				Q = sqrt(4 * c - b * b),
				X0 = x0 * x0 + b * x0 + c;

			Eigen::MatrixXcd res(n.rows(), 1);

			for (int i = 0; i < n.rows(); ++i)
			{
				const double ro = n(i, 0).real();
				const double rs = pow(4.*M_PI / 3.*ro, -1. / 3.);
				const double x = sqrt(rs);
				const double X = x * x + b * x + c;
				const double dx = 0.5 / x;
				res(i, 0) = dx *
					(2.*X1 / (rs*x) + A * (2. / x - (2.*x + b) / X - 4.*b / (Q*Q + (2.*x + b)*(2.*x + b)) - (b*x0) / X0 * (2. / (x - x0) - (2.*x + b) / X - 4.*(2.*x0 + b) / (Q*Q + (2.*x + b)*(2.*x + b)))))
					*(-rs / (3.*ro));
			}

			return res;
		}
	};

}