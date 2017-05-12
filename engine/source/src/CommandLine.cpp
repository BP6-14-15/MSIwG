//
//  CommandLine.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 24.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include <getopt.h>
#include <dlfcn.h>
#include <functional>
#include <iostream>
#include "CommandLine.hpp"

using namespace std;

typedef void(*AnyFcn)();

template <typename Signature>
std::function<Signature> cast(AnyFcn f) {
    return reinterpret_cast<Signature*>(f);
}

AnyFcn loadAnyPlayerFunc(const string& fcnName, void* handler) {
    // reset err
    dlerror();
    auto rawCallback = (AnyFcn)dlsym(handler, fcnName.c_str());
    auto dlsymErr = dlerror();
    
    if (dlsymErr) {
        cerr << "Couldn't load symbol " << fcnName << ", error: " << dlsymErr << endl;
        exit(1);
    }
    
    return rawCallback;
}

CPGame::PlayerSignatureCallback loadPlayerSignatureFunc(void* handler) {
    auto fcn = cast<const char*(void)>(loadAnyPlayerFunc("gameEnginePlayerSignature", handler));
    return fcn;
    
}

CPGame::PlayerControllerCallback loadPlayerCallbackFunc(const string& fcnName, void* handler) {
    auto fcn = cast<void(std::shared_ptr<const CPGame::Board>, CPGame::BoardPlayerUpdateRequest, std::shared_ptr<CPGame::Promise<CPGame::BoardPlayerUpdateResult>> res)>(loadAnyPlayerFunc(fcnName, handler));
    
    return fcn;
}

typedef tuple<void*, CPGame::PlayerControllerCallback, CPGame::PlayerSignatureCallback> LoadedPlayer;

LoadedPlayer
loadPlayer(const string& libraryPath, const string& fcnName, void* existingHandler = NULL) {
    void* handler;
    
    if (existingHandler == NULL) {
        handler = dlopen(libraryPath.c_str(), RTLD_NOW);
        if (!handler) {
            cerr << "Couldn't open library at path: " << libraryPath << endl;
            exit(1);
        }
    } else {
        handler = existingHandler;
        
    }
    
    
    auto fcn = loadPlayerCallbackFunc(fcnName, handler);
    if (!fcn) {
        cerr << "Couldn't setup player function " << fcnName << ", from library at path " << libraryPath << endl;
        exit(1);
    }
    
    auto signatureFcn = loadPlayerSignatureFunc(handler);
    
    if (!fcn) {
        cerr << "Couldn't setup player function " << fcnName << ", from library at path " << libraryPath << endl;
        exit(1);
    }
    
    return make_tuple(handler, fcn, signatureFcn);
}

PlayersSource::~PlayersSource() {
    if (firstPlayerLib) {
        dlclose(firstPlayerLib);
    }
    if (secondPlayerLib) {
        dlclose(secondPlayerLib);
    }
}

PlayersSource::PlayersSource(std::shared_ptr<GameRemotePlayer> player1, std::shared_ptr<GameRemotePlayer> player2)
: player1(player1), player2(player2) {
    
}

void printUsage(const char* progName) {
    printf("Usage: %s [--longOption=value] [-sValue]\n\n", progName);
    printf("Note:\nFirst player starts as a criminal.\n\n");
    printf("Options: \n");
    printf("-h | --help         Usage \n");
    
    printf("-a | --fpLibPath    Path to library with first player\n");
    printf("-b | --fpFcnName    Name of first player callback function\n");
    printf("-c | --spLibPath    Path to library with second player function, pass '-' sign to use same library as for first player\n");
    printf("-d | --spFcnName    Name of second player callback function\n");

    printf("-s | --size         Board size (>20)\n");
    printf("-e | --nPolice      Police amount (>0)\n");
    printf("-g | --nGates       Gates amount (>0)\n");
    printf("-j | --wGates       Gates width\n");
    printf("-n | --nWalls       Walls amount (>0)\n");
    printf("-m | --lWalls       Walls length\n");
    printf("-p | --pGM          Probability of gate move [0-100]\n");
    printf("-o | --pGDC         Probability of gate direction change [0-100]\n");
    printf("-w | --pWM          Probability of wall move [0-100]\n");
    printf("-y | --pWDC         Probability of wall direction change [0-100]\n");
    printf("-t | --clock        Clock limit points\n");
    
    printf("-r | --boardSeed    Custom board seed\n");
    printf("-l | --applySeed    Flag | Apply custom board seed to default client function");
    printf("-x | --clientsPrint  Flag | If given allows client functions to print text to stdout");
    printf("-z | --printsDirs   Flag | If given prints in console move directions received from clients");
    printf("\n\n");
    
}

void parseArg(int argc, char* args[], GameConfiguration& conf, PlayersSource& src) {
    int c;
    optional<string> firstPlayerLib;
    optional<string> firstPlayerFcn;
    optional<string> secondPlayerLib;
    optional<string> secondPlayerFcn;
    
    while (1) {
        static struct option long_options[] = {
            {"size",     required_argument, 0, 's'},
            {"nPolice",  required_argument, 0, 'e'},
            {"nGates",  required_argument, 0, 'g'},
            {"wGates",  required_argument, 0, 'j'},
            {"nWalls",    required_argument, 0, 'n'},
            {"lWalls",    required_argument, 0, 'm'},
            {"pGM",    required_argument, 0, 'p'},
            {"pGDC",    required_argument, 0, 'o'},
            {"pWM",    required_argument, 0, 'w'},
            {"pWDC",    required_argument, 0, 'y'},
            {"fpLibPath", required_argument, 0, 'a'}, // first player lib path
            {"fpFcnName", required_argument, 0, 'b'}, // first player callback fcn name
            {"spLibPath", required_argument, 0, 'c'}, // second player lib path
            {"spFcnName", required_argument, 0, 'd'}, // second player callback fcn name
            {"boardSeed", required_argument, 0, 'r'},
            {"clock", required_argument, 0, 't'},
            {"applySeed", no_argument, 0, 'l'},
            {"clientsPrint", no_argument, 0, 'x'},
            {"printsDirs", no_argument, 0, 'z'},
            {"help", no_argument, 0, 'h'},
            {0, 0, 0, 0}
        };
        
        /* getopt_long stores the option index here. */
        int option_index = 0;
        
        c = getopt_long(argc, args, "ha:b:c:d:s:e:g:h:n:r:m:p:o:w:y:t:lxz",
                        long_options, &option_index);
        
        /* Detect the end of the options. */
        if (c == -1)
            break;
        
        switch (c) {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
                printf ("option %s", long_options[option_index].name);
                if (optarg)
                    printf (" with arg %s", optarg);
                printf ("\n");
                break;
            case 'h':
                printUsage(args[0]);
                exit(0);
                break;
            case 'x':
                conf.allowsPlayersSTDOut = true;
                break;
            case 'z':
                conf.printsMoveDirections = true;
                break;
            case 'r':
                conf.customSeed = atof(optarg);
                break;
            case 'a':
                firstPlayerLib = string(optarg);
                break;
            case 'b':
                firstPlayerFcn = string(optarg);
                break;
            case 'c':
                secondPlayerLib = string(optarg);
                break;
            case 'd':
                secondPlayerFcn = string(optarg);
                break;
            case 's':
                conf.boardSize = atoi(optarg);
                break;
            case 'e':
                conf.nPolice = atoi(optarg);
                break;
            case 'g':
                conf.nGates = atoi(optarg);
                break;
            case 'j':
                conf.wGates = atoi(optarg);
                break;
            case 'n':
                conf.nWalls = atoi(optarg);
                break;
            case 'm':
                conf.lWalls = atoi(optarg);
                break;
            case 'p':
                conf.pGM = atoi(optarg);
                break;
            case 'o':
                conf.pGDC = atoi(optarg);
                break;
            case 'w':
                conf.pWM = atoi(optarg);
                break;
            case 'y':
                conf.pWDC = atoi(optarg);
                break;
            case 't':
                conf.clockLimit = atoi(optarg);
                break;
            case 'l':
                conf.applyCustomSeedToDefaultClient = true;
                break; 
            case '?':
                /* getopt_long already printed an error message. */
                break;
            default:
                abort();
        }
    }
    
    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", args[optind++]);
        putchar('\n');
    }
    
    if (firstPlayerLib || firstPlayerFcn) {
        if (!firstPlayerFcn) {
            cerr << "First player lib path given, but function name is missing" << endl;
            exit(1);
        }
        if (!firstPlayerLib) {
            cerr << "First player function name given, but library path is missing" << endl;
            exit(1);
        }
        
        auto res = loadPlayer(*firstPlayerLib, *firstPlayerFcn);
        src.firstPlayerLib = get<0>(res);
        src.player1 = std::make_shared<GameRemotePlayer>(get<1>(res), get<2>(res));
    }
    
    if (secondPlayerLib || secondPlayerFcn) {
        if (!secondPlayerLib) {
            cerr << "Second player function name given, but library path is missing" << endl;
            exit(1);
        }
        
        LoadedPlayer res;
        
        if (!secondPlayerFcn || *secondPlayerFcn == "-") {
            secondPlayerFcn = firstPlayerFcn;
        }
        
        if (!secondPlayerFcn) {
            cerr << "Second player missing function name" << endl;
            exit(1);
        }
        
        if (*secondPlayerLib == *firstPlayerLib || *secondPlayerLib == "-") {

            res = loadPlayer(*firstPlayerLib, *secondPlayerFcn, src.firstPlayerLib);
        } else {
            
            res = loadPlayer(*secondPlayerLib, *secondPlayerFcn);
        }
        
        src.player2 = std::make_shared<GameRemotePlayer>(get<1>(res), get<2>(res));
        
    }
}
