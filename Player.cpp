#include "Player.h"
#include <algorithm>
#include <cassert>

using namespace std;

class Human : public Player {
public:
    
    Human(const std::string &name_in): name(name_in) {}
    
    //EFFECTS returns player's name
    const std::string & get_name() const override {
        return name;
    }
    
    //REQUIRES player has less than MAX_HAND_SIZE cards
    //EFFECTS  adds Card c to Player's hand
    void add_card(const Card &c) override {
        assert(static_cast<int>(hand.size()) < MAX_HAND_SIZE);
        hand.push_back(c);
        std::sort(hand.begin(), hand.end());
    }
    
    //REQUIRES round is 1 or 2
    //MODIFIES order_up_suit
    //EFFECTS If Player wishes to order up a trump suit then return true and
    //  change order_up_suit to desired suit.  If Player wishes to pass, then do
    //  not modify order_up_suit and return false.
    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, std::string &order_up_suit) const override {
        assert(round == 1 || round == 2);
        
        print_hand();
        
        // Screw the dealer implementation
        if(is_dealer && round == 2) {
            // prompt the user for input without option to pass
            string userOrder;
            cout << "Human player " << name << ", please enter a suit:" << endl;
            cin >> userOrder;
            
            order_up_suit = userOrder;
            return true;
        }
        
        // prompt the user for input
        string userOrder;
        cout << "Human player " << name << ", please enter a suit, or \"pass\":" << endl;
        cin >> userOrder;
        
        if(userOrder == "pass") {
            return false;
        }
        order_up_suit = userOrder;
        return true;
    }
    
    //REQUIRES Player has at least one card
    //EFFECTS  Player adds one card to hand and removes one card from hand.
    void add_and_discard(const Card &upcard) override {
        
        print_hand();
        
        int userCardIndex;
        cout << "Discard Upcard: [-1]" << endl;
        cout << "Human player " << name << ", please select a card to discard:" << endl;
        cin >> userCardIndex;
        
        if(userCardIndex != -1) {
            // Discard the desired card if not the upcard AND ADD UPCARD
            // Card leadCard = hand.at(userCardIndex);
            hand.erase(hand.begin() + userCardIndex);
            add_card(upcard);
        }
        
        // If the upcard is discarded, do nothing
    }
    
    //REQUIRES Player has at least one card, trump is a valid suit
    //EFFECTS  Leads one Card from Player's hand according to their strategy
    //  "Lead" means to play the first Card in a trick.  The card
    //  is removed the player's hand.
    Card lead_card(const std::string &trump) override {
        
        print_hand();
        
        int userCardIndex;
        cout << "Human player " << name << ", please select a card:" << endl;
        cin >> userCardIndex;
        
        // Discard the lead card
        Card leadCard = hand.at(userCardIndex);
        hand.erase(hand.begin() + userCardIndex);
        
        return leadCard;
    }
    
    //REQUIRES Player has at least one card, trump is a valid suit
    //EFFECTS  Plays one Card from Player's hand according to their strategy.
    //  The card is removed from the player's hand.
    Card play_card(const Card &led_card, const std::string &trump) override {
        return lead_card(trump);
    }
    
    void print_hand() const {
        for(int i = 0; i < static_cast<int>(hand.size()); i++) {
            Card currCard = hand.at(i);
            cout << "Human player " << name << "'s hand: [" << i << "] " << currCard;
            cout << endl;
        }
    }
    
private:
    // your member vars here
    
    //name
    string name;
    //hand (vector of cards)
    vector<Card> hand;
};


class Simple : public Player {
public:
    //copy function stubs w/o virtual and w/ override<-(not req.)
    Simple(const std::string &name_in): name(name_in) {}
    
    //EFFECTS returns player's name
    const std::string & get_name() const override {
        return name;
    }
    
    //REQUIRES player has less than MAX_HAND_SIZE cards
    //EFFECTS  adds Card c to Player's hand
    void add_card(const Card &c) override {
        assert(static_cast<int>(hand.size()) < MAX_HAND_SIZE);
        hand.push_back(c);
    }
    
    //REQUIRES round is 1 or 2
    //MODIFIES order_up_suit
    //EFFECTS If Player wishes to order up a trump suit then return true and
    //  change order_up_suit to desired suit.  If Player wishes to pass, then do
    //  not modify order_up_suit and return false.
    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, std::string &order_up_suit) const override {
        
        // Could be 1 or 0 for the first round
        assert(round == 1 || round == 2);
        string trump = upcard.get_suit();
        string next = Suit_next(trump);
        
        // Screw the dealer implementation
        if(is_dealer && round == 2) {
            order_up_suit = next;
            return true;
        }
        
        // First Round
        if(round == 1) {
            int faceTrumpCounter = 0;
            
            // Check if the player has two or more trump face cards
            for(int i = 0; i < static_cast<int>(hand.size()); i++) {
                Card currCard = hand.at(i);
                if(currCard.is_face() && currCard.is_trump(trump)) {
                    faceTrumpCounter++;
                }
            }
            
            // If they do, order up and return true
            if (faceTrumpCounter >= 2) {
                // edit order up suit idk if this is right
                order_up_suit = trump;
                return true;
            }
        }
        
        // Second Round
        if(round == 2) {
            int faceColorCounter = 0;
            
            // Check if the player has two or more trump face cards
            for(int i = 0; i < static_cast<int>(hand.size()); i++) {
                Card currCard = hand.at(i);
                
                // if the card is a face next suit
                if(currCard.is_face() && currCard.is_trump(next)) {
                    faceColorCounter++;
                }
            }
            
            // If they do, order up and return true
            if (faceColorCounter >= 1) {
                order_up_suit = next;
                return true;
            }
        }
        return false;
    }
    
    //REQUIRES Player has at least one card
    //EFFECTS  Player adds one card to hand and removes one card from hand.
    void add_and_discard(const Card &upcard) override {
        assert(static_cast<int>(hand.size()) >= 1);
        string trump = upcard.get_suit();
        Card disCard = hand.at(0); // this is hilarious i hope it's appreciated
        int disCardIndex = 0;
        //add_card(upcard);
        hand.push_back(upcard);
        
        // Find lowest and remove from hand
        for(int i = 1; i < static_cast<int>(hand.size()); i++) {
            Card currCard = hand.at(i);
            if(Card_less(currCard, disCard, trump)) {
                disCard = currCard;
                disCardIndex = i;
            }
        }
        hand.erase(hand.begin() + disCardIndex);
    }
    
    //REQUIRES Player has at least one card, trump is a valid suit
    //EFFECTS  Leads one Card from Player's hand according to their strategy
    //  "Lead" means to play the first Card in a trick.  The card
    //  is removed the player's hand.
    Card lead_card(const std::string &trump) override {
        assert(static_cast<int>(hand.size()) >= 1);
        
        int trumpCounter = 0;
        int bestCardIndex = 0;
        Card bestCard;
        if(bestCard.get_suit(trump) == trump) {
            Card bestCard2(Card::RANK_TWO, Card::SUIT_HEARTS);
            bestCard = bestCard2;
        }
        
        // Count the number of trump
        for(int i = 0; i < static_cast<int>(hand.size()); i++) {
            Card currCard = hand.at(i);
            if (currCard.is_trump(trump)) {
                trumpCounter++;
            }
        }
        
        if(trumpCounter == static_cast<int>(hand.size())){
            for(int i = 0; i < static_cast<int>(hand.size()); i++) {
                Card currCard = hand.at(i);
                if(Card_less(bestCard, currCard, trump)) {
                    bestCard = currCard;
                    bestCardIndex = i;
                }
            }
        }
        
        
        // Look for best card in the hand
        else {
            for(int i = 0; i < static_cast<int>(hand.size()); i++) {
                Card currCard = hand.at(i);
                if(!currCard.is_trump(trump) && Card_less(bestCard, currCard, trump)) {
                    bestCard = currCard;
                    bestCardIndex = i;
                }
            }
        }
        
        // Remove card from hand
        hand.erase(hand.begin() + bestCardIndex);
        
        return bestCard;
    }
    
    //REQUIRES Player has at least one card, trump is a valid suit
    //EFFECTS  Plays one Card from Player's hand according to their strategy.
    //  The card is removed from the player's hand.
    Card play_card(const Card &led_card, const std::string &trump) override {
        assert(static_cast<int>(hand.size()) > 0);
        
        Card bestCard;
        if(bestCard.get_suit(trump) == trump) {
            Card bestCard2(Card::RANK_TWO, Card::SUIT_HEARTS);
            bestCard = bestCard2;
        }
        int bestCardIndex = 0;
        int ledCounter = 0;
        Card worstCard = hand.at(0);
        int worstCardIndex = 0;
        
        // Look for best card in the hand but also consider led card
        for(int i = 0; i < static_cast<int>(hand.size()); i++) {
            Card currCard = hand.at(i);
            if(hand.at(i).get_suit(trump) == led_card.get_suit(trump)) {
                if(Card_less(bestCard, currCard, led_card, trump)) {
                    bestCard = currCard;
                    bestCardIndex = i;
                }
                ledCounter++;
            }
            else {
                if(!Card_less(worstCard, currCard, led_card, trump)) {
                    worstCard = currCard;
                    worstCardIndex = i;
                }
            }
        }
        if(ledCounter > 0) {
            hand.erase(hand.begin() + bestCardIndex);
            
            return bestCard;
        }
        hand.erase(hand.begin() + worstCardIndex);
        
        return worstCard;
    }
    
private:
    // your member vars here
    
    //name
    string name;
    //hand (vector of cards)
    vector<Card> hand;
};


//EFFECTS: Returns a pointer to a player with the given name and strategy
//To create an object that won't go out of scope when the function returns,
//use "return new Simple(name)" or "return new Human(name)"
//Don't forget to call "delete" on each Player* after the game is over
Player * Player_factory(const std::string &name, const std::string &strategy) {
    if(strategy == "Simple") {
        return new Simple(name);
    }
    else if(strategy == "Human") {
        return new Human(name);
    }
    //assert(false);
    return nullptr;
}

//EFFECTS: Prints player's name to os
std::ostream & operator<<(std::ostream &os, const Player &p) {
    return os << p.get_name();
}
