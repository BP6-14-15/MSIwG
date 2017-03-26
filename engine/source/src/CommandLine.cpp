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
#include "CommandLine.hpp"

using namespace std;

typedef void(*AnyFcn)();

template <typename Signature>
std::function<Signature> cast(AnyFcn f){
    return reinterpret_cast<Signature*>(f);
}

CPGame::PlayerControllerCallback loadPlayerFunc(const string& fcnName, void* handler) {
    // reset err
    dlerror();
    auto rawCallback = (AnyFcn)dlsym(handler, fcnName.c_str());
    auto dlsymErr = dlerror();
    
    if (dlsymErr) {
        cerr << "Couldn't load symbol " << fcnName << ", error: " << dlsymErr << endl;
        exit(1);
    }
    
    auto fcn = cast<void(std::shared_ptr<const CPGame::Board>, CPGame::BoardPlayerUpdateRequest, std::shared_ptr<CPGame::Promise<CPGame::BoardPlayerUpdateResult>> res)>(rawCallback);
    
    return fcn;
}

tuple<void*, CPGame::PlayerControllerCallback> loadPlayer(const string& libraryPath, const string& fcnName) {
    void* handler = dlopen(libraryPath.c_str(), RTLD_NOW);
    if (!handler) {
        cerr << "Couldn't open library at path: " << libraryPath << endl;
        exit(1);
    }
    
    auto fcn = loadPlayerFunc(fcnName, handler);
    if (!fcn) {
        cerr << "Couldn't setup player function " << fcnName << ", from library at path " << libraryPath << endl;
        exit(1);
    }
    
    return make_tuple(handler, fcn);
}

PlayersSource::~PlayersSource() {
    if (firstPlayerLib) {
        dlclose(firstPlayerLib);
    }
    if (secondPlayerLib) {
        dlclose(secondPlayerLib);
    }
}

PlayersSource::PlayersSource(CPGame::PlayerControllerCallback player1, CPGame::PlayerControllerCallback player2): player1(player1), player2(player2) {
    
}

void printUsage(const char* progName) {
    printf("Usage: %s [--longOption=value] [-sValue]\n\n", progName);
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
    printf("\n");
    
}

void parseArg(int argc, char* args[], CPGame::GameConfiguration& conf, PlayersSource& src) {
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
            {"fpFcnName", required_argument, 0, 'b'}, // first player lib path
            {"spLibPath", required_argument, 0, 'c'}, // second player lib path
            {"spFcnName", required_argument, 0, 'd'}, // first player lib path
            {"help", no_argument, 0, 'h'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;
        
        c = getopt_long(argc, args, "ha:b:c:d:s:e:g:h:n:m:p:o:w:y:",
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
            cerr << "First player function name given, but lib path is missing" << endl;
            exit(1);
        }
        
        auto res = loadPlayer(*firstPlayerLib, *firstPlayerFcn);
        src.firstPlayerLib = get<0>(res);
        src.player1 = get<1>(res);
    }
    
    if (secondPlayerLib || secondPlayerFcn) {
        if (!secondPlayerFcn) {
            cerr << "Second player lib path given, but function name is missing" << endl;
            exit(1);
        }
        if (!secondPlayerLib) {
            cerr << "Second player function name given, but lib path is missing" << endl;
            exit(1);
        }
        
        if (*secondPlayerLib == *firstPlayerLib || *secondPlayerLib == "-") {
            src.player2 = loadPlayerFunc(*secondPlayerFcn, src.firstPlayerLib);
        } else {
            auto res = loadPlayer(*firstPlayerLib, *firstPlayerFcn);
            src.secondPlayerLib = get<0>(res);
            src.player2 = get<1>(res);
        }
    }
}
