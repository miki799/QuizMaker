/**
	QuizMaker
	QuizMaker.h
	Purpose: Header file for QuizMaker class declaration
	@author: Miko³aj Telec
*/
#ifndef QUIZMAKER_H
#define QUIZMAKER_H
#include <iostream>
#include <algorithm> 
#include <vector>
#include <random>	
#include <fstream> 
#include <string>
#include <ctime>

using namespace std;

/**
*   Loads Quiz questions from QuizQuestions.txt .
*	Generates Quiz with specified amount of questions.
*   Runs Quiz, checks answers and saves the result in 
*   QuizResults.txt .
 */
class QuizMaker
{
private:
	// Quiz configuration
	string m_quiz_mode = "None";									    // "EASY MODE" or "HARD MODE"
	int hard_mode = 0;													// if 1 "HARD MODE" enabled
	int m_number_of_questions {};										// specified by user
	// Question and answers containers
	vector<string> m_questions, m_shuffled_questions;					// m_shuffled... - after mixing	
	vector<string> m_answers, m_shuffled_answers;
	vector<string> m_correct_answers, m_shuffled_correct;
	// Student evidence, results and statistics 																
	string m_student_name = "None";
	int m_student_score = 0;											
	int m_good_answers {}, m_wrong_answers {}, m_skipped_answers {};	// keeps count (statistiscs)	
	vector<int> m_failed_questions_index{};								// keeps failed questions numbers (used for showing proper answers)
	vector<string> m_student_bad_answers;								// keeps student bad answers	
public:
/**
*   Class constructor.  
*	Selects Quiz mode and loads: questions, answers,
*   correct answers from QuizQuestions.txt
*   and saves them in vectors.
*	@param string quiz_mode - "EASY MODE" or "HARD MODE"
*   @return None
*/
	QuizMaker(const string& quiz_mode);
/**
*   Shuffles (mixes) loaded questions and selects
*   only specified amount of them for the Quiz.
*	@param int question_amount - number of Quiz questions
*   @return None
*/
	void quiz_generator(int question_amount);
/**
*   Asks for Student name and runs the quiz.
*   In the end displays result and shows correct
*   answers (if needed).
*	@param None
*   @return None
*/
	void run(void);
/**
*   Saves info about Quiz and Student 
*	result in QuizResults.txt
*	@param None
*   @return None
*/
	void save_results(void);
/**
*   Checks if Student input during Quiz is valid.
*	@param string input - Student answer
*   @return boolean true/false - if input is valid true is returned
*/
	bool valid_input(string input);
};

#endif // !QUIZMAKER_H