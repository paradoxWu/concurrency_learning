#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
#include <string>

#ifndef WITHOUT_CV
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#endif

using namespace std;
using namespace cv;

static const double pi = 3.1415926;

// @param[in]: src_img, size,simga:(gaussian window size and sigma value)
// @param[out]: dst_img
void gaussian_thread(const cv::Mat &src_img, cv::Mat &dst_img,const size_t &size,const double &sigma)
{
    dst_img = cv::Mat::zeros(src_img.size(),CV_8UC1);
    double arr[size*size];
    const auto size_2 = size>>1;
    const auto max_thread = std::thread::hardware_concurrency();
    std::vector<std::thread> thread_bar;
    const auto t_rows = src_img.rows / (max_thread);
    //initialize the gaussian window
    auto getGassianArray = [&]()
    {
        double sum = 0.0;
        auto sigma_2 = sigma * sigma;
        int center = size / 2; 

        for (int i = 0; i < size; ++i)
        {
            auto dx_2 = pow(i - center, 2);
            for (int j = 0; j < size; ++j)
            {
                auto dy_2 = pow(j - center, 2);
                double g = exp(-(dx_2 + dy_2) / (2 * sigma_2));
                g /= 2 * pi * sigma;
                arr[i * size + j] = g;
                sum += g;
            }
        }
        //归一化，卷积核，窗内和必须为1，保证原图的总值强度不变
        for (size_t i = 0; i < size; ++i)
        {
            for (size_t j = 0; j < size; ++j)
            {
                arr[i * size + j] /= sum;
            }
        }
    };
    getGassianArray();
#ifdef DEBUG
    for(size_t i = 0; i < size; i++){
        for(size_t j = 0; j < size; j++){
            cout<<arr[i*size+j]<<" ";
        }
        cout<<endl;
    }
#endif

    auto compGassion_thread = [&](const int thread_id){
        for(auto i{ t_rows * (thread_id - 1)}; i < t_rows *thread_id; ++i)
        {
            auto out_p = &dst_img.data[i * src_img.cols];
            for(auto j{size_2}; j < src_img.cols - size_2; ++j)
            {
                double sum = 0.0;
                for(size_t y = 0; y < size; ++y)
                {
                    auto in_p = &src_img.data[(i+y) * src_img.cols + j];
                    for(size_t x = 0; x < size; ++x)
                    {
                        sum += *(in_p + x) * arr[x * size + y];
                    }
                }
                *(out_p + j)=static_cast<char>(sum);
            }
        }
    };

    for(int thread_id = 1; thread_id <= max_thread; ++thread_id){
        thread_bar.emplace_back(compGassion_thread, thread_id);
    }
    for(auto &i : thread_bar){
        i.join();
    }
}


int main(int argc, char *argv[])
{
    std::cout<<"It begins"<<std::endl;
    double sigma = atof(argv[1]);
    int window_size = atoi(argv[2]);
    auto img =cv::imread("images/sample.jpg");
    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
    // cv::imwrite("images/sample_gray.jpg",img_gray);
    cv::Mat gaussian;
    gaussian_thread(img_gray, gaussian,window_size,sigma);
    string save_name = "images/gaussian_cuda"+to_string(window_size)+to_string(sigma)+".jpg";
    imwrite(save_name, gaussian);
    std::cout<<"It ends"<<std::endl;
}
