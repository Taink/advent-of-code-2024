#include <cstdio>
#include <cstring>

#ifdef AOC_TEST
#define INPUT_FILENAME "day6/input_test.txt"
#define DIMENSIONS 10
#else
#define INPUT_FILENAME "day6/input.txt"
#define DIMENSIONS 130
#endif

constexpr char GUARD_CHAR = '^';
constexpr char EMPTY_CHAR = '.';
constexpr char BLOCK_CHAR = '#';
constexpr char VISIT_CHAR = 'X';

struct coords_t {
    int x;
    int y;
};
enum direction_t { UP, DOWN, LEFT, RIGHT };
constexpr coords_t DIR_UP = {0, -1};
constexpr coords_t DIR_DOWN = {0, 1};
constexpr coords_t DIR_LEFT = {-1, 0};
constexpr coords_t DIR_RIGHT = {1, 0};

constexpr coords_t direction_to_coords(const direction_t direction) {
    switch (direction) {
        case UP: {
            return DIR_UP;
        }
        case DOWN: {
            return DIR_DOWN;
        }
        case LEFT: {
            return DIR_LEFT;
        }
        default: {
            return DIR_RIGHT;
        }
    }
}

struct guard_t {
    coords_t *position;
    direction_t orientation;
};

void rotate_guard(guard_t *guard) {
    switch (guard->orientation) {
        case UP: {
            guard->orientation = RIGHT;
            return;
        }
        case RIGHT: {
            guard->orientation = DOWN;
            return;
        }
        case DOWN: {
            guard->orientation = LEFT;
            return;
        }
        case LEFT: {
            guard->orientation = UP;
        }
    }
}

constexpr bool valid_move(const coords_t *pos) {
    return pos->x >= 0 && pos->y >= 0 && pos->x < DIMENSIONS && pos->y < DIMENSIONS;
}

constexpr coords_t get_guard_front_position(const guard_t *guard) {
    coords_t result = {};
    coords_t move = direction_to_coords(guard->orientation);
    result.x = guard->position->x + move.x;
    result.y = guard->position->y + move.y;
    return result;
}

char get_char(const char grid[DIMENSIONS][DIMENSIONS], const coords_t *pos) {
    char result = grid[pos->y][pos->x];
    return result;
}

void set_char(char grid[DIMENSIONS][DIMENSIONS], const coords_t *pos, const char c) {
    grid[pos->y][pos->x] = c;
}

bool char_is_at_pos(const char grid[DIMENSIONS][DIMENSIONS], const coords_t *pos, const char test_char) {
    return valid_move(pos) && get_char(grid, pos) == test_char;
}

int simulate_guard_route(char grid[DIMENSIONS][DIMENSIONS], guard_t *guard) {
    coords_t next_guard_move = get_guard_front_position(guard);
    printf("[%d;%d]\n", next_guard_move.y, next_guard_move.x);
    int visits = 1;
    set_char(grid, guard->position, VISIT_CHAR);
    while (valid_move(&next_guard_move)) {
        if (char_is_at_pos(grid, &next_guard_move, BLOCK_CHAR)) {
            rotate_guard(guard);
            next_guard_move = get_guard_front_position(guard);
            continue;
        }
        guard->position->x = next_guard_move.x;
        guard->position->y = next_guard_move.y;
        if (!char_is_at_pos(grid, &next_guard_move, VISIT_CHAR)) {
            ++visits;
        }
        set_char(grid, guard->position, VISIT_CHAR);
        next_guard_move = get_guard_front_position(guard);
        printf("[%d;%d]\n", next_guard_move.y, next_guard_move.x);
    }
    printf("STOP\n");
    return visits;
}

int main() {
    char grid[DIMENSIONS][DIMENSIONS];
    guard_t guard = {};
    coords_t guard_pos = {};
    guard.orientation = UP;
    guard.position = &guard_pos;
    int positions = 0;

    // read file input
    char line[DIMENSIONS + 2];
    FILE *input_file = nullptr;
    if (!fopen_s(&input_file, INPUT_FILENAME, "r")) {}
    int y = 0;
    while (fgets(line, sizeof(line), input_file)) {
        memcpy(grid[y], line, DIMENSIONS * sizeof(char));
        for (int x = 0; x < DIMENSIONS; ++x) {
            if (line[x] == GUARD_CHAR) {
                guard.position->x = x;
                guard.position->y = y;
            }
        }
        ++y;
    }

    // IMPORTANT: there seems to be an issue where the last position is not marked properly
    positions = simulate_guard_route(grid, &guard);

    for (y = 0; y < DIMENSIONS; ++y) {
        for (int x = 0; x < DIMENSIONS; ++x) {
            printf("%c", grid[y][x]);
        }
        printf("\n");
    }

    printf("positions: %d\n", positions);

    return 0;
}
