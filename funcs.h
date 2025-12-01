#ifndef FUNCS_H
#define FUNCS_H

void sallen_key_menu(void);

void schmitt_feedback_menu(void);
void schmitt_output_menu(void);

// These functions are used in Schmitt Trigger Menu
float para_res(float R);
float find_r_feedback(float thresholdMargin, float R, float Vsat);
void print_schmitt(float thresholdMargin, float R, float Vsat, float Rfeed);

float get_user_float(void);
int is_float(const char *s);

void print_sallenkey(int type);
void chebyshev_sub(float RB, float fc, float R, int pass_type);
void butterworth_sub(float RB, float fc, float R, int pass_type);
void sallen_key_summarize(float RA, long double C, float K);

static int enter_choice(void);

#endif