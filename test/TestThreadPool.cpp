
#include <gtest/gtest.h>
#include "ThreadPool.hpp"

using com::thread::Pool;

void printVal(void)
{
    static int val = 1;
    std::cout << "Thread job: " << val++ << std::endl;
}

TEST(ThreadPool, BasicTest)
{
    Pool p(1);

    for (auto i = 0; i < 5; ++i)
    {
        p.add(printVal);
    }

    std::cout << "Pool is healthy? " << std::boolalpha << p.isHealthy() << std::endl;

    while (p.getsize() > 0)
    {
        std::cout << "Pool has " << p.getsize() << " jobs left" << std::endl;
        // wait for threads to execute
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

auto main(int argc, char* argv[]) -> int 
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}