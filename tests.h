#include <string>

void run_all_tests();

void test_int_basics();
void test_string_type();
void test_custom_type();
void test_range_constructor_raw_pointer();
void test_rule_of_three();
void test_iterators_and_const();
void test_math_operators_edge_cases();
void test_filter_predicate();
void test_stress_reallocation();
void test_files();

// Predicati per filter_out
struct IsEven
{
    bool operator()(int n) const;
};

struct IsLongString
{
    bool operator()(const std::string &s);
};
