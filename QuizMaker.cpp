/**
	QuizMaker
	QuizMaker.cpp
	Purpose: QuizMaker.h source file with class constructor and methods implementation 
			 and methods implementation 
	@author: Miko³aj Telec
*/
#include "QuizMaker.h"
/**
*   Selects Quiz mode and loads questions, answers, 
*   correct answers from QuizQuestions.txt  
*   and saves them in vectors.
*	@param string quiz_mode - "EASY MODE" or "HARD MODE"
*   @return None
*/
QuizMaker::QuizMaker(const string& quiz_mode = "EASY MODE") : m_quiz_mode{ quiz_mode }
{
	if (quiz_mode == "HARD MODE")			// enables "HARD MODE"
		hard_mode = 1;

	// loading questions from .txt file
	string line;							// temporary buffer	
	int i = 0;								// line number
	fstream file("QuizQuestions.txt");		
	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (i % 2 == 0)						// appends questions
			{
				m_questions.push_back(line);
			}
			else								// appends answers and correct answers
			{
				m_correct_answers.push_back(line.substr(line.find(":") + 1));		// adds correct answer to the vector  
				line.erase(line.find("CorrectAnswer:"));							// erases "CorrectAnswer:..." from answers line
				m_answers.push_back(line);											// adds all possible answers to the vector
			}
			i++;
		}
		file.close();
	}
}
/**
*   Shuffles (mixes) loaded questions and selects
*   only specified amount of them for the Quiz.
*	@param int question_amount - number of Quiz questions
*   @return None
*/
void QuizMaker::quiz_generator(int question_amount)
{
	random_device rd;		// uniformly-distributed integer random number generator that produces non-deterministic random numbers
	mt19937 g(rd());		// Mersenne Twister pseudo-random generator of 32-bit numbers
	string line;			// temporary buffer
	int iterations = 0;		// keeps count of question added to the quiz

	// Generating vector with shuffled numbers
	vector<int> v;											
	for (int i = 0; i < (int) m_questions.size(); i++)
		v.push_back(i);
	shuffle(v.begin(), v.end(), g);

	// Selecting random questions (v vector specifies which one)
	m_number_of_questions = question_amount;	// limits Quiz size
	for (auto i : v)							
	{
		line = m_questions[i];
		m_shuffled_questions.push_back(line);
		line = m_answers[i];
		m_shuffled_answers.push_back(line);
		line = m_correct_answers[i];
		m_shuffled_correct.push_back(line);
		if (iterations == m_number_of_questions - 1)	// checks Quiz size
			break;
		iterations++;
	}
}
/**
*   Asks for Student name and runs the quiz. 
*   In the end displays result and shows correct 
*   answers (if needed).
*	@param None
*   @return None
*/
void QuizMaker::run(void)
{
	string answer{};			// holds user answer
	bool booley;				// used for input validation

	// How Quiz works info 
	cout <<
		"\nBefore start:\n"
		"\t - possible input: 'A', 'B', 'C', 'D' or '0' (nothing else),\n"
		"\t - you can skip question by hitting '0', but you can't return to them later.\n"
		"\t   Skipping doesn't give and take any points (in both modes),\n"
		"\t - After Quiz ends result and correct answers (if needed) will be shown."
		<< endl;
	cout <<
		"--------------------------------------------------------------------\n"
		"What is your name, dear Student?\n"
		"Name: ";
	getline(cin, m_student_name);	

	// !!! QUIZ !!!
	cout << "\n\t!!!QUIZ!!!" << endl;
	cout << "--------------------------------------------------------------------" << endl;
	for (int i = 0; i < m_number_of_questions; i++)
	{
		cout << m_shuffled_questions[i] << endl;	// display question and answer
		cout << m_shuffled_answers[i] << endl;
		cout << "Answer: ";

		while (1)
		{
			getline(cin, answer);									// gets user answer
			booley = valid_input(answer);							// checks if input is valid

			if (booley == true)										// if input is valid
			{
				if (answer == m_shuffled_correct[i])				// if correct answer
				{
					m_student_score++;
					m_good_answers++;
				}
				else if (answer != m_shuffled_correct[i])			// if input differs from correct answer 
				{
					if (answer != "0")								// if wrong answer
					{
						if (hard_mode)								// -1 point if hard mode selected	
							m_student_score--;
						m_wrong_answers++;
					}
					else											// if '0' skips question
					{
						cout << "\nQuestion skipped..." << endl;
						m_skipped_answers++;
					}
					m_failed_questions_index.push_back(i);			// adds index of failed (wrong or skipped) questions
					m_student_bad_answers.push_back(answer);		// adds Student answers to the vector
				}
				break;
			}
			else if (booley == false)								// if input is invalid 
			{
				cout <<
					"\nWrong input. Try again...\n"
					"Answer: ";
			}
		}
		cout << "--------------------------------------------------------------------" << endl;
	}

	// Displaying final result
	cout << "\nQuiz has ended!!!" << endl;
	cout << "\nYour final score is: " << m_student_score << "/" << m_number_of_questions << " points." << endl;
	cout << "Good answers: " << m_good_answers << " Wrong answers: " << m_wrong_answers << " Skipped answers: " << m_skipped_answers << endl;
	// Displaying correct answers, if wrong or skipped occured 
	if (m_wrong_answers != 0 || m_skipped_answers != 0)
	{
		cout << "--------------------------------------------------------------------" << endl;
		cout << "Correct answers in question you failed or skipped:" << endl << endl;;
		int iterator = 0;
		for (auto i : m_failed_questions_index)
		{
			cout << m_shuffled_questions[i] << endl;										// displaying "problematic" questions
			cout << m_shuffled_answers[i] << endl;
			if (m_student_bad_answers[iterator] == "0")										// what Student did?
				cout << "You have skipped that question." << endl;
			else
				cout << "Your answer was: " << m_student_bad_answers[iterator] << endl; 
			cout << "Correct answer is: " << m_shuffled_correct[i] << endl;				
			cout << "----------------------------------------------" << endl;
			iterator++;
		}
	}
}
/**
*   Saves info about Quiz and Student 
*	result in QuizResults.txt
*	@param None
*   @return None
*/
void QuizMaker::save_results(void)
{
	// Getting Quiz finish and date
	// static buffers are unsafe, so had to use pre-allocated buffers
	time_t now = time(NULL);		// gets current time		
	char finish_date[26] = {};
	ctime_s(finish_date, 26, &now);	// returns date in readable format

	// File opened in append mode
	fstream file("QuizResults.txt", ios::app);		
	if (file.is_open())
	{
		file << "--------------------------------------------------------------------" << endl;
		if (hard_mode)
			file << "HARD MODE" << endl;
		else
			file << "EASY MODE" << endl;
		file << "Date: " << finish_date;
		file << "Student name: " << m_student_name << " Final score: " << m_student_score << "/" << m_number_of_questions << " points" << endl;
		file <<
			"Statistics:\n"
			"\t - good answers: " << m_good_answers << "\n"
			"\t - wrong answers: " << m_wrong_answers << "\n"
			"\t - skipped answers: " << m_skipped_answers
			<< endl;
		file.close();
	}
	cout << "\nResults has been saved to QuizResult.txt" << endl;
}
/**
*   Checks if Student input during Quiz is valid. 
*	@param string input - Student answer
*   @return boolean true/false - if input is valid true is returned
*/
bool QuizMaker::valid_input(string input)
{
	vector <string> valid_characters{ "A", "B", "C", "D", "0" };	// valid input
	int indicator = 0;
	if (input.size() != 1)	// input must be a single character
		return false;
	else
	{
		for (auto i : valid_characters)	// checks for match with valid_characters
		{
			if (input == i)		// if there is a match with one of the valid characters
				return true;
		}	
		return false;	// no match means invalid input (e.g. "x")
	}
}