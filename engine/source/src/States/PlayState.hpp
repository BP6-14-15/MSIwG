//
//  PlayState.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 13.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef PlayState_hpp
#define PlayState_hpp

#include <functional>
#include <optional>
#include "GameState.hpp"
#include "BoardSprite.hpp"
#include "Player.hpp"
#include "UIButton.hpp"
#include "Gate.hpp"
#include "CPGame.hpp"
#include "PlayStateException.hpp"

GameConfiguration DefaultGameConfiguration();

enum class PlayStateEvent {
    didRequestExit
};

struct Margins {
    int top, right, bottom, left;
};

enum class PlayPhase {
    first, // choosing board
    second, // first vs second player
    third, // results, about to start sec vs first (automatically)
    fourth, // sec vs first
    fifth // final results (click mouse or ESC to exit)
    
};

struct PlayConfiguration {
    std::vector<std::unique_ptr<BoardSprite>> initialBoardSprites;
    std::vector<std::unique_ptr<Player>> initialPlayerSprites;
    PlayPhase currentPhase = PlayPhase::first;
    
};

class PlayState: public GameState {
    const int UPDATE_INTERVAL = 40;
    const int boardSquareMaxSize = 30;
    const Margins MARGINS = { 32, 32, 32, 200 };
    const int CLOCK_INTERVAL = 6; // one bigger than amount of required moves. -- one update is reserved for fetch and rendering of last move and
    
    int updateCounter = 0;
    int updateClock = CLOCK_INTERVAL - 1; // incremented prior to update, fetch moves at first update thus {interval - 1}

    // NOTE: firstPlayer starts as criminal 
    int firstPlayerPoints;
    int secondPlayerPoints;
    
    void manualCleanup();
    
    std::shared_ptr<GameCtx> gameCtx = nullptr;
    
    
    // purely a helper reference taken from GraphicsCtx, doesn't have to bother with memory management
    Drawing::DrawingCtx& drawingCtx;
    
    int boardStartX = 0;
    int boardStartY = 0;
    int squareSize;
    int boardWidth, boardHeight;
    
    
    std::vector<std::unique_ptr<BoardSprite>> boardSprites;
    std::vector<std::unique_ptr<Player>> playerSprites;
    std::unique_ptr<PlayConfiguration> conf;
    
    // raw ptrs, those objects will live only through first phase, no need to keep them later in memory. 
    UIButton* startButton;
    UIButton* reloadButton;
    
    // moves
    std::unique_ptr<Texture> arrowUpTexture;
    std::unique_ptr<Texture> arrowDownTexture;
    std::unique_ptr<Texture> arrowLeftTexture;
    std::unique_ptr<Texture> arrowRightTexture;
    std::unique_ptr<Texture> pointTexture;

    
    std::unique_ptr<Texture> resultTexture;
    
    CPGame::Board boardStateCache;

    
    void initBoard(const GameConfiguration& gc, bool reload = false);
    void update();
    void computeBoardPosition(const GameConfiguration& gc);

    void reloadBtnAction(UIButton& btn);
    void startBtnAction(UIButton& btn);
    void updatePlayers();
    void controlPlayersUpdate(CPGame::BoardPlayerUpdateRequest& request, std::shared_ptr<CPGame::BoardPlayerUpdateResult> result, const std::string& name);
    void populateBoardState();
    void nextPhase(); 
    void exit(); 
    
public:

    
    PlayState(std::shared_ptr<GraphicsCtx> ctx, std::shared_ptr<GameCtx> gameCtx, std::function<void(const PlayStateEvent& e)> delegate);
    ~PlayState();

    virtual void draw() override;
    virtual void handleEvent(const SDL_Event& e) override;
    std::function<void(const PlayStateEvent& e)> delegate;
    std::vector<const Gate*> getGates() const;
    
};

#endif /* PlayState_hpp */
