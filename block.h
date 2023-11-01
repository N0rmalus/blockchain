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
using std::setfill;
using std::to_string;

string hasher(string input);

struct User {
    std::string name;
    std::string public_key;
    int balance;

    User(std::string n, std::string pk, int b) : name(n), public_key(pk), balance(b) {}
};
struct Transaction {
    std::string transactionId;
    std::string sender;
    std::string recipient;
    int amount;

    Transaction(std::string tid, std::string s, std::string r, int a) : transactionId(tid), sender(s), recipient(r), amount(a) {}
};

struct BlockHeader {
    std::string prev_block_hash;
    long timestamp;
    int version;
    std::string merkle_root_hash;
    int nonce;
    int difficulty_target;

    BlockHeader(std::string prev_hash, int ver, int diff_target) : prev_block_hash(prev_hash), version(ver), difficulty_target(diff_target) {
        timestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        nonce = 0;
        merkle_root_hash = "random_merkle_root_hash";
    }
};

class Block {
private:
    BlockHeader header;
    std::vector<Transaction> transactions;

public:
    Block(std::string prev_hash, int ver, int target) : header(prev_hash, ver, target) {
        vector<User> users;
        int usersAmount = 10, txAmount = usersAmount * 10;

        mt19937 mt(time(0));
        std::uniform_int_distribution<int> balanceIntr(100, 1000000);
        std::uniform_int_distribution<int> txIntr(0, txAmount - 1);
        std::uniform_int_distribution<int> userIntr(0, usersAmount - 1);

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
            transactions.push_back(Transaction(transactionId, sender, recipient, amount));
        }
    }

    const BlockHeader& getHeader() const {
        return header;
    }

    const std::vector<Transaction>& getTransactions() const {
        return transactions;
    }
};