# Projekt grafiki za pomocą aplikacji konsolowej

## Spis treści
- [Opis projektu]
- [Możliwości]
- [Ostatnie zmiany]
- [Wymagania wstępne]
- [Znane problemy i decyzje]
- [Plany]

---
## Opis projektu - 0.3.3
Projekt ma na celu, wyciśnięcia z cmd tego co jest możliwe, służąc również jako projekt do samorozwoju.

## Możliwości

- Kolory RGB - true color, domyślnie kolory w konsoli przyjmuja tylko 16 wartosci
- Renderer - Nie rysujemy calego ekranu w kazdej klate, pierw są sprawdzane różnice między klatką bieżącą a następną
i rysowane jest tylko to się zmieniło
- Rysowanie w trybie pixelowym - korzystając z właściwości utf8 mamy do dyspozycji znak '▀', manipulując kolorami jesteśmy
w stanie uzyskać rysowanie pixelowe
- Rysowanie prostych kształtów
- Rysowanie Textur (pixelowe jak i ASCII)
- Input - rozróżnienie na pojedycze wciśnięcia jak i przytrzymanie klawiszy
- Zegar - stała dt między klatkami
- Resize - zmiana rozmiaru okna przez użytkownika jest obsłużona, natomiast, wymuszenie zmianny na windowsie nie jest możliwe


## Ostatnie zmiany
0.3.3
- Wymuszenie resize
- Utworzenie Log


0.3.2
- Animacje
- Odczyt Png


0.3.1
- Ctrl + c
- Skalowanie textur


0.3.0
- Tryb pixelowy (Halfblock)
- Kształty (linia, prostokat, okrag, kolo) - na znakach jak i pixelach
- Textury rgb
- Ascii 
0.2.0
Dodano:
- Zegar FPS - staly dt
- Prosty Resize - więcej pod "nowe problemy i cele"
- Input - kliknięcie, i przytrzymanie
- Unicode - utf8, polskie znaki, znaki specjalne jak: '▀'

0.1.0 - stworzenie struktury "silnika"
- ansi - zajmujsię wypisywaniem komend konsolowych
- terminal - dosłownie odpowiada za obsługe terminala, ustawień z nim związanych
- canva - jest buforem na którym rysujemy
- renderer rzutuje bufor na ekran
- dodatkowe structy, i operatory

0.0.1. Pierwsze proste funkcje graficzne tj. przejdź do danego miejsca, zmień kolor itp.

## Wymagania wstępne
Wystarczy konsola. (w przypadku win wymagane włączenie wirtualizacji)

## Znane problemy i decyzje

### Resize
W przypadku linux nie ma problemu, w przypadku windows problem jest.

Windows domyślnie uruchamia aplikacje w swojej nowej nakładce terminali która blokuje takie kontrolowanie ekranem
2 opcje (sprawdzić czy możliwe, i ewentualnie konsekwencje)
- wymusić uruchomienie aplikacji jako conhost
- poddać się i zaufać użytkownikowi że sam zmieni rozmiar
Możemy odczytywać rozmiar okna więc, zaufanie polegałoby na tym
aby wyświetlić wielki napis "ZWIĘKSZ ROZMIAR OKNA" który blokowałby
aplikacje do momentu aż użytkownik łaskawie zmieniłby rozmiar okna

## Plany

### Polskie znaki, w input
Tak jak w temacie, zająć się polskimi znakami w input

### Audio
Poczytać czy wgl możliwe jest jakakolwiek obsługa audio (jakaś musi istnieć) i zastanowić się czy możliwa jest implementacja i jakim kosztem

#### Pomysły na dalszą przyszłość
- UI (ramki z unicode ("┌─┐│└┘"))
- Debug Overlay (np. fps)
- Obsługa Myszy
- Sceny / stan gry (ale to za milion lat)




 
