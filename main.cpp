#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

class SequenceGame {
private:
    vector<vector<string>> board;         // 10x10 grid for the game board
    vector<string> deck;                  // Deck with 52 randomly selected cards
    vector<vector<string>> playerHands;   // Hands for each player
    int playerTurn;                        // The current player's turn
    int numPlayers;                       // Number of players
    vector<string> players;               // List of player names
    vector<string> teams;                 // List of team names
    vector<int> teamSequences;            // Sequence count for each team
    vector<string> stack;                 // Stack for remaining cards

    // Initialize a 10x10 grid with 100 unique cards
    void initializeBoard() {
        board.resize(10, vector<string>(10, " "));
        string suits[4] = {"H", "D", "C", "S"};
        string ranks[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

        int cardIndex = 0;
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                string card = ranks[cardIndex % 13] + "(" + suits[cardIndex % 4] + ")";
                board[i][j] = card;
                cardIndex++;
            }
        }
    }

    // Shuffle and store 52 cards from the grid into deck and remaining in stack
    void shuffleDeck() {
        vector<string> availableCards;
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                availableCards.push_back(board[i][j]);  // Add all cards from the board
            }
        }

        random_device rd;
        mt19937 g(rd());
        shuffle(availableCards.begin(), availableCards.end(), g);  // Shuffle the cards

        // Set deck with the first 52 cards
        deck = vector<string>(availableCards.begin(), availableCards.begin() + 52);
        // Set stack with the remaining cards
        stack = vector<string>(availableCards.begin() + 52, availableCards.end());
    }

    // Function to assign cards to each player based on number of players
    void dealCards() {
        playerHands.resize(numPlayers);
        int cardsPerPlayer = 7;  // Default cards for 2 players

        // Adjust the number of cards each player gets based on the number of players
        if (numPlayers == 2) {
            cardsPerPlayer = 7;
        } else if (numPlayers == 3) {
            cardsPerPlayer = 6;
        } else if (numPlayers == 4) {
            cardsPerPlayer = 5;
        } else if (numPlayers == 5 || numPlayers == 6) {
            cardsPerPlayer = 4;
        } else if (numPlayers >= 7 && numPlayers <= 10) {
            cardsPerPlayer = 3;
        } else if (numPlayers >= 11) {
            cardsPerPlayer = 2;
        }

        // Deal the cards to each player
        for (int i = 0; i < numPlayers; ++i) {
            for (int j = 0; j < cardsPerPlayer; ++j) {
                if (!deck.empty()) {
                    playerHands[i].push_back(deck.back());
                    deck.pop_back();  // Remove the card from the deck
                }
            }
        }

        // Store the remaining cards in the stack
        stack = deck;

        // Display the number of cards left in the stack
        cout << "Cards left in stack: " << stack.size() << endl;
    }

    // Display the current board in the required format
    void displayBoard() {
        cout << "\nGame Board:\n";
        for (int i = 0; i < 10; ++i) {
            cout << "  +";
            for (int j = 0; j < 10; ++j) {
                cout << "--------";
            }
            cout << "+\n";
            for (int j = 0; j < 10; ++j) {
                cout << " | " << board[i][j];
            }
            cout << " |\n";
        }
        cout << "  +";
        for (int i = 0; i < 10; ++i) {
            cout << "--------";
        }
        cout << "+\n";
    }

    // Show the cards in the player's hand
    void showPlayerHand(int player) {
        cout << players[player] << "'s hand: ";
        for (const auto &card : playerHands[player]) {
            cout << card << " , ";
        }
        cout << endl;
    }

    // Check if a player has won (by forming a sequence)
    int checkForSequences() {
        int sequences = 0;
        // A sequence is 5 consecutive cards in a row (horizontal, vertical, or diagonal)
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                // Check horizontal
                if (j + 4 < 10) {
                    bool horizontal = true;
                    for (int k = 0; k < 5; ++k) {
                        if (board[i][j + k] != players[playerTurn]) {
                            horizontal = false;
                            break;
                        }
                    }
                    if (horizontal) sequences++;
                }

                // Check vertical
                if (i + 4 < 10) {
                    bool vertical = true;
                    for (int k = 0; k < 5; ++k) {
                        if (board[i + k][j] != players[playerTurn]) {
                            vertical = false;
                            break;
                        }
                    }
                    if (vertical) sequences++;
                }

                // Check diagonal (top-left to bottom-right)
                if (i + 4 < 10 && j + 4 < 10) {
                    bool diagonal = true;
                    for (int k = 0; k < 5; ++k) {
                        if (board[i + k][j + k] != players[playerTurn]) {
                            diagonal = false;
                            break;
                        }
                    }
                    if (diagonal) sequences++;
                }

                // Check diagonal (bottom-left to top-right)
                if (i - 4 >= 0 && j + 4 < 10) {
                    bool diagonal = true;
                    for (int k = 0; k < 5; ++k) {
                        if (board[i - k][j + k] != players[playerTurn]) {
                            diagonal = false;
                            break;
                        }
                    }
                    if (diagonal) sequences++;
                }
            }
        }
        return sequences;
    }

    // Function to handle a turn where the player plays a card
    void playTurn() {
        int row, col;
        bool validMove = false;  // Flag to track if the move is valid
        string chosenCard;

        // Loop to keep asking for a valid move until the player enters one
        while (!validMove) {
            showPlayerHand(playerTurn);
            cout << players[playerTurn] << ", it's your turn. Enter row and column (0-9): ";
            cin >> row >> col;

            // Check if the row and column are within bounds
            if (row < 0 || row >= 10 || col < 0 || col >= 10) {
                cout << "Invalid position. Try again.\n";
                continue;
            }

            cout << "Enter the card you want to play (e.g., 2(H)): ";
            cin >> chosenCard;

            // Check if the card exists in player's hand
            auto it = find(playerHands[playerTurn].begin(), playerHands[playerTurn].end(), chosenCard);
            if (it == playerHands[playerTurn].end()) {
                cout << "Invalid card. Try again.\n";
                continue;
            }
