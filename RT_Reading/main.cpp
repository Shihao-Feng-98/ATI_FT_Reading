#include <iostream>
#include <vector>
#include <string>
#include <pthread.h> // -lpthread
#include <sys/mman.h> // mlockall(MCL_CURRENT|MCL_FUTURE)
#include <unistd.h> // sleep
#include <fstream>
using namespace std;
#include <periodic_rt_task.h>
#include <C_timer.h>
#include <ATI_reading.h>

vector<float> Fz_vec;

// ======== Main Control Thread Function ========  
void* main_control_loop(void* argc)
{
    CTimer timer_step, timer_total;
    double dt = 0.001;
    double time_since_run = 0.;
    int iter = 0;

    string ip_addr = "192.168.1.1";
    AtiReading ATI(ip_addr.c_str());
    FTData res;
    
    timer_total.reset();
    // run periodic task
    while(time_since_run < 1.)
    {
        timer_step.reset();

        res = ATI.send_recv();
        Fz_vec.emplace_back(res.Fz);
        
        time_since_run += dt;
        iter++;
        while (timer_step.end() < dt*1000*1000);
    }
    cout << "Actual time: " << timer_total.end()/1000 << " ms\n";
    cout << "Iteration: " << iter << endl;

    return nullptr;
}

int main(int argc, char** argv)
{
    /*
    mlockall 锁定进程中所有映射到地址空间的页
    MCL_CURRENT 已经映射的进程地址，MCL_FUTURE 将来映射的进程地址
    */
    if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
        cout << "mlockall failed: %m\n"; // 输入上一个函数的错误信息
        return -2;
    }
    
    // 主控制线程
    PeriodicRtTask *main_control_task = new PeriodicRtTask("[Main Control Thread]", 95, main_control_loop);
    sleep(1); 
    // 析构函数会join线程，等待子线程结束
    delete main_control_task;

    ofstream ofs1("/home/feng_shihao/fsh_file/ATI/ATI_Reading/data.txt");
    for (auto Fz : Fz_vec)
    {
        ofs1 << Fz << endl;
    }
    ofs1.close();

    return 0;
}
