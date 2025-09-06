# Notat (C++14 / WinAPI) 

Et simpelt tekstprogram og bibliotek skrevet i C++14 der gør direkte af Windows’ WinAPI. Projektet blev lavet med formål om at lære hvordan man kan lave et grafisk program uden at gøre brug af eksterne frameworks. 

 ! Projektet er stadig under udvikling og er til tider ustabilt. 
 
<img width="838" height="513" alt="image" src="https://github.com/user-attachments/assets/512e5fba-bc61-4f2c-8125-eb2a6f90a749" />

# Formål 

Under udviklingen af projektet arbejdede jeg meget med WinAPI – herunder: 

    At åbne og håndtere et vindue 

    Tegne tekst og grafik 

    Håndtere brugerinput  

Jeg byggede et bibliotek kaldt “Tavle” som forenkler og deler processen af at lave en WinAPI applikation op i mindre genanvendelige moduler. 

# Struktur og moduler  

Projektet består af en template for et Windows vindue samt en række moduler der håndterer forskellige beskeder fra Windows: 

    “BaseWindow”: En base vindue klasse der opretter et vindue og giver moduler adgang til at registrere sig til beskeder. 

    “GraphcisModule”: Kan tegne et “Scene”-objekt, som kan indeholde ting som tekstelementer eller grafiske elementer. 

    “SaveFileModule”: Reagerer på “quit” besked og spørger brugeren om de vil gemmen ændringer. 

    “TextInputModule”: Tager input fra bruger og redigerer et tekstelement så det passer med hvad brugeren gør. 

Med den endelige version af Tavle-bibliotek er det drømmen at man vil kunne lave simple GUI-applikationer uden at direkte skrive WinAPI-kode. 

# Status  

Programmet fungerer teknisk set, men bør ikke bruges som et færdigt værktøj da der mangler vigtige features og er meget et proof-of-concept. 

# Læringsmål 

Gennem projektet har jeg arbejdet med: 

    Windows beskedhåndtering (WM_PAINT, WM_DESTROY, osv.) 

    Modulært design med C++ 

    Low-Level GUI-Programming uden brug af frameworks 

Link til github siden: https://github.com/malkaersig/notat/

