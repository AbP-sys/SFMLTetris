#ifndef game_h
#define game_h
#define ROWS 13
#define COLM 10

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<ctime>
#include<iostream>

class Tetromino;

class Game
{

private:
  sf::RenderWindow* window;
  sf::Event ev;
  Tetromino* current_tetromino;

  void initVariables();
  void initWindow();

  void tick();            //counts time
  float Spawntimer;
  float SpawntimerMax;
  void textures();
  void Pause();
  void displayScore();
  void terminal_log(std::string);


  std::string score_string;
  sf::Font font;
  sf::Font font1;

  sf::Texture wallpaper;
  sf::Texture tile0,tile1,tile2,tile3,tile4,tile5;
  sf::Texture vtile[6] = {tile0,tile1,tile2,tile3,tile4,tile5};
  float score_factor;

  void renderTetromino();

public:
  Game();             //constructor and deconstructor
  virtual ~Game();

  const bool running() const;

  void pollEvents();

  bool isPaused;
  void gameover();
  void playMusic();
  void update();
  void render();
};


class Tetromino
{

private:
  //Map tetrominos on a 2x8 grid
  int shapes[7][4] =
  {
    {1,3,5,7},
    {2,4,5,7},
    {3,5,4,6},
    {3,5,4,7},
    {2,3,5,7},
    {3,5,7,6},
    {2,3,4,5}
  };

  bool collisionDetect();   //detects collision of current block
  void retreat();           //puts back the current block in previous location

public:

  void createTetromino();
  void moveTetromino();
  void rotateTetromino();
  void autodrag();
  void updateTetromino();
  void checkline();

  int grid[ROWS][COLM] = {0};
  int displayblock;
  int move;
  int down;
  bool rotate;
  bool isgameover = 0;

  int spawn_loc;
  int k = 0;
  float score =0;

};

struct Point
{int x,y;} a[4], b[4];


sf::Clock keep;
sf::Music music;


#endif
