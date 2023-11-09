#ifndef DOOM_DEPTH_C_HELP_SCREEN_TYPES_H
#define DOOM_DEPTH_C_HELP_SCREEN_TYPES_H

#define NB_TIPS 8

static char * tips[NB_TIPS][2] = {
        {"z", "Go up / previous"},
        {"d", "Go right / next"},
        {"s", "Go down / next"},
        {"q", "Go left / previous"},
        {"i", "Open inventory"},
        {"Q", "Quit"},
        {"Enter", "Select"},
        {"Esc", "Go back"}
};

#endif //DOOM_DEPTH_C_HELP_SCREEN_TYPES_H
