#include <iostream>
#include <stack>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
#include <algorithm>  // For random_shuffle

using namespace std;

class Card {
public:
    string value;
    string suit;

    Card(string val, string s) : value(val), suit(s) {}

    string getCardRepresentation() {
        return value + "(" + suit + ")";
    }

    string displayCard() {
        return " " + value + "(" + suit + ") ";
    }
};

class Player {
public:
    string name;
    Card* hand[7];
    int handSize;

    Player(string playerName) : name(playerName), handSize(0) {}

    void drawCard(Card* card) {
        if (handSize < 7) {
            hand[handSize++] = card;
        }
    }

    void playCard(int index) {
        if (index >= 0 && index < handSize) {
            for (int i = index; i < handSize - 1; i++) {
                hand[i] = hand[i + 1];
            }
            handSize--;
        } else {
            cout << "Invalid card index.\n";
        }
    }

    void showHand() {
        cout << name << "'s hand: ";
        for (int i = 0; i < handSize; i++) {
            cout << hand[i]->displayCard() << (i == handSize - 1 ? "" : ", ");
        }
        cout << endl;
    }
};

class Board {
public:
    string grid[10][10];
    Card* wildSpace;

    Board() {
        // Initialize the wild space at lower-left corner
        wildSpace = new Card("Wild", "X");
        grid[9][0] = wildSpace->getCardRepresentation();  // Wild space at lower-left corner

        // Fill the board with shuffled cards from the deck
        fillBoardWithCards();
    }

    void fillBoardWithCards() {
        vector<Card*> deck;
        string cardValues[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
        string suits[] = {"H", "S", "C", "D"};
        
        // Fill the deck with cards
        for (const string& suit : suits) {
            for (const string& value : cardValues) {
                deck.push_back(new Card(value, suit));
            }
        }

        // Shuffle the deck
        random_shuffle(deck.begin(), deck.end());

        // Check if the deck is large enough (it should have 100 cards for a 10x10 grid)
        while (deck.size() < 100) {
            // Adding extra cards if deck size is less than 100
            for (const string& suit : suits) {
                for (const string& value : cardValues) {
                    deck.push_back(new Card(value, suit));
                }
            }
        }

        // Place cards on the board, ensuring no "Empty" cards are placed
        int index = 0;
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (index < deck.size()) {
                    grid[i][j] = deck[index]->getCardRepresentation();
                    index++;
                }
            }
        }
    }

    void displayBoard() {
        cout << "\nGame Board (10x10 grid with cards):\n";
        cout << "  +----------------------------------------------------------------------------------------------------------------------------------------------+\n";

        for (int i = 0; i < 10; i++) {
            cout << "  |";
            for (int j = 0; j < 10; j++) {
                cout << setw(12) << grid[i][j] << " |";
            }
            cout << "\n  +------------------------------------------------------------------------------------------------------------------------------------------+\n";
        }
    }

    bool placeCard(int row, int col, Card* card, string playerName) {
        if (row >= 0 && row < 10 && col >= 0 && col < 10) {
            // Check if the spot is either "Empty", "Wild" or if it matches the player's card
            if (grid[row][col] == "Empty" || grid[row][col] == wildSpace->getCardRepresentation() || 
                grid[row][col].find(card->value) != string::npos) {
                grid[row][col] = playerName + "'s " + card->getCardRepresentation();  // Display player's name with the card
                return true;
            }
        }
        return false;
    }

    bool checkSequence() {
        // Check for 5 consecutive cards horizontally, vertically, or diagonally
        return false;
    }
};

class Game {
private:
    vector<Player*> players;
    int playerCount;
    int teamCount;
    vector<vector<Player*>> teams;
    Board board;
    stack<Card*> deck;
    int currentPlayerIndex;

public:
    Game() : currentPlayerIndex(0) {
        initializeDeck();
        teamCount = 0;
        playerCount = 0;
    }

    void askPlayersAndTeams() {
        // Ask for number of teams (2 or 3)
        do {
            cout << "Enter the number of teams (2 or 3): ";
            cin >> teamCount;
        } while (teamCount != 2 && teamCount != 3);

        // Ask for number of players (between 2 and 12)
        do {
            cout << "Enter the number of players (between 2 and 12): ";
            cin >> playerCount;
        } while (playerCount < 2 || playerCount > 12);

        // Create teams and players
        teams.resize(teamCount);
        string playerName;
        for (int i = 0; i < playerCount; i++) {
            cout << "Enter name for Player " << i + 1 << ": ";
            cin >> playerName;
            Player* newPlayer = new Player(playerName);
            players.push_back(newPlayer);
            teams[i % teamCount].push_back(newPlayer);  // Assign players to teams
        }
    }

    void startGame() {
        shuffleDeck();
        dealCards();
        play();
    }

private:
    void initializeDeck() {
        string cardValues[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
        string suits[] = {"H", "S", "C", "D"};

        for (const string &suit : suits) {
            for (const string &value : cardValues) {
                deck.push(new Card(value, suit));
            }
        }

        deck.push(new Card("Joker", "X"));  // Add Jokers
    }

    void shuffleDeck() {
        Card* tempDeck[52];
        int index = 0;

        while (!deck.empty()) {
            tempDeck[index++] = deck.top();
            deck.pop();
        }

        // Shuffle the cards
        for (int i = 0; i < 52; i++) {
            int randIndex = rand() % 52;
            swap(tempDeck[i], tempDeck[randIndex]);
        }

        // Refill the deck
        for (int i = 0; i < 52; i++) {
            deck.push(tempDeck[i]);
        }
    }

    void dealCards() {
        for (int i = 0; i < playerCount; i++) {
            for (int j = 0; j < 7; j++) {
                players[i]->drawCard(deck.top());
                deck.pop();
            }
        }
    }

    void play() {
        while (true) {
            board.displayBoard();
            players[currentPlayerIndex]->showHand();

            cout << players[currentPlayerIndex]->name << ", it's your turn.\n";
            int cardIndex;
            cout << "Enter the index of the card you want to play (0-6): ";
            cin >> cardIndex;

            // Validate the card index
            if (cardIndex < 0 || cardIndex >= players[currentPlayerIndex]->handSize) {
                cout << "Invalid card index. Try again.\n";
                continue;
            }

            // Handle Jack card special conditions
            Card* currentCard = players[currentPlayerIndex]->hand[cardIndex];
            int row, col;
            cout << "Enter the row and column (0-9) to place the card: ";
            cin >> row >> col;

            // Loop until the player makes a valid move
            while (true) {
                if (currentCard->value == "J") {
                    if (currentCard->suit == "X") {  // Two-eyed Jack
                        cout << "You have a Two-eyed Jack! Enter the row and column to place the card: ";
                        if (board.placeCard(row, col, currentCard, players[currentPlayerIndex]->name)) {
                            players[currentPlayerIndex]->playCard(cardIndex);
                            break;
                        } else {
                            cout << "Invalid move. Try again.\n";
                            cout << "Enter the row and column (0-9) to place the card: ";
                            cin >> row >> col;
                        }
                    } else {  // One-eyed Jack
                        cout << "You have a One-eyed Jack! Pick a card from the opponent team.\n";
                        // Implement picking card from the opponent team (if necessary)
                        break;
                    }
                } else {
                    if (board.placeCard(row, col, currentCard, players[currentPlayerIndex]->name)) {
                        players[currentPlayerIndex]->playCard(cardIndex);
                        cout << players[currentPlayerIndex]->name << " played a card.\n";
                        break;
                    } else {
                        cout << "Invalid move. Try again.\n";
                        cout << "Enter the row and column (0-9) to place the card: ";
                        cin >> row >> col;
                    }
                }
            }

            // Check if any team has won
            if (board.checkSequence()) {
                cout << "Game Over! We have a winner!\n";
                break;
            }

            // Switch to the next player
            currentPlayerIndex = (currentPlayerIndex + 1) % playerCount;
        }
    }
};

int main() {
    srand(time(0));

    Game game;
    game.askPlayersAndTeams();
    game.startGame();

    return 0;
}
