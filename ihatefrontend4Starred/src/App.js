import React, { useState } from "react";

function WatchList({ stocks, onToggle }) {
  // TODO 1: Calculate how many stocks have (isWatched === true)
  const watchedCount = stocks.filter((stock) => stock.isWatched === true).length; 

  return (
    <div style={{ padding: 20, fontFamily: "sans-serif", maxWidth: 400 }}>
      <h3>Market Watchlist</h3>
      
      {/* TODO 2: Display the Count, gemini alr did this for us */}
      <div style={{ marginBottom: 15 }}>
        <strong>Watching: {watchedCount} stocks</strong>
      </div>

      <div style={{ border: "1px solid #ccc", borderRadius: 8 }}>
        {stocks.map((stock) => (
          <div key={stock.id} style={{ display: "flex", alignItems: "center", justifyContent: "space-between", padding: "10px", borderBottom: "1px solid #eee" }}>
            
            <div>
              <strong>{stock.symbol}</strong>
              <span style={{ marginLeft: 10 }}>${stock.price}</span>
            </div>

            {/* TODO 3: The Star Button */}
            <button 
              onClick={() => onToggle(stock.id)}
              style={{ 
                cursor: "pointer", 
                fontSize: "20px", 
                background: "none", 
                border: "none",
                // TODO 4: Change color based on stock.isWatched
                color: stock.isWatched ? "gold" : "gray" 
              }}
            >
              {/* Star Icon Character */}
              ★ 
            </button>

          </div>
        ))}
      </div>
    </div>
  );
}

// --- MAIN APP ---
export default function App() {
  const [stocks, setStocks] = useState([
    { id: 1, symbol: "NVDA", price: 420.50, isWatched: false },
    { id: 2, symbol: "AMD", price: 105.00, isWatched: true }, // Starts as watched
    { id: 3, symbol: "INTC", price: 30.25, isWatched: false }
  ]);

  const handleToggle = (idToToggle) => {    
    const newArray = stocks.map((stock) => { // 1) Create a NEW array based on the old one

      if (stock.id === idToToggle) { // 2) Is this the one we want to change?
        // YES: Return a COPY with the switch flipped
        // The "..." copies all existing fields (symbol, price)
        return { ...stock, isWatched: !stock.isWatched }; 
      }

      return stock; // 4) return original items untouched
    });

    setStocks(newArray); // 3) Update the state
  };

  return (
    <WatchList stocks={stocks} onToggle={handleToggle} />
  );
}