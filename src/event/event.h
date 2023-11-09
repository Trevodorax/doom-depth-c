#ifndef DOOM_DEPTH_C_EVENT_H
#define DOOM_DEPTH_C_EVENT_H

#include "../game_window/game_window.h"

typedef enum {
    UNKNOWN_EVENT = -1,
    QUIT,
    A_KEY, B_KEY, C_KEY, D_KEY, E_KEY, F_KEY, G_KEY, H_KEY, I_KEY, J_KEY, K_KEY, L_KEY, M_KEY, N_KEY, O_KEY, P_KEY, Q_KEY, R_KEY, S_KEY, T_KEY, U_KEY, V_KEY, W_KEY, X_KEY, Y_KEY, Z_KEY,
    a_KEY, b_KEY, c_KEY, d_KEY, e_KEY, f_KEY, g_KEY, h_KEY, i_KEY, j_KEY, k_KEY, l_KEY, m_KEY, n_KEY, o_KEY, p_KEY, q_KEY, r_KEY, s_KEY, t_KEY, u_KEY, v_KEY, w_KEY, x_KEY, y_KEY, z_KEY,
    SPACE_KEY, LPAREN_KEY, RPAREN_KEY, GT_KEY,
    ENTER_KEY, ESCAPE_KEY,
    BACKSPACE_KEY,
} event_t;

/**
 * @brief retrieves a ui type agnostic event
 *
 * @param ui_type The ui_type currently used
 * @param event The event to assign the value to
 *
 * @return true if there was an event, false otherwise
 */
int get_event(ui_type_t ui_type, event_t * event);

/**
 * @brief Gets a char out of an event
 *
 * @param event The event
 * @return The char equivalent
 */
char event_to_char(event_t event);

#endif //DOOM_DEPTH_C_EVENT_H
