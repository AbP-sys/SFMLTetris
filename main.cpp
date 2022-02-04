#include "game.cpp"

int main()
{
    std::srand(static_cast<unsigned>(time(NULL))); //initialising seed for random numbers
    std::cout<<"System log\n\n";
    Game tetris;     //creating game object
    tetris.playMusic();

    while(tetris.running())
    {
        tetris.pollEvents(); //process events and keyboard input
        if(!(tetris.isPaused))
          tetris.update();  //update positions of all game objects
        tetris.render();    //displays all game objects
    }
    std::cout<<"End here\n";
    return 0;
}
