#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funcs.h"

#define PI 3.142L

void schmitt_feedback_menu(void) {
    printf("\n--- Schmitt Trigger Component Conqueror ---\n");
    printf("\nLet's Determine Your Feedback Resistor\n");
    printf("Begin By Providing Your Exisiting Values\n\n");
    printf("What is the resistance of your matching resistors?\n\n");
    float R = get_user_float();
    float Rparallel = para_res(R);
    printf("\nWhat is the voltage of your positive supply rail?\n\n");
    float Vsat = get_user_float();
    printf("\nWhat is your desired threshold margin?\n\n");
    float thresholdmarg = get_user_float();
    printf("\n-----+++CALCULATING+++-----\n\n");
    float rfeed = find_r_feedback(thresholdmarg, Rparallel, Vsat);
    // Now We Know All The Require Variables To Calculate The Feedback Resistor
    print_schmitt(thresholdmarg, R, Vsat, rfeed);
}

void schmitt_output_menu(void){
    printf("\n--- Schmitt Trigger Output Previewer ---\n\n");
    printf("What is your nominal threshold?\n");
    float threshold = get_user_float();
    printf("What is your noise margin?\n");
    float margin = get_user_float();
    printf("\nPlease import your time series!\n");
    char filename[256];
    FILE *file = NULL; // initialize a file variable first
    do {
        printf("Enter filename: ");
        // Takes a string input, make sure fgets and allocate!
        if (fgets(filename, sizeof(filename), stdin) != NULL){
            filename[strcspn(filename, "\n")] = '\0'; // remove newline symbol and cut the string there
        }
        // Now we open the file (in turn, checking if the destination exists)
        file = fopen(filename, "r");
        if (!file){ //not a valid file
            printf("Error, file not found! Try again.\n");
        }
    } while(!file); //repeats until a valid file is given

    printf("File Found! Reading...\n\n\n");
    // Create a struct called signal
    typedef struct{
        float inputAmp; //input amplitude
        int outputAmp; //output signal
    } Signal;

    /*
    With everything ready, we can begin reading the content of the file into the array
    */
    float currentInputSignal; // temp reader
    float recentState;
    int signal_count = 0;
    while(fscanf(file, "%f", &currentInputSignal) == 1) {
        if (signal_count == 0){ // For assigning initial Stage
            if (currentInputSignal < threshold){ // Initial State is low
                recentState = 0;
            } else {
                recentState = 1;
            }
        }
         signal_count++; //we need to know how many lines so we could use malloc
    }
    rewind(file); // go back to start

    // Initialize Array
    Signal *inputSignalArray = malloc(signal_count * sizeof(Signal));
    if (inputSignalArray == NULL){ // Handle bad malloc
        printf("Memory Allocation Failed, sorry!\n");
    }

    // Read and calculate output signal
    for (int i = 0; i < signal_count; i++){
        fscanf(file,"%f", &inputSignalArray[i].inputAmp);
        printf("%f\n",inputSignalArray[i].inputAmp);
        inputSignalArray[i].outputAmp = schmitt_output(inputSignalArray[i].inputAmp, threshold, margin, recentState);
        // Update the recent State
        if (inputSignalArray[i].outputAmp == 1){
            recentState = 1;
        } else {
            recentState = 0;
        }
    }
    fclose(file); //finished reading!
    printf("Continues Here");
}

int schmitt_output(float inputAmp, float nomThreshold, float margin, int recentState){
    float newThreshold;
    // Adjust the threshold according to the previous stage
    // If previous stage is low
    if (recentState == 0){
        newThreshold = nomThreshold + (float)margin/2;
    } else { // Previous stage is high
        newThreshold = nomThreshold - (float)margin/2;
    }
    // Calculate the output
    if (inputAmp > newThreshold){ // if input is higher than threshold
        return 1; // high signal
    } else { // if input is lower than threshold
        return 0; // low signal
    }
}
void sallen_key_menu(void) {
    printf("\n--- Sallen-Key Conqueror ---\n");
    printf("\nLet's Build a Sallen Key Filter\n");
    // Ask for signal characteristic
    printf("Select Your Signal Characteristic\n\n");
    printf("1. Chebyshev (Steeper Roll-off, Ripple)\n");
    printf("2. Butterworth (Smooth, Shallower Roll-off)\n");
    int character = enter_choice();
    // Ask for filter's pass type
    printf("Select Your Filter\n\n");
    printf("1. High-pass\n");
    printf("2. Low-pass\n");
    int pass_type = enter_choice();
    print_sallenkey(pass_type);
    printf("\n\n!!! Consider This Topology For Your Passband !!!\n\n");
    // Ask for specifications:
    printf("What is your resistance for RB?\n");
    float RB = get_user_float();
    printf("What is your desired cut-off freuqnecy fc?\n");
    float fc = get_user_float();
    printf("What is your desired resistance for R1 and R2?\n");
    float R = get_user_float();
    switch (character) {
        case 1: // For Chebyshev
            chebyshev_sub(RB,fc,R,pass_type);
            break;
        case 2: // For Butterworth
            butterworth_sub(RB,fc,R,pass_type);
            break;
    }
}
void chebyshev_sub(float RB, float fc, float R, int pass_type){
    // Ask for ripple
    printf("What is your desired ripple?\n");
    printf("1. 0.5 dB\n");
    printf("2. 2 dB\n");
    int ripple_choice = enter_choice();
    // Assign the appropriate gain and the appropriate Chebyshev normalizing factor
    float K;
    float Cn;
    switch (ripple_choice) {
        case 1: // For 0.5 dB
            K = 1.842;
            if (pass_type == 1){ // High pass
                Cn = 0.812;
            } else { // Low pass
                Cn = 1.231;
            }
            break;
        case 2: // For 2 dB
            K = 2.114;
            if (pass_type == 1){ // High pass
                Cn = 1.103;
            } else { // Low pass
                Cn = 0.907;
            }
            break;
    }
    // Calculate RA
    float RA = RB * (K-1);
    // calculate C
    long double C = 1.0L / (2.0L * PI * fc * R * Cn);
    // Summarize and output the findings
    printf("\n\nWith the given specification based on this topology:\n");
    print_sallenkey(pass_type);
    sallen_key_summarize(RA,C,K);
}

void butterworth_sub(float RB, float fc, float R, int pass_type){
    float K = 1.586;
    float RA = RB * (K-1);
    long double C = 1.0L / (2.0L * PI * fc * R);
    printf("\n\nWith the given specification based on this topology:\n");
    print_sallenkey(pass_type);
    sallen_key_summarize(RA,C,K);
}

void sallen_key_summarize(float RA, long double C, float K){
    printf("\nYou would need a gain of K = %.3f\n",K);
    printf("Making your RA = %f Ohms\n", RA);
    printf("Consequently, the capacitance for C1 and C2 would be C = %Le F\n\n", C);
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

    // Ignore the positive symbol at the start
    if (*s == '+') s++;

    // No negative number!
    if (*s == '-') return 0;
    // No zero!
    if (*s == '0' && *(s+1)  != '.') return 0;

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
    char buf[15];
    int valid_input = 0;
    float value = 0.0f;

    do{
        printf("Enter Positive Float Value: ");
        if (!fgets(buf, sizeof(buf), stdin)){
            puts("\nInput Error. Exiting.");
            exit(1);
        }

        buf[strcspn(buf, "\r\n")] = '\0';

        if (!is_float(buf)){
            printf("Invalid :( Try again!\n");
            valid_input = 0;
        } else if (strlen(buf) > 10) {
            printf("Too Long!\n");
            valid_input = 0;
        } else {
            valid_input = 1;
        }
    } while (!valid_input);
    value = strtof(buf, NULL);
    return value;
}

// This function prints the diagram of schmitt trigger with the desired values
void print_schmitt(float thresholdMargin, float R, float Vsat, float Rfeed){
    printf("+Vin ----|=====|\n");
    printf("         | OP  |--+-- Vout\n");
    printf("+Vs      | AMP |  |\n");
    printf(" |    +--|=====|  |\n");
    printf(" R    |           |\n");
    printf(" |----+---Rfeed---+\n");
    printf(" R\n");
    printf(" |\n");
    printf("-Vs\n\n");
    printf("Given Vs = %.2fV, R = %.2f Ohm, with your desired thershold margin of %.2fV\n\n",Vsat,R,thresholdMargin);
    printf("Your Feedback Resistor (Rfeed) Must be: %.2f Ohm\n", Rfeed);
}

void print_sallenkey(int type){
    if (type == 2){ //low pass
        printf("\n=============LOW PASS SALLEN-KEY=============\n\n");
        printf("            +-----C1----------------+\n");
        printf("            |                       |\n");
        printf("+Vin ---R1--+-R2-+--------|=====|   |\n");
        printf("                 |        | OP  |---+--Vout\n");
        printf("                 |        | AMP |   |\n");
        printf("                C2     +--|=====|  RA \n");
        printf("                 |     |            |\n");
        printf("                 =     +------------+\n");
        printf("                                    |\n");
        printf("                                   RB\n");
        printf("                                    |\n");
        printf("                                    =\n");
    } else { // high pass
        printf("\n=============HIGH PASS SALLEN-KEY=============\n\n");
        printf("            +-----R1----------------+\n");
        printf("            |                       |\n");
        printf("+Vin ---C1--+-C2-+--------|=====|   |\n");
        printf("                 |        | OP  |---+--Vout\n");
        printf("                 |        | AMP |   |\n");
        printf("                R2     +--|=====|  RA \n");
        printf("                 |     |            |\n");
        printf("                 =     +------------+\n");
        printf("                                    |\n");
        printf("                                   RB\n");
        printf("                                    |\n");
        printf("                                    =\n");
    }
}
static int enter_choice(void)
{
    enum { MENU_ITEMS = 2};
    char buf[128];
    int valid_input = 0;
    int value = 0;

    do {
        printf("\nSelect item: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            /* EOF or error; bail out gracefully */
            puts("\nInput error. Exiting.");
            exit(1);
        }

        // strip trailing newline
        buf[strcspn(buf, "\r\n")] = '\0';

        if (!is_float(buf)) {
            printf("Enter an integer!\n");
            valid_input = 0;
        } else {
            value = (int)strtol(buf, NULL, 10);
            if (value >= 1 && value <= MENU_ITEMS) {
                valid_input = 1;
            } else {
                printf("Invalid menu item!\n");
                valid_input = 0;
            }
        }
    } while (!valid_input);

    return value;
}