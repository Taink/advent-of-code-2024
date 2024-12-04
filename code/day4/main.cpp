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

constexpr char WORD[] = {'X', 'M', 'A', 'S'};

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

bool search_direction(const char grid[DIMENSIONS][DIMENSIONS],
                      const coords_t *pos, const coords_t *dir,
                      int next_char_index) {
    if (next_char_index == sizeof(WORD)) {
        return true;
    }
    coords_t test = get_move_position(pos, dir);
    if (valid_move(&test) && get_char(grid, &test) == WORD[next_char_index]) {
        return search_direction(grid, &test, dir, next_char_index + 1);
    }
    return false;
}

int search_clockwise(const char grid[DIMENSIONS][DIMENSIONS], const coords_t *pos) {
    if (get_char(grid, pos) != WORD[0]) {
        return false;
    }
    int result = 0;
    result += search_direction(grid, pos, &DIR_UP_RIGHT, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_RIGHT, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_DOWN_RIGHT, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_DOWN, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_DOWN_LEFT, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_LEFT, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_UP_LEFT, 1) ? 1 : 0;
    result += search_direction(grid, pos, &DIR_UP, 1) ? 1 : 0;
    return result;
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

    int total = 0;
    coords_t current_pos = {};
    for (y = 0; y < DIMENSIONS; ++y) {
        current_pos.y = y;
        for (int x = 0; x < DIMENSIONS; ++x) {
            current_pos.x = x;
            int matches = search_clockwise(grid, &current_pos);
            total += matches;
        }
    }

    printf("Found %d matches\n", total);

    return 0;
}
