/*
Question taken from here - https://leetcode.com/discuss/interview-question/1125380/rubrik-system-coding-interview-question

Implement following method of ScheduledExecutorService interface in Java

schedule(Runnable command, long delay, TimeUnit unit)
Creates and executes a one-shot action that becomes enabled after the given delay.

scheduleAtFixedRate(Runnable command, long initialDelay, long period, TimeUnit unit)
Creates and executes a periodic action that becomes enabled first after the given initial delay, and subsequently with the given period; that is executions will commence after initialDelay then initialDelay+period, then initialDelay + 2 * period, and so on.

scheduleWithFixedDelay(Runnable command, long initialDelay, long delay, TimeUnit unit)
Creates and executes a periodic action that becomes enabled first after the given initial delay, and subsequently with the given delay between the termination of one execution and the commencement of the next.



*/



#include <iostream>
#include <condition_variable>
#include <mutex>


class Runnable{

}
class TimeUnit{
    
}

class Task{
    Runnable func;
    long long delay;
    Timeunit unit;
    int type;   // 0,1,2
    
    public:
    Task(Runnable r, long long d, TimeUnit u, int t){
        func=r, delay=d, unit=u, type=t;
    }
}

class ScheduledExecutorService{
    private:
    priority_queue<pair<long long,Task>, vector<pair<long long, Task>,greater<pair<long long,Task>> pq;    //min priority_queue -> initial time of start, task
    mutex pq_mt;
    condition_variable cv;
    
    
    public:
    schedule(Runnable command, long long delay, TimeUnit unit){
        unique_lock<mutex> lk(pq_mt);
        pq.push(make_pair( chrono::,Task(command, delay, unit,0)));
    }
    
    
}



int main()
{
    

    return 0;
}
