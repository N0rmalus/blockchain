#include "block.h"

bool isTargetMet(const string& hash, regex DT) {
    return regex_search(hash, DT);
}

int main() {
    Blockchain container;

    // Transakcijų skaičius bloke
    int txCount = 100;

    std::mt19937 mt(std::random_device {}());

    // Genesis blokas
    Block block("0", 0, "0", 0);
    container.addBlock(block);

    // Difficulty target 
    string target = "4"; int intT = stoi(target);
    regex DT = regex("^[0]{" + target + ",}+");

    double fixedTime = 5.0; // Tam tikras fiksuotas laikas per kurį reikia iškasti bloką
    int attempts = 1; // Hash'avimų bandymų kiekis

    do {
        cout << "Current number of unmined transactions: " << block.getAllTransactions().size() << endl << endl;

        vector<Transaction> tx;
        std::shuffle(block.getAllTransactions().begin(), block.getAllTransactions().end(), mt);
        std::copy_n (block.getAllTransactions().begin(), txCount, back_inserter (tx));
        block.setTransactions(tx);
        block.getAllTransactions().erase(block.getAllTransactions().begin(), block.getAllTransactions().begin() + txCount);

        block.updateUserBalances();

        vector<string> transactionHashes;
        for (const Transaction& transaction : block.getTransactions()) {
            string transactionHash = hasher(transaction.transactionId);
            transactionHashes.push_back(transactionHash);
        }
        string merkleRoot = calculateMerkleRoot(transactionHashes);
        block.getHeaderInfo().setMerkle(merkleRoot);

        auto sMining = std::chrono::high_resolution_clock::now();

        // Bandymas tenkinti Difficulty Target reikalavimą
        while(true) {
            string fullHash = 
                hasher(block.getHeaderInfo().prev_block_hash) +
                hasher(to_string(block.getHeaderInfo().timestamp)) +
                hasher(block.getHeaderInfo().version) +
                hasher(block.getHeaderInfo().merkle_root_hash) +
                hasher(to_string(block.getHeaderInfo().nonce)) +
                hasher(target);
            string finishedHash = hasher(fullHash);
            block.setHeader(finishedHash);

            if(isTargetMet(block.getHeader(), DT)) { // Reikalavimas įvykdytas
                attempts = 1; // Iš naujo skaičiuoti bandymus

                // Blokas pridedamas į blockchain'ą
                block.getHeaderInfo().setPrevHash(finishedHash);
                container.addBlock(block);

                auto eMining = std::chrono::high_resolution_clock::now(); // Įrašyti pabaigos laiką
                std::chrono::duration<double> dMining = eMining - sMining;

                cout << "--------------------------------------------------------------------" << endl;
                cout << "Hash: " << block.getHeader() << endl;
                cout << "Nonce: " << block.getHeaderInfo().nonce << endl;
                cout << "Time taken to mine this block: " << dMining.count() << " seconds" << endl;
                cout << "Block has been added to the blockchain!" << endl;
                cout << "--------------------------------------------------------------------" << endl;

                block.getHeaderInfo().nonce = 0;

                break;
            }

            auto eMining = std::chrono::high_resolution_clock::now(); // Įrašyti pabaigos laiką
            std::chrono::duration<double> dMining = eMining - sMining;
            if(dMining.count() > fixedTime) { // Kasimas užtruko ilgiau nei 5 sekundes
                cout << "Mining took longer than " << fixedTime  << " second(s)! (" << attempts << ")" << endl;
                attempts++;

                 // Jei badyta daugiau, nei 5 kartus, pridėti 5 sekundes laiko ir iš naujo skaičiuoti bandymus
                if(attempts > 5) {
                    fixedTime += 5.0;
                    attempts = 1;
                    cout << "> Too many failed attempts! Increasing mining time to " << fixedTime << " seconds." << endl;
                }

                // Atstatoma sMining naujam laikui
                sMining = std::chrono::high_resolution_clock::now();
            }

            block.getHeaderInfo().nonce++;
        }
    } while(block.getAllTransactions().size() > txCount);
    // container.printBlocks();
    // block.printUsers();

    return 0;
}