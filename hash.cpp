#include "block.h"

string padBinaryRepresentationShort(string binaryRepresentation, int targetLength) {
    while(binaryRepresentation.length() < targetLength) {
        // Pridėti randominius simbolius
        char padChar = 'a' + (binaryRepresentation.length() % targetLength);
        binaryRepresentation += bitset<8>(padChar).to_string();
    }
    return binaryRepresentation;
}
string padBinaryRepresentationLong(string binaryRepresentation, int targetLength) {
    for(int i = 0; i < targetLength / 8; i++) {
        // Pridėti randominius simbolius
        char padChar = 'a' + (binaryRepresentation.length() % targetLength);
        binaryRepresentation += bitset<8>(padChar).to_string();
    }
    return binaryRepresentation;
}

string generateHexadecimalRepresentation(string binaryRepresentation, const string& seed) {
    // Dabartinio laiko ir inputo kombinacija kaip sydas mt19937 generatoriui
    random_device rd;
    mt19937 mt(rd());
    for(char c : seed)
        mt.seed(mt() ^ static_cast<unsigned int>(c));

    // Indeksų kiekis nustatomas pagal dvejetainio kodo ilgį
    int indexAmount = binaryRepresentation.length() / 8;
    vector<int> indexes(indexAmount);
    for(int i = 0; i < indexAmount; i++) {
        indexes[i] = i;
    }

    // Indeksai maišomi pagal mt19937 atsitiktinių skaičių generatorių
    shuffle(indexes.begin(), indexes.end(), mt);

    // Dvejetainiai konteineriai pertvarkomi pagal išmaišytus indeksus
    vector<bitset<8>> rearrangedBinaryArray(indexAmount);
    for(int i = 0; i < indexAmount; i++) {
        rearrangedBinaryArray[indexes[i]] = bitset<8>(binaryRepresentation.substr(i * 8, 8));
    }

    // Pertvarkytas dvejetainis kodas konvertuoajamas į hex
    stringstream hexStream;
    for(const auto &binaryByte : rearrangedBinaryArray) {
        hexStream << setw(2) << setfill('0') << hex << static_cast<int>(binaryByte.to_ulong());
    }

    // Hex kodas nukerpamas iki 64 simbolių
    return hexStream.str().substr(0, 64);
}

string hasher(string input) {
    std::chrono::high_resolution_clock::time_point sHashing, eHashing;
    std::chrono::duration<double> tHashing;

    string binaryRepresentation;
    string seed;

    seed = input;
    for(char c : seed) {
        // Simbolis konvertuojamas į dvejetainį
        bitset<8> binary(c);
        binaryRepresentation += binary.to_string();
    }

    if(binaryRepresentation.length() < 256) { // Dvejetainis kodas trumpesnis, nei 256 simboliai
        binaryRepresentation += "1"; // Pridedamas vienetas į galą
        binaryRepresentation = padBinaryRepresentationShort(binaryRepresentation, 256);
    } else { // Dvejetainis kodas ilgesnis, nei 256 simboliai
        binaryRepresentation = padBinaryRepresentationLong(binaryRepresentation, 512);
    }

    return generateHexadecimalRepresentation(binaryRepresentation, seed);
}