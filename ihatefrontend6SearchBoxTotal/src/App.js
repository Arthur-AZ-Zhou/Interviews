import React, { useState } from "react";

function Portfolio({ assets }) {
  // State for the active tab: "All", "Stock", or "Crypto"
  const [selectedType, setSelectedType] = useState("All");

  // If selectedType is "All", keep everyone.
  // Otherwise, only keep assets where asset.type === selectedType
  const visibleAssets = selectedType === "All" ? assets : assets.filter(asset => asset.type === selectedType);

  // TODO 2: Calculate the Total Value of 'visibleAssets'
  // Hint: You can use .reduce() or a simple for-loop.
  let totalValue = 0;

  for (let i = 0; i < visibleAssets.length; i++) {
    totalValue += visibleAssets[i].value;
  }

  return (
    <div style={{ padding: 20, fontFamily: "sans-serif", width: 400, border: "1px solid #ccc" }}>
      <h3>My Portfolio</h3>

      {/* THE TABS */}
      <div style={{ display: "flex", gap: "10px", marginBottom: 20 }}>
        {/* I wrote the buttons for you, but notice how they update state */}
        <button onClick={() => setSelectedType("All")}>All</button>
        <button onClick={() => setSelectedType("Stock")}>Stocks</button>
        <button onClick={() => setSelectedType("Crypto")}>Crypto</button>
      </div>

      {/* THE LIST */}
      <div style={{ marginBottom: 20 }}>
        {visibleAssets.map(asset => (
          <div key={asset.id} style={{ display: "flex", justifyContent: "space-between", padding: "5px 0", borderBottom: "1px solid #eee" }}>
            
            {/* TODO 3: Apply Conditional Color */}
            {/* Stock -> Blue, Crypto -> Purple */}
            <span style={{ fontWeight: "bold", color: (asset.type == "Stock")? "blue" : "purple" }}>
              {asset.name}
            </span>
            
            <span>${asset.value}</span>
          </div>
        ))}
      </div>

      {/* THE TOTAL */}
      <div style={{ textAlign: "right", borderTop: "2px solid black", paddingTop: 10 }}>
        <strong>Total Value: ${totalValue}</strong>
      </div>

    </div>
  );
}

// --- MAIN APP ---
export default function App() {
  const [data] = useState([
    { id: 1, name: "Tesla", type: "Stock", value: 5000 },
    { id: 2, name: "Bitcoin", type: "Crypto", value: 12000 },
    { id: 3, name: "Apple", type: "Stock", value: 3000 },
    { id: 4, name: "Ethereum", type: "Crypto", value: 4000 }
  ]);

  return <Portfolio assets={data} />;
}