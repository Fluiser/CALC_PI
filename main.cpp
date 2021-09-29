#include <iostream>
#include <cmath>
#include <string>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <thread>
#include <mutex>

#ifndef MAX_INDEX
#define MAX_INDEX 50
#endif
#ifndef THREADS_SIZE
#define THREADS_SIZE 4
#endif

using float256 = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<MAX_INDEX>>;

int main()
{
	long long int n = 0;
	std::cout << "number: ";
	std::cin >> n;


	std::vector<float256> PI_buffer{};
	PI_buffer.reserve(n);
	std::mutex mtx;

	std::thread threads[THREADS_SIZE];

	std::cout << "MAKE THREADS CALC...\n";

	for(int threadId = 0; threadId < THREADS_SIZE; ++threadId)
	{
		threads[threadId] = std::thread([&](int thid) -> int {
				size_t index = thid;
				for(float256 x = thid; x < n; x += THREADS_SIZE, index += THREADS_SIZE)
				{
					float256 y = pow(-1, x);
					y /= pow(2, 10*x);
					y *= ( 
							-( 32.0f/(4.0f*x+1.0f) )
							-
							( 1.0f/(4.0f*x+3.0f) )
							+
							( 256.0f/(10.0f*x+1.0f) )
							-
							( 64.0f/(10.0f*x+3.0f) )
							-
							( 4.0f/(10.0f*x+5.0f) )
							-
							( 4.0f/(10.0f*x+7.0f) )
							+
							( 1.0f/(10.0f*x+9.0f) )
						);
					//std::lock_guard<std::mutex> __(mtx);
					//PI_buffer.emplace_back(y);
					PI_buffer[index] = y;
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
	
	float256 PI = 0.0f;
	for(const auto& n: PI_buffer)
	{
		PI += n;
	}

	PI *= (1.0f/64.0f);

	std::cout << "VALUE: " << PI.convert_to<std::string>() << "\n";
	system("pause");

	return 0;
}
