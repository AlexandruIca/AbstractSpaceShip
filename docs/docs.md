# Abstract Spaceship
În acest proiect am dezvoltat o simplă aplicație în care se poate vedea o rachetă care zboară în spațiu la infinit:

![Aplicația exemplificată](./AbstractSpaceship.png)

# Cum funcționează per ansamblu? 
Ne folosim de 2 shader-e, unul din ele este pentru rachetă, shader-ul este trivial, iar celălalt este pentru fundal(pentru stele). Pentru a desena stele la infinit ne folosim de același shader de 2 ori, schimbând pentru fiecare shader în parte translația. Când o tranlsație ajunge să fie îndeajuns de mult în afara ferestrei de vizualizare îi schimbăm translația înapoi cum era la început.