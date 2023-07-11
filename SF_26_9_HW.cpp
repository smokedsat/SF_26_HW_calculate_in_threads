#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <iostream>
#include <chrono>

class SumArray
{
    private:
        int size;
        int num_threads;
        std::vector<int> values;
        int result;

        void calc_sum_part(int l_index, int r_index, int index)
        {
            int s = 0;
            while (l_index <= r_index)
            {
                s += values[l_index++];
            }
            result += s;
        }

    public:
        SumArray(int _size, int _num_threads) : size(_size), num_threads(_num_threads)
        {
            const int min_value = 0;
            const int max_value = 10;
            srand(std::time(0));

            for(int i =0; i < size; i++)
            {
                values.push_back(min_value + rand() %(max_value - min_value + 1));
            }
        }

        ~SumArray()
        {

        }

        int calculate_using_threads()
        {
            std::vector<std::thread> threads;

            int l_index = 0;
            int r_index = 0;

            int elements_count = size/ num_threads;
            result = 0;

            for(int i =0 ;i < num_threads; i++)
            {
                if(i == num_threads - 1)
                {
                    r_index = values.size();
                }
                else
                {
                    r_index += elements_count;
                }

                std::thread t(&SumArray::calc_sum_part, this, l_index,r_index -1, i);
                threads.push_back(std::move(t));
                l_index += elements_count;
            }

            for(auto & t : threads)
            {
                if(t.joinable())
                {
                    t.join();
                }
            }

            return result;
        }

        int calculate_without_threads()
        {
            int s = 0;
            for (int i = 0; i < values.size(); i++)
            {
                s += values[i];
            }
            return s;
        }

        void printArray()
        {
             std::cout << "Array:" << std::endl;
            for (int i = 0; i < size; i++)
                std::cout << values[i] << " ";
            std::cout << "\n" << std::endl;
        }
};



using namespace std::chrono;

int main()
{
    const int array_size = 10000000;
    const int number_threads = 5;
    SumArray sum(array_size, number_threads);
    //sum.printArray();

    //calsulate sum using threads
    auto t_start = system_clock::now();
    int s = sum.calculate_using_threads();
    auto t_end = system_clock::now();
    long diff = duration_cast<milliseconds>(t_end - t_start).count();
    std::cout << "SUM (using threads: " << number_threads << ") is " << s << "  Time (ms): " << diff << std::endl;

    //calculate sum in main thread
    t_start = system_clock::now();
    s = sum.calculate_without_threads();
    t_end = system_clock::now();
    diff = duration_cast<milliseconds>(t_end - t_start).count();
    std::cout << "SUM (main thread) is " << s << "  Time (ms): " << diff << std::endl;

    return 0;
}