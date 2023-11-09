#ifndef DOOM_DEPTH_C_LEVEL_UP_SCREEN_TYPES_H
#define DOOM_DEPTH_C_LEVEL_UP_SCREEN_TYPES_H

typedef enum {
    STAT_HP = 0,
    STAT_MANA,
    STAT_DEFENCE,
    STAT_ATTACK,
    STAT_ACTION_POINTS,
    STAT_COUNT
} stat_t;

static char * stat_display_name[STAT_COUNT] = {
        "Max health",
        "Max mana",
        "Defence",
        "Attack",
        "Action points"
};

static char * stat_display_image_path[STAT_COUNT] = {
        "../assets/stat_icons/image/hp.png",
        "../assets/stat_icons/image/mana.png",
        "../assets/stat_icons/image/defence.png",
        "../assets/stat_icons/image/attack.png",
        "../assets/stat_icons/image/action_points.png"
};

static char * stat_display_ascii_path[STAT_COUNT] = {
        "../assets/stat_icons/ascii/hp.asciiart",
        "../assets/stat_icons/ascii/mana.asciiart",
        "../assets/stat_icons/ascii/defence.asciiart",
        "../assets/stat_icons/ascii/attack.asciiart",
        "../assets/stat_icons/ascii/action_points.asciiart"
};

#endif //DOOM_DEPTH_C_LEVEL_UP_SCREEN_TYPES_H
