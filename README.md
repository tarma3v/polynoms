English:
<big>Polynomials Project</big>  
Tarmaev Alexander, 2021

<big>Purpose of the project</big>  
The main goal of this project is to create a calculator that helps perform basic arithmetic operations with polynomials, such as multiplication, addition, and differentiation.

<big>Project structure</big>  
The project is written in C++ using the SFML library. Here is the start menu:
![](https://disk.yandex.ru/i/TWXYAhL96_ZyMw)  
(MENU.PNG)

<big>How to use the program</big>  
1. Click on the "Enter Polynom" button, then enter a string in the format: {coefficient} {x} {^} {degree}. By default, the coefficient is 1, and the degree is 0 (if not entered, they become equal to the default value). The coefficient and degree are integers (can be negative).
2. Press Enter, after which the polynomial will be displayed in the database (it will appear at the end of the list, and the database can be scrolled to access the desired polynomial).
3. You can click on two polynomials from the database, then choose the operation to be performed with them. The new polynomial is also saved in the database. (For differentiation, only one polynomial is sufficient).

<big>How is data stored?</big>  
All monomials of a polynomial are stored in a struct called Polynom.

struct Polynom {
    int deg; // degree of the monomial
    int coef; // coefficient of the monomial
    Polynom* next; // pointer to the next monomial
};

The polynomials are stored in a struct called Polynom_database.

struct Polynom_database {
    int index; // polynomial number in the database
    Polynom* pol; // pointer to the first monomial of the polynomial
    Polynom_database* next; // pointer to the next polynomial
};

Thus, we use custom-written linked lists to store and process data.

<big>What polynomial functions are implemented?</big>  
Algebraic operations such as *addition*, *subtraction*, *multiplication*, and *differentiation* are implemented. Monomials with coefficients of 0 or multiple monomials with the same degree do not appear (**all like terms are combined**). Functions for *evaluating a polynomial at a point*, *solving reduced polynomials*, and *deleting polynomials from the database* are also written but not yet integrated into the application.

<big>What have I learned?</big>  
While working on this project, I learned to work with pointers, write singly linked lists and work with them, and also gained a more detailed understanding of the SFML library.

Russian / Русский:
<big>Проект Многочлены  
Тармаев Александр, 2021
</big>
____
<big>Цель проекта</big>  
Основная цель этого проекта - сделать 
калькулятор, помогающий выполнять базовые арифметические 
операции с многочленами, такие как умножение, сложение,
взятие производной.

<big>Устройство проекта</big>  
Проект написан на языке C++ с помощью библиотеки SFML.
Вот стартовое меню:
![](https://disk.yandex.ru/i/TWXYAhL96_ZyMw)
(MENU.PNG)  
<big>Как пользоваться программой</big>
1. Нажать на кнопку "Enter Polynom", затем ввести 
   строку в формате: {коэффициент} {x} {^} {степень}. По умолчанию коэффициент = 1,
   а степень = 0 (если их не ввести, то становятся 
   равными значению по умолчанию). Коэффициент и степень -
   целые числа (могут быть отрицательными).
2. Нажать Enter, после этого многочлен отобразится в базе
   (появится сразу в конце списка, базу можно скроллить,
   чтобы обратиться к нужному многочлену).
3. Можно нажать на 2 многочлена из базы, после этого выбрать 
   операцию, которую необходимо сделать с ними. Новый многочлен
   также сохраняется в базу. (Для взятия производной достаточно 
   одного многочлена).
   

<big>Как хранятся данные?</big>   
Все мономы одного многочлена хранятся в `struct Polynom`.

```
struct Polynom {
    int deg; // степень монома
    int coef; // коэфициент монома
    Polynom* next; // указатель на следующий моном
};
```

Многочлены в свою очередь хранятся в `struct Polynom_database`
```
struct Polynom_database {
    int index; // номер многочлена в базе
    Polynom* pol; // указатель на первый моном многочлена
    Polynom_database* next; // указатель на следующий многочлен
};
```
Таким образом, мы используем собственноручно написанные списки для 
хранения и обработки данных.

<big>Какие функции с многочленами реализованы?</big>  
Реализованы алгебраические операции, такие как
*сложение*, *вычитание*, *умножение*, *взятие производной*. При этом не появляются мономы
с коэффициентами 0 или несколько мономов с одинаковой степенью
(**все подобные сокращаются**).
Написаны *значение многочлена в точке*, 
*решение приведённых многочленов*, *удаление многочленов из базы*,
но пока не прикручены к приложению.

<big>Чему я научился?</big>  
Во время работы с данным проектом я научился работать с
указателями, писать односвязные списки и работать с ними, а
также более подробно изучил библиотеку SFML.
