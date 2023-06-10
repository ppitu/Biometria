# Biometria

Temat: Rozpoznawanie twarzy wg. metody Fishera
Autorzy: Aleksandra Chrzanowska, Piotr Chmura

Projekt na zajęcia z Biometri na Uniwersytecie Jagielońskim na wydziale Fizyki, Astronomii i Informatyki stosowanej.

## Baza danych

W projekcie została użyta baza danych: "AT&T Database of Faces"

Zawiera zbiór obrazów twarzy wykonanych między kwietniem 1992 a kwietniem 1994 roku.

Baza zawiera dziesięć różnych obrazów dla każdego z 40 różnych podmiotów. Dla niektórych podmiotów wykonano zdjęcia w różnych czasach, zmieniając oświetlenie, wyraz twarzy (otwarte / zamknięte oczy, uśmiechnięta / nieuśmiechnięta twarz) oraz detale twarzy (z okularami / bez okularów). Wszystkie zdjęcia wykonano na jednolitym ciemnym tle, a osoby znajdowały się w pozycji pionowej, frontalnej (z pewnym marginesem tolerancji dla ruchu bocznego).

Pliki są w formacie PGM i mogą być wygodnie wyświetlane na systemach UNIX przy użyciu programu "xv". Rozmiar każdego obrazu to 92x112 pikseli, z 256 poziomami szarości na piksel. Obrazy są zorganizowane w 40 katalogach (po jednym dla każdego podmiotu).

https://www.kaggle.com/datasets/kasikrit/att-database-of-faces

## Metoda Fisher

Algorytm Fishera służy do rozpoznawania twarzy i wykorzystuje liniową analizę dyskryminacyjną (LDA) w połączeniu z analizą głównych składowych (PCA).

Algorytm ten opiera się na obliczeniach macierzy kowariancji wewnątrzklasowej (Sw) i międzyklasowej (Sb). Optymalne wektory własne (Fisherfaces) są uzyskiwane poprzez rozwiązanie ogólnego rozkładu wartości własnych. Wektory własne odpowiadające niezerowym wartościom własnym są wykorzystywane jako podstawowe wektory definiujące podprzestrzeń, w której odbywa się klasyfikacja.

Ogólne kroki algorytmu:

1. Przygotowanie danych treningowych: Algorytm Fischer wymaga zbioru danych treningowych, które składają się z obrazów twarzy wraz z odpowiadającymi im        etykietami (np. identyfikatorami osób). Każda twarz musi być reprezentowana jako wektor cech.

2. Obliczanie średniej twarzy: Na początku oblicza się średnią twarz na podstawie zbioru danych treningowych. Średnia twarz jest obliczana jako średnia wartość pikseli dla wszystkich twarzy w zbiorze treningowym.

3. Obliczanie macierzy kowariancji: Następnie oblicza się macierz kowariancji na podstawie danych treningowych. Macierz ta zawiera informacje o zależnościach między pikselami na twarzach.

4. Obliczanie wektorów własnych: Algorytm Fishera wykorzystuje analizę głównych składowych (PCA) do obliczenia wektorów własnych. Wektory własne są obliczane na podstawie macierzy kowariancji i reprezentują kierunki największej wariancji w danych treningowych.

5. Obliczanie wektorów Fishera: W tym kroku wykorzystuje się analizę dyskryminacyjną Fishera (LDA), aby przekształcić wektory własne PCA w tzw. wektory Fishera. Wektory Fishera są obliczane tak, aby maksymalizować odległość między różnymi klasami twarzy, a jednocześnie minimalizować wariancję wewnątrzklasową.

6. Redukcja wymiarów: W celu zmniejszenia złożoności obliczeniowej i redukcji wymiarowości danych można wybrać tylko kilka najważniejszych wektorów Fishera. Ostateczna reprezentacja twarzy jest tworzona przez projekcję twarzy treningowych na wybrane wektory Fishera.

7. Klasyfikacja: Po redukcji wymiarowości danych, można zastosować klasyfikator, tak jak w przypadku innych metod rozpoznawania twarzy, aby przyporządkować nową twarz do jednej z klas na podstawie porównania z twarzami treningowymi.

Algorytm Fishera jest wykorzystywany do rozpoznawania twarzy w różnych zastosowaniach, takich jak systemy monitoringu, systemy kontroli dostępu i identyfikacja osób na zdjęciach. Pozwala on na efektywne wyodrębnienie istotnych cech twarzy i skuteczne rozpoznawanie.

## Projekt

## Wnioski