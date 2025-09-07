#pragma once

namespace sugoma::core 
{
	class Algorithm 
	{
	public:
		template<typename T>
		static T GCD(T a, T b) 
		{
			while (b != 0) {
				T temp = b;
				b = a % b;
				a = temp;
			}
			return a;
		}
	};
}