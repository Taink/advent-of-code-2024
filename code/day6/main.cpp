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
constexpr char TEMP_BLOCK_CHAR = 'O';
constexpr char VISIT_CHAR = 'X';

constexpr char UP_CHAR = 'U';
constexpr char DOWN_CHAR = 'D';
constexpr char LEFT_CHAR = 'L';
constexpr char RIGHT_CHAR = 'R';

struct coords_t {
    int x;
    int y;
};
enum direction_t { UP, DOWN, LEFT, RIGHT };
constexpr coords_t DIR_UP = {0, -1};
constexpr coords_t DIR_DOWN = {0, 1};
constexpr coords_t DIR_LEFT = {-1, 0};
constexpr coords_t DIR_RIGHT = {1, 0};

constexpr char direction_to_char(const direction_t direction) {
    switch (direction) {
        case UP: {
            return UP_CHAR;
        }
        case DOWN: {
            return DOWN_CHAR;
        }
        case LEFT: {
            return LEFT_CHAR;
        }
        default: {
            return RIGHT_CHAR;
        }
    }
}

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

constexpr char get_char(const char grid[DIMENSIONS][DIMENSIONS], const coords_t *pos) {
    char result = grid[pos->y][pos->x];
    return result;
}

void set_char(char grid[DIMENSIONS][DIMENSIONS], const coords_t *pos, const char c) {
    grid[pos->y][pos->x] = c;
}

constexpr bool char_is_at_pos(const char grid[DIMENSIONS][DIMENSIONS], const coords_t *pos, const char test_char) {
    return valid_move(pos) && get_char(grid, pos) == test_char;
}

constexpr bool pos_is_blocked(const char grid[DIMENSIONS][DIMENSIONS], const coords_t *pos) {
    return char_is_at_pos(grid, pos, BLOCK_CHAR) ||
           char_is_at_pos(grid, pos, TEMP_BLOCK_CHAR);
}

constexpr bool pos_is_visited(const char grid[DIMENSIONS][DIMENSIONS], const coords_t *pos) {
    return char_is_at_pos(grid, pos, UP_CHAR) ||
           char_is_at_pos(grid, pos, DOWN_CHAR) ||
           char_is_at_pos(grid, pos, LEFT_CHAR) ||
           char_is_at_pos(grid, pos, RIGHT_CHAR) ||
           char_is_at_pos(grid, pos, VISIT_CHAR);
}

int count_guard_positions(char grid[DIMENSIONS][DIMENSIONS], guard_t *guard) {
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

void reset_grid_to_input(char grid[DIMENSIONS][DIMENSIONS]) {
    for (int y = 0; y < DIMENSIONS; ++y) {
        for (int x = 0; x < DIMENSIONS; ++x) {
            switch (grid[y][x]) {
                case UP_CHAR:
                case DOWN_CHAR:
                case LEFT_CHAR:
                case RIGHT_CHAR:
                case VISIT_CHAR:
                case TEMP_BLOCK_CHAR:
                case GUARD_CHAR:
                    grid[y][x] = EMPTY_CHAR;
                    break;
                default:
                    break;
            }
        }
    }
}

bool guard_route_has_loop(char grid[DIMENSIONS][DIMENSIONS], guard_t *guard) {
    coords_t next_guard_move = get_guard_front_position(guard);
    set_char(grid, guard->position, direction_to_char(guard->orientation));
    while (valid_move(&next_guard_move)) {
        if (char_is_at_pos(grid, &next_guard_move, direction_to_char(guard->orientation))) {
            return true;
        }
        if (pos_is_blocked(grid, &next_guard_move)) {
            rotate_guard(guard);
            next_guard_move = get_guard_front_position(guard);
            continue;
        }
        guard->position->x = next_guard_move.x;
        guard->position->y = next_guard_move.y;
        set_char(grid, guard->position, direction_to_char(guard->orientation));
        next_guard_move = get_guard_front_position(guard);
    }
    return false;
}

int count_looping_obstructions(char grid[DIMENSIONS][DIMENSIONS], guard_t *guard) {
    const coords_t original_guard_position = *guard->position;
    coords_t obstacle_position = {};
    int obstacles = 0;
    for (int y = 0; y < DIMENSIONS; ++y) {
        obstacle_position.y = y;
        for (int x = 0; x < DIMENSIONS; ++x) {
            obstacle_position.x = x;
            if (char_is_at_pos(grid, &obstacle_position, BLOCK_CHAR)) {
                continue;
            }
            set_char(grid, &obstacle_position, TEMP_BLOCK_CHAR);
            *guard->position = original_guard_position;
            guard->orientation = UP;
            if (guard_route_has_loop(grid, guard)) {
                ++obstacles;
            }
            reset_grid_to_input(grid);
        }
    }
    return obstacles;
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

    for (y = 0; y < DIMENSIONS; ++y) {
        for (int x = 0; x < DIMENSIONS; ++x) {
            printf("%c", grid[y][x]);
        }
        printf("\n");
    }

    // positions = count_guard_positions(grid, &guard);
    positions = count_looping_obstructions(grid, &guard);

    printf("positions: %d\n", positions);

    return 0;
}
