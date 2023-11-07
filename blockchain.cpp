#include "block.h"

int main() {
    Blockchain container;
    Block block("0", 0, "0", 0);
    container.addBlock(block);

    int txCount = 10;
    string target = regex_replace("", block.getHeaderInfo().difficulty_target, "");

    std::mt19937 mt(std::random_device {}());

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
        block.getHeaderInfo().merkle_root_hash = merkleRoot;

        while (true) {
            block.getHeaderInfo().timestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            string fullHash = 
                hasher(block.getHeaderInfo().prev_block_hash) +
                hasher(to_string(block.getHeaderInfo().timestamp)) +
                hasher(block.getHeaderInfo().version) +
                hasher(block.getHeaderInfo().merkle_root_hash) +
                hasher(to_string(block.getHeaderInfo().nonce)) +
                hasher(target);
            string finishedHash = hasher(fullHash);

            block.setHeader(finishedHash);
            if(block.isTargetMet(block.getHeader())) {
                block.getHeaderInfo().setPrevHash(finishedHash);
                container.addBlock(block);

                // cout << block.getHeader() << " : " << block.getHeaderInfo().nonce << " <----- " << endl;
                cout << "--------------------------------------------------------------------" << endl;
                cout << "Bloko hash'as: " << block.getHeader() << endl;
                cout << "Nonce: " << block.getHeaderInfo().nonce << endl;
                cout << "Block added." << endl;
                cout << "--------------------------------------------------------------------" << endl;

                block.getHeaderInfo().nonce = 0;

                break;
            } else { 
                // cout << block.getHeader() << " : " << block.getHeaderInfo().nonce << endl; 
            }

            block.getHeaderInfo().nonce++;
        }
    } while(block.getAllTransactions().size() > txCount);
    container.printBlocks();
    block.printUsers();

    return 0;
}