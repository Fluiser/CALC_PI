#include <iostream>
#include <cmath>
#include <string>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <thread>
#include <mutex>

#ifndef MAX_INDEX
#define MAX_INDEX (1024*200)
#endif
#ifndef THREADS_SIZE
#define THREADS_SIZE 8
#endif

#define NUMBER 409600

typedef boost::multiprecision::number<boost::multiprecision::cpp_dec_float<MAX_INDEX, int, std::allocator<boost::multiprecision::cpp_dec_float<MAX_INDEX>>>> float256;

struct iter_info {
	size_t per_second = 0;
	size_t taktcount = 0;
};

int main() 
{
	long long int n = NUMBER;



	std::vector<float256> PI_buffer{};
	std::vector<iter_info> iter_info{};
	PI_buffer.reserve(n);
	iter_info.reserve(n);
	for(size_t i = 0; i < THREADS_SIZE; ++i){
		PI_buffer.emplace_back(0); // Fill vector. Because it's drops range error, if element from index not initialized.
		iter_info.emplace_back();
	}
	

	std::thread threads[THREADS_SIZE];

	std::cout << "MAKE THREADS CALC...\n";

	for(int threadId = 0; threadId < THREADS_SIZE; ++threadId)
	{
		threads[threadId] = std::thread([&PI_buffer, &iter_info, &n](int thid) -> int {
				size_t index = thid;
				for(float256 x = thid; x < n; x += THREADS_SIZE)
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
					PI_buffer[index] += y;
					++iter_info[index].per_second;
					++iter_info[index].taktcount;
				}
				return 0;
		}, threadId);
	}


	std::cout << "WAIT THREADS...\n";
	std::thread printer([&iter_info, &threads]{
		std::string title;
		title.reserve(128);
		while(threads[0].joinable())
		{
			size_t tCount = 0;
			size_t tackrate = 0;
			for(auto& it: iter_info){ tCount += it.taktcount; tackrate+= it.per_second; it.per_second = 0;}

			title = "title [";
			title += std::to_string(tCount);
			title += "] ";
			title += std::to_string(tackrate);
			title += "/sec";

			system(title.c_str());

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	});
	for(int i = THREADS_SIZE-1; i >= 0; --i)
	{
		std::cout << "THREAD[" << i << "] - ";
		if(threads[i].joinable()){
			try{
				threads[i].join();
				if(THREADS_SIZE-1 == i) printer.detach();
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

	return 0;
}