#pragma once

#ifndef HISTORY_H
#define HISTORY_H

typedef struct History_list History;

struct History_list {
    char *node;
    History *next;
};

History *update_history();

#endif
