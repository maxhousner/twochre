#include "Pack.h"
using namespace std;

// EFFECTS: Initializes the Pack to be in the following standard order:
//          the cards of the lowest suit arranged from lowest rank to
//          highest rank, followed by the cards of the next lowest suit
//          in order from lowest to highest rank, and so on.
// NOTE: The standard order is the same as that in pack.in.
// NOTE: Do NOT use pack.in in your implementation of this function
Pack::Pack(): next(0) {
    int rank = 7;
    for(int i = 0; i < PACK_SIZE; i++)
    {
        if(rank == NUM_RANKS) {
            rank = 7;
        }
        cards[i] = Card(RANK_NAMES_BY_WEIGHT[rank], SUIT_NAMES_BY_WEIGHT[i/6]);
        rank++;
    }
}

// REQUIRES: pack_input contains a representation of a Pack in the
//           format required by the project specification
// MODIFIES: pack_input
// EFFECTS: Initializes Pack by reading from pack_input.
Pack::Pack(std::istream& pack_input): next(0) {
    for(int i = 0; i < PACK_SIZE; i++)
    {
        string rank, suit, theWordOf;
        pack_input >> rank >> theWordOf >> suit;
        cards[i] = Card(rank, suit);
    }
}

// REQUIRES: cards remain in the Pack
// EFFECTS: Returns the next card in the pack and increments the next index
Card Pack::deal_one() {
    Card nextCard = cards[next];
    next++;
    return nextCard;
}

// EFFECTS: Resets next index to first card in the Pack
void Pack::reset() {
    next = 0;
}

// EFFECTS: Shuffles the Pack and resets the next index. This
//          performs an in shuffle seven times. See
//          https://en.wikipedia.org/wiki/In_shuffle.
void Pack::shuffle() {
    int counter = 0;
    int j = 0;
    Card tempCards[PACK_SIZE];
    while(counter < 7)
    {
        // top 12 cards start the shuffle (top 2nd from top)
        j = 1;
        for(int i = 0; i < PACK_SIZE/2; i++)
        {
            tempCards[j] = cards[i];
            j += 2;
        }
        
        // bottom 12 cards end the shuffle
        j = 0;
        for(int i = PACK_SIZE/2; i < PACK_SIZE; i++)
        {
            tempCards[j] = cards[i];
            j += 2;
        }
        
        // assign the shuffled cards to the card array
        for(int k = 0; k < PACK_SIZE; k++)
        {
            cards[k] = tempCards[k];
        }
        
        counter++;
    }
}

// EFFECTS: returns true if there are no more cards left in the pack
bool Pack::empty() const {
    if(next < PACK_SIZE) {
        return false;
    }
    return true;
}
