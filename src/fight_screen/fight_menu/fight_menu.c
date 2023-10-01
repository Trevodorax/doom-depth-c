#include "fight_menu.h"
#include <stdlib.h>
#include <stdio.h>
#include "../fight_actions/fight_actions.h"

menu_t* create_menu(int nb_options, const char * title, const char * image_path, fight_action action) {
    menu_t *new_menu = malloc(sizeof(menu_t));
    new_menu->nb_options = nb_options;
    new_menu->options = malloc(sizeof(menu_t *) * nb_options);
    new_menu->title = title;
    new_menu->image_path = image_path;
    new_menu->action = action;
    return new_menu;
}

menu_t* build_nested_menu() {
    menu_t *root_menu = create_menu(2, "Root Menu", "root_image.png", NULL);

    menu_t *submenu1 = create_menu(2, "Submenu1", "submenu1_image.png", NULL);
    submenu1->options[0] = create_menu(0, "Option1", "option1_image.png", action_1);
    submenu1->options[1] = create_menu(0, "Option2", "option2_image.png", action_2);

    menu_t *submenu2 = create_menu(1, "Submenu2", "submenu2_image.png", NULL);
    submenu2->options[0] = create_menu(0, "Option3", "option3_image.png", action_3);

    root_menu->options[0] = submenu1;
    root_menu->options[1] = submenu2;

    return root_menu;
}

void free_menu(menu_t * menu) {
    if (!menu)
        return;

    for (int i = 0; i < menu->nb_options; i++) {
        free_menu(menu->options[i]);
    }

    free(menu->title);
    free(menu->image_path);
    free(menu->options);
    free(menu);
}
