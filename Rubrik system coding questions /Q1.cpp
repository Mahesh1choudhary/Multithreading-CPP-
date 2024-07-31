/*
Question taken from here - https://leetcode.com/discuss/interview-question/1857832/rubrik-phone-screen

Given following classes:

Class Client:
init()
request()
close()

Class ClientWrapper(Client):
init()
request()
close()

Here, class Client is like an abstract class that deals with responsibilities like initializing connection to the server using init(), request() is used to make request to the server and close() is used to close the connection to the server.
Question: Multiple threads can concurrently access methods of class ClientWrapper. One needs to implement all the methods of class ClientWrapper such that following use cases are covered -

If init() is in progress by some thread, then block other concurrent threads to execute request() or close() method
Do not allow concurrent calls to init() method.
Allow concurrent calls to request() method if init() is already called successfully.
If some thread is still using request(), then block any other thread to close the connection using close() method
Once connection is closed, do not allow subsequent calls to close() method
Do not allow concurrent calls to close() method
...etc.
Thanks!!


*/


#include <iostream>
#include <mutex>
#include <condition_variable>
using namespace std;

class Client{
    public:
    void init();
    void request();
    void close();
};

// once some thread calls close, we want entertain further requests calls from other threads
// if request is called before init, then request have to wait till init is called by some thread, we will not discard them

// once connection is closed, then further request have to wait till connection is opened again
class ClientWrapper{
    bool connection_startd=false;
    int cnt=0;
    mutex mt;
    condition_variable waiting_to_request, waiting_to_close;
    Client obj;
    bool closed_called=false;
    
    
    public:
    void init(){
        unique_lock<mutex> lk(mt);
        if(!connection_startd){
            obj.init();
            connection_startd=true;
        }
        lk.unlock();
        waiting_to_request.notify_all();
        waiting_to_close.notify_all();
    }
    
    void request(){
        unique_lock<mutex> lk(mt);
        waiting_to_request.wait(lk,[&]{return connection_startd || closed_called;});
        if(closed_called) return;   // once close is called, discard all further requests;
        cnt++;
        lk.unlock();
        waiting_to_request.notify_all();
        
        obj.request();
        
        waiting_to_request.wait(lk,[&]{return true;});
        cnt--;
        lk.unlock();
        waiting_to_close.notify_all();
        waiting_to_request.notify_all();
    }
    
    void close(){
        unique_lock<mutex> lk(mt);
        closed_called=true;
        waiting_to_close.wait(lk, [&]{return connection_startd && cnt==0;});
        obj.close();
        connection_startd=false;
        closed_called=false;
    }
    
};

int main()
{
    

    return 0;
}
