#include <iostream>
#include <cmath>
#include <string>
#include <gmp.h>
#include <gmpxx.h>
#include <vector>
#include <thread>
#include <mutex>


#ifndef THREADS_SIZE
#define THREADS_SIZE 4
#endif


int main()
{
	long long int n = 0;
	std::cout << "number: ";
	std::cin >> n;
	mpf_set_default_prec((float)n * 1.5f - (n/989));


	std::vector<mpf_class> PI_buffer{};
	PI_buffer.resize(THREADS_SIZE);
	

	std::thread threads[THREADS_SIZE];

	std::cout << "MAKE THREADS CALC...\n";

	for(int threadId = 0; threadId < THREADS_SIZE; ++threadId)
	{
		threads[threadId] = std::thread([&](int thid) -> int {
				size_t index = thid;

				mpf_class& RES = PI_buffer[thid];
				for(size_t _x = thid; _x < n; _x += THREADS_SIZE, index += THREADS_SIZE)
				{
					mpf_class y((_x&1) == 1 ? -1 : 1);
					mpf_class x((float)_x);

					{
						mpf_class delta(0.0f);
						mpf_class two(2.0f);
						mpf_pow_ui(delta.get_mpf_t(), two.get_mpf_t(), 10.0f*(float)_x);

						y /= delta;
					}

					y *= ( 
							-( mpf_class(32.0f)/(mpf_class(4.0f)*x+mpf_class(1.0f)) )
							-
							( mpf_class(1.0f)/(mpf_class(4.0f)*x+mpf_class(3.0f)) )
							+
							( mpf_class(256.0f)/(mpf_class(10.0f)*x+mpf_class(1.0f)) )
							-
							( mpf_class(64.0f)/(mpf_class(10.0f)*x+mpf_class(3.0f)) )
							-
							( mpf_class(4.0f)/(mpf_class(10.0f)*x+mpf_class(5.0f)) )
							-
							( mpf_class(4.0f)/(mpf_class(10.0f)*x+mpf_class(7.0f)) )
							+
							( mpf_class(1.0f)/(mpf_class(10.0f)*x+mpf_class(9.0f)) )
						);
					RES += y;

					if(thid == 0) {
						system((std::string("title ") + std::to_string(_x*THREADS_SIZE)).c_str());
					}
				}
				return 0;
		}, threadId);
	}


	std::cout << "WAIT THREADS...\n";
	for(int i = THREADS_SIZE-1; i >= 0; --i)
	{
		std::cout << "THREAD[" << i << "] - ";
		if(threads[i].joinable()){
			try{
				threads[i].join();
			} catch(const std::system_error& err)
			{
				std::cout << "ERROR: " << err.what();
			}
		}
		std::cout << "\n";
	}

	std::cout << "SUMMARING...\n";
	
	mpf_class PI (0.0f);
	for(const auto& x: PI_buffer)
	{
		PI += x;
	}

	PI /= mpf_class(64.0f);

	mp_exp_t d;

	std::cout << "VALUE: " << PI.get_str(d, 10, 0) << '\n';
	

	return 0;
}
