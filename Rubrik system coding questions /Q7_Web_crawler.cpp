/*
Question Link - https://www.geeksforgeeks.org/rubrik-interview-experience-for-swe/


*/


#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <unordered_set>
using namespace std;



unordered_set<string> web_crawler(string startUrl, HtmlParser htmlParser){
    vector<string> ans;
    unordered_set<string> visited;
    queue<string> q;
    mutex mt;
    condition_variable cv;
    
    q.push(startUrl);
    visited.insert(startUrl);
    string hostname=getHostname(startUrl);
    
    
    auto thread_work=[&]()->void{
        while(true){
            unique_lock<mutex> lk(mt);
            cv.wait_for(lk,30ms, [&]{return !q.empty();});
            if(q.empty()) return;
            
            string url=q.front();
            q.pop();
            lk.unlock();
            cv.notify_all();
            
            // assuming htmlParser is a class and can be called from different threads
            auto allurls= htmlParser.get_URLS(url);
            
            for(auto &x:allurls){
                if(x.find(hostname)!=string::npos){
                    cv.wait(lk, [&]{return true;});
                    if(!visited.count(x)){
                        visited.insert(x);
                        q.push(x);
                    }
                    lk.unlock();
                    cv.notify_all();
                }
            }
            
        }   
        
    };
    
    int num_threads= max(2, (int) thread::hardware_concurrency());
    vector<thread> vec(num_threads-1);
    for(int i=0;i<num_threads-1;i++){
        vec[i]=thread(thread_work);
    }
    thread_work();
    
    for(int i=0;i<num_threads-1;i++){
        vec[i].join();
    }
    
    return visited;
    
}

int main()
{
    

    return 0;
}
