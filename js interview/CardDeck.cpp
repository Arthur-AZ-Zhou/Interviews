#include <bits/stdc++.h>
using namespace std;

const vector<string> SUITS = {"Hearts", "Clubs", "Diamonds", "Spades"};
const vector<string> RANKS = {"2","3","4","5","6","7","8","9","T","J","Q","K","A"};

/**
 * @brief represents a single playing card
 */
struct Card {
    int rank;
    string suit;

    /**
     * @brief Constructs a card from rank and suit string, converts rank to interger
     * @param rankString is rank
     * @param suitString is suit
     */
    Card(string rankString, string suitString) {
        if (rankString == "A") rank = 14;
        else if (rankString == "K") rank = 13;
        else if (rankString == "Q") rank = 12;
        else if (rankString == "J") rank = 11;
        else if (rankString == "T") rank = 10;
        else rank = stoi(rankString); // "2"-"9"

        suit = suitString;
    }
};

/**
 * @brief reprsents a deck of cards
 */
class Deck {
public:
    Deck() {
        init(true);
    }

    /**
     * @brief initializes a standard 52-card deck
     * @param addJoker true if Jokers should be added (54 cards), default false
     */
    void init(bool addJokers = false) {
        //TODO: populate cardsInDeck
    }

    /**
     * @brief shuffles deck
     */
    void shuffle() {
        //TODO
    }

    /**
     * @brief deals card from either top or bottom of deck
     */
    void dealCard(bool topDeck) {

    }

private:
    deque<Card> cardsInDeck;
    stack<Card> discardCards;
}