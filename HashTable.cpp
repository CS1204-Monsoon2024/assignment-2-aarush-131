#include <iostream>
#include <vector>
#include <cmath>

// Helper function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

// Helper function to find the next prime number greater than or equal to n
int nextPrime(int n) {
    while (!isPrime(n)) {
        ++n;
    }
    return n;
}

class HashTable {
private:
    std::vector<int> table;
    std::vector<bool> occupied;
    int tableSize;
    int count;
    const double loadFactorThreshold = 0.8;

    // Hash function
    int hash(int key) {
        return key % tableSize;
    }

    // Quadratic probing function
    int probe(int key) {
        int index = hash(key);
        for (int i = 0; i < tableSize; ++i) {
            int newIndex = (index + i * i) % tableSize;
            if (!occupied[newIndex] || table[newIndex] == key) {
                return newIndex;
            }
        }
        return -1; // Indicates that probing failed
    }

    // Resize function to maintain load factor
    void resize() {
        int oldSize = tableSize;
        tableSize = nextPrime(2 * tableSize);
        std::vector<int> oldTable = table;
        std::vector<bool> oldOccupied = occupied;

        table = std::vector<int>(tableSize, -1);
        occupied = std::vector<bool>(tableSize, false);
        count = 0;

        for (int i = 0; i < oldSize; ++i) {
            if (oldOccupied[i]) {
                insert(oldTable[i]);
            }
        }
    }

public:
    // Constructor
    HashTable(int size) : tableSize(nextPrime(size)), count(0) {
        table = std::vector<int>(tableSize, -1);
        occupied = std::vector<bool>(tableSize, false);
    }

    // Insert function
    void insert(int key) {
        if (count >= loadFactorThreshold * tableSize) {
            resize();
        }

        int index = probe(key);
        if (index == -1) {
            std::cout << "Max probing limit reached!" << std::endl;
            return;
        }

        if (occupied[index] && table[index] == key) {
            std::cout << "Duplicate key insertion is not allowed" << std::endl;
            return;
        }

        table[index] = key;
        occupied[index] = true;
        ++count;
    }

    // Remove function
    void remove(int key) {
        int index = search(key);
        if (index == -1) {
            std::cout << "Element not found" << std::endl;
            return;
        }

        table[index] = -1;
        occupied[index] = false;
        --count;

        // Rehashing elements in the cluster after deletion
        for (int i = 1; i < tableSize; ++i) {
            int newIndex = (index + i * i) % tableSize;
            if (!occupied[newIndex]) break;

            int rehashKey = table[newIndex];
            table[newIndex] = -1;
            occupied[newIndex] = false;
            --count;
            insert(rehashKey);
        }
    }

    // Search function
    int search(int key) {
        int index = hash(key);
        for (int i = 0; i < tableSize; ++i) {
            int newIndex = (index + i * i) % tableSize;
            if (!occupied[newIndex]) return -1; // Key not found
            if (occupied[newIndex] && table[newIndex] == key) {
                return newIndex;
            }
        }
        return -1; // Key not found
    }

    // Print function
    void printTable() {
        for (int i = 0; i < tableSize; ++i) {
            if (occupied[i]) {
                std::cout << table[i] << " ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
};
