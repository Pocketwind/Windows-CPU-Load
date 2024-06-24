#include <iostream>
#include<time.h>
#include<Windows.h>

using namespace std;

class Queue
{
private:
    int size, current = -1;
    double* list;
public:
    Queue(int size) { this->size = size; list = new double[size] {}; }
    ~Queue() { delete[] list; }
    void Push(double n) { current++;  list[current % size] = n; }
    double GetSumN(int);
};
double Queue::GetSumN(int n)
{
    double sum = 0;
    for (int i = 0; i < n; ++i)
    {
        sum = sum + list[(i + current) % size];
    }
    return sum;
}

double GetCpuLoad()
{
    double ret;
    FILETIME idle1, kernel1, user1, idle2, kernel2, user2;
    GetSystemTimes(&idle1, &kernel1, &user1);
    Sleep(1000);
    GetSystemTimes(&idle2, &kernel2, &user2);
    ULARGE_INTEGER idle, kernel, user;
    idle.LowPart = idle2.dwLowDateTime - idle1.dwLowDateTime;
    idle.HighPart = idle2.dwHighDateTime - idle1.dwHighDateTime;
    kernel.LowPart = kernel2.dwLowDateTime - kernel1.dwLowDateTime;
    kernel.HighPart = kernel2.dwHighDateTime - kernel1.dwHighDateTime;
    user.LowPart = user2.dwLowDateTime - user1.dwLowDateTime;
    user.HighPart = user2.dwHighDateTime - user1.dwHighDateTime;
    ret = (double)(user.QuadPart + kernel.QuadPart - idle.QuadPart) / (user.QuadPart + kernel.QuadPart) * 100;
    if (ret > 100.0)
        ret = ret / 10000000000000;
    return ret;
}

int main()
{
    Queue Last15(15);
    int counter = 0;
    while (1)
    {
        time_t t = time(NULL);
        struct tm timer;
        localtime_s(&timer, &t);
        cout << counter << "\t" << timer.tm_year + 1900 << '/' << timer.tm_mon + 1 << '/' << timer.tm_mday << ' ' << timer.tm_hour << ":" << timer.tm_min << ":" << timer.tm_sec;
        cout << "\t";
        cout.precision(2);
        if (counter >= 1)
        {
            cout << "cpu load : " << Last15.GetSumN(1) << "%   \t";
        }
        if (counter >= 5)
        {
            cout << "5초간 평균 : " << Last15.GetSumN(5) / 5.0 << "%   \t";
        }
        if (counter >= 10)
        {
            cout << "10초간 평균 : " << Last15.GetSumN(10) / 10.0 << "%   \t";
        }
        if (counter >= 15)
        {
            cout << "15초간 평균 : " << Last15.GetSumN(15) / 15.0 << "%   \t";
        }
        cout << "\n";
        ++counter;
        Last15.Push(GetCpuLoad());
    }
    return 0;
}

