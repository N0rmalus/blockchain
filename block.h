#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <bitset>
#include <vector>
#include <random>
#include <chrono>
#include <random>
#include <ctime>
#include <regex>
#include <map>

using std::ostringstream;
using std::unordered_map;
using std::random_device;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::mt19937;
using std::size_t;
using std::vector;
using std::bitset;
using std::string;
using std::hash;
using std::cout;
using std::cin;
using std::hex;
using std::endl;
using std::left;
using std::setw;
using std::right;
using std::regex;
using std::setfill;
using std::to_string;
using std::regex_traits;
using std::regex_replace;
using std::back_inserter;

string hasher(string input);
string calculateMerkleRoot(const vector<string>& transactionHashes);

struct User {
    string name;
    string public_key;
    int balance;

    User(string n, string pk, int b) : name(n), public_key(pk), balance(b) {}

    void updateBalance(const int& b) {
        balance += b;
    }
};
struct Transaction {
    string transactionId;
    string sender;
    string recipient;
    int amount;

    Transaction(string tid, string s, string r, int a) : transactionId(tid), sender(s), recipient(r), amount(a) {}
};

struct BlockHeader {
    string prev_block_hash;
    long timestamp;
    string version;
    string merkle_root_hash;
    long nonce;
    regex difficulty_target;

    BlockHeader(string prev_hash, long timestamp, string merkle_root_hash, long nonce) : prev_block_hash(prev_hash), timestamp(timestamp), merkle_root_hash(merkle_root_hash), nonce(nonce) {
        version = "00001";
        difficulty_target = regex("^[0]{1,}+");
    }

    void setMerkle(const string& mr) {
        merkle_root_hash = mr;
    }  
    void setPrevHash(const string& ph) {
        prev_block_hash = ph;
    }
};

class Block {
private:
    BlockHeader headerInfo;
    string header;
    vector<User> users;
    vector<Transaction> transactions, allTransactions;
public:
    Block(string prev_hash, long timestamp, string merkle_root_hash, long nonce) : headerInfo(prev_hash, timestamp, merkle_root_hash, nonce) {
        int usersAmount = 10, txAmount = 100;

        mt19937 mt(time(0));
        std::uniform_int_distribution<int> balanceIntr(100, 1000000);
        std::uniform_int_distribution<int> txIntr(0, txAmount - 1);
        std::uniform_int_distribution<int> userIntr(0, usersAmount - 1);

        // Generuojami naudotojai ir transakcijos
        for(int i = 0; i < usersAmount; i++) {
            string name = "User" + to_string(i + 1);
            string public_key = hasher(name);
            int balance = balanceIntr(mt);
            users.push_back(User(name, public_key, balance));
        } for(int i = 0; i < txAmount; ++i) {
            string transactionId = hasher(to_string(txIntr(mt)));
            string sender = users[userIntr(mt)].public_key;
            string recipient = users[userIntr(mt)].public_key; 
            int amount = balanceIntr(mt);
            allTransactions.push_back(Transaction(transactionId, sender, recipient, amount));
        }
    }

    BlockHeader& getHeaderInfo() {
        return headerInfo;
    } const vector<User>& getUsers() const {
        return users;
    } const string& getHeader() const {
        return header;
    } const vector<Transaction>& getTransactions() const {
        return transactions;
    } vector<Transaction>& getAllTransactions() {
        return allTransactions;
    }

    void setHeader(const string& newHeader) {
        header = newHeader;
    } void setTransactions(const vector<Transaction> tx) {
        transactions = tx;
    }

    bool isTargetMet(const string& hash) {
        return regex_search(hash, headerInfo.difficulty_target);
    }

    // Funkcija atnaujinanti naudotojų balansus pagal atliktas transakcijas
    void updateUserBalances() {
        for (const Transaction& transaction : transactions)
            for (User& user : users)
                if (user.public_key == transaction.recipient)
                    user.balance += transaction.amount;
    }

    // Funkcija išvedanti visus naudotojus
    void printUsers() {
        for (const User& user : users) {
            cout << user.name;
            cout << " { Balance: " << user.balance << "; public_key: " << user.public_key << " }" << endl; 
        }
    }

    ~Block() {}
};

class Blockchain {
private:
    vector<Block> blocks;

public:
    Blockchain() {}

    // Pridėti naują iškastą bloką
    void addBlock(const Block& block) {
        blocks.push_back(block);
    }

    // Funkcija, kuri išveda visus blokus su informacija
    void printBlocks() {
        for (int i = 0; i < blocks.size(); ++i) {
            Block& block = blocks[i];
            BlockHeader& header = block.getHeaderInfo();
            
            cout << "Block" << i << endl;
            cout << "Previous Block Hash: " << header.prev_block_hash << endl;
            cout << "Timestamp: " << header.timestamp << endl;
            cout << "Version: " << header.version << endl;
            cout << "Merkle Root Hash: " << header.merkle_root_hash << endl;
            cout << "Nonce: " << header.nonce << endl;
            
            const vector<Transaction>& transactions = block.getTransactions();

            cout << "Transaction amount: " << transactions.size() << endl;
            for (const Transaction& transaction : transactions) {
                cout << "  Transaction ID: " << transaction.transactionId << endl;
                cout << "  Sender: " << transaction.sender << endl;
                cout << "  Recipient: " << transaction.recipient << endl;
                cout << "  Amount: " << transaction.amount << endl;
            }
            
            cout << "--------------------------------------------------------------------" << endl;
        }
    }

    ~Blockchain() {}
};