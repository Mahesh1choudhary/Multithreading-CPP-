/*



*/


#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;

class person{
    public:
    string name;
    int type;   // 0- democrate, 1-republican
};



class scheduler{
    queue<person> q;
    mutex mtq, mt;
    condition_variable cvq, cv;
    int cnt;
    int curr_type;   // 0- democrate, 1- republican in bathroom
    bool all_done;  // well there is more people left
    
    vector<thread> vec;
    
    public:
    scheduler(int num_threads){
        cnt=0, curr_type=0, all_done=false;
        vec=vector<thread>(num_threads);
        for(int i=0;i<num_threads;i++){
            vec[i]= thread(&scheduler::thread_work, this);
        }
        
    }
    ~scheduler(){
        all_done=true;
        for(auto &x:vec) x.join();
    }
    void push(person p){
        unique_lock<mutex> lkq(mtq);
        cvq.wait(lkq,[&]{return true;});
        q.push(p);
        cvq.notify_all();
    }
    
    
    void thread_work(){
        while(true){
            unique_lock<mutex> lkq(mtq);
            cvq.wait(lkq, [&]{return !q.empty() || all_done;});
            
            if(all_done){
                lkq.unlock();
                cvq.notify_all();
                return;
            }
            
            person p =q.front();
            q.pop();
            lkq.unlock();
            cvq.notify_all();
            
            unique_lock<mutex> lk(mt);
            cv.wait(lk, [&]{return (p.type==curr_type && cnt<3) || (p.type!=curr_type && cnt==0);});
            curr_type=p.type;
            cnt++;
            lk.unlock();
            cv.notify_all();
            
            // wait for f(person name time)
            
            lk.lock();
            cv.wait(lk, [&]{return true;});
            cnt--;
            lk.unlock();
            cv.notify_all();
        }
    }
    
    
};


int main()
{
    

    return 0;
}
