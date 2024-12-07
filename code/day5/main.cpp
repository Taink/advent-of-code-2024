#include <cstdio>
#include <cstring>
#include <map>
#include <set>
#include <vector>

#ifdef AOC_TEST
#define INPUT_FILENAME "day5/input_test.txt"
#else
#define INPUT_FILENAME "day5/input.txt"
#endif

#define MAX_LINE_LEN 128

void parse_priority_rule(const char *str, std::map<int, std::set<int>> *priority_map) {
    int lvalue = 0, rvalue = 0;
    sscanf_s(str, "%d|%d", &lvalue, &rvalue);

    if (!priority_map->contains(lvalue)) {
        std::set<int> followers;
        (*priority_map)[lvalue] = followers;
    }

    (*priority_map)[lvalue].insert(rvalue);
}

bool is_valid_update(int page,
                     const std::vector<int> *previous_pages,
                     const std::map<int, std::set<int>> *priority_map) {
    std::set<int> forbidden_pages;
    if (priority_map->contains(page)) {
        forbidden_pages = priority_map->at(page);
    }
    for (int previous_page: *previous_pages) {
        if (forbidden_pages.contains(previous_page)) return false;
    }
    return true;
}

void sort_update(std::vector<int> *pages,
                 const std::map<int, std::set<int>> *priority_map) {
    int n = pages->size();
    int new_n = 0;

    while (n > 1) {
        new_n = 0;
        for (int i = 1; i < n; ++i) {
            int prev = (*pages)[i - 1];
            int page = (*pages)[i];
            std::set<int> cur_page_rules;
            if (priority_map->contains(page)) {
                cur_page_rules = priority_map->at(page);
            }

            if (cur_page_rules.contains(prev)) {
                (*pages)[i - 1] = page;
                (*pages)[i] = prev;
                new_n = i;
            }
        }
        n = new_n;
    }
}

int get_middle_if_valid_update(const char *str,
                               const std::map<int, std::set<int>> *priority_map) {
    std::vector<int> pages;
    char delims[] = ",\n";
    char line[MAX_LINE_LEN];
    char *token = nullptr, *next_token = nullptr;
    bool valid = true;
    strcpy_s(line, str);
    token = strtok_s(line, delims, &next_token);

    while (token != nullptr) {
        int page = atoi(token);
        if (!is_valid_update(page, &pages, priority_map)) {
            valid = false;
        }
        pages.push_back(page);
        token = strtok_s(nullptr, delims, &next_token);
    }

    if (!valid) {
        sort_update(&pages, priority_map);
    }

    if (pages.size() % 2 == 1) {
        // odd length of update
        int middle_index = ((int) pages.size()) / 2;
        return valid ? pages[middle_index] : -pages[middle_index];
    }

    printf("cannot find middle value for line '%s'!\n", str);
    return 0;
}

int main() {
    int valid_middle_total = 0;
    int invalid_middle_total = 0;
    std::map<int, std::set<int>> priority_map;

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
            if (middle_value > 0) {
                valid_middle_total += middle_value;
            } else {
                invalid_middle_total += -middle_value;
            }
        }
    }

    printf("valid: %d, invalid: %d\n", valid_middle_total, invalid_middle_total);

    return 0;
}
