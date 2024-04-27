#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comp_int(const void *key, const void *data)
{
    return (*(int *)key - *(int *)data);
}

int comp_float(const void *key, const void *data)
{
    return (*(float *)key - *(float *)data);
}

int comp_double(const void *key, const void *data)
{
    return (*(double *)key - *(double *)data);
}

int comp_char(const void *key, const void *data)
{
    return (*(char *)key - *(char *)data);
}

void *m_bsearch(const void *key, const void *ptr, size_t count, size_t size, int (*comp)(const void *, const void *))
{
    int low = 0;
    int high = count;

    const char *arr = (const char *)ptr;

    while (high > low)
    {
        int middle = low + (high - low) / 2;
        const void *middle_ptr = arr + (middle * size);

        int comparision = comp(key, middle_ptr);

        if (comparision == 0)
        {
            return (void *)middle_ptr;
        }
        else if (comparision < 0)
        {
            high = middle;
        }
        else
        {
            low = middle + 1;
        }
    }
    return NULL;
}

void test_search(void *arr, size_t count, size_t size, int (*comp)(const void *, const void *), const char *type_name, const char *key_name, void *key)
{
    void *result = m_bsearch(key, arr, count, size, comp);
    if (result != NULL)
    {
        printf("%s %s found at index: %ld\n", key_name, type_name, ((char *)result - (char *)arr) / size);
    }
    else
    {
        printf("%s %s not found\n", key_name, type_name);
    }
}

int main()
{
    int int_arr[] = {1, 3, 5, 7, 9};
    float float_arr[] = {1.1, 3.3, 5.5, 7.7, 9.9};
    double double_arr[] = {1.11, 3.33, 5.55, 7.77, 9.99};
    char char_arr[] = {'a', 'b', 'c', 'd', 'e'};

    size_t int_count = sizeof(int_arr) / sizeof(int);
    size_t float_count = sizeof(float_arr) / sizeof(float);
    size_t double_count = sizeof(double_arr) / sizeof(double);
    size_t char_count = sizeof(char_arr) / sizeof(char);

    int int_key = 1;
    test_search(int_arr, int_count, sizeof(int), comp_int, "int", "Key", &int_key);

    float float_key = 5.5;
    test_search(float_arr, float_count, sizeof(float), comp_float, "float", "Key", &float_key);

    double double_key = 7.77;
    test_search(double_arr, double_count, sizeof(double), comp_double, "double", "Key", &double_key);

    char char_key = 'd';
    test_search(char_arr, char_count, sizeof(char), comp_char, "char", "Key", &char_key);

    return 0;
}
