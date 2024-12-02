#include <algorithm>
#include <iostream>
#include <map>

#ifdef AOC_TEST
#define INPUT_LINE_COUNT 6
#define INPUT_FILENAME "day1/input_test.txt"
#else
#define INPUT_LINE_COUNT 1000
#define INPUT_FILENAME "day1/input.txt"
#endif

int main() {
    int left_list[INPUT_LINE_COUNT];
    int right_list[INPUT_LINE_COUNT];

    // read file input
    char line[32];
    FILE *input_file = nullptr;
    if (!fopen_s(&input_file, INPUT_FILENAME, "r")) {}
    int i = 0;
    while (fscanf_s(input_file, "%d %d", &left_list[i], &right_list[i]) == 2) {
        ++i;
    }

    // sort lists
    std::ranges::sort(left_list);
    std::ranges::sort(right_list);

    printf("DISTANCE\n\n");

    // compute distance
    int total_distance = 0;
    int distance = 0;
    for (i = 0; i < INPUT_LINE_COUNT; ++i) {
        printf("Distance between %d and %d: ", left_list[i], right_list[i]);
        distance = left_list[i] < right_list[i] ? right_list[i] - left_list[i] : left_list[i] - right_list[i];
        total_distance += distance;
        printf("%d (new total: %d)\n", distance, total_distance);
    }

    printf("%d\n", total_distance);
    printf("\n=====\n\nSIMILARITY\n\n");
    int similarity_score = 0;
    int similarity = 0;
    std::map<int, int> occurrences;

    for (i = 0; i < INPUT_LINE_COUNT; ++i) {
        if (!occurrences.contains(right_list[i])) {
            occurrences[right_list[i]] = 0;
        }
        occurrences[right_list[i]] += 1;
    }
    for (i = 0; i < INPUT_LINE_COUNT; ++i) {
        printf("Similarity for %d (%d occurrences in right list): ", left_list[i], occurrences[left_list[i]]);
        similarity = left_list[i] * occurrences[left_list[i]];
        similarity_score += similarity;
        printf("%d (new total: %d)\n", similarity, similarity_score);
    }

    return 0;
}
