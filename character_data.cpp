#include "character_data.h"
#include <fstream>
#include <sstream>

void loadCharacterData(const std::string& filename,
                       std::vector<CharacterOption>& backgrounds,
                       std::vector<CharacterOption>& weapons)
{
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    bool readingBackgrounds = true;

    while (std::getline(file, line)) {
        if (line == "---") {
            readingBackgrounds = false;
            continue;
        }
        size_t delimiterPos = line.find('|');
        if (delimiterPos == std::string::npos) continue;

        CharacterOption option;
        option.name = line.substr(0, delimiterPos);
        option.description = line.substr(delimiterPos + 1);

        if (readingBackgrounds) {
            backgrounds.push_back(option);
        } else {
            weapons.push_back(option);
        }
    }
}
