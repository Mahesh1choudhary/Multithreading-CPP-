/*
This is implemented using semaphores
*/

#include <iostream>
#include <thread>
#include <mutex> 
#include <semaphore>
#include <stdexcept>
#include <queue>

using namespace std;


template<typename A> class blocking_queue{
    private:
        std::counting_semaphore<> remaining; // represents how many more element can be inserted
        std::counting_semaphore<> filled;    // represents how many many elements are present in queue
        queue<A> q;
        mutex mt;

    public:
        blocking_queue(int capacity): remaining(capacity), filled(0) {
            if(capacity <=0){
                throw std:: invalid_argument("capacity must be positive");
            }
        }


        void push(const A& value){
            remaining.acquire();
            {
                unique_lock<mutex> lk(mt);
                q.push(value);
            }
            filled.release();
        }

        A pop(){
            A result;
            filled.acquire();
            {
                unique_lock<mutex> lk(mt);
                result=q.front();
                q.pop();
            }
            remaining.release();
            return result;
        }


};


