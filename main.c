
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <locale.h>
#include <wchar.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#define SIZE_STEP 1048576

#define pptp L"ポプテピピック"
#define src_pptp L"ポプテピック"

static gsl_rng *r = NULL;

int init_gsl()
{
    const gsl_rng_type *t = gsl_rng_default;
    if (r == NULL)
    {
        gsl_rng_env_setup();
        r = gsl_rng_alloc(t);
        if(r == NULL){
            return EXIT_FAILURE;
        }
        gsl_rng_set(r, 721);
    }
    return EXIT_SUCCESS;
}

void clean_gsl()
{
    if (r != NULL)
    {
        gsl_rng_free(r);
        r = NULL;
    }
}

wchar_t getPPTPChar()
{
    return src_pptp[(size_t)gsl_ran_flat(r, 0, 6)];
}

int main(int argc, char **argv)
{
    size_t cmp_length = wcslen(pptp);
    wchar_t *array = NULL;
    wchar_t *tmp_array_ptr = NULL;
    //wchar_t* tmp_array_ptr = NULL;
    size_t array_size = SIZE_STEP + 1;
    size_t tmp_array_size = 0;
    size_t set_offset = 0;
    size_t cmp_offset = 0;
    int find_flg = 0;

    if(init_gsl()){
        exit(EXIT_FAILURE);
    }
    setlocale(LC_ALL, "ja_JP.UTF-8");
    array = malloc(sizeof(wchar_t) * array_size);
    if (array == NULL)
    {
        perror(NULL);
        goto done;
    }
    memset(array, 0, sizeof(wchar_t) * array_size);
    wcsncpy(array, pptp, cmp_length);

    /*
    set_offset : 0 1 2 3 4 5 6 7 8 9 10
    cmp_offset : 0 0 0 0 0 0 0 1 2 3  4
    */
    for (; set_offset < cmp_length; set_offset++)
    {
        array[set_offset] = getPPTPChar();
        printf("%lc", array[set_offset]);
    }

    for (;; set_offset++, cmp_offset++)
    {
        if (wcsncmp(&(array[cmp_offset]), pptp, cmp_length) == 0)
        {
            find_flg = 1;
            goto finish;
        }
        array[set_offset] = getPPTPChar();
        printf("%lc", array[set_offset]);
        if (set_offset == array_size)
        {
#ifdef _DEBUG
            printf("realloc...\n");
#endif /* _DEBUG */
            tmp_array_size = array_size + SIZE_STEP;
            tmp_array_ptr = realloc(array, tmp_array_size);
            if (tmp_array_ptr == NULL)
            {
                /* realloc fail */
                goto done;
            }
            else
            {
                /* realloc success */
                array = tmp_array_ptr;
                array_size = tmp_array_size;
                memset(&(array[set_offset + 1]), 0, array_size - (set_offset + 1));
            }
#ifdef _DEBUG
            printf("realloc Successful\n");
#endif /* _DEBUG */
        }
    }
finish:
    if (find_flg)
    {
        printf("find!(%"PRIu64")\n", set_offset);
    }
done:
    free(array);
    clean_gsl();
    return 0;
}
