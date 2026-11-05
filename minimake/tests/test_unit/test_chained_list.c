#include <criterion/criterion.h>
#include <stdlib.h>

#include "../../src/include/chained_list.h"

Test(variable_insert, insert)
{
    struct variable *variables = NULL;
    bool result = variable_insert(&variables, "CC", "GCC");

    cr_assert(result);
    cr_assert_str_eq(variables->name, "CC");
    cr_assert_str_eq(variables->value, "GCC");
    cr_assert_null(variables->next);

    free(variables->name);
    free(variables->value);
    free(variables);
}

Test(variable_insert, one_of_the_three_NULL)
{
    bool result = variable_insert(NULL, "CC", "GCC");
    cr_assert_not(result);

    struct variable *variables = NULL;
    bool result1 = variable_insert(&variables, NULL, "GCC");
    cr_assert_not(result1);

    bool result2 = variable_insert(&variables, "CC", NULL);
    cr_assert_not(result2);
    free(variables);
}

Test(variable_set, set_value)
{
    struct variable *variables = NULL;
    bool result = variable_set(&variables, "CC", "GCC");

    cr_assert(result);
    cr_assert_str_eq(variables->name, "CC");
    cr_assert_str_eq(variables->value, "GCC");
    cr_assert_null(variables->next);

    free(variables->name);
    free(variables->value);
    free(variables);
}

Test(variable_get, empty_list)
{
    struct variable *variables = NULL;
    const char *result = variable_get(variables, "CC");
    cr_assert_null(result);
}

Test(variable_get, one_element)
{
    struct variable *variables = NULL;
    cr_assert(variable_set(&variables, "CC", "GCC"));
    const char *result = variable_get(variables, "CC");
    cr_assert_not_null(result);
    cr_assert_str_eq(result, "GCC");

    variables_free(variables);
}

Test(variable_get, one_element_not_found)
{
    struct variable *variables = NULL;

    cr_assert(variable_set(&variables, "CC", "GCC"));
    const char *result = variable_get(variables, "CFLAGS");
    cr_assert_null(result);
    variables_free(variables);
}

Test(variable_get, multiple_elements)
{
    struct variable *variables = NULL;

    cr_assert(variable_set(&variables, "CC", "GCC"));
    cr_assert(variable_set(&variables, "CFLAGS", "-std=c99"));
    cr_assert(variable_set(&variables, "DEBUG", "-g"));

    cr_assert_str_eq(variable_get(variables, "DEBUG"), "-g");
    cr_assert_str_eq(variable_get(variables, "CFLAGS"), "-std=c99");
    cr_assert_str_eq(variable_get(variables, "CC"), "GCC");

    variables_free(variables);
}

Test(variable_get, element_not_found)
{
    struct variable *variables = NULL;

    cr_assert(variable_set(&variables, "CC", "GCC"));
    cr_assert(variable_set(&variables, "CFLAGS", "-std=c99"));

    const char *result = variable_get(variables, "DEBUG");

    cr_assert_null(result);

    variables_free(variables);
}

Test(variable_get, after_update)
{
    struct variable *variables = NULL;

    cr_assert(variable_set(&variables, "CC", "GCC"));
    cr_assert(variable_set(&variables, "CC", "CLANG"));

    const char *result = variable_get(variables, "CC");

    cr_assert_not_null(result);
    cr_assert_str_eq(result, "CLANG");

    variables_free(variables);
}

Test(variable_get, null_name)
{
    struct variable *variables = NULL;

    cr_assert(variable_set(&variables, "CC", "GCC"));

    const char *result = variable_get(variables, NULL);

    cr_assert_null(result);

    variables_free(variables);
}

Test(variable_get, empty_name)
{
    struct variable *variables = NULL;

    cr_assert(variable_set(&variables, "", "VALUE"));

    const char *result = variable_get(variables, "");

    cr_assert_not_null(result);
    cr_assert_str_eq(result, "VALUE");

    variables_free(variables);
}

Test(variables_free, empty_list)
{
    struct variable *variables = NULL;
    variables_free(variables);
}

Test(variables_free, one_element)
{
    struct variable *variables = NULL;

    cr_assert(variable_insert(&variables, "CC", "GCC"));
    cr_assert_not_null(variables);

    variables_free(variables);
}

Test(variables_free, two_elements)
{
    struct variable *variables = NULL;

    cr_assert(variable_insert(&variables, "CC", "GCC"));
    cr_assert(variable_insert(&variables, "CFLAGS", "-std=c99"));

    cr_assert_not_null(variables);
    cr_assert_not_null(variables->next);
    cr_assert_null(variables->next->next);

    variables_free(variables);
}

Test(variables_free, after_update)
{
    struct variable *variables = NULL;

    cr_assert(variable_insert(&variables, "CC", "GCC"));
    cr_assert(variable_set(&variables, "CC", "CLANG"));

    cr_assert_str_eq(variable_get(variables, "CC"), "CLANG");

    variables_free(variables);
}

Test(variables_free, many_elements)
{
    struct variable *variables = NULL;

    for (int i = 0; i < 100; i++)
    {
        cr_assert(variable_insert(&variables, "VAR", "VALUE"));
    }

    variables_free(variables);
}

Test(variable_set, set_two_value)
{
    struct variable *variables = NULL;
    bool result = variable_set(&variables, "CC", "GCC");

    cr_assert(result);
    cr_assert_str_eq(variables->name, "CC");
    cr_assert_str_eq(variables->value, "GCC");
    cr_assert_null(variables->next);

    bool result2 = variable_set(&variables, "CFLAGS", "-std=c99");
    cr_assert(result2);
    cr_assert_str_eq(variables->name, "CFLAGS");
    cr_assert_str_eq(variables->value, "-std=c99");
    cr_assert_not_null(variables->next);

    struct variable *next_variable = variables->next;
    cr_assert_not_null(next_variable);

    cr_assert_str_eq(next_variable->name, "CC");
    cr_assert_str_eq(next_variable->value, "GCC");
    cr_assert_null(next_variable->next);

    free(next_variable->name);
    free(next_variable->value);
    free(next_variable);

    free(variables->name);
    free(variables->value);
    free(variables);
}

Test(get, get_value)
{
    struct variable *variables = NULL;
    bool result = variable_insert(&variables, "CC", "GCC");

    cr_assert(result);
    cr_assert_str_eq(variables->name, "CC");
    cr_assert_str_eq(variables->value, "GCC");
    cr_assert_null(variables->next);

    free(variables->name);
    free(variables->value);
    free(variables);
}
