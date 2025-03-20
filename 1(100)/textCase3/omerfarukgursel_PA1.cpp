#include <iostream>
#include <ctime>
#include <fstream>
#include <cstring>

using namespace std;

const int MAX_ALPHABET_SIZE = 50;

// Function to read unique characters from "alphabet.txt" and store them in an array
void alphabetfile(char alphabet[MAX_ALPHABET_SIZE], int &alphabetSize) {
    ifstream file("alphabet.txt");
    char ch;
    alphabetSize = 0;

    if (file.is_open()) {
        while (file.get(ch)) {
            // Check if the character is already in the alphabet array
            bool there = false;
            for (int i = 0; i < alphabetSize; i++) {
                if (alphabet[i] == ch) {
                    there = true;
                    break;
                }
            }
            // Add the character if it's not already in the array and is not a whitespace or delimiter
            if (!there && ch != ' ' && ch != ',' && ch != '\n' && ch!='\r') {
                alphabet[alphabetSize] = ch;
                alphabetSize++;
            }
        }
        file.close();
    } else {
        cout << "Fail to open file" << endl;
        exit(1);
    }
}

// Function to generate a random word from the alphabet array
void generateRandomWord(char alphabet[MAX_ALPHABET_SIZE], int alphabetSize, int wordLength, char randomWord[]) {
    if (wordLength > alphabetSize) {
        cout << "INPUT ERROR" << endl;
        exit(1);
    }

    // Shuffle the alphabet array to ensure randomness
    for (int i = 0; i < alphabetSize - 1; i++) {
        int j = i + rand() % (alphabetSize - i);
        swap(alphabet[i], alphabet[j]);
    }

    // Select the first `wordLength` characters as the random word
    for (int i = 0; i < wordLength; i++) {
        randomWord[i] = alphabet[i];
    }
    randomWord[wordLength] = '\0'; // Null-terminate the string
}

// Function to validate if a given word is valid based on the alphabet array
bool isValidWord(char word[], int wordLength, char alphabet[MAX_ALPHABET_SIZE], int alphabetSize) {
    if (wordLength > alphabetSize) return false;

    bool seen[MAX_ALPHABET_SIZE] = {false};
    for (int i = 0; i < wordLength; i++) {
        bool found = false;
        for (int j = 0; j < alphabetSize; j++) {
            if (word[i] == alphabet[j]) {
                found = true;
                if (seen[j]) return false; // Avoid duplicate letters
                seen[j] = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

// Function to compare the guessed word with the secret word and provide hints
void getHints(char secret[], char guess[], int length, int &Cexact, int &Cmisplaced) {
    Cexact = 0;
    Cmisplaced = 0;
    bool checkedSecret[MAX_ALPHABET_SIZE] = {false};
    bool checkedGuess[MAX_ALPHABET_SIZE] = {false};

    // Check for exact matches (correct letter in the correct position)
    for (int i = 0; i < length; i++) {
        if (guess[i] == secret[i]) {
            Cexact++;
            checkedSecret[i] = true;
            checkedGuess[i] = true;
        }
    }

    // Check for misplaced matches (correct letter in the wrong position)
    for (int i = 0; i < length; i++) {
        if (!checkedGuess[i]) {
            for (int j = 0; j < length; j++) {
                if (!checkedSecret[j] && guess[i] == secret[j]) {
                    Cmisplaced++;
                    checkedSecret[j] = true;
                    break;
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    // Ensure correct command-line arguments are provided
    if (argc < 3) {
        cout << "INPUT ERROR" << endl;
        return 0;
    }

    srand(static_cast<unsigned>(time(0))); // Seed random number generator
    char alphabet[MAX_ALPHABET_SIZE];
    int alphabetSize = 0;
    alphabetfile(alphabet, alphabetSize);

    // Display the available alphabet characters
    cout << "Alphabet:[" ;
    for (int i = 0; i < alphabetSize; ++i) {
        cout << alphabet[i];
        if (i != alphabetSize - 1) 
            cout << ",";
        
    }
    cout << "]"<< endl;

    char secretWord[MAX_ALPHABET_SIZE] = {0};

    // Handle random word generation (-r) or user-defined word (-u)
    if (strcmp(argv[1], "-r") == 0) {
        int wordLength = atoi(argv[2]);
        if (wordLength <= 0 || wordLength > alphabetSize) {
            cout << "INPUT ERROR" << endl;
            return 0;
        }
        generateRandomWord(alphabet, alphabetSize, wordLength, secretWord);
       
    }
    else if (strcmp(argv[1], "-u") == 0) {
        strncpy(secretWord, argv[2], MAX_ALPHABET_SIZE);
        if (!isValidWord(secretWord, strlen(secretWord), alphabet, alphabetSize)) {
            cout << "INPUT ERROR" << endl;
            return 0;
        }
    }
    else {
        cout << "INPUT ERROR" << endl;
        return 0;
    }

    int attempts = 0;
    char guess[MAX_ALPHABET_SIZE] = {0};
    
    // Allow up to 100 guessing attempts
    while (attempts < 100) {
        cin >> guess;
        
        if (!isValidWord(guess, strlen(guess), alphabet, alphabetSize)) {
            cout << "INPUT ERROR" << endl;
            return 0;
        }

        attempts++;

        // Get hints for the guessed word
        int Cexact, Cmisplaced;
        getHints(secretWord, guess, strlen(secretWord), Cexact, Cmisplaced);
        cout << Cexact << " " << Cmisplaced << endl;

        // If all characters are exactly matched, the word is found
        if (Cexact == strlen(secretWord)) {
            cout << "FOUND " << attempts << endl;
            return 0;
        }
    }

    // If the word is not found within 100 attempts, print "FAILED"
    cout << "FAILED" << endl;
    return 0;
}
