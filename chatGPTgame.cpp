#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <cstdlib>

class Quest {
public:
    Quest(const std::string& name, const std::string& description, const std::string& objective)
        : name(name), description(description), objective(objective), completed(false) {}

    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    std::string getObjective() const { return objective; }
    bool isCompleted() const { return completed; }

    void complete() { completed = true; }

private:
    std::string name;
    std::string description;
    std::string objective;
    bool completed;
};


class Item {
public:
    Item(std::string name, int value) : name(name), value(value) {}
    std::string getName() const { return name; }
    int getValue() const { return value; }

private:
    std::string name;
    int value;
};

class Location {
public:
    Location(std::string description) : description(description) {}
    std::string getDescription() const { return description; }

private:
    std::string description;
};

class NPC {
public:
    NPC(const std::string& name, int health, int damage, const std::string& greeting)
        : name(name), health(health), greeting(greeting) {}
    std::string getName() const { return name; }
    std::string getGreeting() const { return greeting; }
    int getHealth() const { return health; }
    void setHealth(int health) { this->health = health; }
    int getAttack() const { return attack; }
    void addDialogueOption(const std::string& option) {
        dialogueOptions.push_back(option);
    }
    const std::vector<std::string>& getDialogueOptions() const {
        return dialogueOptions;
    }

private:
    std::string name;
    int health;
    int attack;
    std::string greeting;
    std::vector<std::string> dialogueOptions;
};

class Player {
public:
    Player(int health, int attack) : health(health), attack(attack) {}

    int getHealth() const { return health; }
    void setHealth(int health) { this->health = health; }
    int getAttack() const { return attack; }
    void addToInventory(const Item& item) { inventory.push_back(item); }
    void showInventory() const;

private:
    int health;
    int attack;
    std::vector<Item> inventory;
};

void Player::showInventory() const {
    if (inventory.empty()) {
        std::cout << "Your inventory is empty.\n";
    } else {
        std::cout << "Inventory:\n";
        for (const Item& item : inventory) {
            std::cout << "- " << item.getName() << " (Value: " << item.getValue() << ")\n";
        }
    }
}

class Game {
public:
    Game();
    void run();

private:
    void processInput(const std::string& input);
    void showLocation() const;
    void interactWith(const std::string& target);
    void attackNPC(const std::string& target);
    void lookAround() const;
    void showHelp() const;
    void checkQuestCompletion(const std::string& itemName);
    void startConversation(const NPC& npc);

    Player player;
    std::vector<Location> locations;
    std::vector<NPC> npcs;
    std::vector<Item> items;
    size_t currentLocation;
    std::vector<Quest> quests;
};

Game::Game() : player(100, 10), currentLocation(0) {
    srand(static_cast<unsigned>(time(0))); // Seed random number generator

    // Initialize the game state, such as adding locations, NPCs, and items
    locations.emplace_back("You are in a dark room.");
    locations.emplace_back("You are in a bright room.");
    npcs.emplace_back("John", 50, 5, "Hello, traveler!");
    npcs.emplace_back("Jane", 80, 8, "Greetings!");
    items.emplace_back("Sword", 100);
    items.emplace_back("Shield", 50);
    quests.emplace_back("Find the Sword", "Find the legendary sword and bring it back to John.", "Sword");
    quests.emplace_back("Find the Shield", "Find the legendary shield and bring it back to Jane.", "Shield");
    npcs.emplace_back("John", 50, 5, "Hello, traveler!");
    npcs.back().addDialogueOption("Tell me about yourself.");
    npcs.back().addDialogueOption("What do you do here?");

    npcs.emplace_back("Jane", 80, 8, "Greetings!");
    npcs.back().addDialogueOption("Who are you?");
    npcs.back().addDialogueOption("How long have you been here?");
}

void Game::run() {
    std::string input;
    std::cout << "Welcome to Fallout 4 Text-Based RPG!\n";

    while (true) {
        showLocation();
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "exit") {
            break;
        }
        processInput(input);
    }

    std::cout << "Thanks for playing!\n";
}

void Game::processInput(const std::string& input) {
    if (input.substr(0, 9) == "interact ") {
        std::string target = input.substr(9);
        interactWith(target);
    } else if (input.substr(0, 7) == "attack ") {
        std::string target = input.substr(7);
        attackNPC(target);
    } else if (input == "inventory") {
        player.showInventory();
    } else if (input == "look") {
        lookAround();
    } else if (input == "help") {
        showHelp();
    } else {
        std::cout << "Unknown command.\n";
    }
}

void Game::showLocation() const {
    std::cout << locations[currentLocation].getDescription() << '\n';
}


void Game::checkQuestCompletion(const std::string& itemName) {
    for (Quest& quest : quests) {
        if (!quest.isCompleted() && quest.getObjective() == itemName) {
            quest.complete();
            std::cout << "You completed the quest: " << quest.getName() << "!\n";
        }
    }
}

void Game::interactWith(const std::string& target) {
    // Interact with NPCs
    for (const NPC& npc : npcs) {
        if (npc.getName() == target) {
            std::cout << npc.getGreeting() << '\n';
            startConversation(npc);
            if (npc.getName() == "John" || npc.getName() == "Jane") {
                for (Quest& quest : quests) {
                    if (!quest.isCompleted() && quest.getObjective() == target) {
                        std::cout << "Quest: " << quest.getName() << '\n';
                        std::cout << quest.getDescription() << '\n';
                    }
                }
            }
            return;
        }
    }

    // Interact with items
    for (const Item& item : items) {
        if (item.getName() == target) {
            std::cout << "You picked up a " << item.getName() << ".\n";
            player.addToInventory(item);
            checkQuestCompletion(item.getName());
            return;
        }
    }

    std::cout << "There's no '" << target << "' here.\n";
}

void Game::attackNPC(const std::string& target) {
    for (NPC& npc : npcs) {
        if (npc.getName() == target) {
            npc.setHealth(npc.getHealth() - player.getAttack());
            std::cout << "You attacked " << npc.getName() << " for " << player.getAttack() << " damage.\n";

            if (npc.getHealth() <= 0) {
                std::cout << npc.getName() << " has been defeated!\n";
            } else {
                player.setHealth(player.getHealth() - npc.getAttack());
                std::cout << npc.getName() << " attacked you for " << npc.getAttack() << " damage.\n";
                if (player.getHealth() <= 0) {
                    std::cout << "You have been defeated!\n";
                    exit(0);
                }
            }
            return;
        }
    }

    std::cout << "There's no '" << target << "' here.\n";
}

void Game::startConversation(const NPC& npc) {
    std::cout << "Choose a dialogue option:\n";
    const auto& options = npc.getDialogueOptions();
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << (i + 1) << ". " << options[i] << '\n';
    }
    std::cout << "0. [End conversation]\n";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice > 0 && choice <= static_cast<int>(options.size())) {
        std::cout << npc.getName() << ": ";
        switch (choice) {
            case 1:
                std::cout << "I'm just an ordinary wastelander trying to survive in this harsh world.\n";
                break;
            case 2:
                std::cout << "I scavenge for valuable items and trade with other survivors.\n";
                break;
            default:
                std::cout << "I don't have much to say about that.\n";
        }
        startConversation(npc);
    }
}


void Game::lookAround() const {
    std::cout << "You see:\n";
    std::cout << "NPCs:\n";
    for (const NPC& npc : npcs) {
        std::cout << "- " << npc.getName() << '\n';
    }
    std::cout << "Items:\n";
    for (const Item& item : items) {
        std::cout << "- " << item.getName() << '\n';
    }
}

void Game::showHelp() const {
    std::cout << "List of commands:\n";
    std::cout << "interact [target] - Interact with an NPC or item.\n";
    std::cout << "attack [target] - Attack an NPC.\n";
    std::cout << "inventory - Display the contents of your inventory.\n";
    std::cout << "look - See what is around you.\n";
    std::cout << "help - Show this help message.\n";
    std::cout << "exit - Quit the game.\n";
}

int main() {
    Game game;
    game.run();
    return 0;
}
