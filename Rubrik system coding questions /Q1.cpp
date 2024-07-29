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
    void init(){}
    void request(){}
    void close(){}
};

class ClientWrapper{
    private:
        Client obj;
        bool connection_type; // 0- not opened yet, 1- open, 2- closed 
        mutex mt;
        condition_variable can_request, can_close;
        int cnt;    // no of requests still running or wants to run
    
    public:
        ClientWrapper(Client &x){
            connection_type=0;
            obj=x;
            cnt=0;
        }
    
        void init(){
            unique_lock<mutex> lk(mt);
            if(connection_type==0){
                connection_type=1;
                obj.init();
                lk.unlock();
                can_close.notify_all();
                can_request.notify_all();
            }
        }
        
        void request(){
            unique_lock<mutex> lk(mt);
            if(connection_type==2) return;
            can_request.wait(lk, [&]{return connection_type==1;});
            cnt++;
            lk.unlock();
            can_request.notify_all();
            
            obj.request();
            
            
            can_request.wait(lk, [&]{return connection_type==1;});
            cnt--;
            lk.unlock();
            can_close.notify_all();
            can_request.notify_all();
            
        }
        
        void close(){
            unique_lock<mutex> lk(mt);
            if(connection_type==2) return;
            can_close.wait(lk,[&]{return connection_type==1 && cnt==0;});
            obj.close();
            connection_type=0;
            
        }
    
};





int main()
{
    std::cout<<"Hello World";

    return 0;
}
