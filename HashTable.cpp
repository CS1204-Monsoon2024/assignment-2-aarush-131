#include <iostream>
#include <vector>


using namespace std;

class HashTable {
private:
    vector<int> table;
    int currentSize;
    int numElements;
    const int EMPTY = -1;
    const int DELETED = -2;
    const double LOAD_FACTOR_THRESHOLD = 0.8;

    // Helper function to check if a number is prime
    bool isPrime(int n) {
        if (n <= 1) return false;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) return false;
        }
        return true;
    }

    // Find the next prime number greater than or equal to n
    int nextPrime(int n) {
        while (!isPrime(n)) {
            n++;
        }
        return n;
    }

    // Hash function
    int hash(int key) {
        return key % currentSize;
    }

    // Function to resize the table
    void resize() {
        int oldSize = currentSize;
        currentSize = nextPrime(2 * currentSize);
        vector<int> oldTable = table;
        table = vector<int>(currentSize, EMPTY);
        numElements = 0;

        // Rehashing all elements in the old table
        for (int i = 0; i < oldSize; i++) {
            if (oldTable[i] != EMPTY && oldTable[i] != DELETED) {
                insert(oldTable[i]);
            }
        }
    }

public:
    // Constructor
    HashTable(int size) {
        currentSize = nextPrime(size);
        table = vector<int>(currentSize, EMPTY);
        numElements = 0;
    }

    // Insert function
    void insert(int key) {
        int idx = hash(key);
        int i = 0;
        int newIdx;

        while (i < (currentSize + 1) / 2) {
            newIdx = (idx + i * i) % currentSize;

            if (table[newIdx] == EMPTY || table[newIdx] == DELETED) {
                table[newIdx] = key;
                numElements++;
                if (static_cast<double>(numElements) / currentSize >= LOAD_FACTOR_THRESHOLD) {
                    resize();
                }
                return;
            } else if (table[newIdx] == key) {
                cout << "Duplicate key insertion is not allowed" << endl;
                return;
            }

            i++;
        }
        cout << "Max probing limit reached!" << endl;
    }

    // Search function
    int search(int key) {
        int idx = hash(key);
        int i = 0;
        int newIdx;

        while (i < (currentSize + 1) / 2) {
            newIdx = (idx + i * i) % currentSize;

            if (table[newIdx] == EMPTY) {
                return -1;
            } else if (table[newIdx] == key) {
                return newIdx;
            }

            i++;
        }
        return -1;
    }

    // Remove function
    void remove(int key) {
        int idx = search(key);
        if (idx == -1) {
            cout << "Element not found" << endl;
        } else {
            table[idx] = DELETED;
            numElements--;
        }
    }

    // Print the hash table
    void printTable() {
        for (int i = 0; i < currentSize; i++) {
            if (table[i] == EMPTY) {
                cout << "- ";
            } else if (table[i] == DELETED) {
                cout << "- ";
            } else {
                cout << table[i] << " ";
            }
        }
        cout << endl;
    }
};
