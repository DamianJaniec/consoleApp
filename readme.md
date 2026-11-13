# Projekt grafiki za pomocą aplikacji konsolowej

## Spis treści
- [Opis projektu]
- [Możliwości]
- [Ostatnie zmiany]
- [Wymagania wstępne]
- [Znane problemy i decyzje]
- [Plany]

---
## Opis projektu - 0.3.4.1
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
- Log zapis do pliku .txt
- App szkielet programu, pętal licznik fps, blokada za małego okna
- Przeźroczysztość/Mieszanie kolorów
- Menadżer Zasobów - kazdy plik wczytywany raz
- Odtwarzanie plikó mp3
- Generacja prostych dźwięków
- "Mumble Speech"

## Ostatnie zmiany
0.3.4.1
- SetVolume - każdy dźwięk osobno
- Zatrzymanie dźwięku
- Generowany głos (Talk, Voice)
- Pominięcie kwesti


0.3.4
- Klasa App, szkieleto programu
- Menadżer zasobów
- Kolory - lerp, blend pixel
- Audio(miniaudio) - odtwarzanie mp3, proste dźwięki


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

- po usuwać wszystkie warningi - trochę się już nazbierało, choć nie ma jeszcze żadnych poważnych błędów, większości to są niedokładne rzutowania (np. int zamiast size_t)
- do dodanych super funkcjonlaności super obsługi cmd, pododawać logi
(na obecną chwilę, log się tworzy i wyświetla tylko date rozpoczęcia i zakończenia działania trwania aplikacji)
(podopisywać logi w przypadkach błędowych np. bład wczytania pliku, nie poprawnie wybrany id Beep'a etc.)
Będzie to żmudna i długa robota, której na ten moment strasznie mi się nie chce robić - zostawie sobie na koniec
Albo wrzuce do jakiegoś ai, może chociaż tym razem ta kupa elektrozłomu się na coś przyda i łaskawie zrobi tę katorgę
za mnie.

## Plany

### Polskie znaki, w input
Tak jak w temacie, zająć się polskimi znakami w input

### Audio - ciąg dalszy
Mp3, Dźwięki oraz mowa działają poprawnie :)
Rysowanie w audio też działa, ładnie wypisuje w sposób animowany tekst, zależnie od czasu
a nie klatek na sekundę itp. to nad czym teraz trzeba się zastanowić to jak obrobić to rysowanie tekstu
tzn. dobrze byłoby jakieś zawijanie tekstu zrobić, albo żeby tekst jak przejdzie do nowej linijki to się
nadpisywał, ewentulanie można zrzucić winę na osobe korzystającą z silnika i powiedzieć że wypisanie działa tylko
w jednej linijce, i jak chce to niech sam sobie nadpisze (nie będzie z tym większego problemu ponieważ, drawText w 
klasie Talk nie jest uruchamiany w update, więc działa trochę jakby domyyślnie był wyłączony) - wtedy
trzeba dopisać get'y do ilości pokazanych znaków aby można było jakkolwiek sprawdzić stan wypisanego tekstu

### Wyspać się
tak jak w temacie

#### Pomysły na dalszą przyszłość
- UI (ramki z unicode ("┌─┐│└┘"))
- Debug Overlay (np. fps)
- Obsługa Myszy
- Sceny / stan gry (ale to za milion lat)




 
