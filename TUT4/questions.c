/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "questions.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Initializes the array of questions for the game
void initialize_game(void)
{
    // initialize each question struct and assign it to the questions array
    for (int i = 0; i < 12; i++) {
      questions[i].answered = false;
    }
    questions[0].value = 200;
    strcpy(questions[0].category, categories[0]);
    strcpy(questions[0].question, "Q1");
    strcpy(questions[0].answer, "A1");
    questions[1].value = 400;
    strcpy(questions[1].category, categories[0]);
    strcpy(questions[1].question, "Q2");
    strcpy(questions[1].answer, "A2");
    questions[2].value = 600;
    strcpy(questions[2].category, categories[0]);
    strcpy(questions[2].question, "Q3");
    strcpy(questions[2].answer, "A3");
    questions[3].value = 800;
    strcpy(questions[3].category, categories[0]);
    strcpy(questions[3].question, "Q4");
    strcpy(questions[3].answer, "A4");

    questions[4].value = 200;
    strcpy(questions[4].category, categories[1]);
    strcpy(questions[4].question, "Q5");
    strcpy(questions[4].answer, "A5");
    questions[5].value = 400;
    strcpy(questions[5].category, categories[1]);
    strcpy(questions[5].question, "Q5");
    strcpy(questions[5].answer, "A5");
    questions[6].value = 600;
    strcpy(questions[6].category, categories[1]);
    strcpy(questions[6].question, "Q2");
    strcpy(questions[6].answer, "A2");
    questions[7].value = 800;
    strcpy(questions[7].category, categories[1]);
    strcpy(questions[7].question, "Q3");
    strcpy(questions[7].answer, "A3");

    questions[8].value = 200;
    strcpy(questions[8].category, categories[2]);
    strcpy(questions[8].question, "Q4");
    strcpy(questions[8].answer, "A4");
    questions[9].value = 400;
    strcpy(questions[9].category, categories[2]);
    strcpy(questions[9].question, "Q5");
    strcpy(questions[9].answer, "A5");
    questions[10].value = 600;
    strcpy(questions[10].category, categories[2]);
    strcpy(questions[10].question, "Q1");
    strcpy(questions[10].answer, "A1");
    questions[11].value = 800;
    strcpy(questions[11].category, categories[2]);
    strcpy(questions[11].question, "Q2");
    strcpy(questions[11].answer, "A2");
}

// Displays each of the remaining categories and question dollar values that have not been answered
void display_categories(void)
{
  // print categories and dollar values for each unanswered question in questions array
  printf(ANSI_COLOR_BLUE "\n==============\n" ANSI_COLOR_RESET);
  for (int i = 0; i < 3; i++) {
    printf(ANSI_COLOR_MAGENTA "%s\n" ANSI_COLOR_RESET, categories[i]);
    int c = 0;
    if (i == 0) {c = i;}
    if (i == 1) {c = i + 3;}
    if (i == 2) {c = i + 6;}

    for (int j = 0; j < 4; j++) {
      if (questions[c].answered == 0) {
        printf(ANSI_COLOR_CYAN "  - $%d\n" ANSI_COLOR_RESET, questions[c].value);
      }
      else {
        printf(ANSI_COLOR_CYAN "  - %s\n" ANSI_COLOR_RESET, "Answered");
      }
      c++;
    }
  }
  printf(ANSI_COLOR_BLUE "==============\n\n" ANSI_COLOR_RESET);
}

// Displays the question for the category and dollar value
void display_question(char *category, int value)
{
  for(int i = 0; i < 12; i++) {
    if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
      printf(ANSI_COLOR_BLUE "Question (%s, $%d): %s\n" ANSI_COLOR_RESET, category, value, questions[i].question);
    }
  }
}

// Returns true if the answer is correct for the question for that category and dollar value
bool valid_answer(char *category, int value, char *answer)
{
  for(int i = 0; i < 12; i++) {
    if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
      if (strcasecmp(answer, questions[i].answer) == 0) {
        return true;
      }
    }
  }
  // Look into string comparison functions
  return false;
}

// Returns true if the question has already been answered
bool already_answered(char *category, int value)
{
  for(int i = 0; i < 12; i++) {
    if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
      return questions[i].answered;
    }
  }
  return false;
}
