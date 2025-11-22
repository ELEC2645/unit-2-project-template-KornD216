#include <stdio.h>
#include "funcs.h"

void schmitt_feedback_menu(void) {
    printf("\n--- Schmitt Trigger Component Conqueror ---\n");
    printf("\nLet's Determine Your Feedback Resistor\n");
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
