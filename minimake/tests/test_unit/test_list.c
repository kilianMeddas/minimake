
#include <criterion/criterion.h>

#include "../../src/include/list.h"

Test(rule_insert, insert)
{
    struct parser p = { 0 };
    bool result = rule_insert(&p, "all");
    cr_assert(result);
    cr_assert_not_null(p.rules);
    cr_assert_str_eq(p.rules->target, "all");
    cr_assert_null(p.rules->dependencies);
    cr_assert_null(p.rules->commands);
    cr_assert_null(p.rules->next);
    cr_assert_eq(p.current_rule, p.rules);
    rules_free(p.rules);
}

Test(rule_insert, null_arguments)
{
    struct parser p = { 0 };
    cr_assert_not(rule_insert(NULL, "all"));
    cr_assert_not(rule_insert(&p, NULL));
    cr_assert_null(p.rules);
}

Test(rule_insert, two_rules)
{
    struct parser p = { 0 };

    cr_assert(rule_insert(&p, "all"));
    cr_assert(rule_insert(&p, "clean"));

    cr_assert_str_eq(p.rules->target, "all");
    cr_assert_not_null(p.rules->next);
    cr_assert_str_eq(p.rules->next->target, "clean");
    cr_assert_null(p.rules->next->next);
    cr_assert_eq(p.current_rule, p.rules->next);

    rules_free(p.rules);
}

Test(rule_insert, many_rules)
{
    struct parser p = { 0 };

    for (int i = 0; i < 100; i++)
        cr_assert(rule_insert(&p, "target"));

    int count = 0;
    struct rule *current = p.rules;

    while (current)
    {
        cr_assert_str_eq(current->target, "target");
        count++;
        current = current->next;
    }

    cr_assert_eq(count, 100);

    rules_free(p.rules);
}

Test(dependency_insert, insert)
{
    struct parser p = { 0 };

    cr_assert(rule_insert(&p, "all"));
    cr_assert(dependency_insert(p.current_rule, "main.o"));

    cr_assert_not_null(p.rules->dependencies);
    cr_assert_str_eq(p.rules->dependencies->target, "main.o");
    cr_assert_null(p.rules->dependencies->next);

    rules_free(p.rules);
}

Test(dependency_insert, null_arguments)
{
    struct parser p = { 0 };

    cr_assert_not(dependency_insert(NULL, "main.o"));

    cr_assert(rule_insert(&p, "all"));
    cr_assert_not(dependency_insert(p.current_rule, NULL));
    cr_assert_null(p.rules->dependencies);

    rules_free(p.rules);
}

Test(dependency_insert, multiple_dependencies)
{
    struct parser p = { 0 };

    cr_assert(rule_insert(&p, "all"));
    cr_assert(dependency_insert(p.current_rule, "main.o"));
    cr_assert(dependency_insert(p.current_rule, "utils.o"));
    cr_assert(dependency_insert(p.current_rule, "test.o"));

    struct list *dep = p.rules->dependencies;

    cr_assert_str_eq(dep->target, "main.o");
    dep = dep->next;

    cr_assert_not_null(dep);
    cr_assert_str_eq(dep->target, "utils.o");
    dep = dep->next;

    cr_assert_not_null(dep);
    cr_assert_str_eq(dep->target, "test.o");
    cr_assert_null(dep->next);

    rules_free(p.rules);
}

Test(dependency_insert, different_rules)
{
    struct parser p = { 0 };

    cr_assert(rule_insert(&p, "all"));
    cr_assert(dependency_insert(p.current_rule, "main.o"));

    cr_assert(rule_insert(&p, "clean"));
    cr_assert(dependency_insert(p.current_rule, "file.o"));

    cr_assert_str_eq(p.rules->dependencies->target, "main.o");
    cr_assert_str_eq(p.rules->next->dependencies->target, "file.o");

    rules_free(p.rules);
}

Test(command_insert, insert)
{
    struct parser p = { 0 };

    cr_assert(rule_insert(&p, "all"));
    cr_assert(command_insert(p.current_rule, "echo hello"));

    cr_assert_not_null(p.rules->commands);
    cr_assert_str_eq(p.rules->commands->target, "echo hello");
    cr_assert_null(p.rules->commands->next);

    rules_free(p.rules);
}

Test(command_insert, null_arguments)
{
    struct parser p = { 0 };

    cr_assert_not(command_insert(NULL, "echo hello"));

    cr_assert(rule_insert(&p, "all"));
    cr_assert_not(command_insert(p.current_rule, NULL));
    cr_assert_null(p.rules->commands);

    rules_free(p.rules);
}

Test(command_insert, multiple_commands)
{
    struct parser p = { 0 };

    cr_assert(rule_insert(&p, "all"));
    cr_assert(command_insert(p.current_rule, "echo first"));
    cr_assert(command_insert(p.current_rule, "echo second"));

    struct list *cmd = p.rules->commands;

    cr_assert_str_eq(cmd->target, "echo first");
    cr_assert_not_null(cmd->next);
    cr_assert_str_eq(cmd->next->target, "echo second");
    cr_assert_null(cmd->next->next);

    rules_free(p.rules);
}

Test(command_insert, different_rules)
{
    struct parser p = { 0 };

    cr_assert(rule_insert(&p, "all"));
    cr_assert(command_insert(p.current_rule, "echo all"));

    cr_assert(rule_insert(&p, "clean"));
    cr_assert(command_insert(p.current_rule, "rm -f file"));

    cr_assert_str_eq(p.rules->commands->target, "echo all");
    cr_assert_str_eq(p.rules->next->commands->target, "rm -f file");

    rules_free(p.rules);
}

Test(list_free, empty_list)
{
    list_free(NULL);
}

Test(list_free, one_element)
{
    struct parser p = { 0 };

    cr_assert(rule_insert(&p, "all"));
    cr_assert(dependency_insert(p.current_rule, "main.o"));

    struct list *dependencies = p.rules->dependencies;
    p.rules->dependencies = NULL;

    list_free(dependencies);
    rules_free(p.rules);
}

Test(list_free, many_elements)
{
    struct parser p = { 0 };

    cr_assert(rule_insert(&p, "all"));

    for (int i = 0; i < 100; i++)
        cr_assert(dependency_insert(p.current_rule, "file.o"));

    struct list *dependencies = p.rules->dependencies;
    p.rules->dependencies = NULL;

    list_free(dependencies);
    rules_free(p.rules);
}

/* RULES FREE */

Test(rules_free, empty_list)
{
    rules_free(NULL);
}

Test(rules_free, one_rule)
{
    struct parser p = { 0 };

    cr_assert(rule_insert(&p, "all"));

    rules_free(p.rules);
}

Test(rules_free, multiple_rules)
{
    struct parser p = { 0 };

    cr_assert(rule_insert(&p, "all"));
    cr_assert(rule_insert(&p, "clean"));
    cr_assert(rule_insert(&p, "check"));

    rules_free(p.rules);
}

Test(rules_free, with_dependencies_and_commands)
{
    struct parser p = { 0 };

    cr_assert(rule_insert(&p, "all"));
    cr_assert(dependency_insert(p.current_rule, "main.o"));
    cr_assert(dependency_insert(p.current_rule, "utils.o"));
    cr_assert(command_insert(p.current_rule, "gcc main.o utils.o"));

    cr_assert(rule_insert(&p, "clean"));
    cr_assert(command_insert(p.current_rule, "rm -f program"));

    rules_free(p.rules);
}

Test(rules_free, many_elements)
{
    struct parser p = { 0 };

    for (int i = 0; i < 100; i++)
    {
        cr_assert(rule_insert(&p, "target"));
        cr_assert(dependency_insert(p.current_rule, "file.o"));
        cr_assert(command_insert(p.current_rule, "echo test"));
    }

    rules_free(p.rules);
}
