#include <iostream>
#include <cassert>
#include <functional>
#include "set.hpp"
#include "point.h"
#include "tests.h"

void run_all_tests()
{
    std::cout << "========================================" << std::endl;
    std::cout << "      INIZIO TEST SUITE COMPLETA        " << std::endl;
    std::cout << "========================================" << std::endl;

    test_int_basics();
    test_string_type();
    test_custom_type();
    test_range_constructor_raw_pointer();
    test_rule_of_three();
    test_iterators_and_const();
    test_math_operators_edge_cases();
    test_filter_predicate();
    test_stress_reallocation();
    test_files();

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "   TUTTI I TEST PASSATI CON SUCCESSO!   " << std::endl;
    std::cout << "========================================" << std::endl;
}

// =================================================================
// IMPLEMENTAZIONE
// =================================================================

void test_int_basics()
{
    std::cout << "[1] Test Base Int... ";

    set<int, std::equal_to<int>> s;
    assert(s.size() == 0);

    // Test rimozione da vuoto (non deve crashare)
    s.remove(10);
    assert(s.size() == 0);

    // Aggiunte
    s.add(5);
    s.add(10);
    s.add(5); // Duplicato
    s.add(-5);

    assert(s.size() == 3); // {5, 10, -5}
    assert(s.contains(5));
    assert(s.contains(-5));
    assert(!s.contains(0));

    // Rimozione
    s.remove(5);
    assert(s.size() == 2); // {10, -5}
    assert(!s.contains(5));

    // Rimozione elemento non esistente
    s.remove(999);
    assert(s.size() == 2);

    // Svuotamento manuale
    s.remove(10);
    s.remove(-5);
    assert(s.size() == 0);

    std::cout << "OK" << std::endl;
}

void test_string_type()
{
    std::cout << "[2] Test std::string... ";

    set<std::string, std::equal_to<std::string>> s;

    s.add("Hello");
    s.add("World");
    s.add("C++");
    s.add("Hello"); // Duplicato

    assert(s.size() == 3);
    assert(s.contains("World"));

    // Test copia profonda implicita delle stringhe
    std::string temp = "C++";
    assert(s.contains(temp));

    s.remove("Hello");
    assert(s.size() == 2);

    std::cout << "OK" << std::endl;
}

void test_custom_type()
{
    std::cout << "[3] Test Custom Type (point)... ";

    set<point, ArePointEqual> s;
    point p1 = {0, 0};
    point p2 = {1, 2};
    point p3 = {0, 0}; // Logicamente uguale a p1

    s.add(p1);
    s.add(p2);
    s.add(p3); // Non deve essere aggiunto

    assert(s.size() == 2);
    assert(s.contains({1, 2})); // Test con temporaneo

    // Test operator[]
    // Nota: l'ordine dipende dall'inserimento
    bool found_p1 = false;
    bool found_p2 = false;
    for (unsigned int i = 0; i < s.size(); ++i)
    {
        if (ArePointEqual()(s[i], p1))
            found_p1 = true;
        if (ArePointEqual()(s[i], p2))
            found_p2 = true;
    }
    assert(found_p1 && found_p2);

    std::cout << "OK" << std::endl;
}

void test_range_constructor_raw_pointer()
{
    std::cout << "[4] Test Range Constructor (Raw Array)... ";

    int arr[] = {1, 2, 3, 2, 4, 1}; // Duplicati: 1, 2
    int size_arr = sizeof(arr) / sizeof(arr[0]);

    // Costruiamo usando puntatori come iteratori
    set<int, std::equal_to<int>> s(arr, arr + size_arr);

    assert(s.size() == 4); // {1, 2, 3, 4}
    assert(s.contains(1));
    assert(s.contains(4));
    assert(!s.contains(5));

    // Test range vuoto
    set<int, std::equal_to<int>> s_empty(arr, arr); // begin == end
    assert(s_empty.size() == 0);

    std::cout << "OK" << std::endl;
}

void test_rule_of_three()
{
    std::cout << "[5] Test Rule of Three (Copy/Assign)... ";

    set<int, std::equal_to<int>> s1;
    s1.add(10);
    s1.add(20);

    // 1. Copy Constructor
    set<int, std::equal_to<int>> s2(s1);
    assert(s2 == s1);
    assert(s2.size() == 2);

    // Deep copy check
    s1.remove(10);
    assert(s1.size() == 1);
    assert(s2.size() == 2); // s2 non deve cambiare
    assert(s2.contains(10));

    // 2. Assignment Operator
    set<int, std::equal_to<int>> s3;
    s3.add(99);
    s3 = s2; // s3 ora è {10, 20}

    assert(s3 == s2);
    assert(s3.contains(10));
    assert(!s3.contains(99)); // il 99 deve essere sparito

    // 3. Self Assignment
    s3 = s3; // Non deve esplodere e non deve cancellare i dati
    assert(s3.size() == 2);
    assert(s3.contains(10));

    std::cout << "OK" << std::endl;
}

void test_iterators_and_const()
{
    std::cout << "[6] Test Iterators & Const Correctness... ";

    set<int, std::equal_to<int>> s;
    s.add(1);
    s.add(2);
    s.add(3);

    // Test iterazione manuale
    int count = 0;
    set<int, std::equal_to<int>>::iterator it = s.begin();
    while (it != s.end())
    {
        int val = *it;
        assert(val >= 1 && val <= 3);
        count++;
        ++it;
    }
    assert(count == 3);

    // TEST CONST ITERATOR
    const set<int, std::equal_to<int>> const_s(s);

    // begin() su const set deve ritornare const_iterator (che è typedef di iterator nel tuo caso)
    set<int, std::equal_to<int>>::const_iterator cit = const_s.begin();

    assert(*cit == s[0]); // Verifica lettura
    // *cit = 5; // ERRORE DI COMPILAZIONE SE SCOMMENTATO (Corretto!)

    std::cout << "OK" << std::endl;
}

void test_math_operators_edge_cases()
{
    std::cout << "[7] Test Operatori +, - (Edge Cases)... ";

    set<int, std::equal_to<int>> A;
    A.add(1);
    A.add(2);

    set<int, std::equal_to<int>> B;
    B.add(3);
    B.add(4);

    set<int, std::equal_to<int>> Empty;

    // --- TEST 1: DISGIUNTI ---
    // A={1,2}, B={3,4}
    // Unione: {1,2,3,4}
    set<int, std::equal_to<int>> U = A + B;
    assert(U.size() == 4);

    // Intersezione (operator-): Vuota
    set<int, std::equal_to<int>> I = A - B;
    assert(I.size() == 0);

    // --- TEST 2: IDENTICI ---
    set<int, std::equal_to<int>> A_clone = A;

    // Unione (A + A = A): {1,2}
    assert((A + A_clone).size() == 2);
    assert((A + A_clone) == A);

    // Intersezione (A - A = A): {1,2}  <--- QUI C'ERA L'ERRORE
    // Essendo intersezione, A intersecato A da A stesso, non vuoto!
    assert((A - A_clone).size() == 2);
    assert((A - A_clone) == A);

    // --- TEST 3: CON VUOTO ---
    // A + Vuoto = A
    assert((A + Empty) == A);

    // A intersecato Vuoto = Vuoto
    assert((A - Empty).size() == 0);

    // Vuoto intersecato A = Vuoto
    assert((Empty - A).size() == 0);

    std::cout << "OK" << std::endl;
}

void test_filter_predicate()
{
    std::cout << "[8] Test Filter Out... ";

    set<std::string, std::equal_to<std::string>> s;
    s.add("no");
    s.add("yes");
    s.add("maybe");
    s.add("ok");

    // Filtra stringhe lunghe (> 3 caratteri)
    // "maybe" è l'unica lunga
    set<std::string, std::equal_to<std::string>> res = filter_out(s, IsLongString());

    assert(res.size() == 1);
    assert(res.contains("maybe"));
    assert(!res.contains("yes"));

    std::cout << "OK" << std::endl;
}

void test_stress_reallocation()
{
    std::cout << "[9] Stress Test (Riallocazione continua)... ";

    set<int, std::equal_to<int>> s;

    // Aggiungo 100 elementi (0...99)
    // Questo causa 100 chiamate a new[]/delete[]
    for (int i = 0; i < 100; ++i)
    {
        s.add(i);
    }
    assert(s.size() == 100);

    // Rimuovo 100 elementi (0...99)
    // Questo causa altre 100 riallocazioni
    for (int i = 0; i < 100; ++i)
    {
        s.remove(i);
    }
    assert(s.size() == 0);

    // Aggiungo di nuovo per verificare integrità heap
    s.add(1);
    assert(s.size() == 1);

    std::cout << "OK" << std::endl;
}

void test_files()
{
    std::cout << "[10] Test I/O File Completo... ";

    set<point, ArePointEqual> s_out;
    s_out.add({1, 1});
    s_out.add({2, 2});
    s_out.add({3, 3});

    std::string filename = "test_point_set.txt";

    // Salvataggio
    try
    {
        save(s_out, filename);
    }
    catch (...)
    {
        std::cerr << "Errore save" << std::endl;
        assert(false);
    }

    // Caricamento in set esistente (deve sovrascrivere)
    set<point, ArePointEqual> s_in;
    s_in.add({9, 9}); // Questo dato deve sparire

    try
    {
        load(filename, s_in);
    }
    catch (...)
    {
        std::cerr << "Errore load" << std::endl;
        assert(false);
    }

    assert(s_in.size() == 3);
    assert(s_in == s_out);
    assert(!s_in.contains({9, 9}));

    // Test eccezione file mancante
    bool exception_thrown = false;
    try
    {
        set<point, ArePointEqual> s_fail;
        load("file_fantasma_12345.txt", s_fail);
    }
    catch (const std::runtime_error &)
    {
        exception_thrown = true;
    }
    assert(exception_thrown);

    std::cout << "OK" << std::endl;
}

bool IsEven::operator()(int n) const
{
    return (n % 2) == 0;
}

bool IsLongString::operator()(const std::string &s)
{
    return s.length() > 3;
}