# Systemy operacyjne 2 - Projekt

# Sprawozadnie z projektu

___
## Zadanie 1 - Problem jedzących filozofów
**Autor: Miłosz Halicki 272874**  
 **Politechnika Wrocławska**  
**Wydział Informatyki i Telekomunikacji**  
**Data: Kwiecień 2025**  
**Semestr letni 24/25**

### Kompilacja
1. **Windows (MinGW)**:
   ```bash
   g++ solution.cpp -o philosophers -std=c++17

   ```
2. **Linux**:
   ```bash
    g++ solution.cpp -o philosophers -std=c++17 -pthread
   ```

### Przykład użycia

```bash
./philosophers 5
[OUTPUT]
Filozof 0 myśli...
Filozof 1 myśli...
Filozof 2 zdobył widelce i je...
Filozof 3 czeka na widelce...
...
```

### Wymagania systemowe
- Kompilator C++17 (np. GCC, Clang).
- Obsługa biblioteki `<thread>` i `<mutex>`.


## Temat zadania projektowego
Temtem projektu jest rozwiązanie problemu jedzących filozofów. Jest to klasyczne zadanie, w którym musimy rozwiązać synchronizację wątków, zarządzanie sekcjami krytycznymi oraz zapobieganie zakleszczeniom (ang. Deadlock).

Zadanie koncentruje się na zarządzaniu dostępem do współdzielonych zasobów - widelców - przez filozofów reprezentowanych przez wątki, przy jednoczesnym unikaniu zakleszczeń i zapewnieniu sprawiedliwego dostępu.
Problem przedstawiony jest przez określoną liczbę filozofów, którzy siedzą przy okrągłym stole, między nimi znajduje się taka sama liczba widelców. Każdy z filozofów może być w jednym z trzech stanów: myślenie, głód, jedzenie. Aby jeść, filozof potrzebuje jednocześnie dwóch widelców znajdujących się po jego lewej i prawej stronie. Widelce są współdzielone między sąsiadami, wymaga to odpowiedniej synchronizacji.

## Opis stanów, w których mogą znajdować się filozofowie
Każdy filozof przechodzi cyklicznie przez trzy następujące stany:
1. Myślenie - Filozof nie korzysta z widelców przez losowy okres czasu.
2. Głód - Filozof próbuje zdobyć dwa sąsiadujące widelce, aby rozpocząć jedzenie.
3. Jedzenie - Po uzyskaniu obu widelców, filozof je przez określony czas, a następnie zwraca widelce i wraca do myślenia.


## Koncepcja programu
- Wątki - każdy filozof to osobny wątek, który symuluje niezależne działanie.
- Zasoby współdzielone - widelce są reprezentowane przez mutexy, przechowywane są w wektorze `vector<mutex> forks`.
- Synchronizacja wyjścia: Globalny mutex `cout_mutex` gwarantujący synchronizację wypisywania komunikatów.

## Wyzwania i napotkane problemy
### Wyzwania
1. Zapobieganie sytuacji, w której wszyscy filozofowie blokują jeden widelec, uniemożliwiając dalsze działanie. (ang. Deadlock)
2. Zapewnienie, że tylko jeden filozof może używać danego widelca w danej chwili.
3. Gwarancja, że każdy filozof w końcu uzyska dostęp do zasobów.

### Problemy
- **Możliwy deadlock przy równoczesnym sięganiu po widelce** - Rozwiązano wprowadzeniem hierarchii zasobów – filozofowie zawsze najpierw próbują pobrać widelec o niższym indeksie, co eliminuje możliwość cyklicznego blokowania.
- **Niezsynchronizowany cout** - Rozwiązaniem było dodanie globalnego mutexa `cout_mutex` z wykorzystaniem `std::lock_guard`, aby komunikaty w konsoli były spójne.
- **Inicjalizacja wektora mutexów** - Próba użycia `vector<mutex> forks(philosophersCount)` prowadziła do błędów kompilacji (mutexy nie są kopiowalne). Rozwiązano inicjalizując wektor z domyślnie skonstruowanymi mutexami poprzez `vector<mutex>(n)` - brak potrzeby kopiowania mutexów.

### Zastosowane strategie
1. Zapobieganie zakleszczeniom - Filozofowie zawsze blokują widelce w ustalonej kolejności (najpierw widelec o niższym indeksie). Eliminuje to możliwość powstania cyklu czekania (np. gdy wszyscy czekają na prawy widelec).  
Przykładowo dla `n = 5`:

Filozof 4 próbuje podnieść widelec 0, a potem 4 dzięki obliczeniu `first = min(4,0)`;  

Jeżeli widelec 4 jest zajęty, filozof 4 czeka nie blokując innych.

2. Synchronizacja dostępu do widelców - Każdy widelec jest chroniony przez `std::mutex`. Metody `lock()` oraz `unlock()` zapewniają, że tylko 1 filozof może trzymać dany widelec w danej chwili

