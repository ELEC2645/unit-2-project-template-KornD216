#ifndef FUNCS_H
#define FUNCS_H

void schmitt_menu(void);
void menu_item_2(void);
void menu_item_3(void);
void menu_item_4(void);

// These functions are used in Schmitt Trigger Menu
float para_res(float R);
float find_r_feedback(float thresholdMargin, float R, float Vsat);

#endif