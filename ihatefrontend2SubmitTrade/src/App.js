import React, { useState } from "react";

// --- TODO: IMPLEMENT THIS COMPONENT ---
function TradeForm({ onAddTrade }) {  
  const [symbol, setSymbol] = useState("");
  const [price, setPrice] = useState("");

  const handleSubmit = (e) => {
    e.preventDefault(); // Good practice for forms

    if (!symbol || !price) return;
    
    const newTrade = {
      id: Date.now(),
      symbol: symbol.toUpperCase(),
      price: parseFloat(price),
    }

    onAddTrade(newTrade);

    setSymbol("");
    setPrice("");
  };

  return (
    <div style={{ borderBottom: "2px solid #ccc", paddingBottom: "20px", marginBottom: "20px" }}>
      <h3>New Order Ticket</h3>
      <form onSubmit={handleSubmit} style={{ display: "flex", gap: "10px" }}>

        <input
          type="text" 
          placeholder="Symbol"
          value={symbol}
          onChange={(e) => setSymbol(e.target.value)}
        />
        
        <input 
          type="text" 
          placeholder="Price"
          value={price}
          onChange={(e) => setPrice(e.target.value)}
        />

        <button type="submit">Execute</button>
      </form>
    </div>
  );
}

// --- EXISTING LIST COMPONENT (Already done) ---
function TradeList({ trades, onDelete }) {
  return (
    <div>
      {trades.length === 0 ? <p>NO ACTIVE TRADES</p> : (
        trades.map((t) => (
        <div key={t.id} style={{ display: "flex", justifyContent: "space-between", alignItems: "center", borderBottom: "1px solid #eee", padding: "5px" }}>
          <div>
            <strong>{t.symbol}</strong>: 
            <span style={{ color: t.price >= 0 ? "green" : "red", marginLeft: "10px" }}>
              {t.price}
            </span>
          </div>

          <button 
            onClick={() => onDelete(t.id)}
            style={{ backgroundColor: "red", color: "white", border: "none", borderRadius: "4px", cursor: "pointer" }}
          >
            X
          </button>
        </div>
        ))
      )}
    </div>
  );
}

// --- MAIN APP ---
export default function App() {
  const [trades, setTrades] = useState([
    { id: 1, symbol: "AAPL", price: 150.00 },
    { id: 2, symbol: "TSLA", price: -10.50 }
  ]);

  const handleAddTrade = (newTrade) => {
    setTrades([newTrade, ...trades]);
  };

  const handleDeleteTrade = (id) => {
    const updatedList = trades.filter((trade) => trade.id !== id);
    setTrades(updatedList);
  };

  return (
    <div style={{ padding: 20, fontFamily: "sans-serif" }}>
      {/* Pass the handler down to the form */}
      <TradeForm onAddTrade={handleAddTrade} />
      
      <TradeList trades={trades} onDelete={handleDeleteTrade} />
    </div>
  );
}