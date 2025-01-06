#include <iostream>
#include <stack>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

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

    Board() {
        string cardValues[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
        string suits[] = {"H", "S", "C", "D"};  // Hearts, Spades, Clubs, Diamonds

        // Keeping the same number of cards and positions from the previous random distribution
        grid[0][0] = "A(H)"; grid[0][1] = "2(S)"; grid[0][2] = "10(D)"; grid[0][3] = "8(C)";
        grid[0][4] = "K(D)"; grid[0][5] = "3(S)"; grid[0][6] = "Q(H)"; grid[0][7] = "J(D)";
        grid[0][8] = "5(S)"; grid[0][9] = "6(H)";
        grid[1][0] = "4(D)"; grid[1][1] = "7(H)"; grid[1][2] = "10(C)"; grid[1][3] = "A(S)";
        grid[1][4] = "J(H)"; grid[1][5] = "6(S)"; grid[1][6] = "3(C)"; grid[1][7] = "K(S)";
        grid[1][8] = "2(H)"; grid[1][9] = "8(D)";
        grid[2][0] = "Q(S)"; grid[2][1] = "A(D)"; grid[2][2] = "7(C)"; grid[2][3] = "3(H)";
        grid[2][4] = "9(S)"; grid[2][5] = "K(H)"; grid[2][6] = "10(S)"; grid[2][7] = "8(S)";
        grid[2][8] = "5(C)"; grid[2][9] = "J(S)";
        grid[3][0] = "9(D)"; grid[3][1] = "J(C)"; grid[3][2] = "A(C)"; grid[3][3] = "4(C)";
        grid[3][4] = "7(D)"; grid[3][5] = "Q(D)"; grid[3][6] = "5(D)"; grid[3][7] = "6(C)";
        grid[3][8] = "2(C)"; grid[3][9] = "10(H)";
        grid[4][0] = "A(S)"; grid[4][1] = "Q(H)"; grid[4][2] = "8(H)"; grid[4][3] = "7(S)";
        grid[4][4] = "6(D)"; grid[4][5] = "4(H)"; grid[4][6] = "2(D)"; grid[4][7] = "J(H)";
        grid[4][8] = "9(C)"; grid[4][9] = "10(D)";
        grid[5][0] = "K(C)"; grid[5][1] = "3(D)"; grid[5][2] = "6(S)"; grid[5][3] = "4(S)";
        grid[5][4] = "J(S)"; grid[5][5] = "A(D)"; grid[5][6] = "8(C)"; grid[5][7] = "Q(C)";
        grid[5][8] = "5(S)"; grid[5][9] = "9(H)";
        grid[6][0] = "7(D)"; grid[6][1] = "K(S)"; grid[6][2] = "3(S)"; grid[6][3] = "6(C)";
        grid[6][4] = "A(H)"; grid[6][5] = "2(H)"; grid[6][6] = "10(C)"; grid[6][7] = "9(D)";
        grid[6][8] = "Q(S)"; grid[6][9] = "4(C)";
        grid[7][0] = "5(C)"; grid[7][1] = "K(H)"; grid[7][2] = "J(D)"; grid[7][3] = "3(C)";
        grid[7][4] = "Q(C)"; grid[7][5] = "9(S)"; grid[7][6] = "2(S)"; grid[7][7] = "10(D)";
        grid[7][8] = "7(H)"; grid[7][9] = "A(D)";
        grid[8][0] = "4(H)"; grid[8][1] = "8(S)"; grid[8][2] = "J(H)"; grid[8][3] = "2(C)";
        grid[8][4] = "K(D)"; grid[8][5] = "5(D)"; grid[8][6] = "3(H)"; grid[8][7] = "6(H)";
        grid[8][8] = "A(C)"; grid[8][9] = "Q(H)";
        grid[9][0] = "10(S)"; grid[9][1] = "9(H)"; grid[9][2] = "5(S)"; grid[9][3] = "7(C)";
        grid[9][4] = "2(D)"; grid[9][5] = "8(D)"; grid[9][6] = "6(D)"; grid[9][7] = "A(S)";
        grid[9][8] = "J(S)"; grid[9][9] = "4(D)";
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

    bool placeCard(int row, int col, Card* card) {

        string cardRepresentation = card->getCardRepresentation();
        if (grid[row][col] == cardRepresentation) {
            grid[row][col] = "-";
            return true;
        }
        return false;
    }
};

class Game {
private:
    Player* players[12];
    int playerCount;
    Board board;
    stack<Card*> deck;
    int currentPlayerIndex;

public:
    Game(int numPlayers) : playerCount(numPlayers), currentPlayerIndex(0) {
        initializeDeck();
    }

    void addPlayer(string name) {
        static int playerIndex = 0;
        players[playerIndex++] = new Player(name);
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

        deck.push(new Card("Joker", "X"));
    }

    void shuffleDeck() {

        Card* tempDeck[52];
        int index = 0;

        while (!deck.empty()) {
            tempDeck[index++] = deck.top();
            deck.pop();
        }

        for (int i = 0; i < 52; i++) {
            int randIndex = rand() % 52;
            swap(tempDeck[i], tempDeck[randIndex]);
        }

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


            int row, col;
            cout << "Enter the row and column (0-9) to place the card: ";
            cin >> row >> col;

            if (board.placeCard(row, col, players[currentPlayerIndex]->hand[cardIndex])) {
                players[currentPlayerIndex]->playCard(cardIndex);
                cout << players[currentPlayerIndex]->name << " played a card.\n";
            } else {
                cout << "Invalid move.\n";
            }


            currentPlayerIndex = (currentPlayerIndex + 1) % playerCount;


            if (deck.empty() && players[currentPlayerIndex]->handSize == 0) {
                cout << "Game over.\n";
                break;
            }
        }
    }
};

int main() {
    srand(time(0));

    Game game(2);
    game.addPlayer("Player 1");
    game.addPlayer("Player 2");
    game.startGame();

    return 0;
}
