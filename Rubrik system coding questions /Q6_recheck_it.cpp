/*
Question link - https://leetcode.com/discuss/interview-question/1938910/rubrik-phone-task-scheduler


Implement two methods in a class, schedule() and waitUntilComplete().
schedule() should enqueue work to be performed and should be non-blocking.
waitUntilComplete() should block the call untill all scheduled work is completed.

Builtin thread safe constructs like Deque, BlockingQueue, etc. can't be used. Implement it using locks, etc ensuring thread safety.




*/





#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>
using namespace std;

template<class Task> class new_queue{
    queue<Task> q;
    mutex mt;
    condition_variable cv;
    
    public:
    void push(Task tsk){
        {
            unique_lock<mutex> lk(mt);
            q.push(tsk);
        }
        cv.notify_all();
    }
    
    Task& pop(){
        Task ans;
        unique_lock<mutex> lk(mt);
        cv.wait(lk, [&]{return !q.empty();});
        ans=move(q.front());
        q.pop();
        return ans;
    }
    
    bool empty(){
        unique_lock<mutex> lk(mt);
        return q.empty();
    }
};

template<class Task> class scheduler{
    new_queue<Task> q;
    mutex mt;
    condition_variable cv;
    bool waitUntil=false;
    bool terminate=false;
    
    public:
    void schedule(Task tsk){
        unique_lock<mutex> lk(mt);
        cv.wait(lk,[&]{return !waitUntil;});
        q.push(tsk);
        lk.unlock();
        cv.notify_all();
    }
    
    void waitUntilComplete(){
        unique_lock<mutex> lk(mt);
        waitUntil=true;
        cv.wait(lk,[&]{return q.empty();});
        waitUntil=false;    // once all task done, again start scheduling tasks
        lk.unlock();
        cv.notify_all();
    }
    
    void thread_work(){
        while(true){
            unique_lock<mutex> lk(mt);
            if(terminate && q.empty()){
                return;
            }
            Task tsk= q.pop();
            // perform task
        }
    }
    
    // call terminate when there are no more tasks to schedule
    void terminate_all(){
        unique_lock<mutex> lk(mt);
        terminate=true;
    }
    
    
};

int main()
{
    

    return 0;
}
