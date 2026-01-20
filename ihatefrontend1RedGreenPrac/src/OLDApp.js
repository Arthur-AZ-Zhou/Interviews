import React, { useState } from "react";

// --- TODO: IMPLEMENT THIS COMPONENT ---
function TradeList({ trades = [] }) {
  return (
    <div style={{ padding: "20px", fontFamily: "sans-serif" }}>
      <h3>Recent Trades</h3>

        <div className="header-row" style={{ display: "flex", fontWeight: "bold", borderBottom: "2px solid #000000", paddingBottom: "5px"}}>
            <span style={{ width: 100 }}>SYMBOL</span>
            <span style={{ width: 100 }}>PRICE</span>
        </div>

        {trades.length === 0 ? (
            <p style={{ marginTop: "10px" }}>No trades available to display</p>
        ) : (
            trades.map((trade) => (
                <div key={trade.id} className="trade-row" style={{ display: "flex", borderBottom: "1px solid #eee", padding: "8px 0"}}>
                    <span style={{ width: 100}}>{trade.symbol}</span>
                    <span style={{ width: 100, color: 0 < trade.price ? "green" : "red", fontWeight: "bold"}}>{trade.price}</span>
                </div>
            ))
        )}
    </div>
  );
}


// --- MAIN APP (DO NOT TOUCH) ---
export default function App() {
  const [data] = useState([
    { id: 101, symbol: "AAPL", price: 145.20 },
    { id: 102, symbol: "GOOG", price: -2.50 }, // Negative price represents a drop
    { id: 103, symbol: "MSFT", price: 300.10 },
    { id: 104, symbol: "TSLA", price: -15.00 }
  ]);

  return (
    <div>
      <TradeList trades={data} />
    </div>
  );
}