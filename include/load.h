#ifndef LOAD_H
#define LOAD_H

typedef struct {
    float one_min;
    float five_min;
    float fifteen_min;
} LoadAvg;

void read_load_avg(LoadAvg* load);
void format_load_avg(char* buffer, int buffer_size, const LoadAvg* load);

#endif
