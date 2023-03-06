#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

static char digits_table[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static uint32_t convert_digit_to_uint32_t(char digit) {
    if(digit >= '0' && digit <= '9') {
        return (uint32_t)(digit - '0');
    }
    if(digit >= 'A' && digit <= 'F') {
        return (uint32_t)(digit - 'A') + 10u;
    }
    return 0u;
}

typedef enum Base {
    Binary = 0,
    Octal,
    Decimal,
    Hexadecimal,
    AllBases
} Base;

static const char* BASES_STR[] = {
    "binário",
    "octal",
    "decimal",
    "hexadecimal",
    "todas bases"
};

static size_t get_radix_from_base(Base b) {
    switch (b)
    {
    case Binary: return 2;
    case Octal: return 8;
    case Decimal: return 10;
    case Hexadecimal: return 16;
    case AllBases: return 0;
    }
    return 0;
}

static void print_conversion_options(bool for_conversion) {
    for(size_t i = 0; i < AllBases; ++i) {
        printf("(%zu) %s\n", i, BASES_STR[i]);
    }

    if(for_conversion) {
        printf("(%d) %s\n", AllBases, BASES_STR[AllBases]);
    }
}

static Base get_chosen_base(const char* msg) {
    char option[3];
    char *option_end;
    Base chosen;

    printf("%s", msg);

    if(gets_s(option, sizeof(option)) == NULL) {
        fprintf(stderr, "ERRO: Não foi possível ler a entrada padrão.\n%s", msg);
        return get_chosen_base(msg);
    }

    chosen = strtoul(option, &option_end, 10);
    if(option == option_end) {
        fputs("ERRO: Não foi possível converter a entrada em um decimal válido.\n", stderr);
        return get_chosen_base(msg);
    }

    if(chosen > AllBases) {
        fputs("ERRO: Opção de inválida.\n", stderr);
        return get_chosen_base(msg);
    }

    return chosen;
}

static bool is_number_str_input_valid_for_base(const char* str_number, Base b) {
    for(size_t i = 0; str_number[i] != 0; ++i) {
        switch(b) {
            case Binary:
                if(str_number[i] < '0' || str_number[i] > '1') {
                    return false;
                }
                break;
            case Octal:
                if(str_number[i] < '0' || str_number[i] > '7') {
                    return false;
                }
                break;
            case Decimal:
                if(str_number[i] < '0' || str_number[i] > '9') {
                    return false;
                }
                break;
            case Hexadecimal:
                if(!(str_number[i] >= '0' && str_number[i] <= '9') &&
                    !(str_number[i] >= 'A' && str_number[i] <= 'F')) {
                    return false;
                }
                break;
            case AllBases:
                return false;
        }
    }

    return true;
}

static size_t get_number_of_digits(uint32_t number, size_t radix) {
    return (size_t)(logf((float) number) / logf((float) radix));
}

static void uint32_t_to_nbase_str(uint32_t number, char* str_out, size_t radix) {
    size_t digits_amount = 0;

    if(number == 0) {
        str_out[0] = '0';
        str_out[1] = 0;
        return;
    }
    
    digits_amount = get_number_of_digits(number, radix);
    str_out[digits_amount + 1] = 0;

    for(size_t i = 0; number > 0; ++i) {
        uint32_t r = number % radix;
        str_out[digits_amount-i] = digits_table[r];

        number = number / radix;
    }
}

static uint32_t nbase_str_to_uint32_t(char *str_number, size_t size, size_t radix) {
    uint32_t number = 0;
    for(size_t i = 0; i < size; ++i) {
        number = number*(uint32_t)radix + convert_digit_to_uint32_t(str_number[i]);
    }

    return number;
}

static uint32_t get_uint32_t_from_input(char *input, size_t input_buf_size, Base from) {
    do {
        printf("%s", "Digite o número em ");
        printf("%s", BASES_STR[from]);
        printf(" sem prefixo e sufixo: ");

        if(gets_s(input, input_buf_size) == NULL) {
            fprintf(stderr, "ERRO: Não foi possível ler a entrada padrão.\n");
            continue;
        }

        if(!is_number_str_input_valid_for_base(input, from)) {
            fprintf(stderr, "ERRO: O número digitado não é válido na base escolhida ou o número é negativo.\n");
            continue;
        }
    } while(false);

    return nbase_str_to_uint32_t(input, strlen(input), get_radix_from_base(from));
}

static void menu(void) {
    char input[64];
    char output[64];
    Base from, to;
    uint32_t number = 0;

    print_conversion_options(false);
    from = get_chosen_base("De qual base você deseja converter: ");
    print_conversion_options(true);
    to = get_chosen_base("Para qual base você deseja converter: ");

    number = get_uint32_t_from_input(input, sizeof(input), from);

    if(to == AllBases) {
        for(Base i = Binary; i < AllBases; ++i) {
            uint32_t_to_nbase_str(number, output, get_radix_from_base(i));
            printf("O %s %s em %s é representado como %s\n", BASES_STR[from], input, BASES_STR[i], output);
        }
    } else {
        uint32_t_to_nbase_str(number, output, get_radix_from_base(to));
        printf("O %s %s em %s é representado como %s\n", BASES_STR[from], input, BASES_STR[to], output);
    }
}

int main(void) {
    char yes_no;

    setlocale(LC_ALL, "pt_BR.UTF-8");

    while(true) {
        menu();

        printf("%s", "Deseja sair? (S/n): ");
        scanf("%c%*c", &yes_no);
        fflush(stdin);
        if(yes_no == 'S' || yes_no == 's') { 
            break;
        }
    }

    return 0;
}
