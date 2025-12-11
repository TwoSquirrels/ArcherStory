#pragma once
#include "DxLib.h"
#include <vector>
#include <string>
#include <fstream>
#include "ExternalHeaderFiles/strconv.h"
#include "ExternalHeaderFiles/json.hpp"
#include "input.hpp"
#include "pos.hpp"
#include "player.hpp"
#include "arrow.hpp"
#include "map.hpp"
// monsters
#include "monsters/ball.hpp"
#include "monsters/flower_plant.hpp"
#include "monsters/slime.hpp"
#include "monsters/golem.hpp"
#include "monsters/bat.hpp"
#include "monsters/tree.hpp"
#include "monsters/virus.hpp"

using json = nlohmann::json;

/**
 * @brief Main game class that manages the game loop and scenes
 * 
 * This class handles game initialization, scene management, 
 * entity updates, and rendering.
 */
class Game {

private:

    /**
     * @brief Game scene enumeration
     */
    enum scene {
        INTRO,          // Title screen
        STAGE,          // Main gameplay
        PAUSE,          // Pause menu (not currently used)
        SKILL_SELECT,   // Skill selection between stages
        DIE             // Game over screen
    };

    json Config;
    
    /**
     * @brief Load file content as string
     * @param FilePath Path to file
     * @return File contents or empty string on error
     */
    std::string FileToString(std::string FilePath);

    bool Debug;
    int Frame;
    std::string ConfigFilePath;
    input Input;

    scene Scene = INTRO;

    map Map;
    player Player;
    bool Death = false;
    bool Next = false;
    std::vector<arrow> Arrow;

    int ClearCount = 0;
    std::vector<monster *> Monster;
    std::vector<ball> Ball;

    // Monster vectors by type
    std::vector<flower_plant> FlowerPlant;
    std::vector<slime> Slime;
    std::vector<golem> Golem;
    std::vector<bat> Bat;
    std::vector<tree> Tree;
    std::vector<virus> Virus;

    /**
     * @brief Load resources and initialize game
     * @return 0 on success, -1 on failure
     */
    int Load();
    
    /**
     * @brief Update game state for one frame
     * @return false to exit game, true to continue
     */
    bool Update();
    
    /**
     * @brief Cleanup and unload resources
     * @param Error Whether cleanup is due to an error
     */
    void Unload(bool Error);

    int StartGraph;
    
    /**
     * @brief Update and render intro/title scene
     * @return true to continue
     */
    bool Intro();
    
    int BeforeIntroFrame = 0;
    
    /**
     * @brief Update and render main game stage
     * @return true to continue
     */
    bool Stage();
    
    int FadeOutCount = 0;
    
    /**
     * @brief Update and render pause menu
     * @return true to continue
     */
    bool Pause();
    
    /**
     * @brief Update and render skill selection scene
     * @return true to continue
     */
    bool SkillSelect();
    
    int EndGraph;
    
    /**
     * @brief Update and render game over scene
     * @return true to continue
     */
    bool Die();
    
public:

    /**
     * @brief Construct game and run main loop
     * @param Debug Enable debug mode
     * @param ConfigFilePath Path to configuration JSON file
     */
    Game(bool Debug = false, std::string ConfigFilePath = "config.json");

};
