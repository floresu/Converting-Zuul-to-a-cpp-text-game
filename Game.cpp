#include <cctype>
#include <algorithm>
#include "Game.h"
using namespace std;

/**
 *  This class is the main class of the "World of Zuul" application. 
 *  "World of Zuul" is a very simple, text based adventure game.  Users 
 *  can walk around some scenery. That's all. It should really be extended 
 *  to make it more interesting!
 * 
 *  To play this game, create an instance of this class and call the "play"
 *  method.
 * 
 *  This main class creates and initialises all the others: it creates all
 *  rooms, creates the parser and starts the game.  It also evaluates and
 *  executes the commands that the parser returns.
 * 
 * @author  Michael Kölling and David J. Barnes
 * @author Uriel Flores
 * @version 2011.08.08
 * @version 2015.10.04
 */

        
    /**
     * Create the game and initialise its internal map.
     */
    Game::Game() 
    {
        createRooms();
        Parser* parser = new Parser();
    }

    /**
     * Create all the rooms and link their exits together.
     */
    void Game::createRooms()
    {
      
        // create the rooms
        Room *outside = new Room("outside the main entrance of the university");
        Room *theater = new Room("in a lecture theater");
        Room *pub = new Room("in the campus pub");
        Room *lab = new Room("in a computing lab");
        Room *office = new Room("in the computing admin office");
        
        // initialise room exits
        outside->setExit("east", theater);
        outside->setExit("south", lab);
        outside->setExit("west",  pub);

        theater->setExit("west", outside);

        pub->setExit("east", outside);

        lab->setExit("north", outside);
        lab->setExit("east", office);

        office->setExit("west", lab);

        currentRoom = outside;  // start game outside
}

    /**
     *  Main play routine.  Loops until end of play.
     */
    void Game::play() 
    {            
        printWelcome();

        // Enter the main command loop.  Here we repeatedly read commands and
        // execute them until the game is over.
                
        bool finished = false;
        while (! finished) {
            Command command = parser.getCommand();
            finished = processCommand(command);
        }
        cout << "Thank you for playing.  Good bye." << endl;
    }

    /**
     * Print out the opening message for the player.
     */
    void Game::printWelcome()
    {
        
        cout << "Welcome to the World of Zuul!" << endl;
        cout << "World of Zuul is a new, incredibly boring adventure game." << endl;
        cout << "Type 'help' if you need help." << endl;
        cout << currentRoom->getLongDescription() << endl;
    }

    /**
     * Given a command, process (that is: execute) the command.
     * @param command The command to be processed.
     * @return true If the command ends the game, false otherwise.
     */
    bool Game::processCommand(Command command) 
    {
        bool wantToQuit = false;

        if(command.isUnknown()) {
            cout << "I don't know what you mean..." << endl;
            return false;
        }

        string commandWord = command.getCommandWord();
        if (commandWord == ("help")) {
            printHelp();
        }
        else if (commandWord == ("go")) {
            goRoom(command);
        }
        else if (commandWord == ("quit")) {
            wantToQuit = quit(command);
        }
        // else command not recognised.
        return wantToQuit;
    }

    // implementations of user commands:f

    /**
     * Print out some help information.
     * Here we print some stupid, cryptic message and a list of the 
     * command words.
     */
    void Game::printHelp() 
    {
        
        cout << "You are lost. You are alone. You wander" << endl;
        cout << "around at the university." << endl;
        cout << "Your command words are:" << endl;
        parser.showCommands();
    }

    /** 
     * Try to in to one direction. If there is an exit, enter the new
     * room, otherwise print an error message.
     */
    void Game::goRoom(Command command) 
    {
        if(!command.hasSecondWord()) {
            // if there is no second word, we don't know where to go...
            cout << "Go where?" << endl;
            return;
        }

        string direction = command.getSecondWord();

        // Try to leave current room.
        Room *nextRoom = currentRoom->getExit(direction);

        if (nextRoom == NULL) {
            cout << "There is no door!" << endl;
        }
        else {
            currentRoom = nextRoom;
            cout << currentRoom->getLongDescription() << endl;
        }
    }

    /** 
     * "Quit" was entered. Check the rest of the command to see
     * whether we really quit the game.
     * @return true, if this command quits the game, false otherwise.
     */
    bool Game::quit(Command command) 
    {
        if(command.hasSecondWord()) {
            cout << "Quit what?" << endl;
            return false;
        }
        else {
            return true;  // signal that we want to quit
        }
    }
