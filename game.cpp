#include "game.h"

void Game::initVariables()
{
  this-> window = nullptr;
  this-> isPaused = 0;
  current_tetromino = new Tetromino;
  current_tetromino->move = 0;
  current_tetromino->down = 0;
  current_tetromino->rotate = 0;
  current_tetromino->spawn_loc = rand() % 8;
  current_tetromino->displayblock = rand() % 7;
  this->Spawntimer = 0;
  this->SpawntimerMax = 0.5;
  this->score_factor = 1;

}

void Game::initWindow()
{
  this->window = new sf::RenderWindow(sf::VideoMode(960, 540), "Test", sf::Style::Titlebar | sf::Style::Close);
  this->window->setFramerateLimit(60);
}

void Game::textures()
{
  this->wallpaper.loadFromFile("assets/bgalign.jpg");
  this->vtile[0].loadFromFile("assets/blue.png");
  this->vtile[1].loadFromFile("assets/black.png");
  this->vtile[2].loadFromFile("assets/yellow.png");
  this->vtile[3].loadFromFile("assets/green.png");
  this->vtile[4].loadFromFile("assets/pink.png");
  this->vtile[5].loadFromFile("assets/red.png");
  this->font.loadFromFile("assets/Mandalore.otf");
  this->font1.loadFromFile("assets/BIOWEAPON.otf");
}

Game::Game() //Constructor
{
  this->initVariables();
  this->initWindow();
  this->terminal_log("Game Launched");
  this->textures();
  current_tetromino->createTetromino();
}

Game::~Game() //Destructor
{
  delete this->window;
}

const bool Game::running() const
{
  return this->window->isOpen();
}

void Game::terminal_log(std::string s)
{
  time_t c_time = time(NULL);
  std::cout<<ctime(&c_time)<<s<<"\n\n";
}

void Game::playMusic()
{
  music.openFromFile("assets/dreams.wav");
  music.setLoop(true);
  music.play();
}

void Game::pollEvents()
{
  while(this->window->pollEvent(this->ev))
  {
    switch(this->ev.type)
    {
      case sf::Event::Closed:
        this->window->close(); //window X (close) functionality
        break;
      case sf::Event::KeyPressed:
        if(this->ev.key.code == sf::Keyboard::Escape)
          //Open main menu
          {
            this->isPaused = !this->isPaused;
            if(isPaused)
            {
              this->terminal_log("Game Paused");
              music.pause();
            }
            else music.play();
          }

          //Record controls
        else if(this->ev.key.code == sf::Keyboard::Right)
              current_tetromino->move = 1;
        else if(this->ev.key.code == sf::Keyboard::Left)
               current_tetromino->move = -1;
        else if(this->ev.key.code == sf::Keyboard::Down)
               {
                 current_tetromino->down = 1;
                 this->score_factor = 25;   //score multiplier on acceleration
               }
        else if(this->ev.key.code == sf::Keyboard::Up)
               current_tetromino->rotate = 1;
        break;
    }
  }
}

void Tetromino::createTetromino()
{
  for (int i=0;i<4;i++)
     this->grid[b[i].y][b[i].x] = this->k;

  this->k = 1+ rand() % 5;
  if(!this->isgameover)this->spawn_loc = rand() % 9;

  for (int i = 0; i <4;i++)
  {
    a[i].x = this->shapes[this->displayblock][i] % 2 + this->spawn_loc;
    a[i].y = this->shapes[this->displayblock][i] / 2;
  }
  this->displayblock = rand() % 7;
}

bool Tetromino::collisionDetect()
{
  for(int i = 0; i < 4; i++)
    if( a[i].x < 0 || a[i].x >= COLM || a[i].y >= ROWS) //collision with borders
      return 1;
    else if(this->grid[a[i].y][a[i].x])  //collision with other tetrominos
      return 1;
  return 0;
}

void Tetromino::retreat()  //puts back the current tetromino in previous postion
{
  for(int i = 0;i < 4;i++)
    a[i] = b[i];
}


void Tetromino::moveTetromino() //moves the current tetromino left & right
{                              //also controls downward accelaration
    for(int i = 0;i < 4;i++)
    {
      b[i] = a[i];
      a[i].x += move;
      a[i].y += down;
    }
    if(this->collisionDetect())
      this->retreat();
}

void Tetromino::rotateTetromino() //rotates the current tetromino clockwise
{
    Point p = a[1];
    for(int i = 0; i < 4; i++)
    {
      int x = a[i].y-p.y;
      int y = a[i].x-p.x;
      a[i].x = p.x - x;
      a[i].y = p.y + y;
    }
    if(this->collisionDetect())
      this->retreat();
}

void Tetromino::autodrag()  //moves the current tetromino downward periodically
{
    for(int i = 0;i<4;i++)
    {
      a[i] = b[i];
      a[i].y +=1;
    }
    if(this->collisionDetect())
    {
      this->createTetromino();
      if(this->collisionDetect()) //if tetromino collides upon spawn, end the game
        this->isgameover = 1;
      //this->score+= 100;
      this->checkline();
    }

}

void Tetromino::checkline()
{
  int k=ROWS-1;
  for (int i=ROWS-1;i>0;i--)
  {
    int count=0;
    for (int j=0;j<COLM;j++)
    {
        if (grid[i][j]) count++;
        grid[k][j]=grid[i][j];
    }
    if (count<COLM) {k--;}
    else this->score += 800;    //award 800 points on row completion
  }
}

void Tetromino::updateTetromino()
{
  this->moveTetromino();
  if(this->rotate)
    this->rotateTetromino();
  move = 0;
  down = 0;
  rotate = 0;
}


void Game::renderTetromino()
{
  sf::Sprite s(this->vtile[current_tetromino->k]);
  for (int i=0;i<ROWS;i++)
   for (int j=0;j<COLM;j++)
     {
       if (current_tetromino->grid[i][j]==0) continue;
       s.setTextureRect(sf::IntRect(current_tetromino->grid[i][j]*0,0,41,41));
       s.setPosition(j*41,i*41); //size of block = 41x41
       s.move(281,-36); //Allign to grid
       this->window->draw(s);
     }
  for (int i = 0; i < 4 ;i++)
  {
    s.setTextureRect(sf::IntRect(current_tetromino->k*0,0,41,41));
    s.setPosition(a[i].x*41,a[i].y*41); //size of block = 41x41
    s.move(281,-36); //Allign to grid
    this->window->draw(s);
  }
}

void Game::tick()
{
  float timer = keep.getElapsedTime().asSeconds();
  keep.restart();
  this->Spawntimer+= timer;
}

void Game::Pause()
{
  sf::Text text("Paused",this->font);
  text.setCharacterSize(30);
  text.setPosition(5,0);
  window->draw(text);
}

void Game::displayScore()
{
  int x = static_cast<int>(current_tetromino->score);
  this->score_string = std::to_string(x);
  sf::Text text(this->score_string,this->font);
  text.setCharacterSize(30);
  text.setPosition(800,0);
  window->draw(text);
}

void Game::gameover()
{
  sf::Text text("Game Over",this->font1);
  text.setCharacterSize(50);
  text.setPosition(285,250);
  window->draw(text);
  music.stop();
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
  {
    this->terminal_log("Game Over");
    this->window->close();
  }

}

void Game::update()
{
  this->tick();
  if(this->Spawntimer > this->SpawntimerMax)
    {
      current_tetromino->autodrag();
      this->Spawntimer = 0;
    }
  if(!current_tetromino->isgameover)
    {
      current_tetromino->updateTetromino();
      current_tetromino->score += (this->score_factor*0.5);
      this->score_factor = 1;
    }
}

void Game::render()
{
  this->window->clear();  //clear previous frame

  sf::Sprite background(this->wallpaper);   //render background
  this->window->draw(background);

  this->renderTetromino();  //render blocks
  this->displayScore();
  if(isPaused)
    this->Pause();
  if(current_tetromino->isgameover)
   this->gameover();
  this->window->display(); //display all objects
}
