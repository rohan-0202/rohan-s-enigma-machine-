#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <algorithm>

using namespace std;

std::map<char, int> create_map() {
    // Initialize random number generator
    random_device rd;
    mt19937 gen(rd());
    
    // Create vector of indices 0-25
    vector<int> indices;
    for (int i = 0; i < 26; i++) {
        indices.push_back(i);
    }
    
    // Create the mapping
    std::map<char, int> cipher_map;
    
    while (!indices.empty()) {
        if (indices.size() == 1) {
            // If only one position remains, map it to itself
            int last_idx = indices[0];
            cipher_map[static_cast<char>('A' + last_idx)] = last_idx;
            indices.erase(indices.begin());
        } else {
            // Randomly select two positions and map their letters to each other
            uniform_int_distribution<> dis(0, indices.size() - 1);
            int idx1 = dis(gen);
            int pos1 = indices[idx1];
            indices.erase(indices.begin() + idx1);
            
            int idx2 = dis(gen);
            int pos2 = indices[idx2];
            indices.erase(indices.begin() + idx2);
            
            // Create symmetric mapping
            cipher_map[static_cast<char>('A' + pos1)] = pos2;
            cipher_map[static_cast<char>('A' + pos2)] = pos1;
        }
    }
    
    return cipher_map;
}

std::map<char, int> cipher_map = create_map();

string encrypt(string message) {
    // Create the cipher map
    std::map<char, int> tempmap = cipher_map;
    
    string encrypted = "";
    
    // Convert message to uppercase
    for (char& c : message) {
        c = toupper(c);
    }
    
    // Encrypt each character
    for (char c : message) {
        // Only encrypt alphabetic characters
        if (isalpha(c)) {
            // Get the mapped index and convert back to a letter
            int mapped_idx = tempmap[c];
            encrypted += static_cast<char>('A' + mapped_idx);
            
            // Increment all values in tempmap, wrapping around at 26
            for (auto& pair : tempmap) {
                pair.second = (pair.second + 1) % 26;
            }
        } else {
            // Keep non-alphabetic characters unchanged
            encrypted += c;
        }
    }
    
    return encrypted;
}

string decrypt(string message) {
    std::map<char, int> tempmap = cipher_map;
    
    string decrypted = "";
    
    // Convert message to uppercase
    for (char& c : message) {
        c = toupper(c);
    }
    
    // Decrypt each character
    for (char c : message) {
        // Only decrypt alphabetic characters
        if (isalpha(c)) {
            // Find which letter maps to the current encrypted character
            char decrypted_char = 'A';
            for (const auto& pair : tempmap) {
                if (static_cast<char>('A' + pair.second) == c) {
                    decrypted_char = pair.first;
                    break;
                }
            }
            decrypted += decrypted_char;
            
            // Increment all values in tempmap, wrapping around at 26
            for (auto& pair : tempmap) {
                pair.second = (pair.second + 1) % 26;
            }
        } else {
            // Keep non-alphabetic characters unchanged
            decrypted += c;
        }
    }
    
    return decrypted;
}

int main() {
    string message = "HELLO WORLD";
    
    // Print initial mapping
    cout << "Initial cipher map:" << endl;
    for (const auto& pair : cipher_map) {
        cout << pair.first << " -> " << static_cast<char>('A' + pair.second) << endl;
    }
    cout << endl;
    
    string encrypted = encrypt(message);
    string decrypted = decrypt(encrypted);

    cout << "Original message: " << message << endl;
    cout << "Encrypted message: " << encrypted << endl;
    cout << "Decrypted message: " << decrypted << endl;

    return 0;
}
