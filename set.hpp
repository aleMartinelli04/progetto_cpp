/**
 * @file set.hpp
 *
 * @brief file di dichiarazione e definizione della classe set
 *
 * File di dichiarazione e definizioe della classe templata set e di tutti i suoi metodi.
 * Contiene anche dichiarazione e definizione di alcune funzioni globali per:
 * - scrittura su stream
 * - filtraggio
 * - unionone di due set compatibili
 * - lettura da e scrittura su file di testo
 */
#ifndef SET_HPP
#define SET_HPP

#include <iostream>  // cout, ostream
#include <ostream>   // ostream
#include <istream>   // istream
#include <fstream>   // ofstream, ifstream
#include <algorithm> // swap
#include <iterator>  // std::forward_iterator_tag
#include <cstddef>   // std::ptrdiff_t
#include <stdexcept> // std::logic_error, std::runtime_error
#include <cassert>   // assert
#include <string>

/**
 * @brief classe set che rappresenta un insieme
 *
 * La classe set rappresenta un insieme di elementi senza duplicati, in cui l'ordine non conta.
 * È templata su due tipi, che rappresentano:
 * - T: tipo contenuto nel set. È importante che gli oggetti di questo tipo implementino un metodo per stampare su stream
 * - Eql: funtore che prende in input due oggetti di tipo T e ritorna vero se sono equivalenti, falso altrimenti
 *
 * Per rispettare il requisito della minima occupazione di memoria possibile si è scelto di implementare il set mediante un array.
 * Questo array viene ridimensionato ad ogni aggiunta o rimozione di un elemento.
 */
template <typename T, typename Eql>
class set
{
private:
    T *_set;            ///< puntatore a un array di oggetti di tipo T
    unsigned int _size; ///< dimensione dell'array

    Eql _eql; ///< istanza del funtore di confronto

public:
    /**
     * @brief costruttore di default
     *
     * Costruttore di default della classe.
     * Inizializza il set a uno stato coerente vuoto.
     *
     * @post _set == nullptr
     * @post _size == 0
     */
    set() : _set(nullptr), _size(0) {}

    /**
     * @brief costruttore di copia
     *
     * Costruttore di copia della classe.
     * Crea una copia esatta del contenuto del set passato come parametro.
     * In caso di errore durante l'i-esima copia, viene liberata la memoria occupata dalle prime i-1 copie.
     *
     * @param other set da copiare
     *
     * @post _set != other._set
     * @post _set[i] == other._set[i] i=0,...,_size-1
     * @post _size == other._size
     *
     * @throws std::bad_alloc se l'allocazione del nuovo array fallisce
     * @throws ... eventuali eccezioni lanciate dal costruttore di copia o assegnamento di T
     */
    set(const set &other) : _set(nullptr), _size(0)
    {
        if (other._size > 0)
        {
            try
            {
                _set = new T[other._size];
                for (unsigned int i = 0; i < other._size; ++i)
                {
                    _set[i] = other._set[i];
                }
                _size = other._size;
            }
            catch (...)
            {
                clear();
                throw;
            }
        }
    }

    /**
     * @brief costruttore da sequenza di iteratori
     *
     * Crea un nuovo set inserendo gli elementi contenuti tra i due iteratori.
     * Essendo un set, gli elementi duplicati vengono ignorati.
     * La compatibilità tra il tipo puntato dall'iteratore e il tipo T del set è gestita tramite un cast statico esplicito.
     * In caso di errore duratnte l'i-esimo cast, viene liberata la memoria occupata dai primi i-1 cast.
     *
     * @param begin iteratore all'inizio della sequenza. Il valore puntato da questo iteratore viene incluso nel set creato
     * @param end iteratore alla fine della sequenza. Il valore puntato da questo iteratore viene escluso dal set creato
     *
     * @post _set contiene gli elementi contenuti tra i due iteratori castati al tipo T (esclusi eventuali duplicati)
     * @post _size <= numero di elementi compresi tra begin e end
     *
     * @throw std::bad_alloc se lanciata dal meotodo add
     * @throw ... eventuali eccezioni lanciate dalla conversione dei tipi o dal costruttore di copia di T
     */
    template <typename IterT>
    set(IterT begin, IterT end) : _set(nullptr), _size(0)
    {
        try
        {
            while (begin != end)
            {
                const T &value = static_cast<T>(*begin);
                add(value);
                ++begin;
            }
        }
        catch (...)
        {
            clear();
            throw;
        }
    }

    /**
     * @brief metodo distruttore
     *
     * Distruttore della classe.
     * Libera la memoria occupata da _set e reimposta allo stato coerente vuoto.
     *
     * @post _size == 0
     * @post _set == nullptr
     */
    ~set()
    {
        clear();
    }

    /**
     * @brief metodo per la cardinalità del set
     *
     * Metodo per ottenere la cardinalità del set.
     * Questo metodo non altera lo stato della classe.
     *
     * @return cardinalità del set
     */
    unsigned int size() const
    {
        return _size;
    }

    /**
     * @brief aggiunge un elemento al set
     *
     * Aggiunge l'elemento passato come parametro al set.
     * Se l'elemento era già presente l'operazione viene ignorata, in quanto il set non ammette duplicati.
     * Per rispettare la minima occupazione di memoria viene creato un nuovo array di dimensione _size + 1,
     * che contiene esattamente gli stessi valori contenuti da _set e in ultima posizione l'elemento da aggiungere.
     *
     * @param element valore da aggiungere al set
     *
     * @post contains(element) == true
     * @post _size incrementata di 1 se l'elemento non era già presente
     * @post _size invariata se l'elemento era già presente
     *
     * @throws std::bad_alloc se l'allocazione del nuovo array fallisce
     * @throws ... eventuali eccezioni lanciate dal costruttore di copia o assegnamento di T
     */
    void add(const T &element)
    {
        if (contains(element))
        {
            return;
        }

        T *copySet = new T[_size + 1];

        try
        {
            for (unsigned int i = 0; i < _size; ++i)
            {
                copySet[i] = _set[i];
            }

            copySet[_size] = element;
        }
        catch (...)
        {
            delete[] copySet;
            throw;
        }

        delete[] _set;

        _set = copySet;
        ++_size;
    }

    /**
     * @brief rimuove un elemento dal set
     *
     * Cerca l'elemento specificato nel set e, se presente, lo rimuove.
     * Se l'elemento non è contenuto nel set, l'operazione non ha effetto.
     * L'elemento viene cercato mediante il funtore Eql.
     * In caso di errore durante l'i-esima copia, viene liberata la memoria occupata dalle prime i-1 copie.
     * Inoltre, l'operazione viene annullata senza intaccare lo stato precedente.
     *
     * @param element valore da rimuovere
     *
     * @post contains(element) == false
     * @post _size decrementata di 1 se l'elemento era presente
     * @post _size invariata se l'elemento non era presente
     *
     * @throws std::bad_alloc se l'allocazione del nuovo array fallisce
     * @throws ... eventuali eccezioni lanciate dal confronto o dall'assegnamento di T
     */
    void remove(const T &element)
    {
        if (!contains(element))
        {
            return;
        }

        if (_size == 1)
        {
            clear();
            return;
        }

        T *copySet = new T[_size - 1];

        try
        {
            for (unsigned int i = 0, j = 0; i < _size; ++i)
            {
                if (_eql(_set[i], element))
                {
                    continue;
                }

                copySet[j] = _set[i];
                ++j;
            }
        }
        catch (...)
        {
            delete[] copySet;
            throw;
        }

        delete[] _set;

        _set = copySet;
        --_size;
    }

    /**
     * @brief ricerca un elemento nel set
     *
     * Cerca se l'elemento è presente o meno nel set.
     * Per confrontare gli elementi viene usato il funtore Eql.
     * Questo metodo non altera lo stato della classe.
     *
     * @param element elemento da cercare
     *
     * @return true se l'elemento è presente, false altrimenti
     */
    bool contains(const T &element) const
    {
        for (unsigned int i = 0; i < _size; ++i)
        {
            if (_eql(_set[i], element))
            {
                return true;
            }
        }

        return false;
    }

    /**
     * @brief operatore di assegnamento tra due set
     *
     * Ridefinizione dell'operatore di assegnamento tra due set compatibili.
     * In caso di errore l'operazione viene annullata.
     *
     * @param rhs elemento di destra dell'operazione, da cui vanno copiati i dati
     *
     * @post _set[i] == rhs._set[i] i=0,...,rhs._size
     * @post _size == rhs._size
     *
     * @return reference al set modificato
     *
     * @throws std::bad_alloc se lanciata dal costruttore di copia
     * @throws ... eventuali eccezioni lanciate dal costruttore di copia o assegnamento di T
     */
    set &operator=(const set &rhs)
    {
        if (this != &rhs)
        {
            set tmp(rhs);

            std::swap(_set, tmp._set);
            std::swap(_size, tmp._size);
        }

        return *this;
    }

    /**
     * @brief operatore di accesso diretto all'i-esimo elemento
     *
     * Ridefinizione dell'operatore di accesso diretto all'i-esimo elemento.
     * Non è possibile modificare l'elemento ritornato per non violare il principio di singolarità degli elementi.
     * Questo metodo non altera lo stato della classe.
     *
     * @param i indice dell'elemento da ottenere
     *
     * @pre i < _size
     *
     * @return elemento in posizione i
     */
    const T &operator[](unsigned int i) const
    {
        assert(i < _size);
        return _set[i];
    }

    /**
     * @brief operatore di confronto tra due set
     *
     * Ridefinizione dell'operatore di confronto tra due set.
     * Questo metodo non altera lo stato della classe.
     *
     * @param other secondo set da confrontare
     *
     * @return false se i due set hanno numero diverso di elementi,
     *         oppure true se tutti gli elementi in questo set sono presenti anche in other,
     *         false altrimenti
     */
    bool operator==(const set &other) const
    {
        if (_size != other._size)
        {
            return false;
        }

        for (unsigned int i = 0; i < _size; ++i)
        {
            if (!other.contains(_set[i]))
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief operatore di intersezione tra due set
     *
     * Ridefinizione dell'operatore di sottrazione tra due set.
     * Viene applicata la definizione di intersezione insiemistica.
     * Questo metodo non altera lo stato della classe, in quanto viene creato un nuovo set.
     *
     * @param other secondo set da intersecare
     *
     * @return un set che corrisponde all'intersezione insiemistica tra i due set
     *
     * @throws std::bad_alloc se lanciata da add
     * @throws ... eventuali eccezioni lanciate dal costruttore di copia o assegnamento di T
     */
    set operator-(const set &other) const
    {
        set result;

        for (unsigned int i = 0; i < _size; ++i)
        {
            if (other.contains(_set[i]))
            {
                result.add(_set[i]);
            }
        }

        return result;
    }

    class const_iterator; // forward declaration

    typedef const_iterator iterator; // dichiarazione di iterator come alias di const_iterator

    /**
     * @brief iteratore costante della classe set
     *
     * Rappresenta un iteratore costante per la classe set.
     * È un forward const_iterator.
     * In sostanza è un wrapper per un puntatore a un oggetto di tipo T.
     */
    class const_iterator
    {
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef const T *pointer;
        typedef const T &reference;

        /**
         * @brief costruttore di default
         *
         * Costruttore di default per il const iterator.
         * Inizializza a nullptr.
         *
         * @post _t == nullptr
         */
        const_iterator() : _t(nullptr) {}

        /**
         * @brief costruttore di copia
         *
         * Costruttore di copia per il const iterator.
         * Copia il puntatore dell'iterator passato come parametro.
         *
         * @param other const_iterator da cui copiare il puntatore
         *
         * @post _t == other._t
         */
        const_iterator(const const_iterator &other) : _t(other._t) {}

        /**
         * @brief operatore di assegnamento
         *
         * Assegna il valore dell'iteratore passato come parametro a questo iteratore.
         *
         * @post _t == other._t
         *
         * @return l'iteratore corrente
         */
        const_iterator &operator=(const const_iterator &other)
        {
            _t = other._t;
            return *this;
        }

        /**
         * @brief metodo distruttore
         *
         * Distruttore della classe const_iterator.
         */
        ~const_iterator() {}

        /**
         * @brief operatore di dereferenziamento
         *
         * Operatore di dereferenziamento dell'iteratore.
         *
         * @return il valore puntato dall'iteratore
         */
        reference operator*() const
        {
            return *_t;
        }

        /**
         * @brief operatore freccia
         *
         * Operatore freccia dell'iteratore.
         *
         * @return il puntatore attuale dell'iteratore
         */
        pointer operator->() const
        {
            return _t;
        }

        /**
         * @brief operatore di post-incremento
         *
         * Operatore di post-incremento dell'iteratore.
         * Sposta avanti l'iteratore e ritorna una copia nello stato precendente.
         *
         * @post _t == ++_t_precedente
         *
         * @return un iteratore nello stato precedente alla chiamata
         */
        const_iterator operator++(int)
        {
            const_iterator tmp(*this);
            ++_t;
            return tmp;
        }

        /**
         * @brief operatore di pre-incremento
         *
         * Operatore di pre-incremento dell'iteratore.
         * Sposta avanti l'iteratore e lo ritorna.
         *
         * @return l'iteratore aggiornato
         */
        const_iterator &operator++()
        {
            ++_t;
            return *this;
        }

        /**
         * @brief operatore di uguaglianza
         *
         * Operatore di uguaglianza dell'iteratore.
         *
         * @param other iteratore da confrontare
         *
         * @return true se _t == other._t, false altrimenti
         */
        bool operator==(const const_iterator &other) const
        {
            return _t == other._t;
        }

        /**
         * @brief operatore di disuguaglianza
         *
         * Operatore di disuguaglianza dell'iteratore.
         *
         * @param other iteratore da confrontare
         *
         * @return true se _t != other._t, false altrimenti
         */
        bool operator!=(const const_iterator &other) const
        {
            return _t != other._t;
        }

    private:
        const T *_t; ///< puntatore ad un oggetto costatnte di tipo T

        friend class set;

        /**
         * @brief costruttore privato di inizializzazione
         *
         * Costruttore privato di inizializzazione del const_iterator.
         *
         * @param t puntatore ad un oggetto di tipo T
         *
         * @post _t == t
         */
        const_iterator(pointer t) : _t(t) {}
    }; // const_iterator

    /**
     * @brief iteratore di inizio
     *
     * Ritorna l'iteratore di inizio sequenza del set.
     * Questo metodo non altera lo stato della classe.
     *
     * @return l'iteratore di inizio sequenza del set
     */
    iterator begin() const
    {
        return iterator(_set);
    }

    /**
     * @brief iteratore di fine
     *
     * Ritorna l'iteratore di fine sequenza del set.
     * Questo metodo non altera lo stateo della classe.
     *
     * @return l'iteratore di fine sequenza del set
     */
    iterator end() const
    {
        return iterator(_set + _size);
    }

private:
    /**
     * @brief metodo di pulizia della memoria occupata
     *
     * Libera la memoria occupata dal set corrente e reimposta allo stato coerente iniziale vuoto.
     *
     * @post _set == nullptr
     * @post _size == 0
     */
    void clear()
    {
        delete[] _set;
        _set = nullptr;
        _size = 0;
    }
}; // set

/**
 * @brief funzione globale per stampare un set su stream
 *
 * Stampa un set su stream nel seguente formato:
 * - {e1, e2, ..., en}
 * È importante che sia possibile stampare l'oggetto di tipo T su stream.
 * L'implementazione di questa funzione viene lasciata a chi utilizza la classe set.
 *
 * @param os stream di output su cui stampare
 * @param s set da stampare
 */
template <typename T, typename Eql>
std::ostream &operator<<(std::ostream &os, const set<T, Eql> &s)
{
    typename set<T, Eql>::const_iterator i, ie;

    i = s.begin();
    ie = s.end();

    os << "{";

    while (i != ie)
    {
        os << *i;
        i++;

        if (i != ie)
        {
            os << ", ";
        }
    }

    os << "}";

    return os;
}

/**
 * @brief funzione per filtrare un set
 *
 * Filtra un set in input con un predicato P.
 * Crea un nuovo set che contiene solo gli elementi che rispettano P.
 *
 * @param S set da filtrare
 * @param pred predicato booleano che prende in input un oggetto di tipo T
 *
 * @return un set contenente tutti e soli gli elementi di S che rispettano pred
 */
template <typename T, typename Eql, typename P>
set<T, Eql> filter_out(const set<T, Eql> &S, P pred)
{
    set<T, Eql> result;

    typename set<T, Eql>::const_iterator i, ie;
    i = S.begin();
    ie = S.end();

    while (i != ie)
    {
        if (pred(*i))
        {
            result.add(*i);
        }

        i++;
    }
    return result;
}

/**
 * @brief operatore di unione insiemistica
 *
 * Ridefinizione dell'operatore somma tra due set compatibili.
 * Crea un set che contiene l'unione dei due set su cui viene chiamato l'operatore.
 *
 * @param left set di sinistra
 * @param right set di destra
 *
 * @return nuovo set contenente l'unione insiemistica dei due set precedenti
 */
template <typename T, typename Eql>
set<T, Eql> operator+(const set<T, Eql> &left, const set<T, Eql> &right)
{
    set<T, Eql> result = left;

    typename set<T, Eql>::const_iterator i, ie;
    i = right.begin();
    ie = right.end();

    while (i != ie)
    {
        result.add(*i);
        i++;
    }

    return result;
}

/**
 * @brief funzione per salvare un set su un file
 *
 * Funzione per salvare un set su un file di testo.
 * Il formato per il salvataggio da file è il seguente:
 * - prima riga: lunghezza
 * - dalla seconda riga in poi: un elemento per riga
 * È quindi importante che sia possibile stampare il tipo templato T su stream.
 *
 * @param s set da salvare
 * @param filename stringa contenente il file da salvare
 *
 * @throw std::runtime_error se il file non viene aperto
 */
template <typename T, typename Eql>
void save(const set<T, Eql> &s, const std::string &filename)
{
    std::ofstream ofs(filename);
    if (!ofs.is_open())
    {
        throw std::runtime_error("File can't be opened!");
    }

    ofs << s.size() << std::endl;

    typename set<T, Eql>::const_iterator i, ie;
    i = s.begin();
    ie = s.end();

    while (i != ie)
    {
        ofs << *i << std::endl;
        i++;
    }
    ofs.close();
}

/**
 * @brief funzione per leggere un set da un file di testo
 *
 * Funzione per leggere un set e il suo contenuto da un file di testo.
 * Il formato del file deve essere il seguente:
 * - prima riga: lunghezza
 * - dalla seconda riga in poi: un elemento per riga
 * È necessario che venga implementato l'operatore di lettura da stream per il tipo templato T.
 *
 * @throw std::runtime_error se il file non esiste
 * @throws std::bad_alloc dal metodo add
 * @throws ... eventuali eccezioni lanciate dal costruttore di copia o assegnamento di T
 */
template <typename T, typename Eql>
void load(const std::string &filename, set<T, Eql> &s)
{
    std::ifstream ifs(filename);
    if (!ifs.is_open())
    {
        throw std::runtime_error("File can't be opened!");
    }

    set<T, Eql> temp;
    unsigned int count;
    ifs >> count;

    T val;
    while (count > 0)
    {
        ifs >> val;
        temp.add(val);

        --count;
    }

    s = temp;
    ifs.close();
}
#endif