#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

char **questions = NULL;
char **answers = NULL;
#define QUESTION_SIZE 200
#define ANSWER_SIZE 30

/*
 * loads questions from file into storage array.
 * storage = array of question strings
 * file = name of file
 */
int loadQuestions(char *file) {
    char **storage = NULL;
    FILE *myFile;
    // initial memory allocation
    storage = malloc(sizeof(char *) * 1);
    int i = 0;
    int size = 1;
    // opens file
    myFile = fopen(file, "r");
    // initial memory allocation for the question
    storage[0] = malloc(sizeof(char) * (QUESTION_SIZE + ANSWER_SIZE));
    while (fgets(storage[i++], 1000, myFile)) {
        /*
         * i = current question index
         * size = current size of the question array
         */
        if (i == size) {
            storage = realloc(storage, sizeof(char *) * (size * 2));
            size = size * 2;
        }
        // allocates memory for the question string
        storage[i] = malloc(sizeof(char) * (QUESTION_SIZE + ANSWER_SIZE));
    }

    // allocates memory for questions pointers
    questions = malloc(sizeof(char *));

    // allocates memory for answers pointers
    answers = malloc(sizeof(char *));

    // allocates memory for question and answer pointers
    for (int x = 0; x < i - 1; x++) {
        questions[x] = malloc(sizeof(char *));
        answers[x] = malloc(sizeof(char *));
    }

    // splits questions and answers in storage array and points questions and answers pointers to their respective areas
    for (int x = 0; x < i - 1; x++) {
        questions[x] = strtok(storage[x], "?");
        answers[x] = strtok(NULL, "? ");
        // replaces \n with \0 unless its the last answer string since it doesnt have \n
        if (x != i - 2) {
            answers[x][strlen(answers[x]) - 1] = '\0';
        }
    }

    // for loop to make all answers lower case
    for (int x = 0; x < i - 1; x++) {
        int y = 0;
        while (answers[x][y] != '\0') {
            answers[x][y] = tolower(answers[x][y]);
            y++;
        }
    }

    fclose(myFile);

    return i - 1;
}

/*
 * gets user input and stores it in str
 * str = array of characters
 * returns size of answer
 */
int get_user_input(char *str) {
    // character typed in
    char ch;
    // current index of word
    int index = 0;
    // loop to make all characters lowercase
    while (ch != '\n') {
        ch = getchar();
        if (ch != '\n') {
            if (ch >= 97 && ch <= 122) {
                str[index++] = ch;
            } else if (ch >= 65 && ch <= 90) {
                ch += 32;
                str[index++] = ch;
            } else {
                str[index++] = ch;
            }
        }
    }
    str[index] = '\0';
    return (index);
}

// stores non repeating randomly generated numbers in array from 0 -> (size - 1)
void nonRepeatingNumberGenerator(int *array, int size) {
    int x;
    int i = 0;

    while (i < size) {
        int r = rand() % size;

        for (x = 0; x < i; x++) {
            if (array[x] == r) {
                break;
            }
        }
        if (x == i) {
            array[i++] = r;
        }
    }
}

int main(int argc, char *argv[]) {
    // initializes random number generator seed with current time
    srand(time(NULL));

    // stores difficulty of the quiz
    int difficulty = atoi(argv[2]);

    // if difficulty is 6 chooses a random difficulty, if random number is 0 it chooses again
    while (difficulty == 0 || difficulty == 6) {
        difficulty = rand() % 6;

    }

    // result file
    FILE *resultFile = fopen("quiz_history.txt", "a");

    // amount of correct answers guessed
    int correctAnswers = 0;

    // stores user inputed answers
    char *userAnswer = malloc(sizeof(char) * ANSWER_SIZE);

    // loads questions and answers in their respective arrays and stores the amount of questions
    int amountOfQuestions = loadQuestions(argv[1]);

    // array to store randomly sorted question indexes
    int randomQuestionIndex[amountOfQuestions];
    nonRepeatingNumberGenerator(randomQuestionIndex, amountOfQuestions);

    // array to hold weather the answer was answered correctly or incorrectly
    int correctAnswerBool[amountOfQuestions];

    char userAnswers[amountOfQuestions][ANSWER_SIZE];

    char userSelection[4];

    printf("There is %d questions in this quiz!\n", amountOfQuestions);

    // chooses code to run depending on the difficulty
    switch (difficulty) {
        case 1:
            for (int i = 0; i < amountOfQuestions; i++) {
                printf("Question %d: %s?\n", (i + 1), questions[randomQuestionIndex[i]]);
                printf("Please enter your answer: ");

                // gets user answer
                get_user_input(userAnswer);

                // checks if answer is correct, if it is adds one to correctAnswers
                if (strcmp(answers[randomQuestionIndex[i]], userAnswer) == 0) {
                    correctAnswers++;
                    correctAnswerBool[i] = 1;
                }
                else {
                    correctAnswerBool[i] = 0;
                }

                strcpy(userAnswers[i], userAnswer);

                printf("Number of questions answered correctly: %d | Number of questions asked: %d\n", correctAnswers,
                       (i + 1));
            }

            fprintf(resultFile,
                    "Name of quiz: %s | Number of questions answered correctly: %d | Total number of questions: %d | Difficulty: %d\n",
                    argv[1], correctAnswers, amountOfQuestions, difficulty);

            printf("Would you like to view the answers to the questions you got wrong? yes/no: ");
            get_user_input(userSelection);

            // prints out correct and incorrect answers
            if (strcmp(userSelection, "yes") == 0) {
                for (int k = 0; k < amountOfQuestions; ++k) {
                    printf("For question %d: %s? you said %s - ", k + 1, questions[randomQuestionIndex[k]],
                           userAnswers[k]);
                    if (correctAnswerBool[k] == 0) {
                        printf("You were incorrect! The correct answer was: %s\n", answers[randomQuestionIndex[k]]);
                    } else {
                        printf("You got that correct!\n");
                    }
                }
            }
            break;

        case 2:
            for (int i = 0; i < amountOfQuestions; i++) {
                printf("Question %d: %s?\n", (i + 1), questions[randomQuestionIndex[i]]);
                printf("Please enter your answer: ");

                // prints the correct amount of blank spaces
                for (int j = 0; j < strlen(answers[randomQuestionIndex[i]]); ++j) {
                    printf("_ ");
                }

                get_user_input(userAnswer);

                if (strcmp(answers[randomQuestionIndex[i]], userAnswer) == 0) {
                    correctAnswers++;
                    correctAnswerBool[i] = 1;
                }
                else {
                    correctAnswerBool[i] = 0;
                }

                strcpy(userAnswers[i], userAnswer);

                printf("Number of questions answered correctly: %d | Number of questions asked: %d\n", correctAnswers,
                       (i + 1));
            }

            fprintf(resultFile,
                    "Name of quiz: %s | Number of questions answered correctly: %d | Total number of questions: %d | Difficulty: %d\n",
                    argv[1], correctAnswers, amountOfQuestions, difficulty);

            printf("Would you like to view the answers to the questions you got wrong? yes/no: ");
            get_user_input(userSelection);

            // prints out correct and incorrect answers
            if (strcmp(userSelection, "yes") == 0) {
                for (int k = 0; k < amountOfQuestions; ++k) {
                    printf("For question %d: %s? you said %s - ", k + 1, questions[randomQuestionIndex[k]],
                           userAnswers[k]);
                    if (correctAnswerBool[k] == 0) {
                        printf("You were incorrect! The correct answer was: %s\n", answers[randomQuestionIndex[k]]);
                    } else {
                        printf("You got that correct!\n");
                    }
                }
            }
            break;

        case 3:
            for (int i = 0; i < amountOfQuestions; i++) {

                printf("Question %d: %s?\n", (i + 1), questions[randomQuestionIndex[i]]);

                printf("Please enter your answer: ");

                // prints blank spaces with first and last letter shown
                for (int j = 0; j < strlen(answers[randomQuestionIndex[i]]); ++j) {
                    if (j == 0) {
                        printf("%c ", answers[randomQuestionIndex[i]][0]);
                    } else if (j == strlen(answers[randomQuestionIndex[i]]) - 1) {
                        printf("%c ", answers[randomQuestionIndex[i]][j]);
                    } else {
                        printf("_ ");
                    }
                }

                get_user_input(userAnswer);
                if (strcmp(answers[randomQuestionIndex[i]], userAnswer) == 0) {
                    correctAnswers++;
                    correctAnswerBool[i] = 1;
                }
                else {
                    correctAnswerBool[i] = 0;
                }

                strcpy(userAnswers[i], userAnswer);

                printf("Number of questions answered correctly: %d | Number of questions asked: %d\n", correctAnswers,
                       (i + 1));
            }

            fprintf(resultFile,
                    "Name of quiz: %s | Number of questions answered correctly: %d | Total number of questions: %d | Difficulty: %d\n",
                    argv[1], correctAnswers, amountOfQuestions, difficulty);

            printf("Would you like to view the answers to the questions you got wrong? yes/no: ");
            get_user_input(userSelection);

            // prints out correct and incorrect answers
            if (strcmp(userSelection, "yes") == 0) {
                for (int k = 0; k < amountOfQuestions; ++k) {
                    printf("For question %d: %s? you said %s - ", k + 1, questions[randomQuestionIndex[k]],
                           userAnswers[k]);
                    if (correctAnswerBool[k] == 0) {
                        printf("You were incorrect! The correct answer was: %s\n", answers[randomQuestionIndex[k]]);
                    } else {
                        printf("You got that correct!\n");
                    }
                }
            }
            break;

        case 4:
            for (int i = 0; i < amountOfQuestions; i++) {

                int showed = 0;

                printf("Question %d: %s?\n", (i + 1), questions[randomQuestionIndex[i]]);

                printf("Please enter your answer: ");

                // randomly selects 2 letters to show rest are blank spaces
                for (int j = 0; j < strlen(answers[randomQuestionIndex[i]]); ++j) {
                    if (showed != 2 && (rand() % 4) == 1) {
                        printf("%c ", answers[randomQuestionIndex[i]][j]);
                        showed++;
                    } else if (showed != 2 && (j == strlen(answers[randomQuestionIndex[i]]) - 1 || j == strlen(answers[randomQuestionIndex[i]]) - 2)) {
                        printf("%c ", answers[randomQuestionIndex[i]][j]);
                        showed++;
                    } else {
                        printf("_ ");
                    }
                }

                get_user_input(userAnswer);
                if (strcmp(answers[randomQuestionIndex[i]], userAnswer) == 0) {
                    correctAnswers++;
                    correctAnswerBool[i] = 1;
                }
                else {
                    correctAnswerBool[i] = 0;
                }

                strcpy(userAnswers[i], userAnswer);

                printf("Number of questions answered correctly: %d | Number of questions asked: %d\n", correctAnswers,
                       (i + 1));
            }

            fprintf(resultFile,
                    "Name of quiz: %s | Number of questions answered correctly: %d | Total number of questions: %d | Difficulty: %d\n",
                    argv[1], correctAnswers, amountOfQuestions, difficulty);

            printf("Would you like to view the answers to the questions you got wrong? yes/no: ");
            get_user_input(userSelection);

            // prints out correct and incorrect answers
            if (strcmp(userSelection, "yes") == 0) {
                for (int k = 0; k < amountOfQuestions; ++k) {
                    printf("For question %d: %s? you said %s - ", k + 1, questions[randomQuestionIndex[k]],
                           userAnswers[k]);
                    if (correctAnswerBool[k] == 0) {
                        printf("You were incorrect! The correct answer was: %s\n", answers[randomQuestionIndex[k]]);
                    } else {
                        printf("You got that correct!\n");
                    }
                }
            }
            break;

        case 5:
            for (int i = 0; i < amountOfQuestions; i++) {

                // stores the length of the answer
                int lengthOfAnswer = strlen(answers[randomQuestionIndex[i]]);

                // stores the indexes for the randomly sorted letters
                int randomLetterIndexes[lengthOfAnswer];

                nonRepeatingNumberGenerator(randomLetterIndexes, lengthOfAnswer);

                printf("Question %d: %s?\n", (i + 1), questions[randomQuestionIndex[i]]);

                printf("Please enter your answer: ");

                // prints the randomly sorted letters
                for (int j = 0; j < strlen(answers[randomQuestionIndex[i]]); ++j) {
                    printf("%c ", answers[randomQuestionIndex[i]][randomLetterIndexes[j]]);
                }


                get_user_input(userAnswer);
                if (strcmp(answers[randomQuestionIndex[i]], userAnswer) == 0) {
                    correctAnswers++;
                    correctAnswerBool[i] = 1;
                }
                else {
                    correctAnswerBool[i] = 0;
                }

                strcpy(userAnswers[i], userAnswer);

                printf("Number of questions answered correctly: %d | Number of questions asked: %d\n",
                       correctAnswers,
                       (i + 1));
            }

            fprintf(resultFile,
                    "Name of quiz: %s | Number of questions answered correctly: %d | Total number of questions: %d | Difficulty: %d\n",
                    argv[1], correctAnswers, amountOfQuestions, difficulty);

            printf("Would you like to view the answers to the questions you got wrong? yes/no: ");
            get_user_input(userSelection);

            // prints out correct and incorrect answers
            if (strcmp(userSelection, "yes") == 0) {
                for (int k = 0; k < amountOfQuestions; ++k) {
                    printf("For question %d: %s? you said %s - ", k + 1, questions[randomQuestionIndex[k]],
                           userAnswers[k]);
                    if (correctAnswerBool[k] == 0) {
                        printf("You were incorrect! The correct answer was: %s\n", answers[randomQuestionIndex[k]]);
                    } else {
                        printf("You got that correct!\n");
                    }
                }
            }
            break;

        default :
            break;
    }

    fclose(resultFile);
}
