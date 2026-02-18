/*
 * file:        grade_calculator.c
 * author:      christopher romo
 * created:     2022-10-03
 * description: this program allows the user to enter grades for and calculate
 *              the final grades of multiple students based on weighted categories.
 */

// libraries
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// constants
#define STUDENTS 4																	  // number of students
#define MIN_GRADE 0																	  // minimum grade
#define MAX_GRADE 105																  // maximum grade
#define GRADE_CATEGORIES 5															  // number of grade categories
#define CATEGORIES "1. Learning Activity 2. Homework 3. Project 4. Midterm 5. Final " // grade categories
const double GRADE_CATEGORY_WEIGHT[] = {0.1, 0.3, 0.3, 0.15, .15};					  // weights for each category

// function prototypes
int get_valid_input(void);
void enter_grades(int grades[][GRADE_CATEGORIES], size_t student, size_t category);
void print_grades(const int grades[][GRADE_CATEGORIES], size_t student, size_t category);
void calculate_grades(int grades[][GRADE_CATEGORIES], size_t student, size_t category);

/*
 * function: main
 * ----------------------------
 *  entry point of the program.
 *
 *  returns: exit status code
 */
int main(void)
{
	// declare 2D array
	int grade_book[STUDENTS][GRADE_CATEGORIES];

	// print header
	printf("This program will calculate the grades for these categories:\n%s\nThe category weights are:\n", CATEGORIES);
	for (int i = 0; i < GRADE_CATEGORIES; i++)
	{
		printf("Category %d weight is %.2f\n", i + 1, GRADE_CATEGORY_WEIGHT[i]);
	}
	printf("%s", "\nThe correct order to enter grades for each student is: \n");
	printf("%s\n\n", CATEGORIES);

	// enter grades into the grade book
	enter_grades(grade_book, STUDENTS, GRADE_CATEGORIES);
	print_grades(grade_book, STUDENTS, GRADE_CATEGORIES);
	calculate_grades(grade_book, STUDENTS, GRADE_CATEGORIES);

	return 0;
} // main

/*
 * function: get_valid_input
 * ----------------------------
 *  prompts user for input and validates it.
 *
 *  returns: the valid input
 */
int get_valid_input(void)
{
	// declare variables
	int input;
	unsigned int scanned_var;
	bool is_valid = false;

	// determine if valid
	do
	{
		scanned_var = scanf("%d", &input);
		while ((getchar()) != '\n')
			;

		if (scanned_var == 1)
		{
			if (input >= MIN_GRADE && input <= MAX_GRADE)
			{
				is_valid = true;
			}
			else
			{
				puts("Input not in range!");
			}
		}
		else
		{
			puts("Not a valid input.");
		}
	} while (!is_valid);

	// return once valid
	return input;
} // get_valid_input

/*
 * function: enter_grades
 * ----------------------------
 *  prompts user to enter grades for each student and category.
 *
 *  grades: 2D array to store grades
 *  student: number of students
 *  category: number of grade categories
 */
void enter_grades(int grades[][GRADE_CATEGORIES], size_t student, size_t category)
{
	// declare variables
	double input = 0;

	// scan in grades for each cell of 2D array
	for (size_t student = 0; student < STUDENTS; student++)
	{
		for (size_t category = 0; category < GRADE_CATEGORIES; category++)
		{
			printf("Enter the grade for each category for student %d, category %d: ", student + 1, category + 1);
			input = get_valid_input();
			grades[student][category] = input;
		}
		printf("%s", "\n");
	}
} // enter_grades

/*
 * function: print_grades
 * ----------------------------
 *  prints the grades entered for each student.
 *
 *  grades: 2D array of grades
 *  student: number of students
 *  category: number of grade categories
 */
void print_grades(const int grades[][GRADE_CATEGORIES], size_t student, size_t category)
{
	// go through 2D array and print all values
	printf("%s\n\n", "Grades entered for each student");
	for (size_t student = 0; student < STUDENTS; student++)
	{
		printf("Student %d:	", student + 1);
		for (size_t category = 0; category < GRADE_CATEGORIES; category++)
		{
			printf("%d	", grades[student][category]);
		}
		printf("%s", "\n");
	}
} // print_grades

/*
 * function: calculate_grades
 * ----------------------------
 *  calculates and prints the final grades for each student.
 *
 *  grades: 2D array of grades
 *  student: number of students
 *  category: number of grade categories
 */
void calculate_grades(int grades[][GRADE_CATEGORIES], size_t student, size_t category)
{
	// declare variables
	double adjusted_grades[STUDENTS][GRADE_CATEGORIES];
	double final_grades[STUDENTS];
	double sum = 0;
	double total_sum = 0;
	char grade;

	// instead of going down a row, go down column and apply adjusted weight
	for (size_t category = 0; category < GRADE_CATEGORIES; category++)
	{
		for (size_t student = 0; student < STUDENTS; student++)
		{
			adjusted_grades[student][category] = grades[student][category] * GRADE_CATEGORY_WEIGHT[category];
		}
	}

	// fill 1D array with final grades
	for (size_t student = 0; student < STUDENTS; student++)
	{
		sum = 0;

		for (size_t category = 0; category < GRADE_CATEGORIES; category++)
		{
			sum += adjusted_grades[student][category];
		}

		// add adjusted grade to array
		final_grades[student] = sum;
		total_sum += sum;
	}

	printf("\n%s\n\n", "Final grades for students, respectively:");

	// print individual student's grades
	for (size_t student = 0; student < STUDENTS; student++)
	{
		if (final_grades[student] >= 90)
		{
			grade = 'A';
		}
		else if (final_grades[student] >= 80)
		{
			grade = 'B';
		}
		else if (final_grades[student] >= 70)
		{
			grade = 'C';
		}
		else if (final_grades[student] >= 60)
		{
			grade = 'D';
		}
		else
		{
			grade = 'F';
		}

		printf("Student %d: %.1f %c\n", student + 1, final_grades[student], grade);
	}

	// display average
	total_sum = total_sum / STUDENTS;
	printf("\nClass average is %.1f\n", total_sum);
} // calculate_grades
