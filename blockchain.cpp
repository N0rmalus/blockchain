#include "block.h"

void printTransactions(const std::vector<Transaction>& transactions) {
    for (const Transaction& tx : transactions) {
        std::cout << "Transaction ID: " << tx.transactionId << " Sender: " << tx.sender << " Recipient: " << tx.recipient << " Amount: " << tx.amount << std::endl;
    }
}

int main() {
    Block genesis_block("0", 1, 2);
    const std::vector<Transaction>& block_transactions = genesis_block.getTransactions();

    std::cout << "Transactions in the Block:" << std::endl;
    printTransactions(block_transactions);

    return 0;
}