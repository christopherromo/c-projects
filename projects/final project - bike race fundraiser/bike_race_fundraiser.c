/*
 * File:        bike_race_fundraiser.c
 * Author:      Christopher Romo
 * Created:     2022-12-02
 * Description: This program simulates a bike race fundraiser, allowing an admin
 *  			to set up organizations and riders to register for the race.
 */

// libraries
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

// constants
#define BUFFER_SIZE 80															// buffer size for fgets
#define DISTANCE_MIN 1															// minimum distance of race
#define DISTANCE_MAX 200														// maximum distance of race
#define RIDE_COST_MIN 50														// minimum ride cost
#define RIDE_COST_MAX 250														// maximum ride cost
#define JERSEY_COST_MIN 50														// minimum jersey cost
#define JERSEY_COST_MAX 150														// maximum jersey cost
#define CHARITY_PERCENT_MIN 5													// minimum charity percentage
#define CHARITY_PERCENT_MAX 30													// maximum charity percentage
#define AMOUNT_JERSEY_SIZES 4													// amount of jersey sizes
#define AMOUNT_RESPONSES 2														// amount of responses
const char *password = "B!k34u";												// admin password
const char *responses[] = {"(y)es", "(n)o"};									// yes/no responses
const char *jerseySizes[] = {"(s)mall", "(m)edium", "(l)arge", "(x)tra-large"}; // jersey sizes

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

typedef struct node
{
	Organization orgData;
	struct node *nextPtr;

} Node;

// function prototypes

// tasks & validation
void fgetsRemoveNewLine(char *inputString);
char getValidChar(char *charArray[], size_t charSize);
bool getValidDouble(const char *inputString, double *validDouble, unsigned int min, unsigned int max);
bool validatePassword(void);
void validatePayment(char *const customerName, double personalTotal, double charityPrice);
void promptData(char *const promptString, double *const doublePtr, unsigned int min, unsigned int max);

// linked list functions
void insertNode(Node **headPtr, Organization orgToInsert);
void printOrganizations(Node *headPtr);
unsigned int findOrganizationIndex(Node *headPtr);

// program functions
void adminSetUp(Node *headPtr);
void riderRegistration(Node *headPtr, unsigned int organizationIndex, char *const customerName);
void printSummary(Node *headPtr);

int main(void)
{
	// declare linked list
	Node *headPtr = NULL;

	// main function variables
	int compareResult;
	unsigned int organizationIndex;
	char customerName[BUFFER_SIZE];
	bool isFinished = false; // used to end rider registration while loop
	bool validExit = false;	 // checks if password is correct to end rider registration

	// validate the admin's password
	puts("\n - Log-In - \n");
	puts("Enter admin pin to set up race information:");
	bool isValid = validatePassword();

	if (isValid)
	{
		// admin set-up
		puts("\n - Admin Set-Up Mode - \n");
		adminSetUp(&headPtr);

		// rider registration
		puts("\n - Rider Registration Mode - \n");
		while (!isFinished)
		{
			// list organizations and find index
			organizationIndex = findOrganizationIndex(headPtr);

			// prompt for name and if "quit" begin termination
			puts("Enter your first name and last name to register for the ride; enter \"QUIT\" to end:");
			fgetsRemoveNewLine(customerName);
			compareResult = _strcmpi(customerName, "quit");

			if (compareResult == 0) // user wants to exit rider registration
			{
				puts("Enter admin pin to exit rider registration:");
				validExit = validatePassword();

				if (validExit) // password correct; rider registration terminated
				{
					isFinished = true;
				}
				else // password incorrect; rider registration restarting
				{
					puts("Password incorrect; restarting rider registration.\n");
				}
			}
			else // rider registration
			{
				riderRegistration(headPtr, organizationIndex, customerName);
				puts("Rider Registered!\n");
			}
		}

		// once rider registration is over, print summary and exit
		printSummary(headPtr);
		return EXIT_SUCCESS;
	}
	else
	{
		// password incorrect, program terminates
		puts("Exiting admin set-up.");
		return EXIT_FAILURE;
	}

} // main

// TASKS & VALIDATION

/*
 * fgetsRemoveNewLine
 * parameters: string to be written to
 * purpose: scans in string and removes new line
 * return: void; returns nothing
 */

void fgetsRemoveNewLine(char *inputString)
{
	// read in string and get string length
	fgets(inputString, BUFFER_SIZE, stdin);
	size_t stringLength = strlen(inputString);

	// replace newline character if there is one
	if (inputString[stringLength - 1] == '\n')
	{
		inputString[stringLength - 1] = '\0';
	}

} // fgetsRemoveNewLine

/*
 * getValidChar
 * parameters: a string array (for comparing) and size of that array (for looping)
 * purpose: validates a character by comparing it to the options
 * return: returns a character, which will have been validated
 */

char getValidChar(char *charArray[], size_t charSize)
{
	// declare variables
	char charTest;
	size_t charLength;
	char charString[BUFFER_SIZE];
	bool isValid = false;

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
				puts("Error: Input should be one character!");
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
			puts("Error: Not a valid choice!");
		}

	} while (!isValid);

	return charTest;

} // getValidChar

/*
 * getValidDouble
 * parameters: input string (written to by fgets), double pointer (value we update), and min & max (range checking)
 * purpose: grabs a double from the input string and checks if it is a valid double
 * return: returns a boolean, basically stating if the double is valid or not
 */

bool getValidDouble(const char *inputString, double *validDouble, unsigned int min, unsigned int max)
{
	// declare variables
	char *end;
	bool isValid = false;
	double doubleTest = strtod(inputString, &end);

	if (end == inputString) // catches inputs that aren't numbers
	{
		printf("%s\n", "Error: not a decimal number!");
	}
	else if ('\0' != *end) // catches characters at the end of input
	{
		printf("%s\n", "Error: extra characters at the end of input!");
	}
	else if (doubleTest < min || doubleTest > max) // range check
	{
		printf("Error: Enter a value between %d and %d!\n", min, max);
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
 * validatePassword
 * parameters: no parameters
 * purpose: validates a password
 * return: returns a boolean based on if the attempts were succesful
 */

bool validatePassword(void)
{
	// declare variables
	int compareResult;
	unsigned int count = 0;
	char passwordInput[BUFFER_SIZE];
	bool isValid = false;

	// loop 3 times or until password is valid
	do
	{
		fgetsRemoveNewLine(passwordInput);
		compareResult = strcmp(passwordInput, password);

		if (compareResult == 0)
		{
			// breaks loop, password is valid
			isValid = true;
		}
		else
		{
			// displays an error message & increments count
			puts("Error: password is incorrect!");
			count++;
		}
	} while (count < 3 && !isValid);

	// return the passwords validity
	return isValid;

} // validatePassword

/*
 * validatePayment
 * parameters: takes a string (for printing) and two double pointers (for printing)
 * purpose: loops until the credit card entered is valid
 * return: void; returns nothing
 */

void validatePayment(char *const customerName, double personalTotal, double charityPrice)
{
	// declare variables
	unsigned int hyphenCount = 0;
	size_t cardLength;
	char cardInfo[BUFFER_SIZE];
	bool isValid = false;

	do // loop until correct
	{
		// prompt input and designate hyphens
		printf("Your total cost is $%.2lf, Enter your credit card number: (XXXX-####-#####)\n", personalTotal);
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

		if (cardLength != 15) // catches incorrect length
		{
			puts("Error: card number is incorrect length!");
		}
		else if (firstHyphen != '-' || secondHyphen != '-') // catches incorrect hyphen positions
		{
			puts("Error: hyphens are not in correct position!");
		}
		else if (hyphenCount > 2)
		{
			puts("Error: too many hyphens!");
		}
		else // checks data to see if it is valid
		{
			char *tokenPtr = strtok(cardInfo, "-");
			if (isalpha(*tokenPtr) == 0 || isalpha(*(tokenPtr + 1)) == 0 || isalpha(*(tokenPtr + 2)) == 0 || isalpha(*(tokenPtr + 3)) == 0) // catches incorrect first segment
			{
				puts("Error: correct card format is XXXX-####-#####! (First segment must be all letters)");
			}
			else // first segment is correct
			{
				tokenPtr = strtok(NULL, "-");
				if (isdigit(*(tokenPtr)) == 0 || isdigit(*(tokenPtr + 1)) == 0 || isdigit(*(tokenPtr + 2)) == 0 || isdigit(*(tokenPtr + 3)) == 0) // catches incorrect middle segment
				{
					puts("Error: correct card format is XXXX-####-#####! (Middle segment must be all numbers)");
				}
				else // middle segment is correct
				{
					tokenPtr = strtok(NULL, "-");
					if (isdigit(*tokenPtr) == 0 || isdigit(*(tokenPtr + 1)) == 0 || isdigit(*(tokenPtr + 2)) == 0 || isdigit(*(tokenPtr + 3)) == 0 || isdigit(*(tokenPtr + 4)) == 0) // catches incorrect last segment
					{
						puts("Error: correct card format is XXXX-####-#####! (Last segment must be all numbers)");
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
 * promptData
 * parameters: a string (for printing), a double pointer (one we are checking), and min & max (range checking)
 * purpose: loops until the getValidDouble function returns true, providing us a valid double
 * return: void; returns nothing
 */

void promptData(char *const promptString, double *const doublePtr, unsigned int min, unsigned int max)
{
	// declare variables
	char inputStr[BUFFER_SIZE];
	bool isValid = false;

	// loop until good data has been inputted
	do
	{
		printf("%s\n", promptString);
		fgetsRemoveNewLine(inputStr);
		isValid = getValidDouble(inputStr, doublePtr, min, max);

	} while (!isValid);

} // promptData

// LINKED LIST FUNCTIONS

/*
 * insertNode
 * parameters: a double pointer to the head of linked list & organization to insert
 * purpose: inserts a node alphabetically into the organization linked list
 * return: void; returns nothing (adds organization to the linked list)
 */

void insertNode(Node **headPtr, Organization orgToInsert)
{
	// allocate memory
	Node *newOrgPtr = malloc(sizeof(Node));

	// declare boolean flags
	bool found = false;
	bool placed = false;

	if (newOrgPtr != NULL)
	{
		// assign data and next
		newOrgPtr->orgData = orgToInsert;
		newOrgPtr->nextPtr = NULL;

		// set previous pointer to null & current to head
		Node *previousPtr = NULL;
		Node *currentPtr = *headPtr;

		// find insertion point
		while (currentPtr != NULL && !found)
		{
			int compare = strcmp(newOrgPtr->orgData.organizationName, currentPtr->orgData.organizationName);

			if (compare <= 0) // if the incoming organization name is above the current
			{
				if (previousPtr == NULL) // if the insertion point is at the beginning
				{
					newOrgPtr->nextPtr = *headPtr;
					*headPtr = newOrgPtr;
				}
				else // if the insertion point is in the middle
				{
					currentPtr = previousPtr->nextPtr;
					previousPtr->nextPtr = newOrgPtr;
					newOrgPtr->nextPtr = currentPtr;
				}

				// insertion point found, end while loop
				placed = true;
				found = true;
			}
			else // moves the list
			{
				// the previous pointer now points to current
				previousPtr = currentPtr;

				// the current pointer now points to the next pet
				currentPtr = currentPtr->nextPtr;
			}
		}

		if (*headPtr == NULL) // if list is empty
		{
			*headPtr = newOrgPtr;
			placed = true;
		}

		if (!placed) // if entry is at the end of list
		{
			previousPtr = NULL;
			currentPtr = *headPtr;

			while (currentPtr->nextPtr != NULL)
			{
				previousPtr = currentPtr;
				currentPtr = currentPtr->nextPtr;
			}

			currentPtr->nextPtr = newOrgPtr;
			placed = true;
		}
	}
	else // no memory allocated for node
	{
		printf("Error: No memory to create node for %s\n!", orgToInsert.organizationName);
	}

} // insertNode

/*
 * printOrganizations
 * parameters: a pointer to the head of linked list
 * purpose: iterates through the linked list & prints
 * return: void; returns nothing
 */

void printOrganizations(Node *headPtr)
{
	if (headPtr != NULL) // list is not empty
	{
		// initialize current & print header
		Node *currentPtr = headPtr;
		printf("%s", "Organization\t\tDistance\tCost\t\tPercentage\n");

		while (currentPtr != NULL)
		{
			// display and go to next node
			printf("%s\t\t%.2lf\t\t$%.2lf\t\t%.0lf%%\n", currentPtr->orgData.organizationName, currentPtr->orgData.raceDistance, currentPtr->orgData.raceCost, currentPtr->orgData.charityPercent);
			currentPtr = currentPtr->nextPtr;
		}

		printf("%s", "\n");
	}
	else // list is empty
	{
		puts("Error: List is empty!");
	}

} // printOrganizations

/*
 * findOrganizationIndex
 * parameters: a pointer to the head of linked list
 * purpose: returns index of organization entered
 * return: an unsigned int; index of chosen organization
 */

unsigned int findOrganizationIndex(Node *headPtr)
{
	// declare variables
	int compareResult = 0;
	unsigned int index = 0;
	char inputStr[BUFFER_SIZE];
	char organizationName[BUFFER_SIZE];
	Node *currentPtr = headPtr;
	bool isValid = false;

	// display header
	puts("You can register for one of the following bike races and a percentage will be raised for that organization.\n");
	printOrganizations(headPtr);

	do
	{
		// reset index & linked list
		index = 0;
		currentPtr = headPtr;

		// prompt for organization name
		puts("Enter the name of the organization you want to register:");
		fgetsRemoveNewLine(inputStr);

		// iterate through linked list to find organization name
		while (currentPtr != NULL && !isValid)
		{
			// reset result
			compareResult = 0;

			// compare input string and organization name
			strcpy(organizationName, currentPtr->orgData.organizationName);
			compareResult = _strcmpi(inputStr, organizationName);

			if (compareResult == 0) // break loop if found
			{
				isValid = true;
			}
			else // increment index
			{
				index++;
			}

			// move linked list
			currentPtr = currentPtr->nextPtr;
		}

		// display error message if it isn't valid
		if (!isValid)
		{
			puts("Error: that organization name was not found in the list of organizations, try again!\n");
		}

	} while (!isValid);

	printf("You have chosen %s.\n\n", organizationName);

	// return index
	return index;

} // findOrganizationIndex

// PROGRAM FUNCTIONS

/*
 * adminSetUp
 * parameters: a struct passed by reference (so values can be changed)
 * purpose: initializes variables for use throughout code
 * return: void; returns nothing
 */

void adminSetUp(Node *headPtr)
{
	char response;
	unsigned int orgNum = 0;

	puts("Set up the fundraising information for the organizations.");

	do
	{
		// prompt response
		puts("\nDo you want to add an organization? Please enter (y)es or (n)o: ");
		response = getValidChar(responses, AMOUNT_RESPONSES);

		if (response == 'y')
		{
			// declare organization
			Organization orgToInsert;

			// set-up organization name
			puts("Enter fundraising organizations name:");
			fgetsRemoveNewLine(orgToInsert.organizationName);

			// set-up ride distance
			promptData("Enter distance in miles for bike course:", &(orgToInsert.raceDistance), DISTANCE_MIN, DISTANCE_MAX);
			printf("The bike race distance is %.2lf miles\n", orgToInsert.raceDistance);

			// set-up ride cost
			char racePrompt[BUFFER_SIZE] = "Enter the registration cost of the bike ride for ";
			strcat(racePrompt, orgToInsert.organizationName);
			promptData(racePrompt, &(orgToInsert.raceCost), RIDE_COST_MIN, RIDE_COST_MAX);
			printf("The bike race cost is $%.2lf\n", orgToInsert.raceCost);

			// set-up jersey cost
			char jerseyPrompt[BUFFER_SIZE] = "Enter sales price of jersey for ";
			strcat(jerseyPrompt, orgToInsert.organizationName);
			promptData(jerseyPrompt, &(orgToInsert.jerseyCost), JERSEY_COST_MIN, JERSEY_COST_MAX);
			printf("The bike jersey cost is $%.2lf\n", orgToInsert.jerseyCost);

			// set-up charity percentage
			char charityPrompt[BUFFER_SIZE] = "Enter percentage of the bike race sales that will be donated to ";
			strcat(charityPrompt, orgToInsert.organizationName);
			promptData(charityPrompt, &(orgToInsert.charityPercent), CHARITY_PERCENT_MIN, CHARITY_PERCENT_MAX);
			printf("You entered %.0lf%% of the bike race to go to charity.\n", orgToInsert.charityPercent);

			// initialize totals
			orgToInsert.totalRiders = 0;
			orgToInsert.totalRaceSales = 0;
			orgToInsert.totalJerseys = 0;
			orgToInsert.totalJerseySales = 0;

			// insert organization into linked list
			insertNode(headPtr, orgToInsert);

			// increment number of organizations
			orgNum++;
		}

		if (orgNum == 0)
		{
			puts("Error: you must enter at least one organization!");
		}

	} while (response != 'n' || orgNum == 0);

} // adminSetUp

/*
 * riderRegistration
 * parameters: a struct passed by reference (so values can be changed) and string (for printing)
 * purpose: prompts questions for one rider and updates total variables
 * return: void; returns nothing
 */

void riderRegistration(Node *headPtr, unsigned int organizationIndex, char *const customerName)
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

	// position current to right index
	Node *currentPtr = headPtr;

	if (organizationIndex != 0)
	{
		for (unsigned int i = 0; i < organizationIndex; i++)
		{
			currentPtr = currentPtr->nextPtr;
		}
	}

	// prompt for jersey and record results
	printf("Do you want to purchase a jersey for $%.2lf? Answer with (y)es or (n)o:\n", currentPtr->orgData.jerseyCost);
	chosenResponse = getValidChar(responses, AMOUNT_RESPONSES);

	if (chosenResponse == 'y') // if they want a jersey
	{
		puts("Enter your size (s)mall, (m)edium, (l)arge, or (x)large:");
		chosenSize = getValidChar(jerseySizes, AMOUNT_JERSEY_SIZES);
		personalJersey = currentPtr->orgData.jerseyCost;
		jersey++;
	}

	// computate variables and update organization pointers
	personalTotal = currentPtr->orgData.raceCost + personalJersey;
	charityPrice = personalTotal * (currentPtr->orgData.charityPercent * 0.01);
	currentPtr->orgData.totalRiders = currentPtr->orgData.totalRiders + 1;
	currentPtr->orgData.totalJerseys = currentPtr->orgData.totalJerseys + jersey;
	currentPtr->orgData.totalRaceSales = currentPtr->orgData.totalRaceSales + currentPtr->orgData.raceCost;
	currentPtr->orgData.totalJerseySales = currentPtr->orgData.totalJerseySales + personalJersey;

	// prompt for payment
	validatePayment(customerName, personalTotal, charityPrice);

	// prompt for reciept
	puts("Do you want a receipt (y)es or (n)o?");
	chosenReciept = getValidChar(responses, AMOUNT_RESPONSES);

	if (chosenReciept == 'y') // if they want a reciept
	{
		printf("\nRace :\t\t\t$%.2lf\nJersey :\t\t$%.2lf\n\nTotal Cost :\t\t$%.2lf\nDonation to Charity :\t$%.2lf\n\n", currentPtr->orgData.raceCost, personalJersey, personalTotal, charityPrice);
	}

} // riderRegistration

/*
 * printSummary
 * parameters: a struct passed by value
 * purpose: prints all values to file & standard out
 * return: void; returns nothing
 */

void printSummary(Node *headPtr)
{
	// declare variables
	double charityRace;
	double charityShirts;
	double grandTotal;
	double totalCharity;
	Node *currentPtr = headPtr;
	FILE *filePtr;

	// check if file is null
	if ((filePtr = fopen("orgList.txt", "w")) == NULL)
	{
		puts("Error: Couldn't open file for writing");
	}
	else // file open was a success
	{
		while (currentPtr != NULL)
		{
			// declare variables & computate totals
			charityRace = (currentPtr->orgData.raceCost * (currentPtr->orgData.charityPercent * 0.01)) * currentPtr->orgData.totalRiders;
			charityShirts = (currentPtr->orgData.totalJerseySales * (currentPtr->orgData.charityPercent * 0.01)) * currentPtr->orgData.totalJerseys;
			grandTotal = currentPtr->orgData.totalRaceSales + currentPtr->orgData.totalJerseySales;
			totalCharity = charityRace + charityShirts;

			// print summary of race to standard in
			printf("\nRace\t\tDistance\tPrice\t\tRegistrants\tTotal Sales\tCharity Amount\n%-15s\t%.2lf\t\t$%.2lf\t\t%.0lf\t\t$%.2lf\t\t$%.2lf", currentPtr->orgData.organizationName, currentPtr->orgData.raceDistance, currentPtr->orgData.raceCost, currentPtr->orgData.totalRiders, currentPtr->orgData.totalRaceSales, charityRace);
			printf("\n\nShirts\t\tSales\t\tCharity Amount\n%.0lf\t\t$%.2lf\t\t$%.2lf\n", currentPtr->orgData.totalJerseys, currentPtr->orgData.totalJerseySales, charityShirts);
			printf("\nTotal Sales :\t\t\t\t$%.2lf\nTotal funds raised for charity :\t$%.2lf\n\n", grandTotal, totalCharity);

			// print summary of race to file
			fprintf(filePtr, "\nRace\t\tDistance\tPrice\t\tRegistrants\tTotal Sales\tCharity Amount\n%-15s\t%.2lf\t\t$%.2lf\t\t%.0lf\t\t$%.2lf\t\t$%.2lf", currentPtr->orgData.organizationName, currentPtr->orgData.raceDistance, currentPtr->orgData.raceCost, currentPtr->orgData.totalRiders, currentPtr->orgData.totalRaceSales, charityRace);
			fprintf(filePtr, "\n\nShirts\t\tSales\t\tCharity Amount\n%.0lf\t\t$%.2lf\t\t$%.2lf\n", currentPtr->orgData.totalJerseys, currentPtr->orgData.totalJerseySales, charityShirts);
			fprintf(filePtr, "\nTotal Sales :\t\t\t\t$%.2lf\nTotal funds raised for charity :\t$%.2lf\n\n", grandTotal, totalCharity);

			// move linked list
			currentPtr = currentPtr->nextPtr;
		}
	}

	// close file
	fclose(filePtr);

} // printSummary