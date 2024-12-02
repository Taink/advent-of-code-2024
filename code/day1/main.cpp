#include <algorithm>
#include <iostream>

int main() {
    int left_list[1000];
    int right_list[1000];

    // read file input
    char line[32];
    FILE *input_file = nullptr;
    if (!fopen_s(&input_file, "day1/input1.txt", "r")) {}
    int i = 0;
    while (fscanf_s(input_file, "%d %d", &left_list[i], &right_list[i]) == 2) {
        ++i;
    }
    const int list_size = i;

    // sort lists
    std::ranges::sort(left_list);
    std::ranges::sort(right_list);

    // compute distance
    int total_distance = 0;
    int distance = 0;
    for (i = 0; i < list_size; ++i) {
        printf("Distance between %d and %d: ", left_list[i], right_list[i]);
        distance = left_list[i] < right_list[i] ? right_list[i] - left_list[i] : left_list[i] - right_list[i];
        total_distance += distance;
        printf("%d (new total: %d)\n", distance, total_distance);
    }

    printf("%d\n", total_distance);

    return 0;
}