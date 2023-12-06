#include "appconfig.h"
#include <iostream>
#include <cstdlib>  // For std::getenv on Linux
#include <cstring>  // For std::strrchr on Linux
#include <limits.h> // For PATH_MAX on Linux
#ifdef _WIN32
#include <windows.h>
#endif

// This code is meant to fix the commandline execution problem
std::string GetExecutionDirectory()
{
    std::string directory;
#ifdef _WIN32
    // Windows
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);

    // Extract the directory
    std::string executablePath(buffer);
    size_t lastSlash = executablePath.find_last_of("\\/");
    directory = executablePath.substr(0, lastSlash + 1);
#else
    // Linux
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1)
    {
        buffer[len] = '\0';

        // Extract the directory
        char *lastSlash = std::strrchr(buffer, '/');
        if (lastSlash != nullptr)
        {
            *lastSlash = '\0';
            directory = buffer;
        }
    }
#endif
    return directory;
}
std::string dir = GetExecutionDirectory();
string AppConfig::texture_path = dir + "/texture.png";
string AppConfig::levels_path = dir + "/levels/";
string AppConfig::font_name = dir + "/prstartk.ttf";
string AppConfig::game_over_text = "GaMe OvEr";
SDL_Rect AppConfig::map_rect = {0, 0, 26 * 16, 26 * 16};
SDL_Rect AppConfig::status_rect = {26 * 16, 0, 3 * 16, AppConfig::map_rect.h};
SDL_Rect AppConfig::windows_rect = {0, 0, AppConfig::map_rect.w + AppConfig::status_rect.w, AppConfig::map_rect.h};
SDL_Rect AppConfig::tile_rect = {0, 0, 16, 16};
// Macbook default keyboard does not has a right ctrl key
#if defined(__APPLE__) && defined(__MACH__)
#define P1_FIRE_KEY SDL_SCANCODE_RALT
#define P2_FIRE_KEY SDL_SCANCODE_LALT
#else
#define P1_FIRE_KEY SDL_SCANCODE_RCTRL
#define P2_FIRE_KEY SDL_SCANCODE_LCTRL
#endif

vector<SDL_Point> AppConfig::player_starting_point =
    []
{
    vector<SDL_Point> v;
    v.push_back({128, 384});
    v.push_back({256, 384});
    return v;
}();
vector<SDL_Point> AppConfig::enemy_starting_point =
    []
{
    vector<SDL_Point> v;
    v.push_back({1, 1});
    v.push_back({192, 1});
    v.push_back({384, 1});
    return v;
}();
vector<Player::PlayerKeys> AppConfig::player_keys =
    []
{
    vector<Player::PlayerKeys> v;
    v.push_back({SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, P1_FIRE_KEY});
    v.push_back({SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D, P2_FIRE_KEY});
    return v;
}();
unsigned AppConfig::level_start_time = 2000;
unsigned AppConfig::slip_time = 380;
unsigned AppConfig::enemy_start_count = 20;
unsigned AppConfig::enemy_redy_time = 500;
unsigned AppConfig::player_bullet_max_size = 1;
unsigned AppConfig::score_show_time = 3000;
unsigned AppConfig::bonus_show_time = 10000;
unsigned AppConfig::tank_shield_time = 10000;
unsigned AppConfig::tank_frozen_time = 8000;
unsigned AppConfig::level_end_time = 5000;
unsigned AppConfig::protect_eagle_time = 15000;
unsigned AppConfig::bonus_blink_time = 350;
unsigned AppConfig::player_reload_time = 120;
int AppConfig::enemy_max_count_on_map = 4;
double AppConfig::game_over_entry_speed = 0.13;
double AppConfig::tank_default_speed = 0.06;
double AppConfig::bullet_default_speed = 0.4;
bool AppConfig::show_enemy_target = false;
