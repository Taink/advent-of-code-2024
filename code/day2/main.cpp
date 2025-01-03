#include <iostream>

#ifdef AOC_TEST
#define INPUT_LINE_COUNT 6
#define INPUT_FILENAME "day2/input_test.txt"
#else
#define INPUT_LINE_COUNT 1000
#define INPUT_FILENAME "day2/input.txt"
#endif

struct report_t {
    int levels[10];
    int level_count;
};

bool compute_report_safety(const report_t *report) {
    int increase_factor = 0;
    for (int i = 1; i < report->level_count; ++i) {
        const int previous_level = report->levels[i - 1];
        const int current_level = report->levels[i];
        if (increase_factor == 0) {
            increase_factor = (previous_level < current_level) ? -1 : 1;
        }

        int difference = (previous_level - current_level) * increase_factor;
        if (difference < 1 || difference > 3) {
            return false;
        }
    }
    return true;
}

bool compute_report_safety_dampened(const report_t *report) {
    if (compute_report_safety(report)) {
        return true;
    }

    for (int removed_index = 0; removed_index < report->level_count; ++removed_index) {
        report_t fake_report = {};

        fake_report.level_count = report->level_count - 1;
        int offset = 0;
        for (int i = 0; i < fake_report.level_count; ++i) {
            if (i == removed_index) {
                offset = 1;
            }
            fake_report.levels[i] = report->levels[i + offset];
        }
        if (compute_report_safety(&fake_report)) {
            return true;
        }
    }
    return false;
}

int main() {
    report_t reports[INPUT_LINE_COUNT];

    // read file input
    char line[32];
    FILE *input_file = nullptr;
    if (!fopen_s(&input_file, INPUT_FILENAME, "r")) {}
    int i = 0;
    while (fgets(line, sizeof(line), input_file)) {
        reports[i].level_count = sscanf_s(line, "%d %d %d %d %d %d %d %d %d %d", &reports[i].levels[0],
                                          &reports[i].levels[1], &reports[i].levels[2], &reports[i].levels[3],
                                          &reports[i].levels[4], &reports[i].levels[5], &reports[i].levels[6],
                                          &reports[i].levels[7], &reports[i].levels[8], &reports[i].levels[9]);
        ++i;
    }

    int total_safe = 0;
    for (i = 0; i < INPUT_LINE_COUNT; ++i) {
        report_t *report = &reports[i];
        bool report_is_safe = compute_report_safety_dampened(report);
        printf("Report #%d is %s\n", i, report_is_safe ? "SAFE" : "UNSAFE");
        if (report_is_safe) {
            total_safe += 1;
        }
    }

    printf("Total safe reports : %d\n", total_safe);

    return 0;
}
