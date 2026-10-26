# Projekt grafiki za pomocą aplikacji konsolowej

## Spis treści
- [Opis projektu]
- [Ostatnie zmiany]
- [Wymagania wstępne]
- [Nowe problemy i cele]

---

## Opis projektu - 0.0.1
Projekt ma na celu, wyciśnięcia z cmd tego co jest możliwe, służąc również jako projekt do samorozwoju.

## Ostatnie zmiany
Pierwsze proste funkcje graficzne tj. przejdź do danego miejsca, zmień kolor itp.

## Wymagania wstępne
Wystarczy konsola. (w przypadku win wymagane włączenie wirtualizacji)

## Nowe problemy i cele

### Resize
Gdy pisałem ostatnio projekt wykorzystujący podobne rozwiązania, przy windowsie pojawił się pewien problem, związany z zmianą rozmiaru buffora oraz tekstu - windows domyślnie nie otwierał "starego" typu konsolii na którym taka zmiana jest możliwa tylko coś w rodzaju nowej nakładki 
- dowiedzieć się dlaczego tak się dzieje, i spróbować rozwiązać ten problem
- spróbować napisać resize w linux

### Organizacja
Na razie struktura jest testowa

- namespace engine
 - struct Color //r,g,b
 - struct Vec2  //x,y
 - struct Cell  //znak + kolor + kolor tła
 - namespace ansi //podstawoe funkcje graficzne
  - gotoxy //przenies kursor
  - cursor //pokaz/schowaj kursor
  - kolor //zmien kolor FG/BG
 - class Terminal //okno programu i wszystkie ustawienia z nim zwiazane
 - class Canvas //bufor rysowania
 - class Aplication //petle glowna

 
