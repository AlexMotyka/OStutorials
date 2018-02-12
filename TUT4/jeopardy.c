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
#include "questions.h"
#include "players.h"
#include "jeopardy.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256
#define NUM_PLAYERS 4

// Put global environment variables here

// Processes the answer from the user containing what is or who is and tokenizes it to retrieve the answer.
void tokenize(char *input, char **tokens);

// Displays the game results for each player, their name and final score, ranked from first to last place
void show_results(struct player *players, int num_players);

int game_state;


int main(int argc, char *argv[])
{
    // An array of 4 players, may need to be a pointer if you want it set dynamically
    struct player players[NUM_PLAYERS];

    // Buffer for user input
    char buffer[BUFFER_LEN] = { 0 };

    // Display the game introduction and initialize the questions
    initialize_game();

    // Prompt for players names
    // initialize each of the players in the array
    for (int i = 0; i < 4; i++) {
      printf("Enter player %d name: ", i+1);
      scanf("%s", &players[i].name);
      players[i].score = 0;
    }

    // Perform an infinite loop getting command input from users until game ends
    game_state = 1;
    while (game_state)
    {
        display_categories();

        char entered_player[255];
        printf("Enter a player name to answer this question: \n");
        scanf("%s", &entered_player);
        while(!player_exists(players, 4, entered_player)){
          printf("Player %s not exist.\n", entered_player);
          printf("Enter a player name to answer this question: \n");
          scanf("%s", &entered_player);
        }

        char entered_category[255];
        int entered_value;
        bool found = false;
        do {
          printf("Enter a category: \n");
          scanf("%s", &entered_category);
          printf("Enter a value: \n");
          scanf("%d", &entered_value);

          for(int i = 0; i < 12; i++) {
            if (strcmp(questions[i].category, entered_category) == 0 && questions[i].value == entered_value) {
              found = true;
            }
          }

          if (found == false) {
            printf("Question not found! Try again!\n");
          }
        }
        while (found == false);

        while (already_answered(entered_category, entered_value)){
          printf("This question is already been answered\n");
          printf("Enter a category: \n");
          scanf("%s", &entered_category);
          printf("Enter a value: \n");
          scanf("%d", &entered_value);
        }

        display_question(entered_category, entered_value);

        char entered_answer[255];
        printf("Enter your answer: ");
        getchar();
        fgets(entered_answer, 255, stdin);

        char *token;
        token = strtok(entered_answer, " ");
        if (strcmp(token, "what") == 0 || strcmp(token, "who") == 0) {
          // printf("first is correct\n");
          token = strtok(NULL, " ");
          if (token != NULL) {
            if (strcmp(token, "is") == 0) {
              // printf("second is correct\n");
              token = strtok(NULL, " ");
              if (token != NULL) {
                strtok(token, "\n");
                //printf("token-no-newline: %s\n", token);
              }
            }
          }
        }

        if (valid_answer(entered_category, entered_value, token)) {
          printf("\n== Correct! ==\n");
          update_score(players, 4, entered_player, entered_value);
          printf("== Add $%d to %s==\n", entered_value, entered_player);
        }
        else {
          printf("\n== Wrong! ==\n");
          for(int i = 0; i < 12; i++) {
            if (strcmp(questions[i].category, entered_category) == 0 && questions[i].value == entered_value) {
                printf("== Correct answer: what is %s ==\n", questions[i].answer);
              }
            }
          }

          for(int i = 0; i < 12; i++) {
            if (strcmp(questions[i].category, entered_category) == 0 && questions[i].value == entered_value) {
                questions[i].answered = true;
              }
            }

          bool all_answered = true;
          for(int i = 0; i < 12; i++) {
            if (!questions[i].answered) {
                all_answered = false;
              }
            }

          if (all_answered) {
            show_results(players, 4);
            return EXIT_SUCCESS;
          }

        // Call functions from the questions and players source files

        // Execute the game until all questions are answered

        // Display the final results and exit
    }
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

printf("\n== Leaderboard ==\n");
printf("%s: $%d\n", players[f].name, players[f].score);
printf("%s: $%d\n", players[s].name, players[s].score);
printf("%s: $%d\n", players[t].name, players[t].score);
printf("%s: $%d\n", players[l].name, players[l].score);
if (tie == 0) {
  printf("\n== Winner is %s! ==\n", players[f].name);
}
if (tie == 1) {
  printf("\n== Winner is %s, %s! ==\n", players[f].name, players[s].name);
}
if (tie == 2) {
  printf("\n== Winner is %s, %s, %s! ==\n", players[f].name, players[s].name, players[t].name);
}
if (tie == 3) {
  printf("\n== Winner is %s, %s, %s, %s! ==\n", players[f].name, players[s].name, players[t].name, players[l].name);
}

}
