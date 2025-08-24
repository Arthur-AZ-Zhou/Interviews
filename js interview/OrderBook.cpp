#include <bits/stdc++.h>
using namespace std;

struct Order {
    int price;
    int quantity;
    string type; //buy or sell
};

class OrderBook {
    public:
        OrderBook() {}

        void processOrders(vector<Order> allOrders) {
            for (Order order : allOrders) { //assume only types are "buy" and "sell"
                if (order.type == "buy") {
                    processBuyOrder(order);
                } else {
                    processSellOrder(order);
                }

                cout << "============================" << endl;
                printAllOrders();
            }
        }

        void printAllOrders() {
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> tempSellHeap = sellHeap;
            priority_queue<pair<int, int>> tempBuyHeap = buyHeap;

            cout << "PRINTING SELL HEAP: " << endl;
            while (!tempSellHeap.empty()) {
                cout << "Sell Order (" << tempSellHeap.top().first << ", " << tempSellHeap.top().second << ")" << endl;
                tempSellHeap.pop();
            }

            cout << endl << "PRINTING BUY HEAP: " << endl;
            while (!tempBuyHeap.empty()) {
                cout << "Buy Order (" << tempBuyHeap.top().first << ", " << tempBuyHeap.top().second << ")" << endl;
                tempBuyHeap.pop();
            }
        }

    private:
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> sellHeap; //MINHEAP: price, quantity
        priority_queue<pair<int, int>> buyHeap; //MAXHEAP: price, quantity

        void processSellOrder(Order sellOrder) { //can only execute if largest buy order price >= sellOrder price
            int currentOrderQuantity = sellOrder.quantity;

            while (0 < currentOrderQuantity) {
                if (!buyHeap.empty() && sellOrder.price <= buyHeap.top().first) {
                    if (currentOrderQuantity <= buyHeap.top().second) { // will not overflow current buyheap top
                        pair<int, int> newBuyHeapTopOrder = {buyHeap.top().first, buyHeap.top().second - currentOrderQuantity};
                        buyHeap.pop();
                        buyHeap.push(newBuyHeapTopOrder);
                        break;
                    } else { //overflows
                        currentOrderQuantity -= buyHeap.top().second;
                        buyHeap.pop();
                    }
                } else { //empty stack due to 0 orders or too many sell orders
                    sellHeap.push({sellOrder.price, currentOrderQuantity});     
                    break;
                }       
            }
        }

        void processBuyOrder(Order buyOrder) { //can only execute if smallest sell order price <= buyOrder price
            int currentOrderQuantity = buyOrder.quantity;

            while (0 < currentOrderQuantity) {
                if (!sellHeap.empty() && sellHeap.top().first <= buyOrder.price) {
                    if (currentOrderQuantity <= sellHeap.top().second) { // will not overflow current buyheap top
                        pair<int, int> newSellHeapTopOrder = {sellHeap.top().first, sellHeap.top().second - currentOrderQuantity};
                        sellHeap.pop();
                        sellHeap.push(newSellHeapTopOrder);
                        break;
                    } else { //overflows
                        currentOrderQuantity -= sellHeap.top().second;
                        sellHeap.pop();
                    }
                } else { //empty stack due to 0 orders or too many buy orders
                    buyHeap.push({buyOrder.price, currentOrderQuantity});     
                    break;
                }       
            }
        }
};

int main() {
    vector<Order> orders = {
        {100, 5, "buy"},
        {101, 2, "buy"},
        {99, 3, "sell"},
        {102, 1, "buy"},
        {100, 4, "sell"},
        {98, 6, "sell"}
    };

    OrderBook ob;
    ob.processOrders(orders);

    cout << "done processing orders!" << endl;

    ob.printAllOrders();
}