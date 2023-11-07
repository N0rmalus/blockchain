#include "block.h"

// Merkle medžio hash suradimas
string calculateMerkleRoot(const vector<string>& transactionHashes) {
    vector<string> currentLayer = transactionHashes;

    // Kol turime daugiau nei vieną hash'ą šiame lygyje
    while(currentLayer.size() > 1) {
        vector<string> newLayer;

        // Einame per kiekvieną porą hash'ų šiame lygyje
        for(int i = 0; i < currentLayer.size(); i += 2) {
            string combinedHash = currentLayer[i] + currentLayer[i + 1];
            string newHash = hasher(combinedHash);
            newLayer.push_back(newHash);
        }
        if(currentLayer.size() % 2 != 0) { // Jei yra nelyginis kiekis, dubliuojame paskutinį hash'ą
            string combinedHash = currentLayer.back() + currentLayer.back();
            string newHash = hasher(combinedHash);
            newLayer.push_back(newHash);
        }
        currentLayer = newLayer;
    }

    // Galiausiai likęs hash'as yra Merkle šaknis
    return currentLayer[0];
}

// Hash'inimo funkcija
string hasher(string input) {
    const int hash_size = 8;
    uint64_t length = input.length() * hash_size;
    input += char((length >> 56) & 0xFF);
    input += char((length >> 48) & 0xFF);
    input += char((length >> 40) & 0xFF);
    input += char((length >> 32) & 0xFF);
    input += char((length >> 24) & 0xFF);
    input += char((length >> 16) & 0xFF);
    input += char((length >> 8) & 0xFF);
    input += char(length & 0xFF);
 
    int code_local = 46886, var=7;
    int secret_rand;
    uint32_t hash[hash_size] = {0};
    for(char c : input) {
        for(int i = 0; i < hash_size; i++) {
            secret_rand = (code_local / var) - (var % 10);

            hash[i] = (hash[i] ^ c) + secret_rand * (i + 1);
            hash[i] ^= (hash[i] << 15) | (hash[i] >> 21);
            hash[i] = (hash[i] + 0x85ebca6b) ^ 0xc2b2ae35;
            var++;
        }
    }
    
    stringstream ss;
    for(int i = 0; i < hash_size; i++)
        ss << hex << setw(8) << setfill('0') << hash[i];

    return ss.str();
}