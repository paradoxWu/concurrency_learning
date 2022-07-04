# include <iostream>
# include <thread>
# include <mutex>

std::mutex mtx;

int globalVariable = 0;

void task1(){
    for(int i =0;i<100000;i++){
        // std::lock_guard<std::mutex> lock(mtx);
        std::unique_lock<std::mutex> lock(mtx);
        globalVariable++;
        globalVariable--;
    }
}

int main()
{
    std::thread t1(task1);
    std::thread t2(task1);
    t1.join();
    t2.join();
    std::cout<<globalVariable<<std::endl;
    return 0;
}