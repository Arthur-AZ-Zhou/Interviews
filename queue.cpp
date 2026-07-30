#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>
using namespace std;

class BoundedBlockingQueue {
    private:
        vector<int> buffer; //fixed size array
        int cap;
        int head;
        int tail;
        int numElements; //count how many elements we have
        
        mutex mtx;
        condition_variable notFull; // for enqueue()
        condition_variable notEmpty; // for dequeue()
    
    public:
        BoundedBlockingQueue(int capacity) : buffer(capacity), cap(capacity), head(0), tail(0), numElements(0) {}
        
        void enqueue(int elementX) {
            // _,_,_:h=0,t=0,s=0
            // 1,_,_:h=0,t=1,s=1
            // 1,2,_:h=0,t=2,s=2
            // 1,2,3:h=0,t=3->0,s=3
            // 4,wait
            // dequeue
            // _,2,3:h=1,t=3->0,s=2
            // 4,2,3:h=1,t=0,s=3
            
            unique_lock<mutex> lock(mtx); // unique_lock<mtx> lock(mutex);
            // while numElements = cap we lock, otherwise we set tail to elemment and we wrap around with modulo

            while (numElements == cap) {
                notFull.wait(lock);
            }
            
            buffer[tail] = elementX;
            numElements++;
            tail = (tail + 1) % cap;
            notEmpty.notify_one();
        }
        
        int dequeue() {
            unique_lock<mutex> lock(mtx);
            
            while (numElements == 0) {
                notEmpty.wait(lock);
            }
            
            int queueFront = buffer[head];
            numElements--;
            head = (head + 1) % cap;
            notFull.notify_one();
            
            return queueFront;
        }
        
        int size() {
            return numElements;
        }
};

void producer(BoundedBlockingQueue& q) {
    for (int i = 1; i <= 5; i++) {
        q.enqueue(i);
        cout << "produce: " << i << endl;
    }
}

void consumer(BoundedBlockingQueue& q) {
    for (int i = 1; i <= 5; i++) {
        int queueFront = q.dequeue();
        cout << "consume: " << queueFront << endl;
    }
}

int main() {
    BoundedBlockingQueue q(2); // should block because only 2 and we try to put 5
    
    thread p(producer, ref(q));
    thread c(consumer, ref(q));
    
    p.join();
    c.join();
    
    // BoundedBlockingQueue q(3);
    
    // q.enqueue(1);
    // q.enqueue(2);
    // q.enqueue(3);
    
    // cout << "q size (should be 3): " << q.size() << endl;
    // cout << "dequeue: " << q.dequeue() << endl;
    // cout << "dequeue: " << q.dequeue() << endl;
    // cout << "q size (should be 1): " << q.size() << endl;
    
    // q.enqueue(4);
    // q.enqueue(5); 
    
    // cout << "q size (should be 3): " << q.size() << endl;
    
    return 0;
}