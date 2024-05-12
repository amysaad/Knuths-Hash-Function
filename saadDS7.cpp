/* Name: Amy Saad
 * Files: saadDS7.cpp
 *
 * Program: Hashtable using Knuths hash function to implement a probing scheme to store words and frequencies from an input file text
 * Input: The desired has table size, the value for A, and the name of the input file. The input file consists of plain, lowercase, ASCII text
 * that has no punctuation and capitalization (only words)
 * If the user input for the table size is too small or if the input data file is not valid, error message displays and quits
 * Output: Displays the contents of the hashtable with no more than 25 lines. The table displays the index, the word at the index,
 * the frequency, and the number of collisions at the index. Underneath the table displays the total number of collisions along with
 * the alpha load factor.

 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

class hashTable {
private:
    struct Hash {
        string word;
        int frequency;
        int collisions;
    };

    float A;
    Hash* table;
    int size;
    int totalCollisions;

public:
    hashTable();
    int hash(const string& word);
    void insert(const string& word);
    void display();
};

/*
 * Pre-condition: None
 * Post-condition: Returns an int of the hash value for the word
 * given based on the hash function
 */
hashTable::hashTable() {
    cout << "Enter table size: ";
    cin >> size;                    //user input for size

    cout << "Enter value for A: ";
    cin >> A;                       //user input for A value

    table = new Hash[size];         //allocates memory for hashtable
    totalCollisions = 0;            //initializes total collisions
}

/*
 * Pre-condition: Expects string word which hash value needs to be calculated
 * Post-condition: Returns integer representing hash value for given word
 */
int hashTable::hash(const string& word) {
    int key = 0;                        //initializes key
    for (char ch : word) {
        key += static_cast<int>(ch);    //iterate each character in word to get ASCII value
    }

    return static_cast<int>(size * fmod(key * A, 1.0)); //calculates hash value using key, A, and size
}

/*
 * Pre-condition: Expects string word for inserting
 * Post-condition: Inserts the word into the hash table and updated
 * frequency and collision counters
 */
void hashTable::insert(const string& word) {
    int hashValue = hash(word);         //initial hash value for word input
    int initialHash = hashValue;   //stores hash value in case of collision
    int probing = 1;           //initializes increment


    while (!table[hashValue].word.empty() and table[hashValue].word != word) { //probe as long as slot isn't empty or match found
        if (!table[initialHash].word.empty()) {
            table[initialHash].collisions += probing - 1;       //checks collisions and updates accordingly
            totalCollisions++;
        }

        hashValue = (initialHash + probing * probing) % size; //probing to look for next spot
        probing++;

        if (probing > size) {      //if full loop, exits
            cout << "Hmm...Cant insert item into hash table..." << endl;
            break;
        }
    }

    table[hashValue].frequency++;   //increments frequency of word
    table[hashValue].word = word;   //stores final hash value in hashtable
}

/*
 * Pre-condition: hashTable initialized correctly
 * Post-condition: Displays the contents of the hashtable: the index,
 * word, frequency, collisions, total collisions, and alpha value
 */
void hashTable::display() {
    //displays table
    cout << std::setw(5) << "Index" << setw(10) << "Word" << setw(12) << "Frequency" << setw(12) << "Collisions" << endl;
    cout << "-------------------------------------" << endl;

    for (int i = 0; i < size; ++i) {
        if (!table[i].word.empty()) {
            int collisions = table[i].collisions;
            cout << setw(5) << i << setw(10) << table[i].word << setw(12) << table[i].frequency << setw(12) << collisions << endl;
        }
    }

    cout << "Total collisions: " << totalCollisions << endl;
    cout << "Alpha: " << static_cast<double>(totalCollisions) / size << endl;
}

/*
 * Pre-condition: hashTable object with valid size and A value, takes input
 * file name from user
 * Post-condition: Reads data from input file and inserts each word into hash
 * table using insert method
 */
void readData(hashTable& myTable) {

    string file;
    cout << "Enter file name: ";
    cin >> file;

    ifstream inputFile(file);
    if (!inputFile.is_open()) {
        cout << "Hmm....error opening your file.. " << endl;
        exit(1);
    }

    string word;
    while (inputFile >> word) {     //reads words from file
        myTable.insert(word);       //inserts item into hash
    }
}

int main() {
    hashTable myTable;

    readData(myTable);

    myTable.display();
    return 0;
}
