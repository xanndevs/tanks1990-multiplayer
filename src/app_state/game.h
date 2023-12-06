#ifndef GAME_H
#define GAME_H
#include "appstate.h"
#include "../objects/object.h"
#include "../objects/player.h"
#include "../objects/enemy.h"
#include "../objects/bullet.h"
#include "../objects/brick.h"
#include "../objects/eagle.h"
#include "../objects/bonus.h"
#include <vector>
#include <string>

/**
 * @brief Class responsible for the movement of all tanks and interactions between tanks and other objects on the map.
 */
class Game : public AppState
{
public:
    /**
     * Default constructor - allows for a single-player game.
     */
    Game();
    /**
     * Constructor allowing to specify the initial number of players. The number of players can be 1 or 2; any other value will result in a single-player game.
     * This constructor is called in @a Menu::nextState.
     * @param players_count - number of players (1 or 2)
     */
    Game(int players_count);
    /**
     * Constructor taking existing players. Called in @a Score::nextState.
     * @param players - container with players
     * @param previous_level - variable holding the number of the previous level
     */
    Game(std::vector<Player *> players, int previous_level);

    ~Game();
    /**
     * Function returns @a true if the player destroyed all enemies or the eagle was hit, or the player lost all lives (resulting in a loss).
     * @return @a true or @a false
     */
    bool finished() const;
    /**
     * Function displays the round number at the beginning. During the gameplay, it is responsible for drawing the level (bricks, stones, water, ice, bushes),
     * players, enemies, bonuses, eagle, and game status on the panel on the right (remaining enemies, remaining lives of players, round number).
     * After a loss or during a pause, it displays the appropriate information in the center of the screen.
     */
    void draw();
    /**
     * Function updates the state of all objects on the board (tanks, bonuses, obstacles). It also checks collisions between tanks, between tanks and level elements, and between bullets and tanks and map elements.
     * It involves removing destroyed objects, adding new enemy tanks, and checking the conditions for ending the round.
     * @param dt - time since the last function call in milliseconds
     */
    void update(Uint32 dt);
    /**
     * Reacts to keys:
     * @li Enter - pause the game
     * @li Esc - return to the menu
     * @li N - go to the next round if the game is not lost
     * @li B - go to the previous round if the game is not lost
     * @li T - show paths to enemy tanks
     * @param ev - pointer to SDL_Event union storing the type and parameters of various events, including keyboard events
     */
    void eventProcess(SDL_Event *ev);
    /**
     * Switch to the next state.
     * @return pointer to the @a Scores class object if the player completed the round or lost. If the player pressed Esc, the function returns a pointer to the @a Menu object.
     */
    AppState *nextState();

private:
    /**
     * Load the level map from a file.
     * @param path - path to the map file
     */
    void loadLevel(std::string path);
    /**
     * Clear remaining enemies, players, map objects, and bonuses.
     */
    void clearLevel();
    /**
     * Load a new level and create new players if they do not exist yet.
     * @see Game::loadLevel(std::string path)
     */
    void nextLevel();
    /**
     * Create a new enemy if the number of enemies on the board is less than 4, assuming that not all 20 enemies on the map have been created yet.
     * The function generates different levels of enemy armor depending on the level; the higher the round number, the greater the chance that the enemy will have the fourth level of armor.
     * The armor level indicates how many times a bullet must hit the enemy to destroy it. The level ranges from 1 to 4 and corresponds to different tank colors.
     * The generated enemy has an additional chance that hitting it will spawn a bonus on the board.
     */
    void generateEnemy();
    /**
     * Generate a random bonus on the map and place it in a position that does not collide with the eagle.
     */
    void generateBonus();

    /**
     * Check if the tank can move forward freely; if not, stop it. The function does not allow going off the board.
     * If the tank enters the ice, it causes slipping. If the tank has the "Boat" bonus, it can pass through water. Tanks cannot pass through the eagle.
     * @param tank - tank for which collisions are checked
     * @param dt - the last change in time; assuming small changes in subsequent time steps, we can predict the tank's next position and react accordingly.
     */
    void checkCollisionTankWithLevel(Tank *tank, Uint32 dt);

    /**
     * Check if there is no collision between the two tanks being examined; if so, both are stopped.
     * @param tank1
     * @param tank2
     * @param dt
     */
    void checkCollisionTwoTanks(Tank *tank1, Tank *tank2, Uint32 dt);

    /**
     * Check if the selected bullet does not collide with any map element (water and ice are ignored). If so, destroy the bullet and the object.
     * If the eagle is hit, it results in a loss.
     * @param bullet - bullet
     */
    void checkCollisionBulletWithLevel(Bullet *bullet);

    /**
     * Check the collision of the bullet with the bushes on the map. Destruction of bushes and bullets occurs when the bullet has increased damage.
     * @param bullet - bullet
     * @see Bullet::increased_damage
     */
    void checkCollisionBulletWithBush(Bullet *bullet);

    /**
     * Check if a player has hit a selected enemy. If so, the player gets points and the enemy loses one level of armor.
     * @param player - player
     * @param enemy - enemy
     */
    void checkCollisionPlayerBulletsWithEnemy(Player *player, Enemy *enemy);

    /**
     * Check if an enemy has hit a player with a bullet. If so, the player loses one life unless they had a shield.
     * @param enemy - enemy
     * @param player - player
     */
    void checkCollisionEnemyBulletsWithPlayer(Enemy *enemy, Player *player);

    /**
     * If two bullets collide, both are destroyed.
     * @param bullet1
     * @param bullet2
     */
    void checkCollisionTwoBullets(Bullet *bullet1, Bullet *bullet2);

    /**
     * Check if the player has picked up a bonus. If so, the appropriate reaction occurs:
     * @li Grenade - visible enemies are destroyed
     * @li Helmet - the player gets temporary protection from all bullets
     * @li Clock - stop visible enemies
     * @li Shovel - temporarily create a stone wall around the eagle
     * @li Tank - increase the player's life count by 1
     * @li Star - upgrade the player's tank (increase speed, number of bullets)
     * @li Weapon - maximum upgrade for the player
     * @li Boat - the ability to pass through water
     * Upon receiving a bonus, the player receives additional points.
     * @param player
     * @param bonus
     */
    void checkCollisionPlayerWithBonus(Player *player, Bonus *bonus);

    /**
     * Number of columns in the map grid.
     */
    int m_level_columns_count;
    /**
     * Number of rows in the map grid.
     */
    int m_level_rows_count;
    /**
     * Obstacles on the map.
     */
    std::vector<std::vector<Object *>> m_level;
    /**
     * Bushes on the map.
     */
    std::vector<Object *> m_bushes;

    /**
     * Set of enemies.
     */
    std::vector<Enemy *> m_enemies;
    /**
     * Set of remaining players.
     */
    std::vector<Player *> m_players;
    /**
     * Set of killed players.
     */
    std::vector<Player *> m_killed_players;
    /**
     * Set of bonuses on the map.
     */
    std::vector<Bonus *> m_bonuses;
    /**
     * Eagle object.
     */
    Eagle *m_eagle;

    /**
     * Current level number.
     */
    int m_current_level;
    /**
     * Number of players in the selected game mode: 1 or 2.
     */
    int m_player_count;
    /**
     * Number of remaining enemies to kill on the current level.
     */
    int m_enemy_to_kill;

    /**
     * Variable indicating whether the level start screen is currently displayed.
     */
    bool m_level_start_screen;
    /**
     * Variable indicating whether the eagle is protected by a stone wall.
     */
    bool m_protect_eagle;
    /**
     * Time the level start screen has been displayed.
     */
    Uint32 m_level_start_time;
    /**
     * Time since the last enemy creation.
     */
    Uint32 m_enemy_redy_time;
    /**
     * Time elapsed since winning the map.
     */
    Uint32 m_level_end_time;
    /**
     * Time the eagle has been protected by a stone wall.
     */
    Uint32 m_protect_eagle_time;

    /**
     * Loss state.
     */
    bool m_game_over;
    /**
     * Position of the "GAME OVER" text if @a m_game_over is true.
     */
    double m_game_over_position;
    /**
     * Variable indicating whether to end the current game state and switch to displaying scores or the game menu.
     */
    bool m_finished;
    /**
     * Variable indicating whether pause is enabled.
     */
    bool m_pause;
    /**
     * Counts how long it has been paused for.
     */
    Uint32 m_pauseTime;
    /**
     * Number of the position of the newly created enemy. Changed with each enemy creation.
     */
    int m_enemy_respawn_position;
};

#endif // GAME_H
