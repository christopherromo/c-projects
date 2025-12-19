// Homework 5

/**
@christopherRomo
October 29th, 2022
cromo@uccs.edu

program checks password and prompts admin set-up */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>


// constants
#define BUFFER_SIZE 80
#define DISTANCE_MIN 1
#define DISTANCE_MAX 200
#define RIDE_COST_MIN 50
#define RIDE_COST_MAX 250
#define JERSEY_COST_MIN 50
#define JERSEY_COST_MAX 150
#define CHARITY_PERCENT_MIN 5
#define CHARITY_PERCENT_MAX 30
#define JERSEY_SIZES 4
const char* jerseySizes[] = { "(s)mall", "(m)edium", "(l)arge", "(x)tra-large" };

// function prototypes
void fgetsRemoveNewLine(char* character);
void adminSetUp(char* organizationName, double* distance, double* cost);
bool getValidDouble(const char* inputString, double* validDouble, int min, int max);
char getValidChar(char* stringArray[]);

int main(void)
{
	// declare and initialize variables
	char organizationName[BUFFER_SIZE];
	double raceDistance = 0;
	double raceCost = 0;

	// call admin set up
	adminSetUp(organizationName, &raceDistance, &raceCost);

	// display variables after function
	printf("In Main: organizationName is %s\n", organizationName);
	printf("In Main: raceDistance is %.2lf\n", raceDistance);
	printf("In Main: raceCost is %.2lf\n", raceCost);

	// get a valid char to select shirt size
	puts("Select your shirt size by entering the character in parenthesis : (s)mall, (m)edium, (l)arge, (x)tra-large.");
	char test = getValidChar(jerseySizes);
	printf("In Main: validChar is %c", test);
}

void fgetsRemoveNewLine(char* inputString)
{
	// read in string and get string length
	fgets(inputString, BUFFER_SIZE, stdin);
	size_t stringLength = strlen(inputString);

	// replace newline character if there is one
	if (inputString[stringLength - 1] == '\n')
	{
		inputString[stringLength - 1] = '\0';
	}
}

void adminSetUp(char* organizationName, double* distance, double* cost)
{
	// declare variables
	char distanceString[BUFFER_SIZE];
	char costString[BUFFER_SIZE];
	bool isValid = false;

	// set up organization name
	puts("Enter fundraising organizations name.");
	fgetsRemoveNewLine(organizationName);
	
	// set-up ride distance
	do
	{
		puts("Enter distance in miles for bike course.");
		fgetsRemoveNewLine(distanceString);
		isValid = getValidDouble(distanceString, distance, DISTANCE_MIN, DISTANCE_MAX);

	} while (!isValid);

	// reset boolean flag
	isValid = false;

	// set-up ride cost
	do
	{
		printf("Enter the registration cost of the bike ride for %s\n", organizationName);
		fgetsRemoveNewLine(costString);
		isValid = getValidDouble(costString, cost, RIDE_COST_MIN, RIDE_COST_MAX);

	} while (!isValid);
}

bool getValidDouble(const char* inputString, double* validDouble, int min, int max)
{
	// declare variables
	char* end;
	double doubleTest = strtod(inputString, &end);

	if (end == inputString) // catches inputs that aren't numbers
	{
		printf("%s\n", "Error: not a decimal number");
	}
	else if ('\0' != *end) // catches characters at the end of input
	{
		printf("%s\n", "Error: extra characters at the end of input");
	}
	else if (doubleTest < min || doubleTest > max) // range check
	{
		printf("Error: Enter a value between %d and %d\n", min, max);
	}
	else // update pointer and return true since number is valid
	{
		*validDouble = (double)doubleTest;
		return true;
	}

	// return false since number is not valid
	return false;
}

char getValidChar(char* stringArray[])
{
	// declare variables
	char charString[BUFFER_SIZE];
	char charTest;
	size_t charLength;
	bool isValid = false;

	// validate char
	do
	{
		do 
		{
			// scan in and check string length to ensure one character
			fgetsRemoveNewLine(charString);
			charLength = strlen(charString);
			charString[0] = tolower(charString[0]);
			charTest = charString[0];

			// display error if longer
			if (charLength != 1)
			{
				puts("Error: Input should be one character");
			}

		} while (charLength != 1);

		// compare to array of pointers
		for (int i = 0; i < JERSEY_SIZES; i++)
		{
			if (charTest == stringArray[i][1])
			{
				isValid = true;
			}
		}

		// display error if no matches
		if (!isValid)
		{
			puts("Error: Not a valid choice");
		}

	} while (!isValid);

	return charTest;
}