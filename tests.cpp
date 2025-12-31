/**
 * @file tests.cpp
 *
 * @brief file di implementazione delle funzioni di test
 *
 * File di implementazione delle funzioni di test.
 * Contiene anche l'implementazione di due funtori utili ai fini dei test.
 */
#include <iostream>
#include <cassert>    // assert
#include <functional> // std::equal_to
#include "set.hpp"
#include "point.h"
#include "tests.h"

void run_all_tests()
{
    std::cout << "========================================" << std::endl;
    std::cout << "      INIZIO TEST SUITE COMPLETA        " << std::endl;
    std::cout << "========================================" << std::endl;

    test_set_int();
    test_set_string();
    test_set_point();
    test_constructor_iterators();
    test_copy_constructor_assignment();
    test_iterators();
    test_union_intersection();
    test_filter_out();
    test_stress_reallocation();
    test_files();

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "   TUTTI I TEST PASSATI CON SUCCESSO!   " << std::endl;
    std::cout << "========================================" << std::endl;
}

void test_set_int()
{
    std::cout << "[1] Test int... ";

    set<int, std::equal_to<int>> s;
    assert(s.size() == 0);

    // Test rimozione da vuoto
    s.remove(10);
    assert(s.size() == 0);

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

void test_set_string()
{
    std::cout << "[2] Test std::string... ";

    set<std::string, std::equal_to<std::string>> s;

    s.add("Hello");
    s.add("World");
    s.add("C++");
    s.add("Hello"); // Duplicato

    assert(s.size() == 3);
    assert(s.contains("World"));

    s.remove("Hello");
    assert(s.size() == 2);

    std::cout << "OK" << std::endl;
}

void test_set_point()
{
    std::cout << "[3] Test Custom Type... ";

    set<point, ArePointEqual> s;
    point p1 = {0, 0};
    point p2 = {1, 2};
    point p3 = {0, 0}; // Uguale a p1

    s.add(p1);
    s.add(p2);
    s.add(p3); // Non deve essere aggiunto

    assert(s.size() == 2);
    assert(s.contains({1, 2}));

    // Test operator[]
    bool found_p1 = false;
    bool found_p2 = false;

    for (unsigned int i = 0; i < s.size(); ++i)
    {
        if (ArePointEqual()(s[i], p1))
        {
            found_p1 = true;
        }

        if (ArePointEqual()(s[i], p2))
        {
            found_p2 = true;
        }
    }

    assert(found_p1);
    assert(found_p2);

    s.remove({0, 0});
    assert(!s.contains({0, 0}));

    std::cout << "OK" << std::endl;
}

void test_constructor_iterators()
{
    std::cout << "[4] Test Iterator Constructor... ";

    int arr[] = {1, 2, 3, 2, 4, 1}; // Duplicati: 1, 2
    int size_arr = 6;

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

void test_copy_constructor_assignment()
{
    std::cout << "[5] Test Copy Constructor, Assignment, Auto-assignment... ";

    set<int, std::equal_to<int>> s1;
    s1.add(10);
    s1.add(20);

    // Costruttore di copia
    set<int, std::equal_to<int>> s2(s1);
    assert(s2 == s1);
    assert(s2.size() == 2);

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
    s3 = s3;
    assert(s3.size() == 2);
    assert(s3.contains(10));

    std::cout << "OK" << std::endl;
}

void test_iterators()
{
    std::cout << "[6] Test Iterators... ";

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

void test_union_intersection()
{
    std::cout << "[7] Test Operatori di unione (+) e di intersezione (-)... ";

    set<int, std::equal_to<int>> A;
    A.add(1);
    A.add(2);

    set<int, std::equal_to<int>> B;
    B.add(3);
    B.add(4);

    set<int, std::equal_to<int>> Empty;

    // A={1,2}, B={3,4}, Empty={}

    // Unione (A + B): {1,2,3,4}
    set<int, std::equal_to<int>> U = A + B;
    assert(U.size() == 4);

    // Intersezione (A - B): {}
    set<int, std::equal_to<int>> I = A - B;
    assert(I.size() == 0);

    set<int, std::equal_to<int>> A_clone = A;

    // Unione (A + A = A): {1,2}
    assert((A + A_clone).size() == 2);
    assert((A + A_clone) == A);

    // Intersezione (A - A): {1,2}
    assert((A - A_clone).size() == 2);
    assert((A - A_clone) == A);

    // A + Empty = A
    assert((A + Empty) == A);

    // A - Empty = Empty
    assert((A - Empty).size() == 0);

    // Empty - A = Vuoto
    assert((Empty - A).size() == 0);

    std::cout << "OK" << std::endl;
}

void test_filter_out()
{
    std::cout << "[8] Test Filter Out... ";

    set<std::string, std::equal_to<std::string>> s;
    s.add("no");
    s.add("yes");
    s.add("maybe");
    s.add("ok");

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

    for (int i = 0; i < 100; ++i)
    {
        s.add(i);
    }
    assert(s.size() == 100);

    for (int i = 0; i < 100; ++i)
    {
        s.remove(i);
    }
    assert(s.size() == 0);

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
        std::cout << "Errore save" << std::endl;
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
        std::cout << "Errore load" << std::endl;
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