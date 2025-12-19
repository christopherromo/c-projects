/*
 * File:        grade_calculator.c
 * Author:      Christopher Romo
 * Created:     2022-10-03
 * Description: This program allows the user to enter grades for and calculate
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
int get_valid_input();
void enter_grades(int grades[][GRADE_CATEGORIES], size_t student, size_t category);
void print_grades(const int grades[][GRADE_CATEGORIES], size_t student, size_t category);
void calculate_grades(int grades[][GRADE_CATEGORIES], size_t student, size_t category);

/*
 * Function: main
 * ----------------------------
 *  Entry point of the program.
 *
 *  returns: exit status code
 */
int main(void)
{
	// declare 2D array
	int gradeBook[STUDENTS][GRADE_CATEGORIES];

	// print header
	printf("This program will calculate the grades for these categories:\n%s\nThe category weights are:\n", CATEGORIES);
	for (int i = 0; i < GRADE_CATEGORIES; i++)
	{
		printf("Category %d weight is %.2f\n", i + 1, GRADE_CATEGORY_WEIGHT[i]);
	}
	printf("%s", "\nThe correct order to enter grades for each student is: \n");
	printf("%s\n\n", CATEGORIES);

	// enter grades into the grade book
	enter_grades(gradeBook, STUDENTS, GRADE_CATEGORIES);
	print_grades(gradeBook, STUDENTS, GRADE_CATEGORIES);
	calculate_grades(gradeBook, STUDENTS, GRADE_CATEGORIES);

} // main

/*
 * Function: get_valid_input
 * ----------------------------
 *  Prompts user for input and validates it.
 *
 *  returns: the valid input
 */
int get_valid_input()
{
	// declare variables
	int input;
	unsigned int scannedVar;
	bool isValid = false;

	// determine if valid
	do
	{
		scannedVar = scanf("%d", &input);
		while ((getchar()) != '\n')
			;

		if (scannedVar == 1)
		{
			if (input >= MIN_GRADE && input <= MAX_GRADE)
			{
				isValid = true;
			}
			else
			{
				puts("Input not in range!");
			}
		}
		else
		{
			puts("Not a vaild input.");
		}
	} while (!isValid);

	// return once valid
	return input;

} // get_valid_input

/*
 * Function: enter_grades
 * ----------------------------
 *  Prompts user to enter grades for each student and category.
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
 * Function: print_grades
 * ----------------------------
 *  Prints the grades entered for each student.
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
 * Function: calculate_grades
 * ----------------------------
 *  Calculates and prints the final grades for each student.
 *
 *  grades: 2D array of grades
 *  student: number of students
 *  category: number of grade categories
 */
void calculate_grades(int grades[][GRADE_CATEGORIES], size_t student, size_t category)
{
	// declare variables
	double adjustedGrades[STUDENTS][GRADE_CATEGORIES];
	double finalGrades[STUDENTS];
	double sum = 0;
	double totalSum = 0;
	char grade;

	// instead of going down a row, go down column and apply adjusted weight
	for (size_t category = 0; category < GRADE_CATEGORIES; category++)
	{
		for (size_t student = 0; student < STUDENTS; student++)
		{
			adjustedGrades[student][category] = grades[student][category] * GRADE_CATEGORY_WEIGHT[category];
		}
	}

	// fill 1D array with final grades
	for (size_t student = 0; student < STUDENTS; student++)
	{
		sum = 0;

		for (size_t category = 0; category < GRADE_CATEGORIES; category++)
		{
			sum += adjustedGrades[student][category];
		}

		// add adjusted grade to array
		finalGrades[student] = sum;
		totalSum += sum;
	}

	printf("\n%s\n\n", "Final grades for students, respectively:");

	// print individual student's grades
	for (size_t student = 0; student < STUDENTS; student++)
	{
		if (finalGrades[student] >= 90)
		{
			grade = 'A';
		}
		else if (finalGrades[student] >= 80)
		{
			grade = 'B';
		}
		else if (finalGrades[student] >= 70)
		{
			grade = 'C';
		}
		else if (finalGrades[student] >= 60)
		{
			grade = 'D';
		}
		else
		{
			grade = 'F';
		}

		printf("Student %d: %.1f %c\n", student + 1, finalGrades[student], grade);
	}

	// display average
	totalSum = totalSum / STUDENTS;
	printf("\nClass average is %.1f\n", totalSum);

} // calculate_grades