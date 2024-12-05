#include <cstdio>
#include <cstring>

#ifdef AOC_TEST
#define DIMENSIONS 10
#define INPUT_FILENAME "day4/input_test.txt"
#else
#define DIMENSIONS 140
#define INPUT_FILENAME "day4/input.txt"
#endif

struct coords_t {
    int x;
    int y;
};

constexpr coords_t DIR_UP = {0, 1};
constexpr coords_t DIR_DOWN = {0, -1};
constexpr coords_t DIR_LEFT = {-1, 0};
constexpr coords_t DIR_RIGHT = {1, 0};
constexpr coords_t DIR_UP_LEFT = {-1, 1};
constexpr coords_t DIR_UP_RIGHT = {1, 1};
constexpr coords_t DIR_DOWN_LEFT = {-1, -1};
constexpr coords_t DIR_DOWN_RIGHT = {1, -1};

constexpr char XMAS_WORD[] = {'X', 'M', 'A', 'S'};
constexpr size_t XMAS_LEN = sizeof(XMAS_WORD);
constexpr char MAS_WORD[] = {'M', 'A', 'S'};
constexpr size_t MAS_LEN = sizeof(MAS_WORD);

constexpr bool valid_move(const coords_t *pos) {
    return pos->x >= 0 && pos->y >= 0 && pos->x < DIMENSIONS && pos->y < DIMENSIONS;
}

coords_t get_move_position(const coords_t *src, const coords_t *dir) {
    coords_t result = {};
    result.x = src->x + dir->x;
    result.y = src->y + dir->y;
    return result;
}

char get_char(const char grid[DIMENSIONS][DIMENSIONS], const coords_t *pos) {
    char result = grid[pos->y][pos->x];
    return result;
}

bool char_is_at_pos(const char grid[DIMENSIONS][DIMENSIONS], const coords_t *pos, const char test_char) {
    return valid_move(pos) && get_char(grid, pos) == test_char;
}

bool search_direction(const char grid[DIMENSIONS][DIMENSIONS],
                      const coords_t *pos, const coords_t *dir,
                      const char word[], const size_t word_len, const int next_char_index) {
    if (next_char_index == word_len) {
        return true;
    }
    coords_t test = get_move_position(pos, dir);
    if (char_is_at_pos(grid, &test, word[next_char_index])) {
        return search_direction(grid, &test, dir, word, word_len, next_char_index + 1);
    }
    return false;
}

int search_clockwise(const char grid[DIMENSIONS][DIMENSIONS], const coords_t *pos) {
    if (get_char(grid, pos) != XMAS_WORD[0]) {
        return false;
    }
    int result = 0;
    result += search_direction(grid, pos, &DIR_UP_RIGHT, XMAS_WORD, XMAS_LEN, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_RIGHT, XMAS_WORD, XMAS_LEN, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_DOWN_RIGHT, XMAS_WORD, XMAS_LEN, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_DOWN, XMAS_WORD, XMAS_LEN, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_DOWN_LEFT, XMAS_WORD, XMAS_LEN, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_LEFT, XMAS_WORD, XMAS_LEN, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_UP_LEFT, XMAS_WORD, XMAS_LEN, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_UP, XMAS_WORD, XMAS_LEN, 1) ? 1 : 0;
    return result;
}

int search_cross(const char grid[DIMENSIONS][DIMENSIONS], const coords_t *pos) {
    if (get_char(grid, pos) != MAS_WORD[1]) {
        return false;
    }

    coords_t upper_left_corner = get_move_position(pos, &DIR_UP_LEFT);
    coords_t bottom_right_corner = get_move_position(pos, &DIR_DOWN_RIGHT);
    coords_t bottom_left_corner = get_move_position(pos, &DIR_DOWN_LEFT);
    coords_t upper_right_corner = get_move_position(pos, &DIR_UP_RIGHT);

    bool left_to_right = char_is_at_pos(grid, &upper_left_corner, MAS_WORD[0]) &&
                         char_is_at_pos(grid, &bottom_right_corner, MAS_WORD[2]) &&
                         char_is_at_pos(grid, &bottom_left_corner, MAS_WORD[0]) &&
                         char_is_at_pos(grid, &upper_right_corner, MAS_WORD[2]);
    bool right_to_left = char_is_at_pos(grid, &upper_left_corner, MAS_WORD[2]) &&
                         char_is_at_pos(grid, &bottom_right_corner, MAS_WORD[0]) &&
                         char_is_at_pos(grid, &bottom_left_corner, MAS_WORD[2]) &&
                         char_is_at_pos(grid, &upper_right_corner, MAS_WORD[0]);
    bool downwards = char_is_at_pos(grid, &upper_left_corner, MAS_WORD[0]) &&
                     char_is_at_pos(grid, &bottom_right_corner, MAS_WORD[2]) &&
                     char_is_at_pos(grid, &bottom_left_corner, MAS_WORD[2]) &&
                     char_is_at_pos(grid, &upper_right_corner, MAS_WORD[0]);
    bool upwards = char_is_at_pos(grid, &upper_left_corner, MAS_WORD[2]) &&
                   char_is_at_pos(grid, &bottom_right_corner, MAS_WORD[0]) &&
                   char_is_at_pos(grid, &bottom_left_corner, MAS_WORD[0]) &&
                   char_is_at_pos(grid, &upper_right_corner, MAS_WORD[2]);

    return left_to_right + right_to_left + downwards + upwards;
}

int main() {
    char grid[DIMENSIONS][DIMENSIONS];

    // read file input
    char line[DIMENSIONS + 2];
    FILE *input_file = nullptr;
    if (!fopen_s(&input_file, INPUT_FILENAME, "r")) {}
    int y = 0;
    while (fgets(line, sizeof(line), input_file)) {
        memcpy(grid[y++], line, DIMENSIONS * sizeof(char));
    }

    int xmas_total = 0;
    int cross_total = 0;
    coords_t current_pos = {};
    for (y = 0; y < DIMENSIONS; ++y) {
        current_pos.y = y;
        for (int x = 0; x < DIMENSIONS; ++x) {
            current_pos.x = x;
            int xmas_matches = search_clockwise(grid, &current_pos);
            int cross_match = search_cross(grid, &current_pos);
            xmas_total += xmas_matches;
            cross_total += cross_match;
        }
    }

    printf("Found %d xmas and %d crosses\n", xmas_total, cross_total);

    return 0;
}
