#include "Game.hpp"
#include <algorithm> // for std::remove_if

Game::Game(bool Debug, std::string ConfigFilePath) {

    // Initialize member variables
    this->Debug = Debug;
    this->ConfigFilePath = ConfigFilePath;
    this->Frame = 0;

    // Enable debug logging if in debug mode
    SetOutApplicationLogValidFlag(this->Debug);

    bool Error = false;

    // Load resources and initialize game
    if (this->Load() == -1) {
        return; // Initialization failed
    }
    
    // Main game loop
    while (this->Update()) {
        this->Input.Update();
        
        // Check for window messages (returns non-zero on close)
        if (DxLib::ProcessMessage()) {
            Error = true;
            break;
        }
        
        this->Frame++;
    }
    
    // Cleanup
    this->Unload(Error);

}

int Game::Load() {

    // Load and parse JSON configuration file
    std::string JsonString = this->FileToString(this->ConfigFilePath);
    if (JsonString.empty()) {
        JsonString = "{}";
    }
    this->Config = json::parse(JsonString, nullptr, false, true);

    // Set default values for missing configuration entries
    // Window settings
    if (this->Config["WindowName"].empty()) {
        this->Config["WindowName"] = "�A�[�`���[����"; // Archer Story
    } else {
        this->Config["WindowName"] = utf8_to_sjis(this->Config["WindowName"].get<std::string>());
    }
    if (this->Config["FullScreen"].empty()) this->Config["FullScreen"] = false;
    if (this->Config["WindowExtendRate"].empty()) this->Config["WindowExtendRate"] = 1.0;
    
    // Player settings
    if (this->Config["Player"]["JoystickSize"].empty()) this->Config["Player"]["JoystickSize"] = 64;
    if (this->Config["Player"]["Speed"].empty()) this->Config["Player"]["Speed"] = 7.0;
    if (this->Config["Player"]["DefaultMaxHP"].empty()) this->Config["Player"]["DefaultMaxHP"] = 500;
    if (this->Config["Player"]["GodTimeMax"].empty()) this->Config["Player"]["GodTimeMax"] = 15;
    if (this->Config["Player"]["AttackCooldownMax"].empty()) this->Config["Player"]["AttackCooldownMax"] = 36;
    if (this->Config["Player"]["DefaultAttack"].empty()) this->Config["Player"]["DefaultAttack"] = 40;
    if (this->Config["Player"]["DefaultDefense"].empty()) this->Config["Player"]["DefaultDefense"] = 0;
    if (this->Config["Player"]["Arrow"]["Speed"].empty()) this->Config["Player"]["Arrow"]["Speed"] = 8.0;
    
    // Monster settings
    if (this->Config["Monsters"]["FlowerPlant"]["AttackSpeed"].empty()) {
        this->Config["Monsters"]["FlowerPlant"]["AttackSpeed"] = 120;
    }
    if (this->Config["Monsters"]["Slime"]["MoveCooldown"].empty()) {
        this->Config["Monsters"]["Slime"]["MoveCooldown"] = 60;
    }
    if (this->Config["Monsters"]["Golem"]["AttackSpeed"].empty()) {
        this->Config["Monsters"]["Golem"]["AttackSpeed"] = 120;
    }
    if (this->Config["Monsters"]["Bat"]["AttackSpeed"].empty()) {
        this->Config["Monsters"]["Bat"]["AttackSpeed"] = 90;
    }
    if (this->Config["Monsters"]["Tree"]["AttackSpeed"].empty()) {
        this->Config["Monsters"]["Tree"]["AttackSpeed"] = 180;
    }
    
    // Ball settings
    if (this->Config["Balls"]["Stone"]["Speed"].empty()) this->Config["Balls"]["Stone"]["Speed"] = 8.0;
    if (this->Config["Balls"]["Jump"]["High"].empty()) this->Config["Balls"]["Jump"]["High"] = 64.0;
    if (this->Config["Balls"]["Jump"]["Speed"].empty()) this->Config["Balls"]["Jump"]["Speed"] = 8.0;
    if (this->Config["Balls"]["Beam"]["Speed"].empty()) this->Config["Balls"]["Beam"]["Speed"] = 8.0;

    // Initialize window and graphics
    constexpr int SCREEN_WIDTH = 1280;
    constexpr int SCREEN_HEIGHT = 720;
    constexpr int COLOR_DEPTH = 16;
    
    DxLib::SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_DEPTH);
    DxLib::SetDrawScreen(DX_SCREEN_BACK);
    DxLib::ChangeWindowMode(!this->Config["FullScreen"].get<bool>());
    DxLib::SetMainWindowText(this->Config["WindowName"].get<std::string>().c_str());
    DxLib::SetWindowSizeChangeEnableFlag(TRUE);
    DxLib::SetWindowSizeExtendRate(this->Config["WindowExtendRate"].get<double>());
    DxLib::SetUseTransColor(FALSE);
    
    if (DxLib_Init() == -1) {
        return -1; // Initialization failed
    }
    
    DxLib::SetMouseDispFlag(TRUE);

    return 0;

}

bool Game::Update() {
    // ESC key exits the game
    if (this->Input.GetKey(KEY_INPUT_ESCAPE)) {
        return false;
    }
    
    // Update current scene
    bool Continue = false;
    switch (this->Scene) {
    case INTRO:
        Continue = this->Intro();
        break;
    case STAGE:
        Continue = this->Stage();
        break;
    case PAUSE:
        Continue = this->Pause();
        break;
    case SKILL_SELECT:
        Continue = this->SkillSelect();
        break;
    case DIE:
        Continue = this->Die();
        break;
    default:
        Continue = false;
        break;
    }
    return Continue;
}

void Game::Unload(bool Error) {
    // Cleanup is handled by DxLib
    // Note: Monster cleanup commented out - consider using smart pointers
    // for (monster *m: this->Monster) free(m);
    DxLib::DxLib_End();
}

bool Game::Intro() {

    // Only initialize once (on first frame of intro scene)
    if (BeforeIntroFrame + 1 != Frame) {

        // Load start and end screen graphics
        this->StartGraph = DxLib::LoadGraph("data/stable/img/start.png");
        DxLib::DrawGraph(0, 0, this->StartGraph, FALSE);
        this->EndGraph = DxLib::LoadGraph("data/stable/img/end.png");

        // Load all game graphics
        std::map<std::string, std::map<std::string, int>> Graph;
        
        // Map graphics
        Graph["map"]["ground"] = DxLib::LoadGraph("data/stable/img/map/ground.png");
        Graph["map"]["wall"] = DxLib::LoadGraph("data/stable/img/map/wall.png");
        Graph["map"]["stone"] = DxLib::LoadGraph("data/stable/img/map/stone.png");
        
        // Load pond graphics (divided 3x3 grid)
        constexpr int POND_GRID_SIZE = 3;
        constexpr int POND_TILE_SIZE = 48 / 2;
        int PondGraph[POND_GRID_SIZE * POND_GRID_SIZE];
        DxLib::LoadDivGraph("data/stable/img/map/pond.png", 
                           POND_GRID_SIZE * POND_GRID_SIZE, 
                           POND_GRID_SIZE, POND_GRID_SIZE, 
                           POND_TILE_SIZE, POND_TILE_SIZE, 
                           PondGraph);
        
        // Assign pond tiles to graph map
        for (int row = 0; row < POND_GRID_SIZE; row++) {
            for (int col = 0; col < POND_GRID_SIZE; col++) {
                std::string key = "pond" + std::to_string(row) + std::to_string(col);
                Graph["map"][key] = PondGraph[row * POND_GRID_SIZE + col];
            }
        }
        
        Graph["map"]["shutter"] = DxLib::LoadGraph("data/stable/img/map/shutter.png");
        Graph["map"]["shutter_gate"] = DxLib::LoadGraph("data/stable/img/map/shutter_gate.png");
        
        // Player graphics
        Graph["player"]["ring"] = DxLib::LoadGraph("data/stable/img/player/ring.png");
        Graph["player"]["right"] = DxLib::LoadGraph("data/stable/img/player/right.png");
        Graph["player"]["joystick_base"] = DxLib::LoadGraph("data/stable/img/player/joystick_base.png");
        Graph["player"]["joystick_stick"] = DxLib::LoadGraph("data/stable/img/player/joystick_stick.png");
        Graph["player"]["arrow_arrow"] = DxLib::LoadGraph("data/stable/img/player/arrow.png");
        
        // Monster graphics
        Graph["monsters"]["ring"] = DxLib::LoadGraph("data/stable/img/monsters/ring.png");
        Graph["monsters"]["flower_plant_normal"] = DxLib::LoadGraph("data/stable/img/monsters/flower_plant_normal.png");
        Graph["monsters"]["flower_plant_attack"] = DxLib::LoadGraph("data/stable/img/monsters/flower_plant_attack.png");
        Graph["monsters"]["slime"] = DxLib::LoadGraph("data/stable/img/monsters/slime.png");
        Graph["monsters"]["slime_lvlup"] = DxLib::LoadGraph("data/stable/img/monsters/slime_lvlup.png");
        Graph["monsters"]["golem"] = DxLib::LoadGraph("data/stable/img/monsters/golem.png");
        Graph["monsters"]["bat"] = DxLib::LoadGraph("data/stable/img/monsters/bat.png");
        Graph["monsters"]["tree"] = DxLib::LoadGraph("data/stable/img/monsters/tree.png");
        Graph["monsters"]["virus"] = DxLib::LoadGraph("data/stable/img/monsters/virus.png");
        Graph["monsters"]["virus_lvlup"] = DxLib::LoadGraph("data/stable/img/monsters/virus_lvlup.png");
        
        // Ball projectile graphics
        Graph["monsters"]["ball_stone"] = DxLib::LoadGraph("data/stable/img/monsters/ball/stone.png");
        Graph["monsters"]["ball_jumping"] = DxLib::LoadGraph("data/stable/img/monsters/ball/jumping.png");
        Graph["monsters"]["ball_shadow"] = DxLib::LoadGraph("data/stable/img/monsters/ball/shadow.png");
        Graph["monsters"]["ball_beam"] = DxLib::LoadGraph("data/stable/img/monsters/ball/beam.png");

        // Create fonts
        std::map<std::string, int> Font;
        Font["Parameters"] = CreateFontToHandle("ParametersFont", 24, 1, DX_FONTTYPE_NORMAL);
        Font["NewSkill"] = CreateFontToHandle("ParametersFont", 48, 1, DX_FONTTYPE_NORMAL);

        // Load map data
        json Maps;
        std::string JsonString = this->FileToString("data/stable/map.json");
        if (JsonString.empty()) {
            JsonString = "{}";
        }
        Maps = json::parse(JsonString, nullptr, false, true);

        // Initialize game objects
        this->Input = input(false);
        this->Map = map(Maps, Graph, Font, 
                       &this->FlowerPlant, &this->Slime, &this->Golem, 
                       &this->Bat, &this->Tree, &this->Virus, 
                       &this->Ball, &this->Player, this->Config);
        this->Player = player(&this->Input, &this->Map, &this->Arrow, 
                             &this->Death, &this->Next, &this->Monster, 
                             Graph["player"], Font, this->Config["Player"]);

        // Debug: Spawn test enemies (commented out)
        // for (int i = 0; i < 8; i++) {
        //     this->Virus.push_back(virus(
        //         pos(48.0 + DxLib::GetRand(1072), 48.0 + DxLib::GetRand(496)), 
        //         100, 2, &this->Map, &this->Player, Graph["monsters"]
        //     ));
        // }

    }

    // Input handling - press SPACE or click to start game
    if (this->Input.GetKey(KEY_INPUT_SPACE) || this->Input.GetMouseDown(MOUSE_INPUT_LEFT)) {
        this->Scene = this->STAGE;
    }

    // Rendering
    DxLib::ClearDrawScreen();
    DxLib::DrawGraph(0, 0, this->StartGraph, FALSE);
    DxLib::ScreenFlip();

    this->BeforeIntroFrame = this->Frame;

    return true;

}

bool Game::Stage() {

    constexpr int ARROW_SPEED_MULTIPLIER = 4;
    constexpr int CLEANUP_INTERVAL = 60; // frames (1 second at 60 FPS)
    constexpr int CLEAR_DELAY = 2; // frames

    // Update monster list with all active monsters
    this->Monster.clear();
    this->Monster.reserve(
        this->FlowerPlant.size() + this->Slime.size() + this->Golem.size() +
        this->Bat.size() + this->Tree.size() + this->Virus.size()
    );
    
    for (auto& fp : this->FlowerPlant) this->Monster.push_back(fp.Monster);
    for (auto& s : this->Slime) this->Monster.push_back(s.Monster);
    for (auto& g : this->Golem) this->Monster.push_back(g.Monster);
    for (auto& b : this->Bat) this->Monster.push_back(b.Monster);
    for (auto& t : this->Tree) this->Monster.push_back(t.Monster);
    for (auto& v : this->Virus) this->Monster.push_back(v.Monster);
    
    // Debug functions
    if (this->Debug) {
        // Kill all monsters (K key)
        if (this->Input.GetKey(KEY_INPUT_K)) {
            for (auto* m : this->Monster) {
                m->HP = 0;
                m->Use = false;
            }
        }
        // Double speed (S key)
        if (this->Input.GetKey(KEY_INPUT_S)) {
            this->Player.Update();
        }
    }
    
    // Update player
    this->Player.Update();
    
    // Update arrows (4x speed multiplier)
    for (int j = 0; j < ARROW_SPEED_MULTIPLIER; j++) {
        for (size_t i = 0; i < this->Arrow.size(); i++) {
            std::vector<arrow> ArrowToAdd = this->Arrow[i].Update();
            this->Arrow.insert(this->Arrow.end(), ArrowToAdd.begin(), ArrowToAdd.end());
        }
    }
    
    // Update monsters
    for (auto& fp : this->FlowerPlant) fp.Update();
    
    for (auto& s : this->Slime) {
        std::vector<slime> SlimeToAdd = s.Update();
        this->Slime.insert(this->Slime.end(), SlimeToAdd.begin(), SlimeToAdd.end());
    }
    
    for (auto& g : this->Golem) g.Update();
    for (auto& b : this->Bat) b.Update();
    for (auto& t : this->Tree) t.Update();
    
    for (auto& v : this->Virus) {
        std::vector<virus> VirusToAdd = v.Update();
        this->Virus.insert(this->Virus.end(), VirusToAdd.begin(), VirusToAdd.end());
    }
    
    // Update enemy projectiles
    for (auto& ball : this->Ball) ball.Update();
    
    // Cleanup unused objects (every second)
    if (this->Frame % CLEANUP_INTERVAL == 0) {
        // Remove unused balls
        this->Ball.erase(
            std::remove_if(this->Ball.begin(), this->Ball.end(),
                [](const ball& b) { return !b.Use; }),
            this->Ball.end()
        );
        
        // Remove unused arrows
        this->Arrow.erase(
            std::remove_if(this->Arrow.begin(), this->Arrow.end(),
                [](const arrow& a) { return !a.Use; }),
            this->Arrow.end()
        );
    }
    
    // Check for player death
    if (this->Death) {
        this->Death = false;
        this->Scene = this->DIE;
        this->FadeOutCount = 0;
    }
    
    // Check for stage clear (all monsters defeated)
    this->ClearCount++;
    for (const auto* m : this->Monster) {
        if (m->Use) {
            this->ClearCount = 0;
            break;
        }
    }
    
    if (this->ClearCount == 0) {
        this->Map.ClearCancel();
    } else if (this->ClearCount >= CLEAR_DELAY) {
        this->Map.Clear();
        this->ClearCount = 0;
    }
    
    // Check for gate passage (next stage)
    if (this->Next) {
        this->Next = false;
        this->Scene = this->SKILL_SELECT;
    }

    // Rendering
    DxLib::ClearDrawScreen();
    const int Scroll = static_cast<int>(this->Player.Sprite.Pos.GetX() - this->Player.StartPos.GetX());
    
    // Draw background
    this->Map.Draw(Scroll);
    
    // Draw arrows
    for (const auto& arr : this->Arrow) {
        const_cast<arrow&>(arr).Draw(Scroll);
    }
    
    // Draw monsters
    for (auto& fp : this->FlowerPlant) fp.Draw(Scroll);
    for (auto& s : this->Slime) s.Draw(Scroll);
    for (auto& g : this->Golem) g.Draw(Scroll);
    for (auto& b : this->Bat) b.Draw(Scroll);
    for (auto& t : this->Tree) t.Draw(Scroll);
    for (auto& v : this->Virus) v.Draw(Scroll);
    
    // Draw player
    this->Player.Draw();
    
    // Draw enemy projectiles
    for (auto& ball : this->Ball) ball.Draw(Scroll);
    
    // Draw GUI (joystick)
    this->Player.JoystickDraw();
    
    DxLib::ScreenFlip();

    return true;

}

bool Game::Pause() {

    // Update logic
    // TODO: Implement pause menu functionality

    // Rendering
    DxLib::ClearDrawScreen();
    // TODO: Draw pause menu
    DxLib::ScreenFlip();

    return true;

}

bool Game::SkillSelect() {

    // Update logic
    constexpr int FADE_OUT_THRESHOLD = 15;
    constexpr int SKILL_NORMAL_CHANCE = 80;
    
    if (this->FadeOutCount++ >= FADE_OUT_THRESHOLD) {
        this->FadeOutCount = 0;
        this->Scene = this->STAGE;
        
        // Reset player position
        this->Player.Sprite.Pos.SetPos(48.0, this->Player.StartPos.GetY());
        this->Player.Sprite.Direction = 0.0;
        
        // Give random skill
        std::vector<player::skill> SkillLeft;
        for (const auto& s : this->Player.GetSkillLeft()) {
            SkillLeft.push_back(s.first);
        }
        
        int Rand = DxLib::GetRand(99);
        if (Rand < SKILL_NORMAL_CHANCE && !SkillLeft.empty()) {
            this->Player.GiveSkill(SkillLeft[DxLib::GetRand(static_cast<int>(SkillLeft.size()) - 1)]);
        } else {
            this->Player.GiveSkill(player::HEAL);
        }
        
        // Advance to next stage
        this->Map.NextStage();
    }
    
    // Update joystick input (use stack allocation instead of heap)
    pos InputDirection;
    this->Player.JoystickInput(&InputDirection);
    
    // Update arrows (4x speed multiplier)
    constexpr int ARROW_SPEED_MULTIPLIER = 4;
    for (int j = 0; j < ARROW_SPEED_MULTIPLIER; j++) {
        for (size_t i = 0; i < this->Arrow.size(); i++) {
            this->Arrow[i].Update();
        }
    }
    
    // Update balls
    for (size_t i = 0; i < this->Ball.size(); i++) {
        this->Ball[i].Update();
    }

    // Rendering
    DxLib::ClearDrawScreen();
    const int Scroll = static_cast<int>(this->Player.Sprite.Pos.GetX() - this->Player.StartPos.GetX());
    
    // Draw background
    this->Map.Draw(Scroll);
    
    // Draw arrows (use const reference to avoid copying)
    for (const auto& arr : this->Arrow) {
        const_cast<arrow&>(arr).Draw(Scroll);
    }
    
    // Draw player
    this->Player.Draw();
    
    // Draw enemy projectiles
    for (size_t i = 0; i < this->Ball.size(); i++) {
        this->Ball[i].Draw(Scroll);
    }
    
    // Fade effect
    constexpr int MAX_ALPHA = 255;
    DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, MAX_ALPHA * FadeOutCount / FADE_OUT_THRESHOLD);
    DxLib::DrawBox(0, 0, 1280, 720, 0x000000, TRUE);
    DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, MAX_ALPHA);

    DxLib::ScreenFlip();

    return true;

}

bool Game::Die() {

    // Update death scene timing
    constexpr int DIE_SCENE_DURATION = 240;
    constexpr int SCORE_FADE_START = 180;
    constexpr int SCORE_FADE_DURATION = 60;
    constexpr int MAX_ALPHA = 255;
    
    this->FadeOutCount++;
    
    // Update animations during death scene
    if (this->FadeOutCount < DIE_SCENE_DURATION) {
        // No need to allocate pos object since it's not used
        pos InputDirection; // Stack allocation
        
        // Update arrows (4x speed multiplier)
        constexpr int ARROW_SPEED_MULTIPLIER = 4;
        for (int j = 0; j < ARROW_SPEED_MULTIPLIER; j++) {
            for (size_t i = 0; i < this->Arrow.size(); i++) {
                this->Arrow[i].Update();
            }
        }
        
        // Update balls
        for (size_t i = 0; i < this->Ball.size(); i++) {
            this->Ball[i].Update();
        }
    }

    // Rendering
    DxLib::ClearDrawScreen();
    
    if (this->FadeOutCount < DIE_SCENE_DURATION) {
        const int Scroll = static_cast<int>(this->Player.Sprite.Pos.GetX() - this->Player.StartPos.GetX());
        
        // Draw background
        this->Map.Draw(Scroll);
        
        // Draw arrows
        for (const auto& arr : this->Arrow) {
            const_cast<arrow&>(arr).Draw(Scroll);
        }
        
        // Draw enemies
        for (size_t i = 0; i < this->FlowerPlant.size(); i++) {
            this->FlowerPlant[i].Draw(Scroll);
        }
        for (size_t i = 0; i < this->Slime.size(); i++) {
            this->Slime[i].Draw(Scroll);
        }
        for (size_t i = 0; i < this->Golem.size(); i++) {
            this->Golem[i].Draw(Scroll);
        }
        for (size_t i = 0; i < this->Bat.size(); i++) {
            this->Bat[i].Draw(Scroll);
        }
        for (size_t i = 0; i < this->Tree.size(); i++) {
            this->Tree[i].Draw(Scroll);
        }
        for (size_t i = 0; i < this->Virus.size(); i++) {
            this->Virus[i].Draw(Scroll);
        }
        
        // Draw enemy projectiles
        for (size_t i = 0; i < this->Ball.size(); i++) {
            this->Ball[i].Draw(Scroll);
        }
    }
    
    // Draw score screen with fade-in effect
    if (this->FadeOutCount > SCORE_FADE_START) {
        const int fadeAlpha = MAX_ALPHA * (this->FadeOutCount - SCORE_FADE_START) / SCORE_FADE_DURATION;
        DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlpha);
        DxLib::DrawGraph(0, 0, this->EndGraph, FALSE);
        
        // Draw final stage number
        DxLib::SetFontSize(48 * 3.2);
        DxLib::DrawFormatString(64 * 3.2, 128 * 3.2, 0xff0000, "�X�e�[�W%d", this->Map.GetStage());
        DxLib::SetFontSize(16);
        
        DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, MAX_ALPHA);
    }

    DxLib::ScreenFlip();

    return true;

}

/**
 * @brief Read entire file content into a string
 * @param FilePath Path to the file to read
 * @return File contents as string, or empty string if file cannot be opened
 */
std::string Game::FileToString(std::string FilePath) {

    std::ifstream File(FilePath);
    if (!File) {
        return "";
    }
    
    return std::string(
        (std::istreambuf_iterator<char>(File)),
        std::istreambuf_iterator<char>()
    );

}
