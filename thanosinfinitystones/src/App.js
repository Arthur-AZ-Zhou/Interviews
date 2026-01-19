import React, { useState } from "react";

// ==========================================
// 1. DATA HELPERS & STYLES
// ==========================================

const generateMarketData = () => {
  const data = [];

  for (let i = 0; i < 50; i++) {
    data.push({
      id: "ord-" + i,
      sym: "STK-" + (100 + i),
      price: (Math.random() * 400 - 100).toFixed(2),
      qty: Math.floor(Math.random() * 500),
    });
  }

  return data;
}

// Internal CSS for the dashboard layout
const styles = { //CAN USE LITERALS LIKE "WHITE" OR "BLACK"
  container: { padding: "20px", fontFamily: "sans-serif", backgroundColor: "#f9f9f9", minHeight: "100vh" },
  grid: { display: "grid", gridTemplateColumns: "1fr 1fr 1fr", gap: "20px" },
  card: { backgroundColor: "white", padding: "20px", borderRadius: "8px", border: "1px solid #ddd", boxShadow: "0 2px 4px rgba(0,0,0,0.1)" },
  scrollBox: { height: "300px", overflowY: "auto", border: "1px solid #eee" },
  row: { display: "flex", justifyContent: "space-between", alignItems: "center", padding: "8px 0", borderBottom: "1px solid #eee" },
  input: { padding: "8px", width: "100%", boxSizing: "border-box", marginBottom: "10px" },
  green: { color: "green", fontWeight: "bold" },
  red: { color: "red", fontWeight: "bold" }
};

// ==========================================
// 2. SUB-COMPONENTS
// ==========================================

// --- COMPONENT A: SCROLLABLE ORDER BOOK ---
function OrderBook({ orders }) {
  const [showLarge, setShowLarge] = useState("");
  const visible = showLarge? orders.filter(o => 250 <= o.qty) : orders;

  return (
    <div>
      <div style={{ display: "flex", justifyContent: "space-between", alignItems: "center", marginBottom: 10 }}>
        <h3>LIVE MARKET</h3>
        <button onClick={() => setShowLarge(!showLarge)}>
          {showLarge? "Show All" : "Large Orders Only (<250)"}
        </button>
      </div>

      <div style={styles.scrollBox}>
        {visible.map((order) => (
          <div key={order.id} style={styles.row}>
            <span style={{ width: "33%" }}>{order.sym}</span>
             {/* ... means copy paste in this context, as in copy from styles.green/styles.red */}
            <span style={{ width: "33%", ... (0 <= order.price? styles.green : styles.red) }}>{order.price}</span> 
            <span style={{ width: "33%"}}>{order.qty}</span>
          </div>
        ))}
      </div>
    </div>
  )
}

// --- COMPONENT B: MARKET SEARCH ---
function StockSearch({ stocks }) {
  const [searchTerm, setSearchTerm] = useState("");

  const filtered = stocks.filter(s => {
    // if (searchTerm === "") return false; // Show nothing if empty
    return s.symbol.toLowerCase().includes(searchTerm.toLowerCase());
  });

  return (
    <div style={{ marginBottom: "20px" }}>
      <h4>Symbol Lookup</h4>

      <input
        type="text" placeholder="Search Market (ie GOOG)"
        style={styles.input}
        value={searchTerm} onChange={(e) => setSearchTerm(e.target.value)}
      />

      {(0 < filtered.length)? (
        <div style={{ border: "1px solid #eee", padding: "5px", background: "#fff" }}>
          {filtered.map(s => <div key={s.symbol} style={{ padding: "5px", fontWeight: "bold" }}>{s.symbol} - {s.name}</div>)}
        </div>
      ) : (
        <div style={{ color: "grey", padding: "10px"}}>No results found.</div>
      )}
    </div>
  );
}

// --- COMPONENT C: WATCHLIST (With Stars) ---
function WatchList({ stocks, onToggle }) {
  const count = stocks.filter(s => s.isWatched).length;

  return (
    <div>
      <h3>Watchlist ({count})</h3>

      {stocks.map(stock => (
        <div key={stock.id} style={styles.row}>
          <div>
            <strong>{stock.symbol}</strong>
            <span style={{ marginLeft: "10px" }}>${stock.price}</span>
          </div>

          <button
            onClick={() => onToggle(stock.id)}
            style={{ background: "none", border: "none", fontSize: "20px", cursor: "pointer", color: stock.isWatched? "gold" : "grey"}}
          > 
            *
          </button>
        </div>
      ))}
    </div>
  );
}

// --- COMPONENT D: TRADE EXECUTION (Form + List) ---
function TradeExecution({ trades, onAdd, onDelete }) {
  const [symbol, setSymbol] = useState("");
  const [price, setPrice] = useState("");

  const handleSubmit = (e) => {
    e.preventDefault();
    if (!symbol || !price) return;
    onAdd({ id: Date.now(), symbol: symbol.toUpperCase(), price: parseFloat(price) });
    setSymbol(""); setPrice("");
  };

  return (
    <div>
      <h3>Order Execution</h3>
      
      {/* The Form */}
      <form onSubmit={handleSubmit} style={{ marginBottom: 20, display: "flex", gap: "5px" }}>
        <input style={{...styles.input, marginBottom: 0}} placeholder="SYM" value={symbol} onChange={e => setSymbol(e.target.value)} />
        <input style={{...styles.input, marginBottom: 0}} type="number" placeholder="$$$" value={price} onChange={e => setPrice(e.target.value)} />
        <button type="submit">Buy</button>
      </form>

      {/* The List */}
      <div style={styles.scrollBox}>
        {trades.length === 0 ? <p style={{padding:10, color:"gray"}}>No open positions.</p> : 
          trades.map(t => (
            <div key={t.id} style={styles.row}>
              <span>
                <strong>{t.symbol}</strong> 
                <span style={{ marginLeft: 10, ... (t.price >= 0 ? styles.green : styles.red) }}>{t.price}</span>
              </span>
              <button onClick={() => onDelete(t.id)} style={{background:"red", color:"white", border:"none", cursor:"pointer"}}>X</button>
            </div>
          ))
        }
      </div>
    </div>
  );
}

export default function App() {
  // --- STATE 0: HOW TO PRINT (Random Generated) -- 
  const result = generateMarketData();
  console.log(result);

  // --- STATE 1: Market Data (Generated) ---
  const [marketOrders] = useState(generateMarketData());
  const [searchData] = useState([
    { symbol: "AAPL", name: "Apple" },
    { symbol: "TSLA", name: "Tesla" },
    { symbol: "GOOG", name: "Google" },
  ]);

  // --- STATE 2: Watchlist ---
  const [watchedStocks, setWatchedStocks] = useState([
    { id: 1, symbol: "NVDA", price: 420.50, isWatched: true },
    { id: 2, symbol: "AMD", price: 105.00, isWatched: false },
    { id: 3, symbol: "INTC", price: 30.25, isWatched: false },
  ]);

  // --- HANDLERS ---
  const toggleWatch = (id) => {
    setWatchedStocks(watchedStocks.map(s => 
      s.id === id ? {...s, isWatched: !s.isWatched } : s
    ));
  };

  // --- STATE 3: User Trades ---
  const [myTrades, setMyTrades] = useState([
    { id: 101, symbol: "MSFT", price: 310.00 }
  ]);

  // --- HANDLERS ---
  const addTrade = (trade) => setMyTrades([trade, ...myTrades]);
  const deleteTrade = (id) => setMyTrades(myTrades.filter(t => t.id !== id));


  return (
    <div style={styles.container}>
      <h1 style={{ textAlign: "center", marginBottom: "30px"}}>HFT DASHBOARD</h1>

      <div style={ styles.grid }>

        {/* COLUMN 1: Market Data */}
        <div style={styles.card}>
          <OrderBook orders={ marketOrders } />

          <hr style={{ margin: "20px 0" }} />

          <StockSearch stocks={ searchData } />
        </div>

        {/* COLUMN 2: Watchlist */}
        <div style={styles.card}>
          <WatchList stocks={watchedStocks} onToggle={toggleWatch} />
        </div>

        {/* COLUMN 3: Execution */}
        <div style={styles.card}>
          <TradeExecution trades={myTrades} onAdd={addTrade} onDelete={deleteTrade} />
        </div>
      </div>
    </div>
  )
}