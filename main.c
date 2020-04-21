//Main loop for TextRPG program
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//Object Header Include
static char input[100] = "look around";
static bool getInput(void)
{
   printf("\n--> ");
   return fgets(input, sizeof input, stdin) != NULL;
}


//---------------------------------------------------------------------------
//Parse and Execute
//Function to handle user input and perform actions accordingly
bool parseAndExecute(char *input)
{
   char *verb = strtok(input, " \n");
   char *noun = strtok(NULL, " \n");
   if (verb != NULL)
   {
     //Statement for quitting the game
      if (strcmp(verb, "quit") == 0)
      {
         return false;
      }
     //Allows user to look around current location
      else if (strcmp(verb, "look") == 0)
      {
         executeLook(noun);
      }
     //Allows user to take action and "go"
      else if (strcmp(verb, "go") == 0)
      {
         executeGo(noun);
      }
     //Deals with unrecognized user input
      else
      {
         printf("I am unable to '%s'.\n", verb);
      }
   }
   return true;
}


//------------------------------------------------------------------------------
//Location Function
struct location {
   const char *description;
   const char *tag;
}
locs[] = {
   {"an open field", "field"},
   {"a little cave", "cave"}
};
#define numberOfLocations   (sizeof locs / sizeof *locs)
static unsigned locationOfPlayer = 0;
void executeLook(const char *noun)
{
   if (noun != NULL && strcmp(noun, "around") == 0)
   {
      printf("You are in %s.\n", locs[locationOfPlayer].description);
   }
   else
   {
      printf("I don't understand what you want to see.\n");
   }
}
void executeGo(const char *noun)
{
   unsigned i;
   for (i = 0; i < numberOfLocations; i++)
   {
      if (noun != NULL && strcmp(noun, locs[i].tag) == 0)
      {
         if (i == locationOfPlayer)
         {
            printf("You can't get much closer than this.\n");
         }
         else
         {
            printf("OK.\n");
            locationOfPlayer = i;
            executeLook("around");
         }
         return;
      }
   }
   printf("I don't understand where you want to go.\n");
}


//---------------------------------------------------------------------------
//Main function for the game loop
int main()
{
   //Introduction text
   printf("Welcome to Bludgeon the Bandit!\n");
   printf("You wake up in the tavern bar, your memory is spotty, to try and jog it you ask the bartender what happened last night.\n");
   printf("The bartender responds- Oh yes, last night was one heck of a party, no? An adventurer came in last night, you seemed like you knew eachother, he got a little upset when you\n");
   printf("wouldnt buy his drink, started asking where all the money went.\n");
   
   printf("Your memory begins to come back. You remember that after your bountiful dungeon crawl through Fillastotle's Cave, you went back to town to celebrate.\n");
   printf("You dont remember having any friends with you, but you do remember beating up a couple of young bandits trying to rob you at the cave exit.\n");
   printf("");
   while (parseAndExecute(input) && getInput());
   printf("\nBye!\n");
   return 0;
}
