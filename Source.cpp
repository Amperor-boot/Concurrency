#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std::chrono_literals;
std::atomic_int8_t count(1);

void client_thread(uint8_t clients)
{
	while (count < clients)
	{
		count.fetch_add(1, std::memory_order_relaxed);
		std::this_thread::sleep_for(1s);
		std::cout << count + 1 << " clients in line." << std::endl;
	}
}

void operator_thread()
{
	while (count > 0)
	{
		count.fetch_sub(1, std::memory_order_relaxed);
		std::this_thread::sleep_for(2s);
		if (count > 0)
		{
			std::cout << "\toperator has served the client" << std::endl;
			std::cout << count - 1 << " clients in line." << std::endl;
		}
		else
		{
			break;
		}
	}
}

void task_1(uint8_t clients)
{
	std::thread client_t(client_thread, clients);
	std::thread operator_t(operator_thread);

	client_t.join();
	operator_t.join();

	system("cls");
}

static void sum_vectors(double sum_of_vectors, const std::vector<int> new_vector1, const std::vector<int> new_vector2)
{
	for (int i = 0; i < new_vector1.size(); i++)
		sum_of_vectors += new_vector1.at(i) + new_vector2.at(i);
}

void task_2()
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

			auto run = std::chrono::high_resolution_clock::now();
			for (auto& it : threads)
				it.join();
			auto end = std::chrono::high_resolution_clock::now();

			std::chrono::duration<double, std::milli> total_time = end - run;
			std::cout << std::setw(16) << total_time.count() << "ms";
		}

		std::cout << "\n" << std::endl;
	}
}

int main(int argc, char** argv)
{
	task_1(5);
	task_2();
	return EXIT_SUCCESS;
}
