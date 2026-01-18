import React, { useState } from 'react';

// --- CHILD COMPONENT ---
function OrderBook({ orders = [] }) {
  return (
    <div className="order-book" style={{ border: "1px solid #ccc", padding: "10px", borderRadius: "8px" }}>
      <h3>Live Orders</h3>
      
      {/* 1. HEADER (Kept outside the scroll box so it stays visible) */}
      <div className="header-row" style={{ display: "flex", fontWeight: "bold", borderBottom: "2px solid #333", paddingBottom: "5px", marginBottom: "10px" }}>
        <span style={{ width: 100 }}>Symbol</span>
        <span style={{ width: 100 }}>Price</span>
        <span style={{ width: 100 }}>Qty</span>
      </div>

      {/* 2. SCROLL CONTAINER (This is the "Window" to view the data) */}
      <div style={{ height: "300px", overflowY: "auto", border: "1px solid #eee" }}>
        
        {orders.length === 0 ? (
          <p style={{ color: "gray", padding: "10px" }}>No orders match filter.</p>
        ) : (
          orders.map((order) => (
            <div key={order.sym} className="order-row" style={{ display: "flex", borderBottom: "1px solid #eee", padding: "8px 0" }}>
              <span style={{ width: 100 }}>{order.sym}</span>
              <span style={{ width: 100, color: order.price < 0 ? "red" : "green", fontWeight: "bold" }}>
                {order.price}
              </span>
              <span style={{ width: 100 }}>{order.quantity}</span>
            </div>
          ))
        )}
      </div>
    </div>
  );
}

// --- HELPER TO GENERATE FAKE DATA ---
const generateData = () => {
  const data = [];
  for (let i = 0; i < 50; i++) {
    data.push({
      sym: `STK-${i + 100}`, // Creates symbols like STK-100, STK-101...
      price: (Math.random() * 400 - 100).toFixed(2), // Random price between -100 and 300
      quantity: Math.floor(Math.random() * 500) // Random quantity 0-500
    });
  }
  return data;
};

// --- MAIN COMPONENT ---
export default function App() {
  // Initialize with 50 generated items instead of 5 hardcoded ones
  const [dummyOrders, setDummyOrders] = useState(generateData());
  const [showLargeOnly, setShowLargeOnly] = useState(false);

  const visibleOrders = showLargeOnly
    ? dummyOrders.filter((order) => order.quantity > 250) // Increased filter threshold since data is bigger
    : dummyOrders;

  return (
    <div className="App" style={{ padding: "20px", fontFamily: "sans-serif" }}>
      <div style={{ marginBottom: 20 }}>
        <button 
          onClick={() => setShowLargeOnly(!showLargeOnly)}
          // Inline styles added here just to ensure it looks good without external CSS file
          style={{
            padding: "10px 20px",
            backgroundColor: showLargeOnly ? "blue" : "gray",
            color: "white",
            border: "none",
            borderRadius: "5px",
            cursor: "pointer"
          }}
        >
          {showLargeOnly ? "Show All Orders" : "Show Large Orders Only (>250)"}
        </button>
      </div>
      
      <OrderBook orders={visibleOrders} />
    </div>
  );
}