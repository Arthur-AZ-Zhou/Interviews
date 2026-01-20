import React, { useState, useRef } from 'react';
import './styles.css'; 

// --- HELPER: Generate Data ---
const generateData = () => {
  const data = [];

  for (let i = 0; i < 50; i++) {
    data.push({
      sym: `STK-${i + 100}`,
      price: (Math.random() * 400 - 100).toFixed(2),
      quantity: Math.floor(Math.random() * 500)
    });
  }
  
  return data;
};

// --- CHILD COMPONENT ---
function OrderBook({ orders = [] }) {
  return (
    <div className="order-book">
      <h3>Live Orders</h3>
      
      {/* Header */}
      <div className="header-row">
        <span className="col">Symbol</span>
        <span className="col">Price</span>
        <span className="col">Qty</span>
      </div>

      {/* Scroll Container using CSS class */}
      <div className="scroll-container">
        {orders.length === 0 ? (
          <p style={{ color: "gray", padding: "10px" }}>No orders match filter.</p>
        ) : (
          orders.map((order) => (
            <div key={order.sym} className="order-row">
              <span className="col">{order.sym}</span>
              
              {/* Dynamic Class Name Logic */}
              <span className={`col ${order.price < 0 ? "text-red" : "text-green"}`}>
                {order.price}
              </span>
              
              <span className="col">{order.quantity}</span>
            </div>
          ))
        )}
      </div>
    </div>
  );
}

// --- MAIN COMPONENT ---
export default function App() {
  const [dummyOrders] = useState(generateData());
  const [showLargeOnly, setShowLargeOnly] = useState(false);

  const visibleOrders = showLargeOnly
    ? dummyOrders.filter((order) => order.quantity > 250)
    : dummyOrders;

  return (
    <div className="App">
      <div style={{ marginBottom: 20 }}>
        <button 
          onClick={() => setShowLargeOnly(!showLargeOnly)}
          className={showLargeOnly ? "btn-active" : "btn-inactive"}
        >
          {showLargeOnly ? "Show All Orders" : "Show Large Orders Only (>250)"}
        </button>
      </div>
      
      <OrderBook orders={visibleOrders} />
    </div>
  );
}