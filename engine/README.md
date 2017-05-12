#### Spis treści 
* [Zmiany](#changes)
* [Funkcja klienta](#clientFcn)  
* [Środowisko](#env)
  * [Disclaimer](#env_dsc)
  * [Konfiguracja](#conf)
    * [Automatyczna](#auto)
    * [Manualna](#manual)
  * [Uruchamianie](#launch)
  * [Binarka](#bin)

<a name="changes">

# Zmiany 
## v0.4 
* Każdy klient musi implementować nową funkcję o nazwie `gameEnginePlayerSignature` i sygnaturze `const char*(void)` - funkcja ta wykorzystywana jest do identyfikacji i weryfikacji poprawnego podłączenia biblioteki.
* Domyślnie w czasie 500 ms oczekiwania na kolejne ruchy wyłączona jest możliwość wysyłania do `stdout` (printf, cout), `stderr` pozostaje jednak otwarte.   
Dzięki temu można łatwiej sprawdzać czy silnik poprawnie odbiera zadane dane.   
Przy testowaniu funkcji za pomocą flagi `--clientsPrint` można pozostawić `stdout` otwartym. 
* Nowa flaga `--printsDirs`, gdy aktywna silnik będzie w konsoli wypisywał bezpośrednio otrzymane od klientów zadane ruchy. 
* Jeżeli funkcja drugiego gracza korzysta z tej samej biblioteki co funkcja pierwszego, to wystarczy jako argument opcji `--spLibPath` podać `-`, w takim wypadku jeżeli nazwa funkcji drugiego gracza nie będzie wyraźnie dana (`--spFcnName`) to gracz drugi będzie korzystał z tej samej funkcji co pierwszy. 
* Bramy i ściany także w rundzie drugiej korzystają z tego samego seeda.

## v0.3
* Generalne usprawnienia
* Przedziały indeksów poszczególnych typów obiektów w [stanie planszy](#board_states)
## v0.2.1 

Kompilacja funkcji klienta nie wymaga już standardu C++17, przygotowałem takżę gotową [binarkę](#bin) silnika. 
Możecie więc skompilować swoje funkcje starszymi kompilatorami i nie musi to być `Clang`, możecie użyć `gcc`. 
Na branchu [Exec](https://github.com/BP6-14-15/MSIwG/tree/Exec) w folderze `example`znajdują się skrypty do narzędzia `cmake` i do kompilacji (`build.sh`), których możecie użyć w przypadku korzystania z gotowej binarki. 

<a name="clientFcn">  

# Funkcja klienta 
Zawarłem tu generalny opis sposobu interakcji z funkcją klienta, oraz zasad, których należy przestrzegać. 
Co do praktycznej implementacji to najlepiej spojrzeć na zawarty przykład.


```cpp
void updatePlayer(shared_ptr<const CPGame::Board> states,
                  CPGame::BoardPlayerUpdateRequest req,
                  shared_ptr<CPGame::Promise<CPGame::BoardPlayerUpdateResult>> promise
)
```

<a name="board_states">

## `CPGame::Board` states 
```cpp
struct Board {
        std::vector<BoardState> stateHistory; // last is the actual
        int boardSize; // board { n x n }
        
        // wersja 0.3 
        int firstGateIndex;
        int firstPlayerIndex;
        int criminalIndex;
        
    };
```
Przechowuje on historię ostatnich 5 stanów planszy, oraz jej rozmiar (zawsze n x n). 

**Wersja 0.3** wprowadza pola przedstawiające indeksy różnych typów obiektów, ich przedziały:

* <0, firstGateIndex) - ściany
* <firstGateIndex, firstPlayerIndex) - bramy
* <firstPlayerIndex, criminalIndex) - policjanci
* criminalIndex - indeks złodzieja

### `CPGame::BoardState` 
```cpp
struct BoardState {
    std::vector<BoardObject> objects;
};
```
Stan planszy to po prostu wektor wszystkich jej obiektów. 

### `CPGame::BoardObject` 
```cpp
struct BoardObject {
    std::vector<BoardPosition> coveredFields; // x, y - < 0, n + 1>
    BoardObjectType type;
    BoardObjectData data;
};
```
Obiekt planszy składa sie z wektora zajmowanych pól - **ważne** - pola na planszy mogą mieć wartości <0, N + 1>, gdzie 0 i N + 1 to granice planszy, na nich poruszają się tylko bramy - bądź złodziej o ile w danym miejscu jest brama, policjancji i ściany zajmować mogą jedynie pola z przedziału <1,N>.  

#### `CPGame::BoardObjectType`

```cpp
enum class BoardObjectType {
    player, wall, gate
        
};
```

#### `CPGame::BoardObjectData`
```cpp
enum class PlayerType {
    police, criminal
};

struct PlayerObjectData {
    PlayerType type;
};
    
union BoardObjectData {
    PlayerObjectData player;
        
};

```
## `CPGame::BoardPlayerUpdateRequest` req
```cpp
struct BoardPlayerUpdateRequest {
    std::vector<int> objectIndexes;
    int numberOfMovesRequired = 5;
};
```
Obiekt ten to specyfikacja żądania aktualizacji gracza.  
Wektor `objectIndexes` zawiera indeksy obiektów z wektora `object` obiektu `BoardState`, to właśnie tymi obiektami gracz może sterować.   
Ważne jest aby trzymać się liczby żądanych ruchów (obecnie zawsze 5), jeśli liczba zwróconych ruchów nie będzie się zgadzać to silnik zignoruje te dostarczone. 

## `shared_ptr<CPGame::Promise<CPGame::BoardPlayerUpdateResult>>`
```cpp
struct BoardPlayerUpdateResult {
    std::vector<std::vector<BoardMoveDirection>> moveDirection;
};
```
Poprzez ten obiekt dokonać można aktualizacji postaci gracza, wystarczy wyowałć jego metodę `setValue` z obiektem typu `CPGame::BoardPlayerUpdateResult`, gdzie obiekt ten to dwuwymiarowy wektor - wiersze to sterowane postacie, a kolumny to ich ruchy.   
**UWAGA** 
Kolejność postaci gracza musi odpowiadać kolejności indeksów z obiektu `BoardPlayerUpdateRequest`, podobnie jak w przypadku liczby kroków, liczba aktualizowanych postaci musi być równa liczbie indeksów, które otrzymaliśmy, w przeciwnym wypadku żadna z postaci nie zostanie zaktualizowana.   
Aktualizacji można dokonać dowolną ilość razy w przeciągu 500 ms, silnik odczeka pełen interwał i dopiero pobierze listę ruchów.   
Tutaj dobra wiadomość - jeżeli wywołacie metodę `setValue` tuż przed końcem czasu to silnik zaczeka na jej wykonanie.   
Nie polecałbym jednak częstego wykonywania np. w pętli, zawiera ona synchronizację wątków, więc w przypadku ciągłych wykonań możecie stracić dużo czasu. 

<a name="env">

# Środowisko 

<a name="env_dsc">

## Disclaimer

Zakładam, że będziemy pracować na Linuxie (Ubuntu ?), jednak różne wersje mają różne kompilatory, a silnik wymaga standardu C++17.  
Ja korzystam z Clang, więc przygotowałem krótką instrukcję i skrypty do instalacji najnowszej wersji obsługującej ten standard, przygotowane i przetestowane były one pod (L)Ubuntu 16.10.   

Jeśli nie będziecie korzystać z przygotowanych przeze mnie skryptów itd, to poza C++17, silnik wymaga tych bibliotek zewnętrznych: 

* SDL2
* SDL2_gfx
* SDL2_ttf 
* SDL2_image

Wszystkie z nich można znaleźć [tutaj](https://www.libsdl.org/index.php).
Poza nimi, w Linuxie trzeba także dołączyć przy linkowaniu `-ldl -lpthread`, pierwsza z nich to dynamiczne ładowanie bibliotek a druga to wątki, nie znam odpowiedników na Windowsie. 

Oczywiście możecie korzystać z innych narzędzi, nie wiem jednak jak wygląda wsparcie dla C++17 w GCC, lub innych.   
Jeśli chcecie natomiast pracować na Windowsie to VS2017 zdaje się wspiera już C++17. 

Zalecałbym pracę na maszynie wirtualnej, przynajmniej na początku, testowałem skrypty i konfigurację kilka razy, ale zawsze czegoś mogłem nie uwzględnić.  
Jeśli pojawią się problemy z użyciem skryptów automatycznych, to przygotuję gotowy obraz maszyny wirtualnej i ewentualnie gotowe binarki silnika (na Ubuntu), wtedy tylko będziecie musieli skompilować swoje funkcje. 

<a name="conf">  

## Konfiguracja środowiska

<a name="auto">  

### Automatyczna
#### Wsparcie 

1. Ubuntu 
  * 14.04
  * 15.10
  * 16.04 
  * 16.10 
  * 17.04

#### Krok po kroku 
* Przechodzimy do folderu, do którego pobrane zostanie źródło i niezbędne narzędzia. 
* Pobieramy plik [LLVM Setup](https://gist.githubusercontent.com/dmcyk/592a4b76c4199e7228ad1916a30db83d/raw/d0d05325689f1556d90318441c85383acfaa8710/setupLLVM.sh)
* Po pobraniu należy dać mu uprawnienia do uruchomienia - `chmod +x setupLLVM.sh` i uruchomić (`./setupLLVM.sh`)
* Następnie pobieramy skrypt [Engine autosetup](https://gist.githubusercontent.com/dmcyk/13d6888970c1cb0c470ca6f3107a8740/raw/32627afe5c419812b3fdef77b7d54f997044cebe/MSIwG_setup.sh)
* I tak samo jak w przypadku pierwszego skryptu nadajemy prawa i uruchamiamy. (Może to chwilę potrwać)
* W folderze `engine/example` pojawi się skrypt `buildScript.sh` a w folderze `engine/source` skrypty `buildScript.sh` i `run.sh`

<a name="manual">    
    
### Manualna

#### Kompilacja przykładu klienta 

**System**: Ubuntu  
**Wersja**: 16.10 

* Na początek musimy dodać repozytoria, na końcu pliku `/etc/apt/sources.list` dodajemy: 

       deb http://apt.llvm.org/yakkety/ llvm-toolchain-yakkety-4.0 main  
       deb-src http://apt.llvm.org/yakkety/ llvm-toolchain-yakkety-4.0 main
       
W przypadku innych wersji, dystrybucji [tutaj](http://apt.llvm.org) można znaleźć odpowiednie pakiety. 

* W konsoli wpisujemy: 
`wget -O - http://apt.llvm.org/llvm-snapshot.gpg.key|sudo apt-key add -`

* Następnie  
`sudo apt-get update`

* Bibliotekę standardową należy skompilować oddzielnie gdyż obecnie nie ma pakietu dla wersji 4.0.  
Uruchamiamy więc skrypt `clangSetup.sh`, domyślnie źródło zostanie pobrane do `~/Downloads`, można jednak jako pierwszy argument skryptu podać inną ściężkę. 
Po wykonaniu skryptu, możemy usunąć folder `~/Downloads/LLVM/llvm`, trochę miejsca zajmuje, a potrzebujemy jedynie folderu `build`. 

* Następnie pozostaje nam już tylko kompilacja.  
Będąc w folderze `engine/example` w konsoli wpisujemy `CXX=clang++-4.0 cmake ./`.  
Jeśli jednak kompilując bibliotekę standardową podaliśmy własną ścieżkę to tutaj także należy to zrobić - wtedy kompilacji możemy dokonać wpisując `CXX=clang++-4.0 cmake ./ -DLIBCXXPATH=/moja/sciezka`. Taka uwaga, tutaj podając ścieżkę nie możemy użyć znaku `~`, zamiast tego możemy jednak wpisać `$HOME` np. `-DLIBCXXPATH=$HOME/Dev`

* Jeśli wszystko poprawnie się wykonało, to teraz wystarczy uruchomić z konsoli polecenie `make` i nasz program zostanie skompilowany do biblioteki dynamicznej. 

#### Kompilacja silnika 

**System**: Ubuntu  
**Wersja**: 16.10 

**Zakładam, że kroki kompilacji przykładowego klienta zostały już pomyślnie wykonane.**  

* W folderze `engine` wykonać należy skrpyt `engineDependencies.sh`  
* Jeśli wszystko poprawnie zostało wykonane, teraz już tylko należy skompilować projekt. { patrz dwa ostatnie punkty kompilacji klienta }   
* Silnik możemy uruchomić przy pomocy skryptu `run.sh`, gdzie jako pierwszy argument należy podać ścieżkę standardowej biblioteki (o ile użyliśmy innej przy kompilacji klienta)


<a name="launch">   

## Uruchamianie 

**Poprzez skrypt `run.sh`**

* Domyślnie silnik zostanie uruchomiony z klientami działającymi w sposób losowy. W skrypcie uruchamiającym zakomentowana jest linia obrazująca sposób uruchomienia serwera z pierwszym graczem pochodzącym z wcześniej skompilowanego przykładu.   
* Silnik można konfigurować - rozmiar planszy, gracze itd. aby zobaczyć listę opcję należy w skrypcie konfiguracyjnym odpowiednio linię `./GameEngine` przez `./GameEngine --help` 

<a name="bin">

## Binarka 

W zakładce `releases` GitHuba w wersji 0.2.1 możecie znaleźć binarkę przeznaczoną pod system Lubuntu 16.10 (na zwykłym Ubuntu 16.10 także powinno działać), można ją uruchomić bezpośrednio nawet z obrazu systemu bez instalacji. 

