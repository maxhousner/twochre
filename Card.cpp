#include "Card.h"
using namespace std;

//EFFECTS Initializes Card to the Two of Spades
Card::Card(): rank(RANK_TWO), suit(SUIT_SPADES) {}

//REQUIRES rank is one of "Two", "Three", "Four", "Five", "Six", "Seven",
//  "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"
//  suit is one of "Spades", "Hearts", "Clubs", "Diamonds"
//EFFECTS Initializes Card to specified rank and suit
Card::Card(const std::string &rank_in, const std::string &suit_in):
            rank(rank_in), suit(suit_in) {}

//EFFECTS Returns the rank
std::string Card::get_rank() const {
    return rank;
}

//EFFECTS Returns the suit.  Does not consider trump.
std::string Card::get_suit() const {
    return suit;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns the suit
//HINT: the left bower is the trump suit!
std::string Card::get_suit(const std::string &trump) const {
    if(is_left_bower(trump)) {
        return trump;
    }
    return suit;
}

//EFFECTS Returns true if card is a face card (Jack, Queen, King or Ace)
bool Card::is_face() const {
    if(rank == RANK_ACE || rank == RANK_KING || rank == RANK_QUEEN
       || rank == RANK_JACK) {
        return true;
    }
    return false;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if card is the Jack of the trump suit
bool Card::is_right_bower(const std::string &trump) const {
    if(rank == RANK_JACK && suit == trump) {
        return true;
    }
    return false;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if card is the Jack of the next suit
bool Card::is_left_bower(const std::string &trump) const {
    if(rank == RANK_JACK && suit == Suit_next(trump)) {
        return true;
    }
    return false;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if the card is a trump card.  All cards of the trump
// suit are trump cards.  The left bower is also a trump card.
bool Card::is_trump(const std::string &trump) const {
    if(suit == trump || is_left_bower(trump)) {
        return true;
    }
    return false;
}

//EFFECTS Returns true if lhs is lower value than rhs.
//  Does not consider trump.
bool operator<(const Card &lhs, const Card &rhs) {
    if(lhs == rhs) {
        return false;
    }
    int lrank = 0;
    int rrank = 0;
    int lsuit = 0;
    int rsuit = 0;
    for(int r = 0; r < NUM_RANKS; r++) {
        if(RANK_NAMES_BY_WEIGHT[r] == lhs.get_rank()) {
            lrank = r;
        }
        if(RANK_NAMES_BY_WEIGHT[r] == rhs.get_rank()) {
            rrank = r;
        }
    }
    if(lrank < rrank) {
        return true;
    }
    else if(lrank > rrank) {
        return false;
    }
    
    for(int s = 0; s < NUM_SUITS; s++) {
        if(SUIT_NAMES_BY_WEIGHT[s] == lhs.get_suit()) {
            lsuit = s;
        }
        if(SUIT_NAMES_BY_WEIGHT[s] == rhs.get_suit()) {
            rsuit = s;
        }
    }
    if(lsuit < rsuit) {
        return true;
    }
    return false;
}

//EFFECTS Returns true if lhs is higher value than rhs.
//  Does not consider trump.
bool operator>(const Card &lhs, const Card &rhs) {
    if(lhs == rhs) {
        return false;
    }
    return !(lhs < rhs);
}

//EFFECTS Returns true if lhs is same card as rhs.
//  Does not consider trump.
bool operator==(const Card &lhs, const Card &rhs) {
    int lrank = 0;
    int rrank = 0;
    int lsuit = 0;
    int rsuit = 0;
    for(int r = 0; r < NUM_RANKS; r++) {
        if(RANK_NAMES_BY_WEIGHT[r] == lhs.get_rank()) {
            lrank = r;
        }
        if(RANK_NAMES_BY_WEIGHT[r] == rhs.get_rank()) {
            rrank = r;
        }
    }
    for(int s = 0; s < NUM_SUITS; s++) {
        if(SUIT_NAMES_BY_WEIGHT[s] == lhs.get_suit()) {
            lsuit = s;
        }
        if(SUIT_NAMES_BY_WEIGHT[s] == rhs.get_suit()) {
            rsuit = s;
        }
    }
    return(lrank == rrank && lsuit == rsuit);
}

//EFFECTS Returns true if lhs is not the same card as rhs.
//  Does not consider trump.
bool operator!=(const Card &lhs, const Card &rhs) {
    return !(lhs == rhs);
}

//REQUIRES suit is a valid suit
//EFFECTS returns the next suit, which is the suit of the same color
std::string Suit_next(const std::string &suit) {
    if(suit == Card::SUIT_SPADES) {
        return Card::SUIT_CLUBS;
    }
    else if(suit == Card::SUIT_CLUBS) {
        return Card::SUIT_SPADES;
    }
    else if(suit == Card::SUIT_HEARTS) {
        return Card::SUIT_DIAMONDS;
    }
    return Card::SUIT_HEARTS;
}

//EFFECTS Prints Card to stream, for example "Two of Spades"
std::ostream & operator<<(std::ostream &os, const Card &card) {
    return os << card.get_rank() << " of " << card.get_suit();
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if a is lower value than b.  Uses trump to determine
// order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const std::string &trump) {
    // Check for the right bower
    if(a.is_right_bower(trump)) {
        return false;
    }
    if(b.is_right_bower(trump)) {
        return true;
    }
    // Check for the left bower
    if(a.is_left_bower(trump)) {
        return false;
    }
    if(b.is_left_bower(trump)) {
        return true;
    }
    // Check trump
    if(a.is_trump(trump) && !b.is_trump(trump)) {
        return false;
    }
    if(!a.is_trump(trump) && b.is_trump(trump)) {
        return true;
    }
    return a < b;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if a is lower value than b.  Uses both the trump suit
//  and the suit led to determine order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const Card &led_card,
               const std::string &trump) {
    // check for right bower
    if(b.is_right_bower(trump)) {
        return true;
    }
    if(a.is_right_bower(trump)) {
        return false;
    }
    
    // check for left bower
    if(b.is_left_bower(trump)) {
        return true;
    }
    if(a.is_left_bower(trump)) {
        return false;
    }
    
    // check trump
    if(a.is_trump(trump) && !b.is_trump(trump)) {
        return false;
    }
    if(!a.is_trump(trump) && b.is_trump(trump)) {
        return true;
    }
    if(a.is_trump(trump) && b.is_trump(trump)) {
        return  a < b;
    }
    
    // check led suit
    if((a.get_suit() == led_card.get_suit(trump)) &&
       (b.get_suit() != led_card.get_suit(trump))) {
        return false;
    }
    if((a.get_suit() != led_card.get_suit(trump)) &&
       (b.get_suit() == led_card.get_suit(trump))) {
        return true;
    }
    return a < b;
}
