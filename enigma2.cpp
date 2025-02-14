#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <array>

using namespace std;

class Rotor {
private:
    array<int, 26> wiring;
    array<int, 26> reverse_wiring;
    int position = 0;
    int notch_position;  // Position where next rotor will turn

public:
    Rotor(const array<int, 26>& initial_wiring, int notch) {
        wiring = initial_wiring;
        notch_position = notch;
        // Create reverse wiring for when signal goes back through rotor
        for (int i = 0; i < 26; i++) {
            reverse_wiring[wiring[i]] = i;
        }
    }

    int forward(int input) {
        int shifted_input = (input + position) % 26;
        int output = wiring[shifted_input];
        output = (output - position + 26) % 26;
        return output;
    }

    int backward(int input) {
        int shifted_input = (input + position) % 26;
        int output = reverse_wiring[shifted_input];
        output = (output - position + 26) % 26;
        return output;
    }

    void rotate() {
        position = (position + 1) % 26;
    }

    bool at_notch() {
        return position == notch_position;
    }

    void set_position(int pos) {
        position = pos % 26;
    }
};

class Reflector {
private:
    array<int, 26> wiring;

public:
    // Default constructor with standard B reflector
    Reflector() {
        wiring = {24, 17, 20, 7, 16, 18, 11, 3, 15, 23, 13, 6, 14, 10, 12, 8, 4, 1, 5, 25, 2, 22, 21, 9, 0, 19};
    }

    // New constructor that accepts custom wiring
    Reflector(const array<int, 26>& custom_wiring) {
        wiring = custom_wiring;
    }

    int reflect(int input) {
        return wiring[input];
    }
};

class EnigmaMachine {
private:
    vector<Rotor> rotors;
    Reflector reflector;

    void rotate_rotors() {
        bool rotate_next = true;
        for (int i = 0; i < rotors.size() && rotate_next; i++) {
            if (i < rotors.size() - 1) {
                rotate_next = rotors[i].at_notch();
            }
            rotors[i].rotate();
        }
    }

public:
    // Default constructor with historical rotors
    EnigmaMachine() {
        // Historical rotor wirings from actual Enigma I
        array<int, 26> rotor1 = {4, 10, 12, 5, 11, 6, 3, 16, 21, 25, 13, 19, 14, 22, 24, 7, 23, 20, 18, 15, 0, 8, 1, 17, 2, 9};
        array<int, 26> rotor2 = {0, 9, 3, 10, 18, 8, 17, 20, 23, 1, 11, 7, 22, 19, 12, 2, 16, 6, 25, 13, 15, 24, 5, 21, 14, 4};
        array<int, 26> rotor3 = {1, 3, 5, 7, 9, 11, 2, 15, 17, 19, 23, 21, 25, 13, 24, 4, 8, 22, 6, 0, 10, 12, 20, 18, 16, 14};

        rotors.push_back(Rotor(rotor1, 16));  // Q notch
        rotors.push_back(Rotor(rotor2, 4));   // E notch
        rotors.push_back(Rotor(rotor3, 21));  // V notch
    }

    // New constructor that accepts custom rotors and reflector
    EnigmaMachine(const vector<pair<array<int, 26>, int>>& rotor_configs, 
                 const array<int, 26>& reflector_wiring) 
        : reflector(reflector_wiring) {
        for (const auto& [wiring, notch] : rotor_configs) {
            rotors.push_back(Rotor(wiring, notch));
        }
    }

    void set_positions(const string& positions) {
        for (int i = 0; i < min(positions.length(), rotors.size()); i++) {
            rotors[i].set_position(positions[i] - 'A');
        }
    }

    string encrypt(const string& message) {
        string result;
        for (char c : message) {
            if (isalpha(c)) {
                c = toupper(c);
                rotate_rotors();

                // Forward through rotors
                int signal = c - 'A';
                for (int i = rotors.size() - 1; i >= 0; i--) {
                    signal = rotors[i].forward(signal);
                }

                // Through reflector
                signal = reflector.reflect(signal);

                // Backward through rotors
                for (int i = 0; i < rotors.size(); i++) {
                    signal = rotors[i].backward(signal);
                }

                result += 'A' + signal;
            } else {
                result += c;
            }
        }
        return result;
    }
};

int main() {
    // Default historical configuration
    EnigmaMachine historical_enigma;
    
    // Custom configuration example
    vector<pair<array<int, 26>, int>> custom_rotors = {
        // Custom rotor 1 (wiring and notch position)
        {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25}, 5},
        // Custom rotor 2
        {{25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}, 10},
        // Custom rotor 3
        {{12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 15}
    };
    
    // Custom reflector wiring
    array<int, 26> custom_reflector = {
        25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
    };
    
    EnigmaMachine custom_enigma(custom_rotors, custom_reflector);
    
    string message = "HELLO WORLD";
    cout << "Original message: " << message << "\n\n";
    
    // Test historical machine
    cout << "Historical Enigma:\n";
    historical_enigma.set_positions("AAA");
    string historical_encrypted = historical_enigma.encrypt(message);
    cout << "Encrypted: " << historical_encrypted << endl;
    
    // Reset positions for decryption
    historical_enigma.set_positions("AAA");
    string historical_decrypted = historical_enigma.encrypt(historical_encrypted);
    cout << "Decrypted: " << historical_decrypted << "\n\n";
    
    // Test custom machine
    cout << "Custom Enigma:\n";
    custom_enigma.set_positions("AAA");
    string custom_encrypted = custom_enigma.encrypt(message);
    cout << "Encrypted: " << custom_encrypted << endl;
    
    // Reset positions for decryption
    custom_enigma.set_positions("AAA");
    string custom_decrypted = custom_enigma.encrypt(custom_encrypted);
    cout << "Decrypted: " << custom_decrypted << endl;

    return 0;
}