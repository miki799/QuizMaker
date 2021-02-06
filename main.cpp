/**
	QuizMaker
	main.cpp
	Purpose: QuizMaker MAIN MENU
	@author: Miko³aj Telec
*/

#include <iostream>
#include "QuizMaker.h"

using namespace std;

int question_file_test(void);
void mode_selection(string mode, int max_range);	
bool is_proper_value(string value, int max_range);	

int main()
{
	string choice;		// keeps user choice	
	int err {};			// file error notification
	int max_range {};	// max number of questions

	// Checking if QuizQuestions.txt is okay
	err = question_file_test();
	if (err == 0)			// if something is wrong app is shutdown		 
		return 0;
	else
		max_range = err;	// if okay max number of questions is returned

	// START
	cout << "\tWelcome to the QuizMaker!!!" << endl;
	while(1)
	{
		cout <<																		
			" \nMAIN MENU\n"
			"1. Quiz in EASY MODE:\n"
				"\t - any amount of questions (max " << max_range << " questions),\n"
				"\t - no negative points:\n"
			    "\t\t - correct answer: +1 point, wrong answer: +0 point,\n"
			"2. Quiz in HARD MODE:\n"
				"\t - any amount of questions (max " << max_range << " questions),\n"
				"\t - negative points system:\n"
			    "\t\t - correct answer: +1 point, wrong answer: -1 point,\n"
			"0. Exit.\n"
			"--------------------------------------------------------------------"
			<< endl;
		cout << "Choice: ";
		getline(cin, choice);	// user choice
		if (choice == "1")																				
			mode_selection("EASY MODE", max_range);
		else if (choice == "2")													
			mode_selection("HARD MODE", max_range);
		else if (choice == "0")													
		{
			cout << "\nExiting..." << endl;
			break;
		}
		else
		{																		
			cout << "\nWrong choice. Try again..." << endl;
		}
    }
}


// Functions 

/**
*	Used for Quiz mode selection, configuration (size),
*   and creating class object. It's a part of application MAIN MENU.
*	@param string mode - "EASY MODE" or "HARD MODE"
*   @param int max_range - max possible amount of questions
*   @return None
*/
void mode_selection(string mode, int max_range)
{
	string choice;	// keeps user choice
	string amount;	// question amount specified by user
	bool booley;	// for input validation

	while (1)
	{
		cout <<
			"\nYou have chosen " << mode << ".\n"
			"Are you sure you want to proceed?\n"
			"--------------------------------------------------------------------\n"
			"1. Yes, I am sure.\n"
			"<-- 0. Return to MAIN MENU."
			<< endl;
		cout << "Choice: ";
		getline(cin, choice);	// user choice
		if (choice == "1") {
			while (1) {
				cout << "\nHow many questions you want to add to this quiz? (max " << max_range << " questions)\n"
					"After making this choice there is no going back, before ending the Quiz.\n"
					"--------------------------------------------------------------------\n"
					"  NUMBER\n"
					"<-- 0. Return"
					<< endl;
				cout << "Choice: ";
				getline(cin, amount);	// user choice
				booley = is_proper_value(amount, max_range);	// checks if provided amount is proper number 
				if (booley == true)		
				{
					QuizMaker quiz(mode);				// creating Quiz instance 
					quiz.quiz_generator(stoi(amount));	
					quiz.run();							
					quiz.save_results();				
					cout <<																	// after Quiz ends
						"\nNow you can go back to the MAIN MENU\n"
						"or start another Quiz in " << mode << ".\n"
						"--------------------------------------------------------------------"
						<< endl;
					break;
				}
				else if (booley == false && amount != "0")
				{
					cout << "\nWrong input. Try again..." << endl;
					cout << "Choice: ";
				}
				else if (amount == "0")			// returning                                                  
				{
					cout << "\nGoing back..." << endl;
					break;
				}

			}
		}
		else if (choice == "0")					// returning to MAIN MENU							
		{
			cout << "\nGoing back..." << endl;
			break;
		}
		else
		{
			cout << "\nWrong input. Try again..." << endl;
		}
	}
}
/**
*   Checks if QuizQuestions.txt exists or has even amount of lines.
*   2 lines = { question, answers, correct answer }
*	@param None
*   @return int 0 or max 
*			0 - if something is wrong 
*			max - maximum number of questions
*/
int question_file_test(void)
{
	int line_count = 0;
	string line;
	ifstream file("QuizQuestions.txt");

	// counting lines
	if (file.is_open())
	{
		while (getline(file, line))
			++line_count;
		file.close();
	}
	else    
	{
		cout <<
			"There is no QuizQuestions.txt!!!\n"
			"QuizMaker will be shutdown...\n"
			<< endl;
		return 0;
	}

	// if number of lines is not even
	if (line_count % 2 != 0)
	{
		cout <<
			"Something wrong with QuizQuestions.txt\n"
			"Probably number of lines is not even.\n"
			"QuizMaker will be shutdown...\n"
			<< endl;
		return 0;
	}
	// if everything is okay number of questions is returned
	int max = line_count / 2;  
	return max;
}


/**
*   Checks if provided string is a number in a specified range.
*	@param string value - amount of questions in the QUIZ
*	@param int max_range - max amount of questions
*   @return boolean true/false - if value is valid true is returned
*/
bool is_proper_value(string value, int max_range)
{
	bool is_num = true;
	for (int i = 0; i < (int) value.length(); i++)
		if (isdigit(value[i]) == false)		// if provided string is not a number
			return false;			     
	int num = stoi(value);	// string -> integer
	if ((num > 0) && (num <= max_range))	
		return true;	// if number is in good range
	else
		return false;	// if number is in bad range
}