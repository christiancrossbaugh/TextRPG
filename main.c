//Main loop for TextRPG program
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//#include "parsexec.h"

//Object Header Include
typedef struct object {
   const char    *description;
   const char    *tag;
   struct object *location;
} OBJECT;

extern OBJECT objs[];
#define tavern      (objs + 0)
#define cave       (objs + 1)
#define longsword     (objs + 2)
#define gold       (objs + 3)
#define staff      (objs + 4)
#define dagger     (objs + 5)
#define player     (objs + 6)
#define endOfObjs  (objs + 7)

static char input[100] = "look around";
static bool getInput(void)
{
   printf("\n--> ");
   return fgets(input, sizeof input, stdin) != NULL;
}


//----------------------------------------------------------------------------
//Parse and Execute Function
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
      //Allows user to grab items
      else if (strcmp(verb, "get") == 0)
      {
         executeGet(noun);
      }
      //Allows user to drop items
      else if (strcmp(verb, "drop") == 0)
      {
         executeDrop(noun);
      }
      //Allows user to give items
      else if (strcmp(verb, "give") == 0)
      {
         executeGive(noun);
      }
      //Allows user to ask npcs
      else if (strcmp(verb, "ask") == 0)
      {
         executeAsk(noun);
      }
      //Gives access to inventory
      else if (strcmp(verb, "inventory") == 0)
      {
         executeInventory();
      }
     //Deals with unrecognized user input
      else
      {
         printf("I am unable to '%s'.\n", verb);
      }
   }
   return true;
}



//---------------------------------------------------------------------------
//Array of Objects for use in game
OBJECT objs[] = {
   {"an old tavern", "tavern"   , NULL  },
   {"a little cave", "cave"    , NULL  },
   {"a steel longsword", "longsword"  , tavern },
   {"a gold coin"  , "gold"    , cave  },
   {"a willow staff", "staff"   , tavern },
   {"a poisoned dagger", "dagger"   , tavern },
   {"yourself"     , "yourself", tavern }
};


//Noun function for matching objects
//---------------------------------------------------------------------------
static bool objectHasTag(OBJECT *obj, const char *noun)
{
   return noun != NULL && *noun != '\0' && strcmp(noun, obj->tag) == 0;
}
static OBJECT *getObject(const char *noun)
{
   OBJECT *obj, *res = NULL;
   for (obj = objs; obj < endOfObjs; obj++)
   {
      if (objectHasTag(obj, noun))
      {
         res = obj;
      }
   }
   return res;
}
OBJECT *getVisible(const char *intention, const char *noun)
{
   OBJECT *obj = getObject(noun);
   if (obj == NULL)
   {
      printf("I don't understand %s.\n", intention);
   }
   else if (!(obj == player ||
              obj == player->location ||
              obj->location == player ||
              obj->location == player->location ||
              obj->location == NULL ||
              obj->location->location == player ||
              obj->location->location == player->location))
   {
      printf("You don't see any %s here.\n", noun);
      obj = NULL;
   }
   return obj;
}

//------------------------------------------------------------------------------
//Location Function

void executeLook(const char *noun)
{
   if (noun != NULL && strcmp(noun, "around") == 0)
   {
      printf("You are in %s.\n", player->location->description);
      listObjectsAtLocation(player->location);
   }
   else
   {
      printf("I don't understand what you want to see.\n");
   }
}
void executeGo(const char *noun)
{
   OBJECT *obj = getVisible("where you want to go", noun);
   if (obj == NULL)
   {
      // already handled by getVisible
   }
   else if (obj->location == NULL && obj != player->location)
   {
      printf("OK.\n");
      player->location = obj;
      executeLook("around");
   }
   else
   {
      printf("You can't get much closer than this.\n");
   }
}

//Misc function for returning lists of items present at a specific location
//---------------------------------------------------------------------------
int listObjectsAtLocation(OBJECT *location)
{
   int count = 0;
   OBJECT *obj;
   for (obj = objs; obj < endOfObjs; obj++)
   {
      if (obj != player && obj->location == location)
      {
         if (count++ == 0)
         {
            printf("You see:\n");
         }
         printf("%s\n", obj->description);
      }
   }
   return count;
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
