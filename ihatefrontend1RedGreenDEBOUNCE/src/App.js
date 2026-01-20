// <--- CHANGE 1: Import useRef and useEffect
import React, { useState, useRef, useEffect } from "react";

function TradeList({ trades = [] }) {
  // <--- CHANGE 2: Create a ref to access the scrolling DOM element
  const listRef = useRef(null);

  // <--- CHANGE 3: State to show you the result (only updates when you STOP scrolling)
  const [debouncedScrollPosition, setDebouncedScrollPosition] = useState(0);

  // <--- CHANGE 4: The Debounce Logic
  useEffect(() => {
    const scrollContainer = listRef.current;
    if (!scrollContainer) return;

    let timeoutId = null;

    const handleScroll = () => {
      // Clear existing timer (reset the clock if they scroll again)
      if (timeoutId) clearTimeout(timeoutId);

      // Set new timer
      timeoutId = setTimeout(() => {
        // This code only runs 500ms AFTER the last scroll event
        console.log("User stopped scrolling. Saving position:", scrollContainer.scrollTop);
        setDebouncedScrollPosition(scrollContainer.scrollTop);
      }, 500); 
    };

    // Attach listener
    scrollContainer.addEventListener("scroll", handleScroll);

    // Cleanup listener on unmount
    return () => {
      scrollContainer.removeEventListener("scroll", handleScroll);
      if (timeoutId) clearTimeout(timeoutId);
    };
  }, []); // Empty dependency array = run once on mount

  return (
    <div style={{ padding: "20px", fontFamily: "sans-serif" }}>
      <h3>Recent Trades</h3>
      
      {/* <--- CHANGE 5: Visual feedback for the user */}
      <p style={{ color: "blue", fontSize: "12px" }}>
        Last Saved Position: {debouncedScrollPosition}px (Updates when you stop)
      </p>

      <div className="header-row" style={{ display: "flex", fontWeight: "bold", borderBottom: "2px solid gray", paddingBottom: "5px" }}>
        <span style={{ width: 100 }}>SYMBOL</span>
        <span style={{ width: 100 }}>PRICE</span>
      </div>

      {/* <--- CHANGE 6: Attach the Ref and make it Scrollable */}
      <div 
        ref={listRef} 
        style={{ 
          height: "150px",      // Fixed height forces scroll bar
          overflowY: "auto",    // Enables vertical scrolling
          border: "1px solid #eee" 
        }}
      >
        {trades.length === 0 ? (
          <p style={{ marginTop: "10px" }}>NO TRADES TO SHOW</p>
        ) : (
          trades.map((trade) => (
            <div key={trade.id} className="trade-row" style={{ display: "flex", fontWeight: "bold", borderBottom: "1px solid #eee", padding: "8px 0" }}>
              <span style={{ width: 100 }}>{trade.symbol}</span>
              <span style={{ width: 100, color: (0 < trade.price) ? "green" : "red" }}>{trade.price}</span>
            </div>
          ))
        )}
      </div>
    </div>
  );
}

// --- MAIN APP ---
export default function App() {
  // <--- CHANGE 7: Added more data so the scrollbar actually appears
  const [data] = useState([
    { id: 101, symbol: "AAPL", price: 145.20 },
    { id: 102, symbol: "GOOG", price: -2.50 },
    { id: 103, symbol: "MSFT", price: 300.10 },
    { id: 104, symbol: "TSLA", price: -15.00 },
    { id: 105, symbol: "AMZN", price: 102.00 },
    { id: 106, symbol: "META", price: -5.00 },
    { id: 107, symbol: "NFLX", price: 200.50 },
    { id: 108, symbol: "NVDA", price: 450.00 },
    { id: 109, symbol: "AMD", price: -2.20 },
    { id: 110, symbol: "INTC", price: 30.10 },
  ]);

  return (
    <div>
      <TradeList trades={data} />
    </div>
  );
}