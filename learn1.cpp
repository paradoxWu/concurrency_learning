# include <iostream>
# include <thread>

void printHello()
{
    for(int i = 0;i<10;i++){
        std::cout<<"hello, concurrency!"<<std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

void printEnd(int n,const std::string word)
{
    for(int i = 0;i<n;i++){
        std::cout<<word<<std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

void calbyValue(int n)
{
    n++;
}

void calbyRef(int &n)
{
    n++;
}

void test1()
{
    std::thread thread1(printHello); //此时线程已经启动
    // thread1.detach(); //子线程完全分离，不会阻塞，少用
    std::thread thread2(printEnd,5,"End,Have a break");
    std::cout<<"thread id:"<<thread1.get_id();
    std::cout<<"Support thread number:"<<thread1.hardware_concurrency();
    thread1.join();     //等待子线程执行完毕，会阻塞在这里
    thread2.join();
}

void test2()
{
    int n = 10;
    std::thread thread1(calbyValue,n);
    thread1.join();
    std::cout<<n<<std::endl;
    // std::thread thread2(calbyRef,n);
    // thread2.join();
    // std::cout<<n<<std::endl;
    std::thread thread3(calbyRef,std::ref(n));
    thread3.join();
    std::cout<<n<<std::endl;
}


int main()
{
    // test1();
    test2();
    return 0;
}