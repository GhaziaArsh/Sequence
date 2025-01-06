#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <map>
#include <set>
#include <iomanip>

using namespace std;

// Constants
const string ONE_EYE_JACK = "One-Eye Jack";
const string TWO_EYE_JACK = "Two-Eye Jack";

// Structures
struct Space {
    string card;
    int marbles;
    string team;
};

struct Player {
    string name;
    string team;
    vector<string> cards;
};

// Global Variables
vector<Player> players;
vector<string> deck;
map<string, Space> boardMap;
set<string> completedSequences;

// Board class
class Board {
private:
    string grid[10][10];

public:
    Board() {
        // Initialize board grid with card strings
        string initialGrid[10][10] = {
            {"A(H)", "2(S)", "10(D)", "8(C)", "K(D)", "3(S)", "Q(H)", "J(D)", "5(S)", "6(H)"},
            {"4(D)", "7(H)", "10(C)", "A(S)", "J(H)", "6(S)", "3(C)", "K(S)", "2(H)", "8(D)"},
            {"Q(S)", "A(D)", "7(C)", "3(H)", "9(S)", "K(H)", "10(S)", "8(S)", "5(C)", "J(S)"},
            {"9(D)", "J(C)", "A(C)", "4(C)", "7(D)", "Q(D)", "5(D)", "6(C)", "2(C)", "10(H)"},
            {"A(S)", "Q(H)", "8(H)", "7(S)", "6(D)", "4(H)", "2(D)", "J(H)", "9(C)", "10(D)"},
            {"K(C)", "3(D)", "6(S)", "4(S)", "J(S)", "A(D)", "8(C)", "Q(C)", "5(S)", "9(H)"},
            {"7(D)", "K(S)", "3(S)", "6(C)", "A(H)", "2(H)", "10(C)", "9(D)", "Q(S)", "4(C)"},
            {"5(C)", "K(H)", "J(D)", "3(C)", "Q(C)", "9(S)", "2(S)", "10(D)", "7(H)", "A(D)"},
            {"4(H)", "8(S)", "J(H)", "2(C)", "K(D)", "5(D)", "3(H)", "6(H)", "A(C)", "Q(H)"},
            {"10(S)", "9(H)", "5(S)", "7(C)", "2(D)", "8(D)", "6(D)", "A(S)", "J(S)", "4(D)"}};

        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                grid[i][j] = initialGrid[i][j];
                boardMap[grid[i][j]] = {"", 0, ""}; // Initialize boardMap with empty values
            }
        }
    }

    void displayBoard() const {
        cout << "\nGame Board:\n";
        cout << "  " << string(82, '-') << endl;
        for (int i = 0; i < 10; ++i) {
            cout << "|";
            for (int j = 0; j < 10; ++j) {
                cout << setw(7) << grid[i][j] << " |";
            }
            cout << "\n  " << string(82, '-') << endl;
        }
    }

    void updateBoard(const string &card, const string &team) {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (grid[i][j] == card) {
                    grid[i][j] += " (" + team + ")";
                    return;
                }
            }
        }
    }
};

// Global Board
Board gameBoard;

// Function Prototypes
void generateDeck();
void dealCards(int cardsPerPlayer);
void takeTurn(Player &player);
bool checkForSequence(const string &team);

// Function Implementations
void generateDeck() {
    string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};

    for (const string &suit : suits) {
        for (const string &rank : ranks) {
            if (rank == "Jack") {
                deck.push_back(ONE_EYE_JACK);
                deck.push_back(TWO_EYE_JACK);
            } else {
                deck.push_back(rank + " of " + suit);
            }
        }
    }

    random_shuffle(deck.begin(), deck.end());
}

void dealCards(int cardsPerPlayer) {
    for (Player &player : players) {
        for (int i = 0; i < cardsPerPlayer; ++i) {
            player.cards.push_back(deck.back());
            deck.pop_back();
        }
    }
}

void takeTurn(Player &player) {
    cout << "\n" << player.name << "'s turn (Team " << player.team << "). Your cards:\n";
    for (size_t i = 0; i < player.cards.size(); ++i) {
        cout << i + 1 << ". " << player.cards[i] << endl;
    }

    int choice;
    cout << "Choose a card to play (1-" << player.cards.size() << "): ";
    cin >> choice;

    while (choice < 1 || choice > player.cards.size()) {
        cout << "Invalid choice. Try again: ";
        cin >> choice;
    }

    string chosenCard = player.cards[choice - 1];
    player.cards.erase(player.cards.begin() + choice - 1);

    if (chosenCard == ONE_EYE_JACK) {
        cout << "Play a One-Eye Jack: Remove an opponent's marble.\n";
    } else if (chosenCard == TWO_EYE_JACK) {
        cout << "Play a Two-Eye Jack: Place your marble anywhere.\n";
    } else {
        cout << "You played " << chosenCard << ". Placing your marble.\n";
        boardMap[chosenCard].marbles++;
        boardMap[chosenCard].team = player.team;
        gameBoard.updateBoard(chosenCard, player.team);
    }

    gameBoard.displayBoard();

    if (checkForSequence(player.team)) {
        cout << "\n" << player.team << " has completed a sequence!\n";
        completedSequences.insert(player.team);
    }
}

bool checkForSequence(const string &team) {
    // Implement sequence checking logic (e.g., 5 consecutive marbles for a team)
    return false;
}

// Main Function
int main() {
    srand(static_cast<unsigned>(time(0)));

    cout << "Welcome to the Sequence Game!" << endl;
    int numPlayers;
    cout << "Enter the number of players (2-12): ";
    cin >> numPlayers;

    while (numPlayers < 2 || numPlayers > 12) {
        cout << "Invalid number of players. Enter a number between 2 and 12: ";
        cin >> numPlayers;
    }

    int cardsPerPlayer = (numPlayers <= 2) ? 7 : (numPlayers <= 4) ? 6 : (numPlayers <= 6) ? 5 : (numPlayers <= 9) ? 4 : 3;

    for (int i = 0; i < numPlayers; ++i) {
        Player player;
        cout << "Enter Player " << i + 1 << "'s name: ";
        cin >> player.name;
        player.team = (i % 2 == 0) ? "Team 1" : "Team 2";
        players.push_back(player);
    }

    generateDeck();
    dealCards(cardsPerPlayer);
    gameBoard.displayBoard();

    bool gameWon = false;
    while (!gameWon) {
        for (Player &player : players) {
            takeTurn(player);

            if (completedSequences.size() >= 2) {
                cout << "Game Over! Teams with completed sequences: ";
                for (const auto &team : completedSequences) {
                    cout << team << " ";
                }
                cout << endl;
                gameWon = true;
                break;
            }
        }
    }

    return 0;
}
