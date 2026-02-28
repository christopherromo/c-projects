/*
 * file:        bike_race_fundraiser.c
 * author:      christopher romo
 * created:     2022-12-02
 * description: this program simulates a bike race fundraiser, allowing an admin
 *  			to set up organizations and riders to register for the race.
 */

// libraries
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#if defined(_WIN32)
#include <string.h>
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif

// constants
#define BUFFER_SIZE 80															 // buffer size for fgets
#define DISTANCE_MIN 1															 // minimum distance of race
#define DISTANCE_MAX 200														 // maximum distance of race
#define RIDE_COST_MIN 50														 // minimum ride cost
#define RIDE_COST_MAX 250														 // maximum ride cost
#define JERSEY_COST_MIN 50														 // minimum jersey cost
#define JERSEY_COST_MAX 150														 // maximum jersey cost
#define CHARITY_PERCENT_MIN 5													 // minimum charity percentage
#define CHARITY_PERCENT_MAX 30													 // maximum charity percentage
#define AMOUNT_JERSEY_SIZES 4													 // amount of jersey sizes
#define AMOUNT_RESPONSES 2														 // amount of responses
const char *password = "B!k34u";												 // admin password
const char *responses[] = {"(y)es", "(n)o"};									 // yes/no responses
const char *jersey_sizes[] = {"(s)mall", "(m)edium", "(l)arge", "(x)tra-large"}; // jersey sizes

// structs
typedef struct organization
{
	char organization_name[BUFFER_SIZE];
	double race_distance;
	double race_cost;
	double jersey_cost;
	double charity_percent;
	double total_riders;
	double total_race_sales;
	double total_jerseys;
	double total_jersey_sales;
} Organization;

typedef struct node
{
	Organization org_data;
	struct node *next_ptr;
} Node;

// function prototypes

// task & validation functions
void fgets_remove_newline(char *input_string);
char get_valid_char(char *char_array[], size_t char_size);
bool get_valid_double(const char *input_string, double *valid_double, unsigned int min, unsigned int max);
bool validate_password(void);
void validate_payment(char *const customer_name, double personal_total, double charity_price);
void prompt_data(char *const prompt_string, double *const double_ptr, unsigned int min, unsigned int max);

// linked list functions
void insert_node(Node **head_ptr, Organization org_to_insert);
void print_organizations(Node *head_ptr);
unsigned int find_organization_index(Node *head_ptr);

// program functions
void admin_set_up(Node *head_ptr);
void rider_registration(Node *head_ptr, unsigned int organization_index, char *const customer_name);
void print_summary(Node *head_ptr);

/*
 *  allows an admin to set up organizations and riders to register for the race.
 *
 *  returns: exit status code
 */
int main(void)
{
	Node *head_ptr = NULL;

	int compare_result;
	unsigned int organization_index;
	char customer_name[BUFFER_SIZE];
	bool is_finished = false; // used to end rider registration while loop
	bool valid_exit = false;  // checks if password is correct to end rider registration

	// validate the admin's password
	puts("\n - Log-In - \n");
	puts("Enter admin pin to set up race information:");
	bool is_valid = validate_password();

	if (is_valid)
	{
		// admin set up
		puts("\n - Admin Set-Up Mode - \n");
		admin_set_up(&head_ptr);

		// rider registration
		puts("\n - Rider Registration Mode - \n");
		while (!is_finished)
		{
			// list organizations and find index
			organization_index = find_organization_index(head_ptr);

			// prompt for name and if "quit" begin termination
			puts("Enter your first name and last name to register for the ride; enter \"QUIT\" to end:");
			fgets_remove_newline(customer_name);
			compare_result = strcasecmp(customer_name, "quit");

			if (compare_result == 0) // user wants to exit rider registration
			{
				puts("Enter admin pin to exit rider registration:");
				valid_exit = validate_password();

				if (valid_exit) // password correct; rider registration terminated
				{
					is_finished = true;
				}
				else // password incorrect; rider registration restarting
				{
					puts("Password incorrect; restarting rider registration.\n");
				}
			}
			else // rider registration
			{
				rider_registration(head_ptr, organization_index, customer_name);
				puts("Rider Registered!\n");
			}
		}

		// once rider registration is over, print summary and exit
		print_summary(head_ptr);
		return EXIT_SUCCESS;
	}
	else
	{
		// password incorrect, program terminates
		puts("Exiting admin set-up.");
		return EXIT_FAILURE;
	}
} // main

// task & validation functions

/*
 *  scans in a string and removes the newline character if present.
 *
 *  input_string: the string to read input into
 */
void fgets_remove_newline(char *input_string)
{
	// read in string and get string length
	fgets(input_string, BUFFER_SIZE, stdin);
	size_t string_length = strlen(input_string);

	// replace newline character if there is one
	if (input_string[string_length - 1] == '\n')
	{
		input_string[string_length - 1] = '\0';
	}
} // fgets_remove_newline

/*
 *  validates a character by comparing it to an array of valid options.
 *
 *  char_array: array of valid character options
 *  char_size: size of the character array
 *
 *  returns: the valid character
 */
char get_valid_char(char *char_array[], size_t char_size)
{
	char char_test;
	size_t char_length;
	char char_string[BUFFER_SIZE];
	bool is_valid = false;

	// validate the character
	do
	{
		do
		{
			// scan in and check string length to ensure one character
			fgets_remove_newline(char_string);
			char_length = strlen(char_string);
			char_string[0] = tolower(char_string[0]);
			char_test = char_string[0];

			// display error if longer
			if (char_length != 1)
			{
				puts("Error: Input should be one character!");
			}
		} while (char_length != 1);

		// compare to array of pointers
		for (int i = 0; i < char_size; i++)
		{
			if (char_test == char_array[i][1])
			{
				is_valid = true;
			}
		}

		// display error if no matches
		if (!is_valid)
		{
			puts("Error: Not a valid choice!");
		}
	} while (!is_valid);

	return char_test;
} // get_valid_char

/*
 *  validates a double by checking if it is within a specified range.
 *
 *  input_string: the string to read input from
 *  valid_double: pointer to store the valid double
 *  min: minimum valid value
 *  max: maximum valid value
 *
 *  returns: a boolean indicating if the double is valid
 */
bool get_valid_double(const char *input_string, double *valid_double, unsigned int min, unsigned int max)
{
	char *end;
	bool is_valid = false;
	double double_test = strtod(input_string, &end);

	if (end == input_string) // catches inputs that aren't numbers
	{
		printf("%s\n", "Error: not a decimal number!");
	}
	else if ('\0' != *end) // catches characters at the end of input
	{
		printf("%s\n", "Error: extra characters at the end of input!");
	}
	else if (double_test < min || double_test > max) // range check
	{
		printf("Error: Enter a value between %d and %d!\n", min, max);
	}
	else // update pointer and return true since number is valid
	{
		*valid_double = (double)double_test;
		is_valid = true;
	}

	// return the validity of the test
	return is_valid;
} // get_valid_double

/*
 *  validates a password.
 *
 *  returns: a boolean indicating if the password is valid
 */
bool validate_password(void)
{
	int compare_result;
	unsigned int count = 0;
	char password_input[BUFFER_SIZE];
	bool is_valid = false;

	// loop 3 times or until password is valid
	do
	{
		fgets_remove_newline(password_input);
		compare_result = strcmp(password_input, password);

		if (compare_result == 0)
		{
			// breaks loop, password is valid
			is_valid = true;
		}
		else
		{
			// displays an error message & increments count
			puts("Error: password is incorrect!");
			count++;
		}
	} while (count < 3 && !is_valid);

	// return the validity of the password
	return is_valid;
} // validate_password

/*
 *  validates credit card information.
 *
 *  customer_name: name of the customer
 *  personal_total: total amount to be charged
 *  charity_price: amount to be donated to charity
 */
void validate_payment(char *const customer_name, double personal_total, double charity_price)
{
	unsigned int hyphen_count = 0;
	size_t card_length;
	char card_info[BUFFER_SIZE];
	bool is_valid = false;

	do // loop until correct
	{
		// prompt input and designate hyphens
		printf("Your total cost is $%.2lf, Enter your credit card number: (XXXX-####-#####)\n", personal_total);
		fgets_remove_newline(card_info);
		card_length = strlen(card_info);

		char first_hyphen = card_info[4];
		char second_hyphen = card_info[9];
		hyphen_count = 0;

		// iterate through string and find hyphen count
		for (size_t i = 0; i < card_length; i++)
		{
			if (card_info[i] == '-')
			{
				hyphen_count++;
			}
		}

		if (card_length != 15) // catches incorrect length
		{
			puts("Error: card number is incorrect length!");
		}
		else if (first_hyphen != '-' || second_hyphen != '-') // catches incorrect hyphen positions
		{
			puts("Error: hyphens are not in correct position!");
		}
		else if (hyphen_count > 2)
		{
			puts("Error: too many hyphens!");
		}
		else // checks data to see if it is valid
		{
			char *token_ptr = strtok(card_info, "-");
			if (isalpha(*token_ptr) == 0 || isalpha(*(token_ptr + 1)) == 0 || isalpha(*(token_ptr + 2)) == 0 || isalpha(*(token_ptr + 3)) == 0) // catches incorrect first segment
			{
				puts("Error: correct card format is XXXX-####-#####! (First segment must be all letters)");
			}
			else // first segment is correct
			{
				token_ptr = strtok(NULL, "-");
				if (isdigit(*(token_ptr)) == 0 || isdigit(*(token_ptr + 1)) == 0 || isdigit(*(token_ptr + 2)) == 0 || isdigit(*(token_ptr + 3)) == 0) // catches incorrect middle segment
				{
					puts("Error: correct card format is XXXX-####-#####! (Middle segment must be all numbers)");
				}
				else // middle segment is correct
				{
					token_ptr = strtok(NULL, "-");
					if (isdigit(*token_ptr) == 0 || isdigit(*(token_ptr + 1)) == 0 || isdigit(*(token_ptr + 2)) == 0 || isdigit(*(token_ptr + 3)) == 0 || isdigit(*(token_ptr + 4)) == 0) // catches incorrect last segment
					{
						puts("Error: correct card format is XXXX-####-#####! (Last segment must be all numbers)");
					}
					else // last segment is correct
					{
						// exit payment validation
						printf("Thank you %s for your purchase. $%.2lf of the ticket sales will go to charity.\n", customer_name, charity_price);
						is_valid = true;
					}
				}
			}
		}
	} while (!is_valid);
} // validate_payment

/*
 *  prompts user for input and validates it.
 *
 *  prompt_string: the prompt message to display
 *  double_ptr: pointer to store the valid double
 *  min: minimum valid value
 *  max: maximum valid value
 */
void prompt_data(char *const prompt_string, double *const double_ptr, unsigned int min, unsigned int max)
{
	char input_str[BUFFER_SIZE];
	bool is_valid = false;

	// loop until good data has been inputted
	do
	{
		printf("%s\n", prompt_string);
		fgets_remove_newline(input_str);
		is_valid = get_valid_double(input_str, double_ptr, min, max);
	} while (!is_valid);
} // prompt_data

// linked list functions

/*
 *  inserts a node alphabetically into the organization linked list.
 *
 *  head_ptr: a double pointer to the head of linked list
 *  org_to_insert: organization to insert
 */
void insert_node(Node **head_ptr, Organization org_to_insert)
{
	// allocate memory
	Node *new_org_ptr = malloc(sizeof(Node));

	bool found = false;
	bool placed = false;

	if (new_org_ptr != NULL)
	{
		// assign data and next
		new_org_ptr->org_data = org_to_insert;
		new_org_ptr->next_ptr = NULL;

		// set previous pointer to null & current to head
		Node *previous_ptr = NULL;
		Node *current_ptr = *head_ptr;

		// find insertion point
		while (current_ptr != NULL && !found)
		{
			int compare = strcmp(new_org_ptr->org_data.organization_name, current_ptr->org_data.organization_name);

			if (compare <= 0) // if the incoming organization name is above the current
			{
				if (previous_ptr == NULL) // if the insertion point is at the beginning
				{
					new_org_ptr->next_ptr = *head_ptr;
					*head_ptr = new_org_ptr;
				}
				else // if the insertion point is in the middle
				{
					current_ptr = previous_ptr->next_ptr;
					previous_ptr->next_ptr = new_org_ptr;
					new_org_ptr->next_ptr = current_ptr;
				}

				// insertion point found, end while loop
				placed = true;
				found = true;
			}
			else // moves the list
			{
				// the previous pointer now points to current
				previous_ptr = current_ptr;

				// the current pointer now points to the next pet
				current_ptr = current_ptr->next_ptr;
			}
		}

		if (*head_ptr == NULL) // if list is empty
		{
			*head_ptr = new_org_ptr;
			placed = true;
		}

		if (!placed) // if entry is at the end of list
		{
			previous_ptr = NULL;
			current_ptr = *head_ptr;

			while (current_ptr->next_ptr != NULL)
			{
				previous_ptr = current_ptr;
				current_ptr = current_ptr->next_ptr;
			}

			current_ptr->next_ptr = new_org_ptr;
			placed = true;
		}
	}
	else // no memory allocated for node
	{
		printf("Error: No memory to create node for %s\n!", org_to_insert.organization_name);
	}
} // insert_node

/*
 *  prints the organizations in the linked list.
 *
 *  head_ptr: a pointer to the head of linked list
 */
void print_organizations(Node *head_ptr)
{
	if (head_ptr != NULL) // list is not empty
	{
		// initialize current & print header
		Node *current_ptr = head_ptr;
		printf("%s", "Organization\t\tDistance\tCost\t\tPercentage\n");

		while (current_ptr != NULL)
		{
			// display and go to next node
			printf("%s\t\t%.2lf\t\t$%.2lf\t\t%.0lf%%\n", current_ptr->org_data.organization_name, current_ptr->org_data.race_distance, current_ptr->org_data.race_cost, current_ptr->org_data.charity_percent);
			current_ptr = current_ptr->next_ptr;
		}

		printf("%s", "\n");
	}
	else // list is empty
	{
		puts("Error: List is empty!");
	}
} // print_organizations

/*
 *  finds the index of an organization in the linked list based on user input.
 *
 *  head_ptr: a pointer to the head of linked list
 *
 *  returns: the index of the chosen organization
 */
unsigned int find_organization_index(Node *head_ptr)
{
	int compare_result = 0;
	unsigned int index = 0;
	char input_str[BUFFER_SIZE];
	char organization_name[BUFFER_SIZE];
	Node *current_ptr = head_ptr;
	bool is_valid = false;

	// display header
	puts("You can register for one of the following bike races and a percentage will be raised for that organization.\n");
	print_organizations(head_ptr);

	do
	{
		// reset index & linked list
		index = 0;
		current_ptr = head_ptr;

		// prompt for organization name
		puts("Enter the name of the organization you want to register:");
		fgets_remove_newline(input_str);

		// iterate through linked list to find organization name
		while (current_ptr != NULL && !is_valid)
		{
			// reset result
			compare_result = 0;

			// compare input string and organization name
			strcpy(organization_name, current_ptr->org_data.organization_name);
			compare_result = strcasecmp(input_str, organization_name);

			if (compare_result == 0) // break loop if found
			{
				is_valid = true;
			}
			else // increment index
			{
				index++;
			}

			// move linked list
			current_ptr = current_ptr->next_ptr;
		}

		// display error message if it isn't valid
		if (!is_valid)
		{
			puts("Error: that organization name was not found in the list of organizations, try again!\n");
		}
	} while (!is_valid);

	printf("You have chosen %s.\n\n", organization_name);

	// return index
	return index;
} // find_organization_index

// program functions

/*
 *  sets up fundraising organizations.
 *
 *  head_ptr: a pointer to the head of linked list
 */
void admin_set_up(Node *head_ptr)
{
	char response;
	unsigned int org_num = 0;

	puts("Set up the fundraising information for the organizations.");

	do
	{
		// prompt response
		puts("\nDo you want to add an organization? Please enter (y)es or (n)o: ");
		response = get_valid_char(responses, AMOUNT_RESPONSES);

		if (response == 'y')
		{
			Organization org_to_insert;

			// set-up organization name
			puts("Enter fundraising organizations name:");
			fgets_remove_newline(org_to_insert.organization_name);

			// set-up ride distance
			prompt_data("Enter distance in miles for bike course:", &(org_to_insert.race_distance), DISTANCE_MIN, DISTANCE_MAX);
			printf("The bike race distance is %.2lf miles\n", org_to_insert.race_distance);

			// set-up ride cost
			char race_prompt[BUFFER_SIZE] = "Enter the registration cost of the bike ride for ";
			strcat(race_prompt, org_to_insert.organization_name);
			prompt_data(race_prompt, &(org_to_insert.race_cost), RIDE_COST_MIN, RIDE_COST_MAX);
			printf("The bike race cost is $%.2lf\n", org_to_insert.race_cost);

			// set-up jersey cost
			char jersey_prompt[BUFFER_SIZE] = "Enter sales price of jersey for ";
			strcat(jersey_prompt, org_to_insert.organization_name);
			prompt_data(jersey_prompt, &(org_to_insert.jersey_cost), JERSEY_COST_MIN, JERSEY_COST_MAX);
			printf("The bike jersey cost is $%.2lf\n", org_to_insert.jersey_cost);

			// set-up charity percentage
			char charity_prompt[BUFFER_SIZE] = "Enter percentage of the bike race sales that will be donated to ";
			strcat(charity_prompt, org_to_insert.organization_name);
			prompt_data(charity_prompt, &(org_to_insert.charity_percent), CHARITY_PERCENT_MIN, CHARITY_PERCENT_MAX);
			printf("You entered %.0lf%% of the bike race to go to charity.\n", org_to_insert.charity_percent);

			// initialize totals
			org_to_insert.total_riders = 0;
			org_to_insert.total_race_sales = 0;
			org_to_insert.total_jerseys = 0;
			org_to_insert.total_jersey_sales = 0;

			// insert organization into linked list
			insert_node(head_ptr, org_to_insert);

			// increment number of organizations
			org_num++;
		}

		if (org_num == 0)
		{
			puts("Error: you must enter at least one organization!");
		}
	} while (response != 'n' || org_num == 0);
} // admin_set_up

/*
 *  registers a rider for a selected organization.
 *
 *  head_ptr: a pointer to the head of linked list
 *  organization_index: index of the chosen organization
 *  customer_name: name of the customer
 */
void rider_registration(Node *head_ptr, unsigned int organization_index, char *const customer_name)
{
	// personal rider variables
	unsigned int jersey = 0;
	double personal_jersey = 0;
	double personal_total = 0;
	double charity_price = 0;

	// placeholder variables
	char chosen_size;
	char chosen_response;
	char chosen_receipt;

	// position current to right index
	Node *current_ptr = head_ptr;

	if (organization_index != 0)
	{
		for (unsigned int i = 0; i < organization_index; i++)
		{
			current_ptr = current_ptr->next_ptr;
		}
	}

	// prompt for jersey and record results
	printf("Do you want to purchase a jersey for $%.2lf? Answer with (y)es or (n)o:\n", current_ptr->org_data.jersey_cost);
	chosen_response = get_valid_char(responses, AMOUNT_RESPONSES);

	if (chosen_response == 'y') // if they want a jersey
	{
		puts("Enter your size (s)mall, (m)edium, (l)arge, or (x)large:");
		chosen_size = get_valid_char(jersey_sizes, AMOUNT_JERSEY_SIZES);
		personal_jersey = current_ptr->org_data.jersey_cost;
		jersey++;
	}

	// compute variables and update organization pointers
	personal_total = current_ptr->org_data.race_cost + personal_jersey;
	charity_price = personal_total * (current_ptr->org_data.charity_percent * 0.01);
	current_ptr->org_data.total_riders = current_ptr->org_data.total_riders + 1;
	current_ptr->org_data.total_jerseys = current_ptr->org_data.total_jerseys + jersey;
	current_ptr->org_data.total_race_sales = current_ptr->org_data.total_race_sales + current_ptr->org_data.race_cost;
	current_ptr->org_data.total_jersey_sales = current_ptr->org_data.total_jersey_sales + personal_jersey;

	// prompt for payment
	validate_payment(customer_name, personal_total, charity_price);

	// prompt for receipt
	puts("Do you want a receipt (y)es or (n)o?");
	chosen_receipt = get_valid_char(responses, AMOUNT_RESPONSES);

	if (chosen_receipt == 'y') // if they want a receipt
	{
		printf("\nRace :\t\t\t$%.2lf\nJersey :\t\t$%.2lf\n\nTotal Cost :\t\t$%.2lf\nDonation to Charity :\t$%.2lf\n\n", current_ptr->org_data.race_cost, personal_jersey, personal_total, charity_price);
	}
} // rider_registration

/*
 *  prints a summary of all organizations to standard out and to a file.
 *
 *  head_ptr: a pointer to the head of linked list
 */
void print_summary(Node *head_ptr)
{
	double charity_race;
	double charity_shirts;
	double grand_total;
	double total_charity;
	Node *current_ptr = head_ptr;
	FILE *file_ptr;

	// check if file is null
	if ((file_ptr = fopen("org_list.txt", "w")) == NULL)
	{
		puts("Error: Couldn't open file for writing!");
	}
	else // file open was a success
	{
		while (current_ptr != NULL)
		{
			// compute totals
			charity_race = (current_ptr->org_data.race_cost * (current_ptr->org_data.charity_percent * 0.01)) * current_ptr->org_data.total_riders;
			charity_shirts = (current_ptr->org_data.total_jersey_sales * (current_ptr->org_data.charity_percent * 0.01)) * current_ptr->org_data.total_jerseys;
			grand_total = current_ptr->org_data.total_race_sales + current_ptr->org_data.total_jersey_sales;
			total_charity = charity_race + charity_shirts;

			// print summary of race to standard out
			printf("\nRace\t\tDistance\tPrice\t\tRegistrants\tTotal Sales\tCharity Amount\n%-15s\t%.2lf\t\t$%.2lf\t\t%.0lf\t\t$%.2lf\t\t$%.2lf", current_ptr->org_data.organization_name, current_ptr->org_data.race_distance, current_ptr->org_data.race_cost, current_ptr->org_data.total_riders, current_ptr->org_data.total_race_sales, charity_race);
			printf("\n\nShirts\t\tSales\t\tCharity Amount\n%.0lf\t\t$%.2lf\t\t$%.2lf\n", current_ptr->org_data.total_jerseys, current_ptr->org_data.total_jersey_sales, charity_shirts);
			printf("\nTotal Sales :\t\t\t\t$%.2lf\nTotal funds raised for charity :\t$%.2lf\n\n", grand_total, total_charity);

			// print summary of race to file
			fprintf(file_ptr, "\nRace\t\tDistance\tPrice\t\tRegistrants\tTotal Sales\tCharity Amount\n%-15s\t%.2lf\t\t$%.2lf\t\t%.0lf\t\t$%.2lf\t\t$%.2lf", current_ptr->org_data.organization_name, current_ptr->org_data.race_distance, current_ptr->org_data.race_cost, current_ptr->org_data.total_riders, current_ptr->org_data.total_race_sales, charity_race);
			fprintf(file_ptr, "\n\nShirts\t\tSales\t\tCharity Amount\n%.0lf\t\t$%.2lf\t\t$%.2lf\n", current_ptr->org_data.total_jerseys, current_ptr->org_data.total_jersey_sales, charity_shirts);
			fprintf(file_ptr, "\nTotal Sales :\t\t\t\t$%.2lf\nTotal funds raised for charity :\t$%.2lf\n\n", grand_total, total_charity);

			// move linked list
			current_ptr = current_ptr->next_ptr;
		}
	}

	fclose(file_ptr);
} // print_summary
