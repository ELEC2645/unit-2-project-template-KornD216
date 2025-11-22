#include <stdio.h>
#include "funcs.h"

void schmitt_menu(void) {
    printf("\n--- Schmitt Trigger Conqueror ---\n");
    printf("\nHow can we help you today?\n");
    printf("1. Determine The Feedback Resistor\n");
    printf("2. Determine The Output Signal\n");
    printf("3. I misclicked, take me back home.\n");
    /* you can call a function from here that handles menu 1 */
}

void menu_item_2(void) {
    printf("\n>> Menu 2\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 2 */
}

void menu_item_3(void) {
    printf("\n>> Menu 3\n");
    printf("\nSome code here does something useful\n");
    /* you can call a function from here that handles menu 3 */
}

void menu_item_4(void) {
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
