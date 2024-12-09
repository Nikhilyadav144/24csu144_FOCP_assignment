#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
int main() {
srand(time(0));
char *aiChoice;
char plrChoice[15];
int random = rand() % 101;
if (random < 33)
aiChoice = "Rock";
else if (random < 66)
aiChoice = "Paper";
else
aiChoice = "Scissor";
printf("Computer chose: %s\n", aiChoice);
printf("Let's Play Rock, Paper, Scissor:\n");
printf("Type Rock, Paper, or Scissor: ");
scanf("%s", plrChoice);
strlwr(plrChoice);
if (strcmp(plrChoice, "rock") == 0) {
if (strcmp(aiChoice, "Paper") == 0) {
printf("\nYou Lost! AI chose Paper!!!\n");
}
else if (strcmp(aiChoice, "Scissor") == 0) {
printf("\nYou Won!! AI chose Scissor!!!\n");
}
else {
printf("\nIt's a Tie! You both chose Rock!!!\n");
}
}
else if (strcmp(plrChoice, "paper") == 0) {
if (strcmp(aiChoice, "Scissor") == 0) {
printf("\nYou Lost! AI chose Scissor!!!\n");
}
else if (strcmp(aiChoice, "Rock") == 0) {
printf("\nYou Won!! AI chose Rock!!!\n");
}
else {
printf("\nIt's a Tie! You both chose Paper!!!\n");
}
}
else if (strcmp(plrChoice, "scissor") == 0) {
if (strcmp(aiChoice, "Rock") == 0){
printf("\nYou Lost! AI chose Rock!!!\n");
}
else if (strcmp(aiChoice, "Paper") == 0){
printf("\nYou Won!! AI chose Paper!!!\n");
}
else {
printf("\nIt's a Tie! You both chose Scissor!!!\n");
}
}
else {
printf("\nInvalid input. Please type Rock, Paper, or Scissor.\n");
}
return 0;
}