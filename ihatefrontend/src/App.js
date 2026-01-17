import React, { useState } from 'react';

// --- CHILD COMPONENT ---
function OrderBook({ orders = [] }) {
  return (
    <div className="order-book">
      <h3>Live Orders</h3>
      
      <div className="header-row">
        <span style={{ width: 100 }}>Symbol</span>
        <span style={{ width: 100 }}>Price</span>
        <span style={{ width: 100 }}>Qty</span>
      </div>

      {orders.length === 0 ? (
        <p style={{ color: "gray" }}>No orders match filter.</p>
      ) : (
        orders.map((order) => (
          <div key={order.sym} className="order-row">
            <span style={{ width: 100 }}>{order.sym}</span>
            <span style={{ width: 100, color: order.price < 0 ? "red" : "green", fontWeight: "bold" }}>
              {order.price}
            </span>
            <span style={{ width: 100 }}>{order.quantity}</span>
          </div>
        ))
      )}
    </div>
  );
}

// --- MAIN COMPONENT ---
export default function App() {
  const [dummyOrders, setDummyOrders] = useState([
    { sym: "AAPL", price: 150.25, quantity: 100 },
    { sym: "TSLA", price: -12.50, quantity: 50 },
    { sym: "MSFT", price: 310.00, quantity: 10 },
    { sym: "GOOG", price: -5.00, quantity: 200 },
    { sym: "AMZN", price: 105.00, quantity: 20 }
  ]);

  const [showLargeOnly, setShowLargeOnly] = useState(false);

  const visibleOrders = showLargeOnly
    ? dummyOrders.filter((order) => order.quantity > 50)
    : dummyOrders;

  return (
    <div className="App">
      <div style={{ marginBottom: 20 }}>
        <button 
          onClick={() => setShowLargeOnly(!showLargeOnly)}
          className={showLargeOnly ? "btn-active" : "btn-inactive"}
        >
          {showLargeOnly ? "Show All Orders" : "Show Large Orders Only (>50)"}
        </button>
      </div>
      <OrderBook orders={visibleOrders} />
    </div>
  );
}