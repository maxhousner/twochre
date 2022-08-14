#include "Player.h"
#include "Pack.h"
#include <cassert>
#include <fstream>
using namespace std;

class Game {
public:
    Game(int argc, char **argv, istream &fin): t1pts(0), t2pts(0),
    t1tricks(0), t2tricks(0), dealerIndex(0), leaderIndex(1) {
        
        Pack tempDeck(fin);
        deck = tempDeck;
        
        string shuffleInput = argv[2];
        
        if(shuffleInput == "shuffle") {
            ifshuffle = 1;
        }
        else if(shuffleInput == "noshuffle"){
            ifshuffle = 0;
        }
        else {
            ifshuffle = -1;
        }
        
        ptw = atoi(argv[3]);
        
        for(int i = 0; i < 4; i++) {
            players.push_back(Player_factory(argv[4 + 2*i], argv[5 + 2*i]));
        }
    }
    
    //EFFECTS Returns the points to win
    int get_ptw() const {
        return ptw;
    }
    
    //EFFECTS Returns team 1 points
    int get_t1pts() {
        return t1pts;
    }
    
    //EFFECTS Returns team 2 points
    int get_t2pts() {
        return t2pts;
    }
    
    //REQUIRES
    //MODIFIES
    //EFFECTS
    void set_up() {
        // shuffle
        if(ifshuffle == 1) {
            deck.shuffle();
        }
        
        // deal 3232 2323
        vector< vector<int>> dealOrders;
        dealOrders.push_back({1,1,1,2,2,3,3,3,0,0,1,1,2,2,2,3,3,0,0,0});
        dealOrders.push_back({2,2,2,3,3,0,0,0,1,1,2,2,3,3,3,0,0,1,1,1});
        dealOrders.push_back({3,3,3,0,0,1,1,1,2,2,3,3,0,0,0,1,1,2,2,2});
        dealOrders.push_back({0,0,0,1,1,2,2,2,3,3,0,0,1,1,1,2,2,3,3,3});
        
        vector<int> dealOrder = dealOrders.at(dealerIndex % 4);
        
        for(int i = 0; i < 20; i++) {
            players.at(dealOrder.at(i))->add_card(deck.deal_one());
        }
        
        // Set upcard
        upcard = deck.deal_one();
    }
    
    //REQUIRES
    //MODIFIES
    //EFFECTS
    int determine_trump() {
        // have players go around and pass or call
        int pIndex;
        
        // ROUND 1
        for(int i = dealerIndex + 1; i < dealerIndex + 5; i++) {
            pIndex = (i % 4);
            if(players.at(pIndex)->make_trump(upcard, i == dealerIndex + 4, 1, trump)) {
                cout << players.at(pIndex)->get_name() << " orders up " << trump << endl;
                cout << endl;
                players.at(dealerIndex % 4)->add_and_discard(upcard);
                return (pIndex % 2);
            }
            cout << players.at(pIndex)->get_name() << " passes" << endl;
        }
        
        // ROUND 2
        for(int i = dealerIndex + 1; i < dealerIndex + 4; i++) {
            pIndex = (i % 4);
            if(players.at(pIndex)->make_trump(upcard, false, 2, trump)) {
                // dont call add or discard
                cout << players.at(pIndex)->get_name() << " orders up " << trump << endl;
                cout << endl;
                return (pIndex % 2);
            }
            cout << players.at(pIndex)->get_name() << " passes" << endl;
        }
        
        // screw the dealer
        players.at(dealerIndex % 4)->make_trump(upcard, true, 2, trump);
        return 0;
    }
    
    //REQUIRES
    //MODIFIES
    //EFFECTS
    void play_trick() {
        int pIndex;
        int winningPlayer = leaderIndex % 4;
        // Have players play their cards
        Card ledCard = players.at(leaderIndex % 4)->lead_card(trump);
        Card playedCard;
        Card bestCard = ledCard;
        cout << ledCard << " led by " << players.at(leaderIndex % 4)->get_name() << endl;
        for(int i = leaderIndex + 1; i < leaderIndex + 4; i++) {
            pIndex = (i % 4);
            playedCard = players.at(pIndex)->play_card(ledCard, trump);
            cout << playedCard << " played by " <<
            players.at(pIndex)->get_name() << endl;
            if(Card_less(bestCard, playedCard, ledCard, trump)) {
                bestCard = playedCard;
                winningPlayer = pIndex;
            }
        }
        // Find highest card -> get next leader
        leaderIndex = winningPlayer;
        // Increment team trick counter
        if((winningPlayer % 2) == 0) {
            t1tricks++;
        }
        else {
            t2tricks++;
        }
        cout << players.at(winningPlayer)->get_name() <<
        " takes the trick" << endl;
        cout << endl;
    };
    
    //REQUIRES
    //MODIFIES
    //EFFECTS
    void determine_victory(int teamCalled) {
        if(teamCalled == 0) {
            if(t1tricks < 3) {
                cout << players.at(teamCalled + 1)->get_name() << " and "
                << players.at(teamCalled + 3)->get_name() << " win the hand" << endl;
                cout << "euchred!" << endl;
                t2pts += 2;
            }
            else if(t1tricks == 5) {
                cout << players.at(teamCalled)->get_name() << " and "
                << players.at(teamCalled + 2)->get_name() << " win the hand" << endl;
                cout << "march!" << endl;
                t1pts += 2;
            }
            else {
                cout << players.at(teamCalled)->get_name() << " and "
                << players.at(teamCalled + 2)->get_name() << " win the hand" << endl;
                t1pts++;
            }
        }
        else {
            if(t2tricks < 3) {
                cout << players.at(teamCalled - 1)->get_name() << " and "
                << players.at(teamCalled + 1)->get_name() << " win the hand" << endl;
                cout << "euchred!" << endl;
                t1pts += 2;
            }
            else if(t2tricks == 5) {
                cout << players.at(teamCalled)->get_name() << " and "
                << players.at(teamCalled + 2)->get_name() << " win the hand" << endl;
                cout << "march!" << endl;
                t2pts += 2;
            }
            else {
                cout << players.at(teamCalled)->get_name() << " and "
                << players.at(teamCalled + 2)->get_name() << " win the hand" << endl;
                t2pts++;
            }
        }
        t1tricks = 0;
        t2tricks = 0;
    }
    
    //REQUIRES
    //MODIFIES
    //EFFECTS
    void play_hand(int handCount) {
        set_up();
        cout << "Hand " << handCount << endl;
        cout << players.at(dealerIndex % 4)->get_name() << " deals" << endl;
        cout << upcard << " turned up" << endl;

        int teamCalled = determine_trump();
        
        int i = 0;
        while(i < 5) {
            play_trick();
            i++;
        }
        
        determine_victory(teamCalled);
        
        cout << players.at(0)->get_name() << " and " << players.at(2)->get_name() <<
        " have " << t1pts << " points" << endl;
        cout << players.at(1)->get_name() << " and " << players.at(3)->get_name() <<
        " have " << t2pts << " points" << endl;
        cout << endl;
        
        deck.reset();
        dealerIndex++;
        leaderIndex = dealerIndex + 1;
    }
    
    //REQUIRES
    //MODIFIES
    //EFFECTS
    bool check_valid_input() {
        if(ifshuffle == -1) {
            return false;
        }
        if(ptw > 100 || ptw < 1) {
            return false;
        }
        for(int i = 0; i < 4; i++) {
            if(players.at(i) == nullptr) {
                return false;
            }
        }
        return true;
    }
    
    void endGame() {
        if(t1pts > t2pts) {
            cout << players.at(0)->get_name() << " and " <<
            players.at(2)->get_name() << " win!\n";
        }
        else {
            cout << players.at(1)->get_name() << " and " <<
            players.at(3)->get_name() << " win!\n";
        }
        
        for(int i = 0; i < 4; i++) {
            delete players.at(i);
        }
        //cout << endl;
        players.clear();
    }
    
private:
    int t1pts, t2pts, ptw, t1tricks, t2tricks;
    int ifshuffle;
    string trump;
    vector<Player *> players;
    Pack deck;
    Card upcard;
    int dealerIndex;
    int leaderIndex;
};


int main(int argc, char **argv) {
    // preliminary check input
    if(argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return -1;
    }
    
    for(int i = 0; i < argc; i++) {
        cout << argv[i] << " ";
    }
    cout << endl;
    
    ifstream inputFile;
    inputFile.open(argv[1]);
    
    if(!inputFile.is_open()) {
        cout << "Error opening " << argv[1] << endl;
        return -1;
    }
    
    Game game(argc, argv, inputFile);
    
    inputFile.close();
    
    if(!game.check_valid_input()) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        
        return -1;
    }
    
    int handCount = 0;
    while(game.get_t1pts() < game.get_ptw() && game.get_t2pts() < game.get_ptw()) {
        game.play_hand(handCount);
        handCount++;
    }
    
    // winner
    game.endGame();
    return 0;
}
