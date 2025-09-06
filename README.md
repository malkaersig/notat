## Projektet er ikke færdigt!!! 
Også selvom det er et meget simpelt program er det stadig ustabilt

# Simpel WinAPI tekst program skrevet med C++14
Programmet blev lavet for at jeg kunne blive mere bekendt med at bruge Window's WinAPI til at åbne et vindue, tegne tekst til skærmen og håndtere input fra brugeren.

Det meste af koden ligger i et biblotek jeg lavede kaldt "Tavle", det blev skrevet med at det senere skulle kunne blive udvidet i tankerne,
så der er forskellige "Modules" som lytter til messages fra windows og adder sine egen funktinalitet.
F.eks. er der et modul der hedder "GraphcisModule" der tager et "Scene" objekt som består af forskellige andre "Paintable" objekter, og lytter til "HandlePaintMsg" (WM_PAINT) beskeden fra windows og tegner scene til vinduet i Windows. 
Alt det er fuldstændigt eksternt fra "SaveFileModule" der blot lytter til "HandleDestroyMsg", som bliver kaldt når vinduet lukker, og spørger brugeren om de vil gemme evt. ændringer.

Selve programmet "txt-file-reader" arver fra en "BaseWindow" klasse der håndterer at lave et vindue og lader modules forbinde til beskeder fra vinduet. 
I en ideel verden skal en bruger af "Tavle" bibloteket ikke skrive en linje WinAPI kode, men da jeg er stoppet med at lave på dette projekt i mens jeg tilføjede en file browser er der noget grimt WinAPI kode i MainWindow.cpp.

Screenshot af programmet 
<img width="802" height="724" alt="image" src="https://github.com/user-attachments/assets/a2a3b6e2-3320-4bdb-bb54-ae1a6b350698" />
