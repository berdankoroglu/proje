//olcPixelGameEngine is being developed and shared for education purposes by javidx9 in OneLoneCoder Youtube Channel:
//https://www.youtube.com/channel/UC-yuWVUplUJZvieEligKBkA/about
//olcSimpleEngine is based on olcPixelGameEngine and simplified and modified by Bahadir Pamuk for IE201 course in Industrial Engineering Department of Bogazici University
//olcSimpleEngine.cpp is created in order to resolve object orientation issues, from now on sprites (from olc::Sprite) can be created inside other classes, independently
//olcSimpleEngine.cpp is updated so that they are compatible with shared_ptr's.
//Last Edit Date: 03.02.2020

#include "olcSimpleEngine.h"
#include "olcPGEX_Graphics2D.h"
#include "vector"


using namespace std;

double distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

struct Tile {
    bool IsFull;
    double x, y;
    Tile(int xIn, int yIn) :x(xIn), y(yIn), IsFull(false) {};
};

struct Peashooter {
    double x, y;
    double HP;
    double attackCD, attackRate;

    Peashooter(double xIn, double yIn, double HPIn):attackCD(0),attackRate(3) {
        x = xIn;
        y = yIn;
        HP = HPIn;
    }
};

struct Sunflower {
    double x, y;
    double HP;
    double sunCD, sunRate;

    Sunflower(double xIn, double yIn, double HPIn, double sunRateIn) {
        x = xIn;
        y = yIn;
        HP = HPIn;
        sunCD = 3;
        sunRate =sunRateIn ;
    }
};

struct Sun {
    double x, y;
    int sungain;
    Sun(double xIn, double yIn) :x(xIn), y(yIn), sungain(50) {};
};

struct Zombie {

    double x, y;
    double HP;
    double speed;
    double attackCD, attackRate;
    double damage;

    Zombie(double xIn, double yIn, double HPIn, double speedIn, double attackRateIn) {
    
        x = xIn;
        y = yIn;
        HP = HPIn;
        speed = speedIn;
        attackCD = 0;
        attackRate = attackRateIn;
        damage = 40;

    };
};

struct Card{
    double x, y;
    bool IsSelected;
    Card(double xIn, double yIn) :x(xIn), y(yIn) {
        IsSelected = FALSE;
    };
};

struct Seed {
    double x, y;
    double speed;
    double damage;
    Seed(double xIn, double yIn, double damageIn) : x(xIn), y(yIn), damage(damageIn), speed(50) {};
};




class PlantsGame: public olc::PixelGameEngine {

    double gameTime;
    bool gameEnd;
    bool gamePause;

    bool AnyCardSelected;
    int CurrentSun;
    
    vector<Sun*> Suns;
    vector<Sunflower*> Sunflowers;
    vector <Peashooter*> Peashooters;
    vector <Seed*> Seeds;
    vector<Zombie*> Zombies;
    vector<Tile*> Tiles;
    vector<Card*> Cards;

    
    
    olc::Sprite* zombieSprite;
    olc::Sprite* sunflowerSprite;
    olc::Sprite* peashooterSprite;
    olc::Sprite* sunSprite;
    olc::Sprite* seedSprite;

    
    float spawnCD , spawnRate; //Zombie spawn timers

    int sunflowerCost,peashooterCost;

public:
    bool OnUserCreate() //override
    {
        gameTime = 0;
        gameEnd = false;
        gamePause = false;

        //Create Tiles
        for (int i = 1; i <= 6; i++)
        {
            for (int j = 1; j <= 4; j++)
            {
                Tiles.push_back(new Tile((i-1)*50, j  * 50));
            }

        }
        //Create Tiles
        
        //Create Cards
        Cards.push_back(new Card (0,0));
        Cards.push_back(new Card (50, 0));
        //Create Cards

        //Initialization for zombies
        spawnCD = 3;
        spawnRate = 5;
        zombieSprite = new olc::Sprite("Sprites/zombie1.png");
        //Initialization for zombies

        seedSprite = new olc::Sprite("Sprites/seed.png");
        sunSprite =new olc::Sprite("Sprites/sun1.png");
        peashooterSprite =new olc::Sprite("Sprites/peashooter1.png");
        sunflowerSprite =new olc::Sprite("Sprites/sunflower1.png");
        CurrentSun = 100;
        sunflowerCost = 50;
        peashooterCost = 100;

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) //override
    {
        gameTime += fElapsedTime;

        //GAME END SCREEN
        if (gameEnd) {
            Clear(olc::BLACK);
            DrawString(110, 10, "GAME OVER", olc::WHITE);

            FillRect(100, 90, 100, 50, olc::DARK_GREEN);
            DrawString(110, 110, "PLAY AGAIN", olc::WHITE);
            FillRect(100, 150, 100, 50, olc::DARK_RED);
            DrawString(135, 170, "EXIT", olc::WHITE);
            SetPixelMode(olc::Pixel::ALPHA);
            if (GetMouseX() >= 100 && GetMouseX() <= 200 && GetMouseY() >= 90 && GetMouseY() <= 140)
            {
                FillRect(100, 90, 100, 50, olc::WHITE);
                DrawString(110, 110, "PLAY AGAIN", olc::DARK_GREEN);

                if (GetMouse(0).bReleased)
                {
                    OnUserCreate();

                }
            }
            if (GetMouseX() >= 100 && GetMouseX() <= 200 && GetMouseY() >= 150 && GetMouseY() <= 200)
            {
                FillRect(100, 150, 100, 50, olc::WHITE);
                DrawString(135, 170, "EXIT", olc::DARK_RED);

                if (GetMouse(0).bReleased)
                {
                    delete zombieSprite;
                    delete seedSprite;
                    delete sunSprite;
                    delete peashooterSprite;
                    delete sunflowerSprite;

                    return false;
                }
            }

            return true;
        }
        //GAME END SCREEN


        //GAME PAUSE
        if (GetKey(olc::Key::P).bPressed) {
            gamePause = true;
        }
        if (gamePause == true)
        {
            Clear(olc::BLACK);
            FillRect(100, 100, 100, 50, olc::DARK_GREEN);
            DrawString(90, 10, "PLANTS & ZOMBIES PAUSED", olc::WHITE);
            DrawString(120, 120, "CONTINUE", olc::WHITE);
            SetPixelMode(olc::Pixel::ALPHA);
            if (GetMouseX() >= 100 && GetMouseX() <= 200 && GetMouseY() >= 100 && GetMouseY() <= 150)
            {
                FillRect(100, 100, 100, 50, olc::WHITE);
                DrawString(120, 120, "CONTINUE", olc::DARK_GREEN);

                if (GetMouse(0).bReleased)
                {
                    gamePause = false;
                }
            }
            return true;

        }
        //GAME PAUSE





        //Drawing the background
        Clear(olc::DARK_GREEN);
        FillRect(0, 0, 300, 50, olc::BLACK);
        SetPixelMode(olc::Pixel::ALPHA);
        //Drawing the background

        //Drawing the tiles
        for (auto a:Tiles)
        {
            if (a->IsFull)
            {
                DrawRect(a->x, a->y, 49, 49, olc::BLACK);
            }
            else
            {
                if (GetMouseX() > a->x&& GetMouseX() < a->x + 50 && GetMouseY() > a->y&& GetMouseY() < a->y + 50)
                {
                    DrawRect(a->x, a->y, 49, 49, olc::WHITE);
                }

                else {
                    DrawRect(a->x, a->y, 49, 49, olc::VERY_DARK_GREY);
                }
            }
           
        }
        //Drawing the tiles


        //Drawing the cards
        FillRect(Cards[0]->x, Cards[0]->y, 49, 49, olc::WHITE);
        DrawSprite(Cards[0]->x, Cards[0]->y, sunflowerSprite, 1);
        DrawString(Cards[0]->x + 30, Cards[0]->y + 40, to_string(sunflowerCost) , olc::DARK_RED);

        FillRect(Cards[1]->x, Cards[1]->y, 49, 49, olc::WHITE);
        DrawSprite(Cards[1]->x , Cards[1]->y , peashooterSprite, 1);
        DrawString(Cards[1]->x + 25, Cards[1]->y + 40, to_string(peashooterCost), olc::DARK_RED);

        for (auto a:Cards)
        {
            if (a->IsSelected)
            {
                DrawRect(a->x, a->y, 49, 49, olc::RED);
            }
            else
            {
                if (GetMouseX() > a->x&& GetMouseX() < a->x + 50 && GetMouseY() > a->y&& GetMouseY() < a->y + 50)
                {
                    DrawRect(a->x, a->y, 49, 49, olc::WHITE);
                }

                else 
                {
                    DrawRect(a->x, a->y, 49, 49, olc::BLACK);
                }
            }
        }
        //Drawing the cards

        //Drawing the remaining sun 
        DrawSprite(ScreenWidth() - 50, 0, sunSprite, 1);
        DrawString(ScreenWidth() - 35, 40, to_string(CurrentSun), olc::WHITE);
        //Drawing the remaining sun


        //Zombie Spawns
        if (spawnCD < 0) {
            int s = rand() % Tiles.size();
            Zombies.push_back(new Zombie(ScreenWidth(), Tiles[s]->y, 100, 10, 4));
            spawnCD = spawnRate;
        }
        if (gameTime < 5) { spawnCD -= fElapsedTime; }
        else if (gameTime < 10) { spawnCD -= fElapsedTime*2; }
        else if (gameTime < 15) { spawnCD -= fElapsedTime * 3; }
        else { spawnCD -= fElapsedTime * 4; }
        //Zombie Spawns


        //Zombie Move & Attack
        for (int i = 0; i < Zombies.size(); i++) {
            Zombies[i]->attackCD -= fElapsedTime;
            bool canAttack = false;
            for (int j = 0; j < Peashooters.size(); j++) {
                if (distance(Zombies[i]->x, Zombies[i]->y, Peashooters[j]->x, Peashooters[j]->y) < 5)
                {
                    canAttack = true;
                    if (Zombies[i]->attackCD <= 0) {

                        Peashooters[j]->HP -= Zombies[i]->damage;
                        Zombies[i]->attackCD = Zombies[i]->attackRate;
                    }
                }
            }

            for (int k = 0; k < Sunflowers.size(); k++) {
                if (distance(Zombies[i]->x, Zombies[i]->y, Sunflowers[k]->x, Sunflowers[k]->y) < 5)
                {
                    canAttack = true;
                    if (Zombies[i]->attackCD <= 0) {
                        Zombies[i]->attackCD = Zombies[i]->attackRate;
                        Sunflowers[k]->HP -= Zombies[i]->damage;
                    }
                }
            }

            if (!canAttack) {
                Zombies[i]->x -= Zombies[i]->speed * fElapsedTime;
            }

        }
        //Zombie Move & Attack


        //Plants death
        for (int i = 0; i < Peashooters.size(); i++) {
            if (Peashooters[i]->HP <= 0) {

                for (auto a : Tiles) {
                    if (a->x == Peashooters[i]->x && a->y == Peashooters[i]->y) {
                        a->IsFull = false;
                    }
                }
                delete Peashooters[i];
                Peashooters.erase(Peashooters.begin() + i);
                i--;
            }
        }

        for (int i = 0; i < Sunflowers.size(); i++) {
            if (Sunflowers[i]->HP <= 0) {
                for (auto a : Tiles) {
                    if (a->x == Sunflowers[i]->x && a->y == Sunflowers[i]->y) {
                        a->IsFull = false;
                    }
                }
                Sunflowers.erase(Sunflowers.begin() + i);
                i--;
            }
        }
        //Plants death


        //Peashooter Attack
        for (int i = 0; i < Peashooters.size(); i++) {
            Peashooters[i]->attackCD -= fElapsedTime;
            if (Peashooters[i]->attackCD <= 0) {
                Seeds.push_back(new Seed(Peashooters[i]->x, Peashooters[i]->y, 30));
                Peashooters[i]->attackCD = Peashooters[i]->attackRate;
            }
        }
        //Peashooter Attack

        //Seeds Move & Attack
        for (int i = 0; i< Seeds.size();i++) {
            Seeds[i]->x += Seeds[i]->speed * fElapsedTime;
            for (int j = 0; j < Zombies.size(); j++) {
                if (distance(Seeds[i]->x, Seeds[i]->y, Zombies[j]->x, Zombies[j]->y) < 5) {
                    Zombies[j]->HP -= Seeds[i]->damage;
                    delete Seeds[i];
                    Seeds.erase(Seeds.begin() + i);
                    i--;
                    if (Zombies[j]->HP <= 0) {
                        delete Zombies[j];
                        Zombies.erase(Zombies.begin() + j);
                        j--;
                    }
                    break;


                }

            } 
            
        }
        //Seeds Move & Attack

        //Seeds disappear
        for (int i = 0; i < Seeds.size(); i++) {
            if (Seeds[i]->x >= ScreenWidth()) {
                delete Seeds[i];
                Seeds.erase(Seeds.begin() + i);
                i--;
            }
        }
        //Seeds disappear

        //Sunflower Sun Creation
        for (int i = 0; i < Sunflowers.size();i++) {
            Sunflowers[i]->sunCD -= fElapsedTime;
            if (Sunflowers[i]->sunCD <= 0) {
                Sunflowers[i]->sunCD = Sunflowers[i]->sunRate;
                Suns.push_back(new Sun(Sunflowers[i]->x, Sunflowers[i]->y));
            }
        }
        //Sunflower Sun Creation


        //Mouse operations
        if (GetMouse(0).bPressed) {

            // Selecting-Deselecting Card
            
            //Deselect
            bool anyselection = false;
            for (auto a : Cards) {
                
                anyselection = anyselection || a->IsSelected;
                if (GetMouseX() > a->x&& GetMouseX() < a->x + 50 && GetMouseY() > a->y&& GetMouseY() < a->y + 50) {
                    if (a->IsSelected) { a->IsSelected = false; }
                  
                
                }
                

            }
            //Deselect

            //Select
            if (!anyselection) {
                for (auto a : Cards) {

                    if (GetMouseX() > a->x&& GetMouseX() < a->x + 50 && GetMouseY() > a->y&& GetMouseY() < a->y + 50) {
                        if (!a->IsSelected) { a->IsSelected = true; }
                    }
                }
            }
            //Select
            // Selecting-Deselecting Card



            //Planting 
            for (auto a : Tiles) {
                if (GetMouseX() > a->x&& GetMouseX() < a->x + 50 && GetMouseY() > a->y&& GetMouseY() < a->y + 50) {

                    if (!a->IsFull) {

                        if (Cards[0]->IsSelected && sunflowerCost <= CurrentSun) {

                            Sunflowers.push_back(new Sunflower(a->x , a->y , 100, 5));
                            a->IsFull = TRUE;
                            Cards[0]->IsSelected = false;
                            CurrentSun -= sunflowerCost;

                        }

                        if (Cards[1]->IsSelected && peashooterCost <= CurrentSun) {

                            Peashooters.push_back(new Peashooter(a->x , a->y, 100));
                            a->IsFull = TRUE;
                            Cards[1]->IsSelected = false;
                            CurrentSun -= peashooterCost;
                        }
                    }
                }
            }
            //Planting   


            //Collecting Sun
            for (int i = 0; i < Suns.size();i++) {
                if (distance(Suns[i]->x + 20, Suns[i]->y+20, GetMouseX(), GetMouseY()) < 20) {
                    CurrentSun += Suns[i]->sungain;
                    Suns.erase(Suns.begin() + i);
                    i--;
                }
            }
            //Collecting Sun

            

        }
        //Mouse operation


        //Exit game if pressed E or any zombie passes the left line
        if (GetKey(olc::E).bPressed ) {
            gameEnd = true;
        }

        for (auto a : Zombies) {
            if (a->x <= 0) { gameEnd = true; }
        }

        if (gameEnd) {

            for (int i = 0; i < Zombies.size(); i++) {
                delete Zombies[i];
                Zombies.erase(Zombies.begin() + i);
                i--;
            }

            for (int i = 0; i < Peashooters.size(); i++) {
                delete Peashooters[i];
                Peashooters.erase(Peashooters.begin() + i);
                i--;
            }

            for (int i = 0; i < Sunflowers.size(); i++) {
                delete Sunflowers[i];
                Sunflowers.erase(Sunflowers.begin() + i);
                i--;
            }

            for (int i = 0; i < Suns.size(); i++) {
                delete Suns[i];
                Suns.erase(Suns.begin() + i);
                i--;
            }

            for (int i = 0; i < Seeds.size(); i++) {
                delete Seeds[i];
                Seeds.erase(Seeds.begin() + i);
                i--;
            }

            
            for (int i = 0; i < Tiles.size(); i++) {
                delete Tiles[i];
                Tiles.erase(Tiles.begin() + i);
                i--;
            }

            for (int i = 0; i < Cards.size(); i++) {
                delete Cards[i];
                Cards.erase(Cards.begin() + i);
                i--;
            }

            return true;
        }

        //Exit game if pressed E or any zombie passes the left line



       

        

        //Peashooter Draw
        for (auto i : Peashooters) {
            DrawSprite(i->x, i->y, peashooterSprite, 1);
        }
        //Peashooter Draw

        //Sunflower Draw
        for (auto i : Sunflowers) {
            DrawSprite(i->x, i->y, sunflowerSprite, 1);
        }
        //Sunflower Draw
        
        //Zombie Draw
        for (auto i : Zombies) {
            DrawSprite(i->x, i->y, zombieSprite, 1);
        }
        //Zombie Draw
        
        //Sun Draw
        for (auto i : Suns) {
            DrawSprite(i->x, i->y, sunSprite, 1);
        }
        //Sun Draw

        //Seed Draw
        for (auto i : Seeds) {
            DrawSprite(i->x, i->y, seedSprite, 1);
        }
        //Seed Draw

        return true;
    }

};






int main()
{
    PlantsGame demo;
    if (demo.Construct(300, 250, 3, 3))
        demo.Start();

    return 0;
}