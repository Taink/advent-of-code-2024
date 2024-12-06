#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

#ifdef AOC_TEST
#define INPUT_FILENAME "day5/input_test.txt"
#else
#define INPUT_FILENAME "day5/input.txt"
#endif

#define MAX_LINE_LEN 128

void parse_priority_rule(const char *str, std::map<int, std::vector<int>> *priority_map) {
    int lvalue = 0, rvalue = 0;
    sscanf_s(str, "%d|%d", &lvalue, &rvalue);

    if (!priority_map->contains(lvalue)) {
        std::vector<int> followers;
        (*priority_map)[lvalue] = followers;
    }

    (*priority_map)[lvalue].push_back(rvalue);
}

bool is_valid_update(int page,
                     const std::vector<int> *previous_pages,
                     const std::map<int, std::vector<int>> *priority_map) {
    std::vector<int> forbidden_pages;
    if (priority_map->contains(page)) {
        forbidden_pages = priority_map->at(page);
    }
    for (int previous_page: *previous_pages) {
        for (int forbidden_page: forbidden_pages) {
            if (previous_page == forbidden_page) return false;
        }
    }
    return true;
}

int get_middle_if_valid_update(const char *str,
                               const std::map<int, std::vector<int>> *priority_map) {
    std::vector<int> pages;
    char delims[] = ",\n";
    char line[MAX_LINE_LEN];
    char *token = nullptr, *next_token = nullptr;
    strcpy_s(line, str);
    token = strtok_s(line, delims, &next_token);

    while (token != nullptr) {
        int page = atoi(token);
        if (!is_valid_update(page, &pages, priority_map)) {
            // we can safely return a 0 value since we are computing a sum
            return 0;
        }
        pages.push_back(page);
        token = strtok_s(nullptr, delims, &next_token);
    }

    // everything is valid, hopefully.
    if (pages.size() % 2 == 1) {
        // odd length of update
        int middle_index = ((int) pages.size()) / 2;
        return pages[middle_index];
    }

    printf("cannot find middle value for line '%s'!\n", str);
    return 0;
}

int main() {
    int total = 0;
    std::map<int, std::vector<int>> priority_map;

    // read file input
    char line[MAX_LINE_LEN];
    FILE *input_file = nullptr;
    if (!fopen_s(&input_file, INPUT_FILENAME, "r")) {}
    bool changed_section = false;
    while (fgets(line, sizeof(line), input_file)) {
        if (strcmp("\n", line) == 0) {
            changed_section = true;
            continue;
        }
        if (!changed_section) {
            parse_priority_rule(line, &priority_map);
        } else {
            int middle_value = get_middle_if_valid_update(line, &priority_map);
            total += middle_value;
        }
    }

    printf("total: %d\n", total);

    return 0;
}
