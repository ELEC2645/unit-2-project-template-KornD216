#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funcs.h"

void schmitt_feedback_menu(void) {
    printf("\n--- Schmitt Trigger Component Conqueror ---\n");
    printf("\nLet's Determine Your Feedback Resistor\n");
    printf("Begin By Providing Your Exisiting Values\n");
    printf("What is the resistance of your matching resistors?\n\n");
    float Rparallel = para_res(get_user_float());
    printf("\nWhat is the voltage of your positive supply rail?\n\n");
    float Vsat = get_user_float();
    printf("\nWhat is your desired threshold margin?\n\n");
    float thresholdmarg = get_user_float();
    printf("-----CALCULATING-----\n");
    float rfeed = find_r_feedback(thresholdmarg, Rparallel, Vsat);
    printf("%.2f", Vsat);
}

void sallen_key_menu(void) {
    printf("\n--- Sallen-Key Conqueror ---\n");
    printf("\nLet's Build a Sallen Key Filter\n");
    /* you can call a function from here that handles menu 2 */
}

void schmitt_output_menu(void) {
    printf("\n>> Menu 4\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 4 */
}

// this function determines the parallel resistance of two resistors
float para_res(float R) {
    float parres = (R * R)/(R + R);
    return parres;
}

float find_r_feedback(float thresholdMargin, float R, float Vsat){
    float rfeed = R * ((Vsat - thresholdMargin/2)/(thresholdMargin/2));
    return rfeed;
}

// A modified is_integer function but for float with its own logic
int is_float(const char *s){
    if (!s || !*s) return 0;

    // Ignore the positive/negative symbol at the start
    if (*s == '+' || *s == '-') s++;

    int has_digits = 0;
    int has_point = 0;

    while (*s) {
        if (isdigit((unsigned char)*s)){
            has_digits = 1;
        } else if (*s == '.'){
            if (has_point) return 0; // will return 0 if a decimal point already exist
            has_point = 1; // if not, then we tick the flag
        } else {
            return 0; // if it reaches this stage, then it's probably invalid
        }
        s++;
    }
    return has_digits;
}

// a modified version of get int, but for float
float get_user_float(void){
    char buf[128];
    int valid_input = 0;
    float value = 0.0f;

    do{
        printf("Enter Value: ");
        if (!fgets(buf, sizeof(buf), stdin)){
            puts("\nInput Error. Exiting.");
            exit(1);
        }

        buf[strcspn(buf, "\r\n")] = '0';

        if (!is_float(buf)){
            printf("Enter Float!\n");
            valid_input = 0;
        } else {
            valid_input = 1;
        }
    } while (!valid_input);
    return value;
}
