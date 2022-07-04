# include <iostream>
# include <thread>
# include <mutex>
# include <atomic>

std::atomic<int> globalVariable {0};

void task()
{
    for(int i = 0; i < 100000;i++)
    {
        globalVariable++;
        globalVariable--;
    }
}

int main()
{
    std::thread mythread(task);
    mythread.join();
    std::cout<<globalVariable<<std::endl;
}