#include <stdio.h>
#include <math.h>
#include <locale.h>

#define ROWS_COUNT 17
#define COMP_COUNT 3
#define POINTS_COUNT 5
#define COL_WIDTH 16
#define L_LIMIT 2.0f
#define ST_NORM 0
#define ST_ALARM 1

typedef struct {
    int time;
    int type;
    int count;
    float value;
} Incident;

int main(void) {
    setlocale(LC_ALL, ".UTF8");
    int tm[ROWS_COUNT] = { 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5 };
    int cp[ROWS_COUNT] = { 1, 2, 0, 0, 2, 1, 1, 0, 2, 1, 1, 1, 2, 0, 2, 0, 1 };
    float x[ROWS_COUNT] = { 57.4f, 30.2f, 41.9f, 47.8f, 29.5f, 57.7f, 55.4f, 48.3f, 34.1f,
                            53.4f, 51.4f, 53.7f, 33.4f, 48.8f, 32.6f, 49.3f, 54.8f };

    float q[ROWS_COUNT] = { 0.0f };
    int has_q[ROWS_COUNT] = { 0 };
    int alarm[POINTS_COUNT + 1][COMP_COUNT] = { { 0 } };
    float q_alarm[POINTS_COUNT + 1][COMP_COUNT] = { { 0.0f } };

    Incident incidents[POINTS_COUNT];
    int incident_count = 0;
    float avg;
    int r, j, p, c, i, t, n, problem;

    for (r = 0; r < ROWS_COUNT; r++) {
        p = -1;
        for (j = r - 1; j >= 0; j--) {
            if (cp[j] == cp[r]) {
                p = j;
                break;
            }
        }
        if (p < 0) {
            continue;
        }
        avg = (fabsf(x[r]) + fabsf(x[p])) / 2.0f;
        if (avg == 0.0f) {
            continue;
        }
        q[r] = 100.0f * (x[r] - x[p]) / avg;
        has_q[r] = 1;
        if (fabsf(q[r]) > L_LIMIT) {
            alarm[tm[r]][cp[r]] = ST_ALARM;
            q_alarm[tm[r]][cp[r]] = q[r];
        }
    }

    printf("Контроль облачной инфраструктуры (Вариант 19, КТ1)\n\n");

    printf("Исходные данные телеметрии:\n");
    printf("Отсчет  %-16s%-16s%-16s\n", "RAM", "TMP", "PWR");
    for (t = 1; t <= POINTS_COUNT; t++) {
        printf("%-8d", t);
        for (c = 0; c < COMP_COUNT; c++) {
            n = 0;
            for (r = 0; r < ROWS_COUNT; r++) {
                if (tm[r] == t && cp[r] == c) {
                    n += printf(n ? "; %.1f" : "%.1f", x[r]);
                }
            }
            printf("%*s", COL_WIDTH - n, "");
        }
        printf("\n");
    }
    printf("\n");

    printf("Симметрированное относительное изменение Q (%%) при k = 1:\n");
    printf("Отсчет  %-16s%-16s%-16s\n", "Q(RAM)", "Q(TMP)", "Q(PWR)");
    for (t = 2; t <= POINTS_COUNT; t++) {
        printf("%-8d", t);
        for (c = 0; c < COMP_COUNT; c++) {
            n = 0;
            for (r = 0; r < ROWS_COUNT; r++) {
                if (tm[r] == t && cp[r] == c && has_q[r]) {
                    n += printf(n ? "; %+.2f" : "%+.2f", q[r]);
                }
            }
            printf("%*s", COL_WIDTH - n, "");
        }
        printf("\n");
    }
    printf("\n");

    printf("Состояние компонентов (порог L = %.1f%%):\n", L_LIMIT);
    printf("Отсчет  %-16s%-16s%-16s\n", "RAM", "TMP", "PWR");
    for (t = 1; t <= POINTS_COUNT; t++) {
        printf("%-8d", t);
        for (c = 0; c < COMP_COUNT; c++) {
            printf("%s", alarm[t][c] ? "ТРЕВОГА         " : "НОРМА           ");
        }
        printf("\n");
    }
    printf("\n");

    for (t = 1; t <= POINTS_COUNT; t++) {
        problem = alarm[t][0] + alarm[t][1] + alarm[t][2];
        if (problem == 1) {
            for (c = 0; c < COMP_COUNT; c++) {
                if (alarm[t][c]) {
                    break;
                }
            }
            incidents[incident_count].time = t;
            incidents[incident_count].type = 1;
            incidents[incident_count].count = problem;
            incidents[incident_count].value = q_alarm[t][c];
            incident_count++;
        }
    }

    printf("Журнал инцидентов (ровно один компонент в тревоге):\n");
    if (incident_count == 0) {
        printf("Инцидентов не обнаружено.\n");
    }
    for (i = 0; i < incident_count; i++) {
        printf("Отсчет %d: инцидент (тип %d, проблемных компонентов: %d, значение Q: %+.2f)\n",
            incidents[i].time, incidents[i].type, incidents[i].count, incidents[i].value);
    }

    return 0;
}