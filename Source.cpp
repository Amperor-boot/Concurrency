#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>

static void sum_vectors(double sum_of_vectors, const std::vector<int> new_vector1, const std::vector<int> new_vector2)
{
	for (int i = 0; i < new_vector1.size(); i++)
		sum_of_vectors += new_vector1.at(i) + new_vector2.at(i);
}

int main(int argc, char** argv)
{
	std::cout << "Hardware cores:\t" << std::thread::hardware_concurrency() << std::endl;
	std::cout << std::setw(12) << "\t\t\t10^3\t\t10^4\t\t10^5\t\t10^6" << std::endl;
	std::vector<int> vector1;
	std::vector<int> vector2;
	std::vector<int> cnt_of_threads{ 1, 2, 4, 8, 16 };
	std::vector<int> size_of_vector{ 1000, 10000, 100000, 1'000'000 };
	
	for (auto &count_of_threads : cnt_of_threads)
	{
		if (count_of_threads == 1)
			std::cout << std::endl << count_of_threads << " thread";
		else
			std::cout << std::endl << count_of_threads << " threads";
		
		for (auto& count_vector : size_of_vector)
		{
			vector1.resize(count_vector, 1);
			vector2.resize(count_vector, 2);
			std::vector<std::thread> threads;
			double sum_of_vectors(0);

			int part_of_the_size = static_cast<int>(count_vector / count_of_threads);

			auto run = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < count_of_threads; i++)
			{
				std::vector<int> new_vector1;
				std::vector<int> new_vector2;

				int right_border(0);

				if (i != count_of_threads - 1)
					right_border = part_of_the_size * (i + 1);
				else
					right_border = count_vector;
				
				for (int j = part_of_the_size * i; j < right_border; ++j)
				{
					new_vector1.push_back(vector1.at(j));
					new_vector2.push_back(vector2.at(j));
				}
				threads.push_back(std::thread(sum_vectors, std::ref(sum_of_vectors), new_vector1, new_vector2));
			}

			for (auto& it : threads)
				it.join();

			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> total_time = end - run;
			std::cout << std::setw(16) << total_time.count() << "ms";
		}

		std::cout << "\n" << std::endl;
	}

	return EXIT_SUCCESS;
}
