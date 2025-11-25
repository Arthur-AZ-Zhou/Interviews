def calculate_pie_profit(note, recipe, ingredient_prices, bids):
    parts = note.split()
    have = {}
    for x in parts:
        have[x] = have.get(x, 0) + 1

    need = {}
    for r in recipe:
        cnt = r["count"]
        if "subrecipe" in r:
            for base, amt in r["subrecipe"].items():
                need[base] = need.get(base, 0) + (amt * cnt)
        else:
            ing = r["ingredient"]
            need[ing] = need.get(ing, 0) + cnt

    cost_one = 0
    for ing, c in need.items():
        price_str = ingredient_prices.get(ing, "$0.00")
        price = int(float(price_str.replace("$", "")) * 100)
        cost_one += c * price

    profit = 0

    sorted_bids = sorted(
        bids.items(), 
        key=lambda x: float(x[1]["price"].replace("$", "")), 
        reverse=True
    )

    for name, bid in sorted_bids:
        wanted_pies = bid["count"]
        
        # --- LOGIC FIX: Partial Filling ---
        # Instead of checking if we can make ALL wanted_pies,
        # we calculate the max we can make with current inventory.
        
        actual_pies_to_make = wanted_pies
        
        for ing, cnt in need.items():
            if cnt > 0:
                available = have.get(ing, 0)
                can_make = available // cnt
                actual_pies_to_make = min(actual_pies_to_make, can_make)

        # If we can't make any, skip
        if actual_pies_to_make == 0:
            continue

        # Deduct ingredients based on what we actually make
        for ing, cnt in need.items():
            have[ing] -= cnt * actual_pies_to_make

        sell_price = int(float(bid["price"].replace("$", "")) * 100)
        
        if sell_price > cost_one:
            profit += actual_pies_to_make * (sell_price - cost_one)

    return profit

# --- TEST CASE ---
note = "blueberry butter butter blueberry blue green yellow" # Enough for 2
recipe = [{"ingredient": "blueberry", "count": 1}, {"ingredient": "butter", "count": 1}]
prices = {"blueberry": "$1.00", "butter": "$2.00"}
bids = {
    "anton": {"count": 5, "price": "$20.00"}, # Wants 5, we give 2
    "lillian": {"count": 10, "price": "$10.00"}
}

# Expect: 2 pies * ($20.00 - $3.00) = $34.00 -> 3400
print(f"Profit: {calculate_pie_profit(note, recipe, prices, bids)}")