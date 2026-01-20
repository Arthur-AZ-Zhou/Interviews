import React, { useState } from "react";

function StockSearch({ stocks }) {
  // TODO 1: Create state for the 'searchTerm'
  const [searchTerm, setSearchTerm] = useState("");

  // TODO 2: Create a logical variable 'filteredStocks'
  // Hint: Use .filter() on the 'stocks' array.
  // Hint: Convert both the stock symbol and the search term to .toLowerCase() so casing doesn't matter.
  const filteredStocks = stocks.filter((stocks) => {
    return stocks.symbol.toLowerCase().includes(searchTerm.toLowerCase());
  })

  return (
    <div style={{ padding: "20px", fontFamily: "sans-serif", maxWidth: "400px" }}>
      <h3>Market Search</h3>

      {/* TODO 3: The Search Input */}
      <input 
        type="text"
        placeholder="Search Ticker..."
        style={{ padding: "8px", width: "100%", marginBottom: "15px", boxSizing: "border-box" }}
        value={searchTerm}
        onChange={(e) => setSearchTerm(e.target.value)}
      />

      {/* TODO 4: The List of Results */}
      <div style={{ border: "1px solid #ccc", borderRadius: "5px" }}>
        
        {/* If filteredStocks is empty, show "No results found" */}
        {/* Else, map through filteredStocks and display them */}
        <div>
          {filteredStocks.length === 0? <p>No results found</p> : (
            filteredStocks.map((fS) => (
              <div key={fS.name} style={{ display: "flex", borderBottom: "1px solid #eee", padding: "8px 0" }}>
                <strong>
                  {fS.symbol}
                </strong>
              </div>
            ))
          )}
        </div>

      </div>
    </div>
  );
}

// --- MAIN APP (DO NOT TOUCH) ---
export default function App() {
  const [marketData] = useState([
    { symbol: "AAPL", name: "Apple Inc." },
    { symbol: "AMZN", name: "Amazon.com" },
    { symbol: "MSFT", name: "Microsoft Corp." },
    { symbol: "GOOGL", name: "Alphabet Inc." },
    { symbol: "TSLA", name: "Tesla Inc." },
    { symbol: "META", name: "Meta Platforms" },
    { symbol: "NVDA", name: "Nvidia Corp." }
  ]);

  return (
    <div>
      <StockSearch stocks={marketData} />
    </div>
  );
}