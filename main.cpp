#include <iostream>
#include "fractions.hpp"

using namespace std;

int main()
{
	constexpr Fraction<int> F{4,2};

	int arr[F.get_num()];

	cout << "Reducted(F) = " << reducted(F) << endl;

	Fraction<int> A;
	cout << "Enter fraction A:" << endl;
	cin >> A;

	Fraction<int> B;
	cout << "Enter fraction B:" << endl;
	cin >> B;

	int num{0};
	cout << "Enter an integer:" << endl;
	cin >> num;

	cout << "A = " << A << endl;
	cout << "B = " << B << endl;

	cout << "+A = " << +A << endl;
	cout << "-B = " << -B << endl;

	cout << "A + B = " 	<< A + B << endl;
	cout << "A + num = " << A + num << endl;

	cout << "A - B = " 	<< A - B << endl;
	cout << "A - num = " << A - num << endl;

	cout << "A * B = " << A * B << endl;
	cout << "A * num = " << A * num << endl;

	cout << "A / B = " << A / B << endl;
	cout << "A / num = " << A / num << endl;

	cout << A << endl;
	cout << "(double)A = " << (double)A << endl;
	cout << "(float)B = " << (float)B << endl;
	cout << "(long double)F = " << (long double)F << endl;
	cout << std::boolalpha;
	cout << "(bool)A = " << (bool)A << endl;

	return 0;
}