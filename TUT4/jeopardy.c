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
#include <stdbool.h>
#include <ctype.h>
#include "questions.h"
#include "players.h"
#include "jeopardy.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Put macros or constants here using #define
#define NUM_PLAYERS 4

// Put global environment variables here

// Displays the game results for each player, their name and final score, ranked from first to last place
void show_results(struct player *players, int num_players);

int game_state;

int main(void)
{
    // An array of 4 players, may need to be a pointer if you want it set dynamically
    struct player players[NUM_PLAYERS];

    system("clear");

    // Display the game introduction and initialize the questions
    initialize_game();

    // Prompt for players names
    // initialize each of the players in the array
    for (int i = 0; i < 4; i++) {
      printf("Enter player %d name: ", i+1);
      scanf("%s", players[i].name);
      players[i].score = 0;
    }

    // Perform an infinite loop getting command input from users until game ends
    game_state = 1;
    while (game_state)
    {
        display_categories();

        // Select player and check exist
        char entered_player[256];
        bool exists = false;
        do {
          printf("Enter a player name to answer this question: ");
          scanf("%s", entered_player);
          if (!player_exists(players, 4, entered_player)){
            printf(ANSI_COLOR_RED "Player %s not exist!\n" ANSI_COLOR_RESET, entered_player);
          }
          else {
            exists = true;
          }
        }
        while (exists == false);

        // Select category and value, check exist, check already_answered
        char entered_category[256];
        char entered_value_str[64];
        int entered_value;
        bool found = false;
        do {
          printf("Enter a category: ");
          scanf("%s", entered_category);
          printf("Enter a dollar amount: $");
          scanf("%s", entered_value_str);
          // check value if entered_value is int
          if (isdigit(entered_value_str[0])) {
            entered_value = atoi(entered_value_str);
          }
          else {
            printf(ANSI_COLOR_RED "Please enter a number for dollar amount!\n" ANSI_COLOR_RESET);
          }
          // check exist
          for(int i = 0; i < 12; i++) {
            if (strcasecmp(questions[i].category, entered_category) == 0 && questions[i].value == entered_value) {
              found = true;
            }
          }
          if (found == false) {
            printf(ANSI_COLOR_RED "Question not found! Try again!\n" ANSI_COLOR_RESET);
          }
          // check already_answered
          else if (already_answered(entered_category, entered_value)) {
            printf(ANSI_COLOR_RED "This question is already been answered!\n" ANSI_COLOR_RESET);
            found = false;
          }
        }
        while (found == false);

        // show question
        display_question(entered_category, entered_value);

        // enter answer
        char entered_answer[255];
        printf("Enter your answer: ");
        getchar();
        fgets(entered_answer, 255, stdin);

        // tokenize and check for what is or who is
        char *token;
        token = strtok(entered_answer, " ");
        if (strcasecmp(token, "what") == 0 || strcasecmp(token, "who") == 0) {
          // printf("first is correct\n");
          token = strtok(NULL, " ");
          if (token != NULL) {
            if (strcasecmp(token, "is") == 0) {
              // printf("second is correct\n");
              token = strtok(NULL, " ");
              if (token != NULL) {
                strtok(token, "\n");
                //printf("token-no-newline: %s\n", token);
              }
            }
          }
        }

        // if correct
        if (valid_answer(entered_category, entered_value, token)) {
          printf(ANSI_COLOR_GREEN "\n== Correct! ==\n" ANSI_COLOR_RESET);
          update_score(players, 4, entered_player, entered_value);
          printf(ANSI_COLOR_GREEN "== Add $%d to %s ==\n" ANSI_COLOR_RESET, entered_value, entered_player);
        }
        // if wrong
        else {
          printf(ANSI_COLOR_YELLOW "\n== Incorrect! ==\n" ANSI_COLOR_RESET);
          for(int i = 0; i < 12; i++) {
            if (strcasecmp(questions[i].category, entered_category) == 0 && questions[i].value == entered_value) {
                printf(ANSI_COLOR_YELLOW "== Correct answer: what is %s ==\n" ANSI_COLOR_RESET, questions[i].answer);
              }
            }
          }

          // mark question as answered
          for(int i = 0; i < 12; i++) {
            if (strcasecmp(questions[i].category, entered_category) == 0 && questions[i].value == entered_value) {
                questions[i].answered = true;
              }
            }

          // check if all questions are answered
          bool all_answered = true;
          for(int i = 0; i < 12; i++) {
            if (!questions[i].answered) {
                all_answered = false;
              }
            }

          // If all answered
          if (all_answered) {
            // Show results
            show_results(players, 4);
            // End game
            game_state = 0;
          }

    }
    return EXIT_SUCCESS;
}


void show_results(struct player *players, int num_players) {
  // first, second, thrid, last
  int f = -1;
  int s = -1;
  int t = -1;
  int l = -1;

  int temp_score = -1;
  int highest_score = -1; // for winner tie score
  int tie = 0; // winner tie counter

  for (int i = 0; i < num_players; i++) { //1st
    if (players[i].score > temp_score) {
      temp_score = players[i].score;
      f = i;
      highest_score = players[i].score;
    }
  }
  temp_score = -1;
  for (int i=0; i < num_players; i++) { //2nd
    if (players[i].score > temp_score && players[i].score <= players[f].score) {
      if (players[i].name != players[f].name) {
        temp_score = players[i].score;
        s = i;
        if (players[i].score == highest_score) {
          tie++;
        }
      }
    }
  }
  temp_score = -1;
  for (int i=0; i < num_players; i++) { //3rd
    if (players[i].score > temp_score && players[i].score <= players[s].score) {
      if (players[i].name != players[f].name && players[i].name != players[s].name) {
        temp_score = players[i].score;
        t = i;
        if (players[i].score == highest_score) {
          tie++;
        }
      }
    }
  }
  temp_score = -1;
  for (int i=0; i < num_players; i++) { //last
    if (players[i].score > temp_score && players[i].score <= players[t].score) {
      if (players[i].name != players[f].name && players[i].name != players[s].name && players[i].name != players[t].name) {
        temp_score = players[i].score;
        l = i;
        if (players[i].score == highest_score) {
          tie++;
        }
      }
    }
  }

printf(ANSI_COLOR_BLUE "\n=================\n== Leaderboard ==\n=================\n" ANSI_COLOR_RESET);
printf(ANSI_COLOR_MAGENTA " - %s: $%d\n" ANSI_COLOR_RESET, players[f].name, players[f].score);
printf(ANSI_COLOR_MAGENTA " - %s: $%d\n" ANSI_COLOR_RESET, players[s].name, players[s].score);
printf(ANSI_COLOR_MAGENTA " - %s: $%d\n" ANSI_COLOR_RESET, players[t].name, players[t].score);
printf(ANSI_COLOR_MAGENTA " - %s: $%d\n" ANSI_COLOR_RESET, players[l].name, players[l].score);
printf(ANSI_COLOR_BLUE "\n=================\n==== Winner =====\n=================\n" ANSI_COLOR_RESET);
if (tie == 0) {
  printf(ANSI_COLOR_MAGENTA " - %s\n" ANSI_COLOR_RESET, players[f].name);
}
if (tie == 1) {
  printf(ANSI_COLOR_MAGENTA " - %s\n - %s\n" ANSI_COLOR_RESET, players[f].name, players[s].name);
}
if (tie == 2) {
  printf(ANSI_COLOR_MAGENTA " - %s\n - %s\n - %s\n" ANSI_COLOR_RESET, players[f].name, players[s].name, players[t].name);
}
if (tie == 3) {
  printf(ANSI_COLOR_MAGENTA " - %s\n - %s\n - %s\n - %s\n" ANSI_COLOR_RESET, players[f].name, players[s].name, players[t].name, players[l].name);
}
printf("\n");

}
