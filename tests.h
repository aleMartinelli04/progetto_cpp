/**
 * @file tests.h
 *
 * @brief file di dichiarazione delle funzioni di test
 *
 * File di dichiarazione delle funzioni di test.
 * COntiene anche l'implementazione di due funtori utili ai fini dei test.
 */
#include <string>

/**
 * @brief funzione principale di test
 *
 * Funzione principale di test, si limita a chiamare tutte le altre funzioni.
 */
void run_all_tests();

/**
 * @brief test di set su interi
 *
 * Testa la classe set su degli interi.
 * Vengono testate le funzioni add, remove, contains e size.
 */
void test_set_int();

/**
 * @brief test di set su std::string
 *
 * Testa la classe set su sdt::string.
 * Vengono testati i metodi add, remove, contains e size.
 */
void test_set_string();

/**
 * @brief test di set su tipo custom
 *
 * Testa la classe set sul tipo custo point.
 * Viene utilizzato anche il funtore ArePointEqual.
 * Vengono testati i metodi add, remove, contains e size.
 * Viene testato l'operatore [] per l'accesso random ai dati.
 */
void test_set_point();

/**
 * @brief test del costruttore dati due iteratori
 *
 * Viene testato il costruttore dati due puntatori a inizio e fine sequenza.
 * Viene testato anche il caso in cui i due iteratori non contengano dati in mezzo.
 */
void test_constructor_iterators();

/**
 * @brief test di costruttore di copia, operatore di assegnamento e auto-assegnamento
 *
 * Viene testato il costruttore di copia, l'operatore di assegnamento e l'auto assegnamento.
 * Viene testato l'operatore == tra due set.
 */
void test_copy_constructor_assignment();

/**
 * @brief test di iterator e const_iterator
 *
 * Vengono testati l'iterator e il const_iterator.
 * La modifica degli elementi non è consentita per evitare di violare la non duplicazione dei dati.
 */
void test_iterators();

/**
 * @brief test di union e intersection
 *
 * Vengono testati gli operatori di unione (+) e di intersezione (-).
 */
void test_union_intersection();

/**
 * @brief test della funzione filter_out
 *
 * Viene testata la funzione filter_out con std::string e il predicato IsLongString.
 */
void test_filter_out();

/**
 * @brief test di numerose add e remove
 *
 * Viene testato il comportamento della classe set su numerose add e remove.
 * Questo comporta molte chiamate a new[] e delete[].
 */
void test_stress_reallocation();

/**
 * @brief test delle funzioni sui file
 *
 * Vengono testate le funzioni load e save su file con tipo custom.
 * Il tipo custom point ha implementato il metodo di stampa su stream.
 */
void test_files();

/**
 * @brief implementazione dell'operatore () del funtore IsEven
 *
 * Viene implementato l'operatore () del funtore IsEven.
 * Ritorna se un numero passato come input è pari o no.
 *
 * @param n numero in input
 *
 * @return true se n è pari, false altrimenti
 */
struct IsEven
{
    bool operator()(int n) const;
};

/**
 * @brief implementazione dell'operatore () del funtore IsLongString
 *
 * Viene implementato l'operatore () del funtore IsLongString.
 * Ritorna se una stringa passata in input è "lunga" o no.
 * Una stringa viene considerata lunga se ha più di 3 caratteri.
 *
 * @param s stringa in input
 *
 * @return true se s è lunga, false altrimenti
 */
struct IsLongString
{
    bool operator()(const std::string &s);
};
