#include <criterion/criterion.h>
#include <stdlib.h>

#include "../../src/include/append.h"
#include "../../src/include/chained_list.h"

static void check(const char *str, const struct variable *variables,
                  const char *expected)
{
    char *result = expand_variables(str, variables);
    cr_assert_not_null(result);
    cr_assert_str_eq(result, expected);

    free(result);
}

Test(expansion, simple_parentheses)
{
    struct variable *variables = NULL;
    cr_assert(variable_set(&variables, "CC", "gcc"));

    check("$(CC)", variables, "gcc");

    variables_free(variables);
}

Test(expansion, simple_braces)
{
    struct variable *variables = NULL;
    cr_assert(variable_set(&variables, "CC", "gcc"));

    check("${CC}", variables, "gcc");

    variables_free(variables);
}

Test(expansion, single_character)
{
    struct variable *variables = NULL;
    cr_assert(variable_set(&variables, "C", "clang"));

    check("$C", variables, "clang");

    variables_free(variables);
}

Test(expansion, double_dollar)
{
    check("$$", NULL, "$");
}

Test(expansion, undefined_variable)
{
    check("$(UNKNOWN)", NULL, "");
}

Test(expansion, undefined_single_character)
{
    check("$X", NULL, "");
}

Test(expansion, ordinary_text)
{
    check("Hello World", NULL, "Hello World");
}

Test(expansion, empty_string)
{
    check("", NULL, "");
}

Test(expansion, text_before_and_after)
{
    struct variable *variables = NULL;
    cr_assert(variable_set(&variables, "NAME", "World"));

    check("Hello $(NAME)!", variables, "Hello World!");

    variables_free(variables);
}

Test(expansion, multiple_variables)
{
    struct variable *variables = NULL;

    cr_assert(variable_set(&variables, "CC", "gcc"));
    cr_assert(variable_set(&variables, "FLAGS", "-Wall"));

    check("$(CC) ${FLAGS}", variables, "gcc -Wall");

    variables_free(variables);
}

Test(expansion, mixed_syntax)
{
    struct variable *variables = NULL;

    cr_assert(variable_set(&variables, "C", "clang"));
    cr_assert(variable_set(&variables, "NAME", "test"));

    check("$C ${NAME} $$", variables, "clang test $");

    variables_free(variables);
}

Test(expansion, variable_with_spaces)
{
    struct variable *variables = NULL;

    cr_assert(variable_set(&variables, "FILES", "foo bar"));

    check("$(FILES)", variables, "foo bar");

    variables_free(variables);
}

Test(expansion, variable_with_empty_value)
{
    struct variable *variables = NULL;

    cr_assert(variable_set(&variables, "EMPTY", ""));

    check("a$(EMPTY)b", variables, "ab");

    variables_free(variables);
}

Test(expansion, missing_parenthesis)
{
    char *result = expand_variables("$(CC", NULL);

    cr_assert_null(result);
    free(result);
}

Test(expansion, missing_brace)
{
    char *result = expand_variables("${CC", NULL);

    cr_assert_null(result);
    free(result);
}

Test(expansion, null_input)
{
    char *result = expand_variables(NULL, NULL);

    cr_assert_null(result);
    free(result);
}
