# Projekt grafiki za pomocą aplikacji konsolowej

## Spis treści
- [Opis projektu]
- [Ostatnie zmiany]
- [Wymagania wstępne]
- [Nowe problemy i cele]

---

## Opis projektu - 0.2.0
Projekt ma na celu, wyciśnięcia z cmd tego co jest możliwe, służąc również jako projekt do samorozwoju.

## Ostatnie zmiany
0.2.0
Dodano:
- Zegar FPS
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

## Nowe problemy i cele

### Resize
W przypadku linux nie ma problemu, w przypadku windows problem jest.

Windows domyślnie uruchamia aplikacje w swojej nowej nakładce terminali która blokuje takie kontrolowanie ekranem
2 opcje (sprawdzić czy możliwe, i ewentualnie konsekwencje)
- wymusić uruchomienie aplikacji jako conhost
- poddać się i zaufać użytkownikowi że sam zmieni rozmiar
Możemy odczytywać rozmiar okna więc, zaufanie polegałoby na tym
aby wyświetlić wielki napis "ZWIĘKSZ ROZMIAR OKNA" który blokowałby
aplikacje do momentu aż użytkownik łaskawie zmieniłby rozmiar okna

### Rysowanie
Dodać rysowanie lini, prostych kształtów np.
- koło
- okrąg
- prostokąt pusty w środku
- prostokąt wypełniony
- linie pod różnymi kątami
Ale też proste textury ASCII (najpewnij z plików .txt)

### Polskie znaki, w input
Tak jak w temacie, zająć się polskimi znakami w input

### Audio
Poczytać czy wgl możliwe jest jakakolwiek obsługa audio (jakaś musi istnieć) i zastanowić się czy możliwa jest implementacja i jakim kosztem

#### Pomysły na dalszą przyszłość
- UI (ramki z unicode ("┌─┐│└┘"))
- Debug Overlay (np. fps)
- Obsługa Myszy
- Sceny / stan gry (ale to za milion lat)




 
