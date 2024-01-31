#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <unordered_map>

// Define a basic block structure
struct Block {
    int index;               // Block index
    std::string voterID;     // Voter's identification
    std::string candidate;   // Candidate voted for
    time_t timestamp;        // Time of the vote
    std::string previousHash; // Hash of the previous block in the chain
    std::string hash;         // Hash of the current block

    // Constructor for creating a new block
    Block(int idx, const std::string& voter, const std::string& candidate, const std::string& prevHash)
        : index(idx), voterID(voter), candidate(candidate), previousHash(prevHash) {
        timestamp = time(nullptr);
        // Calculate the hash using block data
        hash = std::to_string(index) + voterID + candidate + std::to_string(timestamp) + previousHash;
    }
};

// Define a basic blockchain structure
class Blockchain {
private:
    std::vector<Block> chain; // Vector to store blocks in the blockchain

public:
    // Constructor to initialize the blockchain with a genesis block
    Blockchain() {
        chain.emplace_back(Block(0, "Admin", "Candidate", "0"));
    }

    // Function to add a new vote block to the blockchain
    void addVote(const std::string& voter, const std::string& candidate) {
        const Block& lastBlock = chain.back();
        // Create and add a new block to the chain
        chain.emplace_back(Block(lastBlock.index + 1, voter, candidate, lastBlock.hash));
    }

    // Function to get the entire blockchain
    const std::vector<Block>& getChain() const {
        return chain;
    }
};

// Define a basic voter registry
class VoterRegistry {
private:
    std::unordered_map<std::string, bool> registeredVoters; // Map to store registered voters

public:
    // Function to add a voter to the registry
    void addVoter(const std::string& aadharNumber) {
        registeredVoters[aadharNumber] = true;
    }

    // Function to check if a voter is registered
    bool isVoterRegistered(const std::string& aadharNumber) const {
        return registeredVoters.count(aadharNumber) > 0;
    }

    // Admin function: Register a voter
    void registerVoterByAdmin(const std::string& adminPassword, const std::string& aadharNumber) {
        // Basic admin password check (for simplicity, use secure authentication in a real-world scenario)
        if (adminPassword == "adminpassword") {
            // Register the voter
            addVoter(aadharNumber);
            std::cout << "Voter with Aadhar Number " << aadharNumber << " registered successfully.\n";
        } else {
            std::cout << "Admin authentication failed. Unable to register voter.\n";
        }
    }
};

// Function to determine the winner based on the votes for each party
void getWinner(std::string parties[], int votes[], int size) {
    int max = votes[0];
    int idx = 0;
    for (int i = 1; i < size; i++) {
        if (max < votes[i]) {
            max = votes[i];
            idx = i;
        }
    }
    std::cout << "Winner: " << parties[idx] << std::endl;
}

int main() {
    // Create a blockchain and a voter registry
    Blockchain myBlockchain;
    VoterRegistry voterRegistry;
    int totalVoters = 0;
    int votesForTMC = 0;
    int votesForBJP = 0;
    int votesForDMK = 0;
    int votesForCPIM = 0;
    int votesForAAP = 0;
    std::string voter = "";
    std::string candidate;
    bool registering = true;

    while (registering) {
        std::cout << "Enter 0000 to terminate registration and voting." << std::endl;
        std::cout << "Enter your Voter no.: ";
        std::cin >> voter;

        if (voter == "0000") {
            std::cout << "Voting ends..." << std::endl;
            break;
        } else if (voter.length() != 10) {
            std::cout << "Invalid Voter ID!!!" << std::endl;
            break;
        }

        // Admin registers voters
        voterRegistry.registerVoterByAdmin("adminpassword", voter);

        // Simulate a user logging in and casting a vote
        if (voterRegistry.isVoterRegistered(voter)) {
            std::cout << "Welcome, Voter with Voter Number: " << voter << "\n";
            std::cout << "Choose your candidate:\n1.TMC\n2.BJP\n3.DMK\n4.CPIM\n5.AAP\n";

            std::cin >> candidate;
            ++totalVoters;
            // Check if the candidate is valid (omitting validation for simplicity)
            if (!candidate.empty()) {
                // Add the vote to the blockchain
                myBlockchain.addVote(voter, candidate);
                std::cout << "Vote cast successful!\n";
            } else {
                std::cout << "Invalid candidate selection.\n";
            }
        } else {
            std::cout << "Voter with Voter Number " << voter << " is not registered.\n";
        }

        // Update the vote count for the selected candidate/party
        if (candidate == "1") {
            ++votesForTMC;
        } else if (candidate == "2") {
            ++votesForBJP;
        } else if (candidate == "3") {
            ++votesForDMK;
        } else if (candidate == "4") {
            ++votesForCPIM;
        } else if (candidate == "5") {
            ++votesForAAP;
        } else {
            std::cout << "Invalid candidate selection.\n";
        }
    }

    std::string parties[5] = {"TMC", "BJP", "DMK", "CPIM", "AAP"};
    int votes[5] = {votesForTMC, votesForBJP, votesForDMK, votesForCPIM, votesForAAP};

    // Display the blockchain
    const std::vector<Block>& chain = myBlockchain.getChain();
    for (const Block& block : chain) {
        std::cout << "Block #" << block.index << "\n";
        std::cout << "Voter ID: " << block.voterID << ", Candidate Voted: " << block.candidate << "\n";
        std::cout << "Timestamp: " << std::ctime(&block.timestamp);
        std::cout << "Previous Hash: " << block.previousHash << "\n";
        std::cout << "Hash: " << block.hash << "\n\n";
    }

    // Display the total votes for each party at the end
    std::cout << "Total votes for each party:\n";
    std::cout << "TMC: " << votesForTMC << " votes\n";
    std::cout << "BJP: " << votesForBJP << " votes\n";
    std::cout << "DMK: " << votesForDMK << " votes\n";
    std::cout << "CPIM: " << votesForCPIM << " votes\n";
    std::cout << "AAP: " << votesForAAP << " votes\n";
    std::cout << "Total Voters: " << totalVoters << std::endl;

    // Determine and display the winner
    getWinner(parties, votes, 5);

    return 0;
}
