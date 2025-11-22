#ifndef FUNCS_H
#define FUNCS_H

void sallen_key_menu(void);

void schmitt_feedback_menu(void);
void schmitt_output_menu(void);

// These functions are used in Schmitt Trigger Menu
float para_res(float R);
float find_r_feedback(float thresholdMargin, float R, float Vsat);

float get_user_float(void);
int is_float(const char *s);

#endif