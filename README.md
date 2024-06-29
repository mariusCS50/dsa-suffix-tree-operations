# TEMA 2 - SDA

A fost o temă foarte interesantă, care mi-a readus aminte de lucrul cu diferite tipuri de arbori, în cazul nostru a trie-ului și arborelui de sufixe.

Tema a mers mult mai rapid ca prima, dat fiind că, în liceu când mă pregăteam de olimpiade, am învâțat cum să lucrez cu unele structuri de arbori și să fac algoritmii de parcurgere în adâncime și lățime, pe care i-am folosit și în temă.

Am mers pe aceleași principii de simplitate a codului, am încercat să-l fac cât mai lizibil și ușor de înțeles, am folosit la această temă și comentarii inline pe alocuri pentru mici precizări.

Logica pe care am folosit-o la rezolvarea taskurilor este destul de simplă, o să explic mai în detaliu la paragrafele respective fiecărui fișier, însă am și niște comentarii prin cod care mai explică mersul rezolvării.

Nu am folosit bucăți de cod de pe net, de la lab-uri sau generate de inteligență artificială, codul este scris în totalitate de mine.

Codul funcționează perfect, exact cum era de așteptat, punctajul obținut local este maxim, inclusiv punctajul bonus pentru valgrind.

Pentru a corecta erorile de conding style am folosit clang-tidy și cpplint.

## tree.h

La această temă am ales să folosesc doar un header file, deoarece funcțiile care execută operațiile aferente unor taskuri necesită utilizarea unei cozi. Am încercat să fac coada într-un header separat la început, însă coada are nevoie de structure de arbore, iar funcțiile pentru structura de arbore au nevoie de coadă, deci se obține un fel de loop de dependențe.

În header file sunt prezente definițiile structurilor pentru coadă și arbore, împreună cu funcțiile de prelucrare ale cozii, respectiv, ale arborelui.

În structura arborelui am ales să mai adaug următoarele elemente pentru a-mi ușura cu mult executarea taskurilor cerute:
```c
int level;  // nivelul nodului curent
int num_children;  // numărul de copii al nodului curent
 char *info;  // informația din nodul curent
```

## tree.c

Aici sunt toate declarațiile funcțiilor din tree.h, începând cu cele care țin de coadă, după care urmează cele ce țin de arbore.

Aici am mai făcut o funcție get_index() pentru a-mi ușura conversia dintre caracterul curent al unui sufix și poziția sa în lista de copii.

Funcțiile ce țin de coadă sunt destul de intuitive, realizeză operațiile clasice de creare / distrugere și inserare / extragere, singura diferență este că în loc de un int ca informație avem un nod.

În ceea ce ține de arbore, avem funcțiile clasice:
1) funcția de alocare a unui element create_node()
2) funcția care la finalul codului eliberează memoria ocupată de arbore destroy_tree()

Funția add_every_suffix() adaugă caracterul '$' la sfârșitul cuvântului, ca în condiția temei, după care iterează prin toate sufixele cuvântului respectiv incrementând adresa sufixului cu iteratorul la fiecare pas, obținându-se de exemplu:

banana$ -> anana$ -> nana$ -> ana$ -> na$ -> a$ -> $

Caracterele fiecărui sufix sunt adăugate pe rănd în arbore în cascadă, adică în jos unul după celălalt.

Funcția show_tree() afișează conținului arborelui pe nivele folosind algoritmul de parcurgere în lățime cu coada de noduri. La început se adaugă rădăcina în coadă, apoi atâta timp cât coada nu e goală, extrage nodul curent și îi adaugă copiii în coadă. Pentru fiecare nod se afișează informația acestuia, adică caracterul, iar variabile check_newline este folosită pentru a verifica dacă am ajuns la sfârșitul unui nivel. În caz afirmativ, se trece în rând nu, iar check_newline devine lungimea curentă a cozii, care reprezintă și nivelul curent.

De exemplu fie orice arbore, rădăcina are 4 fii, deci primul nivel are 4 noduri, check_newline va fi 4. Următoarele 4 parcurgeri vor insera în coadă fiii celor 4 noduri, iar când check_newline va fi 0 în coadă nu vor mai fi cele 4 noduri, ci toate nodurile de pe nivelul 2, mergând pe aceeași logică, mereu când se termină nivelul n, în coadă vor fi exact toate nodurile pentru nivelul n + 1.

Funcția get_tree_stats() calculează numărul de noduri frunză,
numărul de sufixe de lungime k și numărul maxim de descendenți ale unui nod tot printr-o parcurgere în lățime, verificând la fiecare pas dacă numărul de fii ai nodului curent este mai mare ca max_descendants, dacă nodul curent este o frunză sau dacă nivelul curent este egal cu k și există node->children[0], care corespunde cu sfărșitul unui sufix.

Funcția search_suffix este intuitivă, parcurgem căte un caracter pe rând, dacă caracterul curent nu are nodul respectiv alocat atunci nu există sufixul în arbore. Dacă am parcurs toate caracterele verificăm dacă nodul curent este un sfârșit de sufix, în caz contrar returnăm 0.

De exemplu pentru șirul de noduri b-a-n-a-n-a-$, cuvântul bana există pe această ramură din arbore, însă nu este un sufix valid.

Funcția compress_tree() am decis să fac compresia unui arbore deja existend, fiindcă generarea directă a arborelui compresat era mult mai dificilă.

Aici am folosit parcurgerea în adâncime, fiindcă este mult mai ușor de compresat arborele de jos în sus pe câte o ramură, decât la o parcurgere în lățime unde compresia trebuia făcută de sus în jos și în paralel.

Dacă nodul curent are doar un fiu care nu e nod terminal, atunci se concatenează informația fiului la nodul curent și se copie lista și numărul de descendenți ale fiului la nodul părinte, obținându-se un rezultat satisfăcător.

De exemplu ramura b - a - n - a - n - a - $ din temă, parcurgerea în adâncime va ajunge la ultimul nod, și urcânduse în sus va tot comprima ramura:

b - a - n - a - na - $
b - a - n - ana - $
b - a - nana - $
b - anana - $
banana - $

## tema2.c

Aici a rămas doar combinarea tuturor funcționalităților implementate în tree.c.

Identific din parametrii lui *argv[] numărul cerinței curente, după deschid fișierele transmise tot ca parametri prin *argv[], iar dacă numărul cerinței este 2, fișierele sutn cu o poziție mai la dreapta în *argv[] din cauza parametrului adițional k necesar în taskul 2.

După identificarea taskului și deschiderea fișierelor execut funcția respectivă cerinței, după care închid fișierele de I/O.

Toate cele 4 funcții aferente celor 4 cerințe sunt scurte și ușor de înțeles, am încercat diferite variate până am ajuns la această structură, și au cam aceleași 3 părți principale:
1) Aloca arborele și citește numărul de cuvinte
2) Citește cuvintele și construiește arborele
3) Execută funcțiile pentru executarea cerinței și distruge arborele

La început am vrut să fac un array de pointeri la funcții și să execut direct funcția după numărul taskului, însă codul nu era atât de clar, de aceea am ales variata cu switch care e mult mai clară și nu foarte sofisticată.

## Precizare!!!

Am auzit de la unii colegi că trebuie să folosim standarde din c99, și că la for-uri trebuie sp scrim ceva de genul:
```c
int i;
for (i = 0; i < n; i++);
```
Chestia asta este obligatorie? Să vă spun sincer nu îmi place cum arată codul așa, parcă mă dezgustă puțin, cred că variata de variabilă declarată direct în for este mult mai frumoasă.

O să las for-urile din temă cu variabila declarată înăuntru la for, puteți să îmi lăsați un mesaj pe moodle la temă dacă trebuie numaidecât de utilizat acea sintaxă ca să știu pe viitor.