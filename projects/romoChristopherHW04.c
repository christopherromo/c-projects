// Homework 4

/**
@christopherRomo
October 3rd, 2022
cromo@uccs.edu

program tracks grades and calculates weights */

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// constants
#define STUDENTS 4
#define MIN_GRADE 0
#define MAX_GRADE 105
#define GRADE_CATEGORIES 5
#define CATEGORIES "1. Learning Activity 2. Homework 3. Project 4. Midterm 5. Final "
const double GRADE_CATEGORY_WEIGHT[] = { 0.1, 0.3, 0.3, 0.15, .15 };

// function declarations
int getValidInput();
void enterGrades(int grades[][GRADE_CATEGORIES], size_t student, size_t category);
void printGrades(const int grades[][GRADE_CATEGORIES], size_t student, size_t category);
void calculateGrades(int grades[][GRADE_CATEGORIES], size_t student, size_t category);

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
	enterGrades(gradeBook, STUDENTS, GRADE_CATEGORIES);
	printGrades(gradeBook, STUDENTS, GRADE_CATEGORIES);
	calculateGrades(gradeBook, STUDENTS, GRADE_CATEGORIES);

} // main

int getValidInput()
{
	// declare variables
	int input;
	unsigned int scannedVar;
	bool isValid = false;

	// determine if valid
	do
	{
		scannedVar = scanf("%d", &input);
		while ((getchar()) != '\n');

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

} // getValidInput

void enterGrades(int grades[][GRADE_CATEGORIES], size_t student, size_t category)
{
	// declare variables
	double input = 0;

	// scan in grades for each cell of 2D array
	for (size_t student = 0; student < STUDENTS; student++)
	{
		for (size_t category = 0; category < GRADE_CATEGORIES; category++)
		{
			printf("Enter the grade for each category for student %d, category %d: ", student + 1, category + 1);
			input = getValidInput();
			grades[student][category] = input;
		}
		printf("%s", "\n");
	}

} // enterGrades

void printGrades(const int grades[][GRADE_CATEGORIES], size_t student, size_t category)
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

} // printGrades

void calculateGrades(int grades[][GRADE_CATEGORIES], size_t student, size_t category)
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

} // calculateGrades