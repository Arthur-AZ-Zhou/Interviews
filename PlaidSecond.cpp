#include <bits/stdc++.h>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
using namespace std;

/*
# Example Coupon
#
# Exactly one of percent_discount and amount_discount will be non-null (error if not).
# The two 'minimum_...' values can each be null or non-null.
{ 'category': 'fruit',
  'percent_discount': 15,
  'amount_discount': None,
  'minimum_num_items_required': 2,
  'minimum_amount_required': 10.00
}
# Example Cart vector<pair<double, string>>
[ {'price': 2.00, 'category': 'fruit'},
  {'price': 20.00, 'category': 'toy'},
  {'price': 5.00, 'category': 'clothing'},
  {'price': 8.00, 'category': 'fruit'}
]
*/

/*
[
  { 'categories': ['clothing', 'toy'],
    'percent_discount': None,
    'amount_discount': 6,
    'minimum_num_items_required': None,
    'minimum_amount_required': None
  },
  { 'categories': ['fruit'],
    'percent_discount': 15,
    'amount_discount': None,
    'minimum_num_items_required': 2,
    'minimum_amount_required': 10.00
   }
]

//INVALID COUPONS:
## Coupons
[ { 'categories': ['clothing', 'toy'],
    'percent_discount': None,
    'amount_discount': 6,
    'minimum_num_items_required': None,
    'minimum_amount_required': None
  },
  { 'categories': ['toy'],
    'percent_discount': 15,
    'amount_discount': None,
    'minimum_num_items_required': 2,
    'minimum_amount_required': 10.00
   }
]
*/

const double NULL_VALUE = -1.0;

class Coupon {
    public:
    string category;
    double percent_discount;
    double amount_discount;
    int minimum_num_items_required;
    double minimum_amount_required;

    Coupon(string category, double percent_discount, double amount_discount,
        int minimum_num_items_required, double minimum_amount_required)
      : category(category),
        percent_discount(percent_discount),
        amount_discount(amount_discount),
        minimum_num_items_required(minimum_num_items_required),
        minimum_amount_required(minimum_amount_required) {}
};

class CouponExtended : public Coupon {
    public:
    vector<string> categories;

    CouponExtended(vector<string> categories, double percent_discount, double amount_discount,
        int minimum_num_items_required, double minimum_amount_required)
        : Coupon("", percent_discount, amount_discount, minimum_num_items_required, minimum_amount_required),
        categories(categories) {}
};

class Item {
    public:
    double price;
    string category;

    Item(double price, string category) : price(price), category(category) {}
};

double cart_total_m2(vector<CouponExtended> coupons, vector<Item> cart) {
    unordered_map<string, double> categoryTotalPrice; //category -> total price
    unordered_map<string, int> categoryTotalItems; //category -> total amount of items
    double totalAmountAllItems = 0.0;

    for (Item& i : cart) {
        categoryTotalPrice[i.category] += i.price;
        categoryTotalItems[i.category]++;
        totalAmountAllItems += i.price;
    }

    //check all extended coupons for overlap, if invalid return cart total
    unordered_set<string> allCouponCategories; 

    try {
        for (CouponExtended& c : coupons) {
            for (string cat : c.categories) {
                if (allCouponCategories.find(cat) == allCouponCategories.end()) {
                    allCouponCategories.insert(cat);
                } else {
                    throw runtime_error("DUPE COUPON CATEGORY");
                }
            }
        }
    } catch (const exception& e) {
        cerr << "CAUGHT DUPLICATE COUPON CATEGORY" << endl;
        return totalAmountAllItems;
    }

    //if all coupons valid, we can start scanning through coupons, call cart_total for each category and compare greatest discount
    for (CouponExtended& c : coupons) {
        double greatestDiscount = -1.0;

        for (string cat : c.categories) {
            // cout << "current cat: " << cat << endl;

            try {
                if (c.percent_discount != NULL_VALUE && c.amount_discount != NULL_VALUE 
                    || c.percent_discount == NULL_VALUE && c.amount_discount == NULL_VALUE) {
                    throw runtime_error("NOT XOR");
                } else {
                    double discountedPrice = categoryTotalPrice[cat];

                    if ((c.minimum_num_items_required == NULL_VALUE || c.minimum_num_items_required <= categoryTotalItems[cat]) 
                        && (c.minimum_amount_required == NULL_VALUE || c.minimum_amount_required <= categoryTotalPrice[cat])) {
                        if (c.percent_discount != NULL_VALUE) {
                            discountedPrice *= (1 - 0.01 * c.percent_discount);
                            // cout << "discountedPrice: " << discountedPrice << endl;
                        } else {
                            discountedPrice -= c.amount_discount;

                            if (discountedPrice < 0) {
                                discountedPrice = 0;
                            }

                            // cout << "discountedPrice: " << discountedPrice << endl;
                            
                        }

                        greatestDiscount = max(categoryTotalPrice[cat] - discountedPrice, greatestDiscount);
                    }
                }
            } catch (const exception& e) {
                cerr << "CAUGHT ERROR DISCOUNTS NOT FORMATTED CORRECTLY" << endl;
            }
        }

        cout << "greatest discount: " << greatestDiscount << endl;
        totalAmountAllItems -= greatestDiscount;
    }

    return totalAmountAllItems;
}

/**
 * @brief find greatest discount based on category
 */
double cart_total(Coupon coupon, vector<Item> cart) {
    //scan thru cart, sum up all stats based on coupon's category
    //return cart total - discount obtained from coupon
    int numItems = 0;
    double totalAmount = 0.0;
    double totalAmountAllOtherItems = 0.0;

    for (Item& i : cart) {
        if (i.category == coupon.category) {
            numItems++;
            totalAmount += i.price;
        } else {
            totalAmountAllOtherItems += i.price;
        }
    }

    try {
        if (coupon.percent_discount != NULL_VALUE && coupon.amount_discount != NULL_VALUE 
            || coupon.percent_discount == NULL_VALUE && coupon.amount_discount == NULL_VALUE) {
            throw runtime_error("NOT XOR");
        } else {
            if ((coupon.minimum_num_items_required == NULL_VALUE || coupon.minimum_num_items_required <= numItems) 
                && (coupon.minimum_amount_required == NULL_VALUE || coupon.minimum_amount_required <= totalAmount)) {
                if (coupon.percent_discount != NULL_VALUE) {
                    totalAmount *= (1 - 0.01 * coupon.percent_discount);
                } else {
                    totalAmount -= coupon.amount_discount;

                    if (totalAmount < 0) {
                        totalAmount = 0;
                    }
                }
            }
        }
    } catch (const exception& e) {
        cerr << "CAUGHT ERROR DISCOUNTS NOT FORMATTED CORRECTLY" << endl;
    }

    return totalAmountAllOtherItems + totalAmount;
}

int main() {
    CouponExtended c1({"clothing", "toy"}, NULL_VALUE, 6, NULL_VALUE, NULL_VALUE);
    CouponExtended c2({"fruit"}, 15.0, NULL_VALUE, 2, 10.0); 

    vector<CouponExtended> ce = {c1, c2};
    vector<Item> cart;

    cart.push_back(Item(2.00, "fruit"));
    cart.push_back(Item(20.00, "toy"));
    cart.push_back(Item(5.00, "clothing"));
    cart.push_back(Item(8.00, "fruit"));
    auto total = cart_total_m2(ce, cart);

    cout << "cart total: " << total << endl;
}