/*
 * File:        board_rental.c
 * Author:      Christopher Romo
 * Created:     2022-09-21
 * Description: This program tracks the number of hours boards are rented
 *              and calculates the cost based on the hours rented.
 */

// libraries
#include <stdio.h>
#include <math.h>

// constants
#define MIN_RANGE 1		   // minimum amount of hours rented
#define MAX_RANGE 10	   // maximum amount of hours rented
#define HOURLY_RATE 8	   // standard hourly rate
#define MIN_CHARGE 50	   // minimum charge
#define MAX_CHARGE 80	   // maximum charge
#define MIN_CHARGE_HOURS 3 // the amount of hours where only the minimum will be charged

// function prototypes
double get_valid_input(void);
double calculate_charge(double hours);
void print_summary(unsigned int boardNum, double totalHours, double totalCharge);

/*
 * Function: main
 * ----------------------------
 *  Entry point of the program.
 *
 *  returns: exit status code
 */
int main(void)
{
	// initialize variables
	unsigned int board_num = 0;
	double hours = 0;
	double charge = 0;
	double total_hours = 0;
	double total_charge = 0;

	// prompt for input and update variables
	do
	{
		printf("%s", "Enter the number of hours the board was rented or enter -1 to quit.\n");
		hours = get_valid_input();
		charge = calculate_charge(hours);

		if (hours > MAX_RANGE || hours < MIN_RANGE && hours != -1)
		{
			puts("Error: The number of hours must be greater than 0 and not more than 10 hours.");
		}
		else if (hours != -1)
		{
			board_num++;
			total_charge += charge;
			total_hours += hours;
			printf("%s	%s	%s\n%d	%.2lf	%.2lf\n", "Board", "Hours", "Charge", board_num, hours, charge);
		}

	} while (hours != -1);

	// display proper exit message
	if (hours == -1)
	{
		if (total_hours == 0)
		{
			puts("Total for the day\nNo boards were rented today.");
		}
		else
		{
			print_summary(board_num, total_hours, total_charge);
		}
	}

	return 0;

} // main

/*
 * Function: get_valid_input
 * ----------------------------
 *  Prompts user for input and validates it.
 *
 *  returns: the valid input
 */
double get_valid_input(void)
{
	// declare variables
	double input;
	unsigned int scanned_var;

	// validate input and clear buffer
	scanned_var = scanf("%lf", &input);
	while ((getchar()) != '\n')
		;

	if (scanned_var == 1)
	{
		return input;
	}

} // get_valid_input

/*
 * Function: calculate_charge
 * ----------------------------
 *  Calculates the charge based on the hours rented.
 *
 *  hours: the number of hours rented
 *
 *  returns: the calculated charge
 */
double calculate_charge(double hours)
{
	// initialize variables
	double round_hours = ceil(hours);
	double charge_hours = 0;
	double charge_rate = 0;
	double charge = 0;

	// calculate charge and return
	if (round_hours <= MIN_CHARGE_HOURS)
	{
		charge = MIN_CHARGE;
		return charge;
	}
	else if (round_hours > MIN_CHARGE_HOURS)
	{
		charge_hours = round_hours - MIN_CHARGE_HOURS;
		charge_rate = charge_hours * HOURLY_RATE;
		charge = charge_rate + MIN_CHARGE;

		if (charge > MAX_CHARGE)
		{
			charge = MAX_CHARGE;
			return charge;
		}
		else if (charge <= MAX_CHARGE)
		{
			return charge;
		}
	}

} // calculate_charge

/*
 * Function: print_summary
 * ----------------------------
 *  Prints the summary of the day's rentals.
 *
 *  board_num: the number of boards rented
 *  total_hours: the total hours rented
 *  total_charge: the total charge for the day
 */
void print_summary(unsigned int board_num, double total_hours, double total_charge)
{
	// print the totals of the day
	printf("Total for the day\n%s	%s	%s\n%d	%.2lf		%.2lf\n", "Board", "Total Hours", "Total Charge", board_num, total_hours, total_charge);

} // print_summary