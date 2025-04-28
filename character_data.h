#ifndef CHARACTER_DATA_H
#define CHARACTER_DATA_H

#include <string>
#include <vector>

struct CharacterOption {
    std::string name;
    std::string description;
};

void loadCharacterData(const std::string& filename,
                       std::vector<CharacterOption>& backgrounds,
                       std::vector<CharacterOption>& weapons);

#endif
