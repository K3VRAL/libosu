#include "object/std.h"

void oos_timingpoint_free(TimingPoint *tp) {
    if (tp != NULL) {
        free(tp);
    }
}

void oos_timingpoint_sort(TimingPoint *tp, unsigned int num) {
    for (int i = 0; i < num - 1; i++) {
        for (int j = i + 1; j < num; j++) {
            if ((tp + i)->time > (tp + j)->time) {
                TimingPoint temp = *(tp + i);
                *(tp + i) = *(tp + j);
                *(tp + j) = temp;
            }
        }
    }
}

// TimingPoint oos_timingpoint_attime(int time, TimingPoint *tp, unsigned int num) {
//     if (tp == NULL || num == 0 || time < (tp + 0)->time) {
//         return num > 0 ? (tp + 0) : /* TODO figure out what `TimingControlPoint.DEFAULT` is */;
//     }
//     if (time >= (tp + num - 1)->time) {
//         return *(tp + num - 1);
//     }

//     int l = 0;
//     int r = num - 2;
//     while (l <= r) {
//         int pivot = l + ((r - l) >> 1);
//         if ((tp + pivot)->time < time) {
//             l = pivot + 1;
//         } else if ((tp + pivot)->time > time) {
//             r = pivot - 1;
//         } else {
//             return *(tp + pivot);
//         }
//     }
//     return *(tp + l - 1);
// }