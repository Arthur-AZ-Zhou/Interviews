import React, { useState } from "react";

// --- TODO: IMPLEMENT THIS COMPONENT ---
function StrategyList({ strategies, onToggle }) {
  const activeCount = strategies.filter((strats) => strats.isRunning === true).length;
  
  return (
    <div style={{ padding: 20, fontFamily: "sans-serif" }}>
      <h3>Algo Control Panel</h3>
      
      {/* TODO 2: Display "Systems Online: X" */}
      <div style={{marginBottom: 15}}> 
        <strong>Systems Online: {activeCount}</strong>
      </div>
      
      <div style={{ display: "grid", gap: "10px" }}>
        <div className="header-row" style={{ display: "flex", fontWeight: "bold", borderBottom: "1px solid grey", paddingBottom: "5px"}}>
          <span style={{ width: 300 }}>TradeBot Name</span>
          <span style={{ width: 300 }}>Risk Level</span>
          <span style={{ width: 200 }}>Running Status</span>
        </div>

        {strategies.length === 0? (
          <p style={{marginTop: 10}}>NO BOTS TO SHOW</p>
        ) : (
          strategies.map((strats) => (
            <div key={strats.id} className="strat-row" style={{ display: "flex", fontWeight: "bold" }}>
              <span style={{ width: 300 }}>{strats.name}</span>
              <span style={{ width: 300, color: ((strats.risk === "High")? "red" : "blue") }}>{strats.risk}</span>
              <span style={{ width: 200, color: ((strats.isRunning)? "lightgreen" : "grey") }}>{(strats.isRunning)? "ONLINE" : "OFFLINE"}</span>

              {/* Add a Button that says "STOP" if running, "START" if paused */}
              {/* The button onClick should call onToggle(strategy.id) */}
              <button
                onClick={() => onToggle(strats.id)}
                style={{
                  cursor: "pointer",
                  fontSize: "15px",
                  fontWeight: "bold",
                  background: "lightgrey",
                  border: "none",
                  borderRadius: "5px",
                  color: strats.isRunning? "darkgreen" : "black",
                }}
              >
                {(strats.isRunning)? "STOP" : "START"}
              </button>
            </div>
          ))
        )}

        

      </div>
    </div>
  );
}

// --- MAIN APP (DO NOT TOUCH) ---
export default function App() {
  const [algos, setAlgos] = useState([
    { id: 1, name: "Momentum Bot", risk: "High", isRunning: true },
    { id: 2, name: "Arbitrage Safe", risk: "Low", isRunning: false },
    { id: 3, name: "Mean Reversion", risk: "Low", isRunning: true },
    { id: 4, name: "Crypto Yield", risk: "High", isRunning: false }
  ]);

  const handleToggle = (id) => {
    setAlgos(algos.map(algo => {
      if (algo.id === id) {
        return { ...algo, isRunning: !algo.isRunning };
      }
      return algo;
    }));
  };

  return (
    <StrategyList strategies={algos} onToggle={handleToggle} />
  );
}