// Project Iteration 1

/**
@christopherRomo
November 10th, 2022
cromo@uccs.edu

program tracks bike races and customers */

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
#define AMOUNT_JERSEY_SIZES 4
#define AMOUNT_RESPONSES 2
const char* password = "B!k34u";
const char* responses[] = { "(y)es", "(n)o" };
const char* jerseySizes[] = { "(s)mall", "(m)edium", "(l)arge", "(x)tra-large" };

// structs
typedef struct organization
{
	char organizationName[BUFFER_SIZE];
	double raceDistance;
	double raceCost;
	double jerseyCost;
	double charityPercent;
	double totalRiders;
	double totalRaceSales;
	double totalJerseys;
	double totalJerseySales;

} Organization;

// function prototypes
void fgetsRemoveNewLine(char* character);
bool validatePassword();
bool getValidDouble(const char* inputString, double* validDouble, unsigned int min, unsigned int max);
char getValidChar(char* stringArray[]);
void promptData(char* const promptString, double* const doublePtr, unsigned int min, unsigned int max);
void validatePayment(char* const customerName, double personalTotal, double charityPrice);
void adminSetUp(Organization* organizationPtr);
void riderRegistration(Organization* organizationPtr, char* const customerName);
void printSummary(Organization organizationPtr);

int main(void)
{
	// declare organization one
	Organization organizationOne;

	// main function variables
	int compareResult;
	bool isFinished = false; // used to end rider registration while loop
	bool validExit = false; // checks if password is correct to end rider registration

	// validate the admin's password
	puts("\n - Log-In - \n");
	puts("Enter admin pin to set up race information");
	bool isValid = validatePassword();

	if (isValid)
	{
		// admin set-up
		puts("\n - Admin Set-Up Mode - \n");
		puts("Set up the fundraising information for the organization");
		adminSetUp(&organizationOne);

		// rider registration
		puts("\n - Rider Registration Mode - \n");
		while (!isFinished)
		{
			// prompt for name and if "quit" begin termination
			printf("You can register for one of the following races and %.0lf%% will be raised for %s\n\n", organizationOne.charityPercent, organizationOne.organizationName);
			printf("Ride\t\tDistance\tCost\n#1\t\t%.2lf\t\t$%.2lf\n\n", organizationOne.raceDistance, organizationOne.raceCost);
			printf("You can purchase a jersey for $%.2lf\n\n", organizationOne.jerseyCost);
			puts("Enter your first name and last name to register for the ride; enter \"QUIT\" to end");
			char customerName[BUFFER_SIZE];
			fgetsRemoveNewLine(customerName);
			compareResult = _strcmpi(customerName, "quit");

			if (compareResult == 0) // user wants to exit rider registration
			{
				puts("Enter admin pin to exit rider registration");
				validExit = validatePassword();

				if (validExit) // password correct; rider registration terminated
				{
					isFinished = true;
				}
				else // password incorrect; rider registration restarting
				{
					puts("Password incorrect; restarting rider registration");
				}
			}
			else // rider registration
			{
				riderRegistration(&organizationOne, customerName);
				puts("Rider Registered!\n");
			}
		}

		// once rider registration is over, print summary and exit
		printSummary(organizationOne);
		return EXIT_SUCCESS;
	}
	else
	{
		// password incorrect, program terminates
		puts("Exiting admin set-up");
		return EXIT_FAILURE;
	}

} // main

/*
* fgetsRemoveNewLine
* parameters: string to be written to
* purpose: scans in string and removes new line
* return: void; returns nothing
*/

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

} //fgetsRemoveNewLine

/*
* validatePassword
* parameters: no parameters
* purpose: validates a password
* return: returns a boolean based on if the attempts were succesful
*/

bool validatePassword()
{
	// declare variables
	int result;
	unsigned int count = 0;
	bool isValid = false;
	char passwordInput[BUFFER_SIZE];

	// loop 3 times or until password is valid
	do
	{
		fgetsRemoveNewLine(passwordInput);
		result = strcmp(passwordInput, password);

		if (result == 0)
		{
			// breaks loop, password is valid
			isValid = true;
		}
		else
		{
			// displays an error message
			puts("Error: password is incorrect");
			count++;
		}
	} while (count < 3 && !isValid);

	// return the passwords validity
	return isValid;

} // validatePassword

/*
* getValidDouble
* parameters: input string (written to by fgets), double pointer (value we update), and min & max (range checking)
* purpose: grabs a double from the input string and checks if it is a valid double
* return: returns a boolean, basically stating if the double is valid or not
*/

bool getValidDouble(const char* inputString, double* validDouble, unsigned int min, unsigned int max)
{
	// declare variables
	char* end;
	bool isValid = false;
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
		isValid = true;
	}

	// return the validity of the test
	return isValid;

} // getValidDouble

/*
* getValidChar
* parameters: a string array (for comparing) and size of that array (for looping)
* purpose: validates a character by comparing it to the options
* return: returns a character, which will have been validated
*/

char getValidChar(char* charArray[], size_t charSize)
{
	// declare variables
	char charTest;
	bool isValid = false;
	size_t charLength;
	char charString[BUFFER_SIZE];

	// validate the character
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
		for (int i = 0; i < charSize; i++)
		{
			if (charTest == charArray[i][1])
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

} // getValidChar

/*
* promptData
* parameters: a string (for printing), a double pointer (one we are checking), and min & max (range checking)
* purpose: loops until the getValidDouble function returns true, providing us a valid double
* return: void; returns nothing
*/

void promptData(char* const promptString, double* const doublePtr, unsigned int min, unsigned int max)
{
	// declare variables
	bool isValid = false;
	char inputStr[BUFFER_SIZE];

	// loop until good data has been inputted
	do
	{
		printf("%s\n", promptString);
		fgetsRemoveNewLine(inputStr);
		isValid = getValidDouble(inputStr, doublePtr, min, max);

	} while (!isValid);

} // promptData

/*
* validatePayment
* parameters: takes a string (for printing) and two double pointers (for printing)
* purpose: loops until the credit card entered is valid
* return: void; returns nothing
*/

void validatePayment(char* const customerName, double personalTotal, double charityPrice)
{
	// declare variables
	unsigned int hyphenCount = 0;
	char cardInfo[BUFFER_SIZE];
	bool isValid = false;
	size_t cardLength;

	do // loop until correct
	{
		// prompt input and designate hyphens
		printf("Your total cost is $%.2lf, Enter your credit card number (XXXX-####-####)\n", personalTotal);
		fgetsRemoveNewLine(cardInfo);
		cardLength = strlen(cardInfo);

		char firstHyphen = cardInfo[4];
		char secondHyphen = cardInfo[9];
		hyphenCount = 0;

		// iterate through string and find hyphen count 
		for (size_t i = 0; i < cardLength; i++)
		{
			if (cardInfo[i] == '-')				
			{
				hyphenCount++;
			}
		}

		if (cardLength != 14) // catches incorrect length
		{
			puts("Error: card number is incorrect length");
		}
		else if (firstHyphen != '-' || secondHyphen != '-') // catches incorrect hyphen positions
		{
			puts("Error: hyphens are not in correct position");
		}
		else if (hyphenCount > 2)
		{
			puts("Error: too many hyphens");
		}
		else // checks data to see if it is valid
		{
			char* tokenPtr = strtok(cardInfo, "-");
			if (isalpha(*tokenPtr) == 0 || isalpha(*(tokenPtr + 1)) == 0 || isalpha(*(tokenPtr + 2)) == 0 || isalpha(*(tokenPtr + 3)) == 0) // catches incorrect first segment
			{
				puts("Error: correct card format is XXXX-####-#### (First segment must be all letters)");
			}
			else // first segment is correct
			{
				tokenPtr = strtok(NULL, "-");
				if (isdigit(*(tokenPtr)) == 0 || isdigit(*(tokenPtr + 1)) == 0 || isdigit(*(tokenPtr + 2)) == 0 || isdigit(*(tokenPtr + 3)) == 0) // catches incorrect middle segment
				{
					puts("Error: correct card format is XXXX-####-#### (Middle segment must be all numbers)");
				}
				else // middle segment is correct
				{
					tokenPtr = strtok(NULL, "-");
					if (isdigit(*tokenPtr) == 0 || isdigit(*(tokenPtr + 1)) == 0 || isdigit(*(tokenPtr + 2)) == 0 || isdigit(*(tokenPtr + 3)) == 0) // catches incorrect last segment
					{
						puts("Error: correct card format is XXXX-####-#### (Last segment must be all numbers)");
					}
					else // last segment is correct
					{
						// exit payment validation
						printf("Thank you %s for your purchase. $%.2lf of the ticket sales will go to charity.\n", customerName, charityPrice);
						isValid = true;
					}
				}
			}
		}
	} while (!isValid);

} // validatePayment

/*
* adminSetUp
* parameters: a struct passed by reference (so values can be changed)
* purpose: initializes variables for use throughout code
* return: void; returns nothing
*/

void adminSetUp(Organization* organizationPtr)
{
	// set-up organization name
	puts("Enter fundraising organizations name.");
	fgetsRemoveNewLine(organizationPtr->organizationName);

	// set-up ride distance
	promptData("Enter distance in miles for bike course.", &(organizationPtr->raceDistance), DISTANCE_MIN, DISTANCE_MAX);
	printf("The bike race distance is %.2lf miles\n", organizationPtr->raceDistance);

	// set-up ride cost
	char racePrompt[BUFFER_SIZE] = "Enter the registration cost of the bike ride for ";
	strcat(racePrompt, organizationPtr->organizationName);
	promptData(racePrompt, &(organizationPtr->raceCost), RIDE_COST_MIN, RIDE_COST_MAX);
	printf("The bike race cost is $%.2lf\n", organizationPtr->raceCost);

	// set-up jersey cost
	char jerseyPrompt[BUFFER_SIZE] = "Enter sales price of jersey for ";
	strcat(jerseyPrompt, organizationPtr->organizationName);
	promptData(jerseyPrompt, &(organizationPtr->jerseyCost), JERSEY_COST_MIN, JERSEY_COST_MAX);
	printf("The bike jersey cost is $%.2lf\n", organizationPtr->jerseyCost);

	// set-up charity percentage
	char charityPrompt[BUFFER_SIZE] = "Enter percentage of the bike race sales that will be donated to ";
	strcat(charityPrompt, organizationPtr->organizationName);
	promptData(charityPrompt, &(organizationPtr->charityPercent), CHARITY_PERCENT_MIN, CHARITY_PERCENT_MAX);
	printf("You entered %.0lf%% of the bike race to go to charity.\n", organizationPtr->charityPercent);

	// initialize totals
	organizationPtr->totalRiders = 0;
	organizationPtr->totalRaceSales = 0;
	organizationPtr->totalJerseys = 0;
	organizationPtr->totalJerseySales = 0;

} // adminSetUp

/*
* riderRegistration
* parameters: a struct passed by reference (so values can be changed) and string (for printing)
* purpose: prompts questions for one rider and updates total variables
* return: void; returns nothing
*/

void riderRegistration(Organization* organizationPtr, char* const customerName)
{
	// personal rider variables
	unsigned int jersey = 0;
	double personalJersey = 0;
	double personalTotal = 0;
	double charityPrice = 0;

	// placeholder variables
	char chosenSize;
	char chosenResponse;
	char chosenReciept;

	// prompt for jersey and record results
	printf("Do you want to purchase a jersey for $%.2lf? Answer with (y)es or (n)o\n", organizationPtr->jerseyCost);
	chosenResponse = getValidChar(responses, AMOUNT_RESPONSES);

	if (chosenResponse == 'y') // if they want a jersey
	{
		puts("Enter your size (s)mall, (m)edium, (l)arge, or (x)large");
		chosenSize = getValidChar(jerseySizes, AMOUNT_JERSEY_SIZES);
		personalJersey = organizationPtr->jerseyCost;
		jersey++;
	}

	// computate variables and update organization pointers
	personalTotal = organizationPtr->raceCost + personalJersey;
	charityPrice = personalTotal * (organizationPtr->charityPercent * 0.01);
	organizationPtr->totalRiders = organizationPtr->totalRiders + 1;
	organizationPtr->totalJerseys = organizationPtr->totalJerseys + jersey;
	organizationPtr->totalRaceSales = organizationPtr->totalRaceSales + organizationPtr->raceCost;
	organizationPtr->totalJerseySales = organizationPtr->totalJerseySales + personalJersey;

	// prompt for payment
	validatePayment(customerName, personalTotal, charityPrice);

	// prompt for reciept
	puts("Do you want a receipt (y)es or (n)o?");
	chosenReciept = getValidChar(responses, AMOUNT_RESPONSES);

	if (chosenReciept == 'y') // if they want a reciept
	{
		printf("\nRace :\t\t\t$%.2lf\nJersey :\t\t$%.2lf\n\nTotal Cost :\t\t$%.2lf\nDonation to Charity :\t$%.2lf\n\n", organizationPtr->raceCost, personalJersey, personalTotal, charityPrice);
	}

} // riderRegistration

/*
* printSummary
* parameters: a struct passed by value
* purpose: prints all values
* return: void; returns nothing
*/

void printSummary(Organization organizationPtr)
{
	// declare variables & computate totals
	double charityRace = (organizationPtr.totalRaceSales * (organizationPtr.charityPercent * 0.01)) * organizationPtr.totalRiders;
	double charityShirts = (organizationPtr.totalJerseySales * (organizationPtr.charityPercent * 0.01)) * organizationPtr.totalJerseys;
	double grandTotal = organizationPtr.totalRaceSales + organizationPtr.totalJerseySales;
	double totalCharity = charityRace + charityShirts;

	// print summary of race
	printf("\nRace\tDistance\tPrice\t\tRegistrants\tTotal Sales\tCharity Amount\n#1\t%.2lf\t\t$%.2lf\t\t%.0lf\t\t$%.2lf\t\t$%.2lf", organizationPtr.raceDistance, organizationPtr.raceCost, organizationPtr.totalRiders, organizationPtr.totalRaceSales, charityRace);
	printf("\n\nShirts\tSales\t\tCharity Amount\n%.0lf\t$%.2lf\t\t$%.2lf\n", organizationPtr.totalJerseys, organizationPtr.totalJerseySales, charityShirts);
	printf("\nTotal Sales :\t\t\t\t$%.2lf\nTotal funds raised for charity :\t$%.2lf\n", grandTotal, totalCharity);

} // printSummary