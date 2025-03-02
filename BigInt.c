#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX 309

typedef struct
{
    unsigned int num_digits[MAX];
    bool is_neg;
    int size;
} BigInt;

void subtraction(BigInt *num1, BigInt *num2, BigInt *result);

int number_of_digits_in(BigInt *num)
{
    int i = MAX - 1;
    while (i > 0 && num->num_digits[i] == 0) 
    {
        i--;
    } 
    return ++i;
}

int modulus_of_first_is_greater_than_modulus_of_second(BigInt *num1, BigInt *num2)
{
    int i = MAX - 1;
    while (i > 0 && num1->num_digits[i] == num2->num_digits[i])
    {
        i--;
    }
    return (num1->num_digits[i] - num2->num_digits[i]);
}

void initialize(BigInt *big_int, const char *str)
{
    memset(big_int, 0, sizeof(BigInt));
    big_int->size = strlen(str);
    big_int->is_neg = false;
    if (str[0] == '-')
    {
        big_int->is_neg = true;
        ++str;
        --(big_int->size);
    }

    for (int i = 0; i < big_int->size; i++)
    {
        big_int->num_digits[i] = str[big_int->size - i - 1] - '0';
    }
    big_int->size = number_of_digits_in(big_int);
}

void addition(BigInt *num1, BigInt *num2, BigInt *result)
{
    memset(result, 0, sizeof(BigInt));
    bool sign_is_different = num1->is_neg ^ num2->is_neg;
    if (sign_is_different)
    {
        BigInt temp_num1 = *num1;
        temp_num1.is_neg = !temp_num1.is_neg;
        subtraction(&temp_num1, num2, result);
        temp_num1.is_neg = !temp_num1.is_neg;
        if (modulus_of_first_is_greater_than_modulus_of_second(num1, num2) >= 0)
            result->is_neg = num1->is_neg;
        else
            result->is_neg = num2->is_neg;
    }
    else
    {
        int len1 = num1->size, len2 = num2->size;
        int i = 0, carry = 0;

        while (i < len1 && i < len2)
        {
            int sum = num1->num_digits[i] + num2->num_digits[i] + carry;
            result->num_digits[i] = sum % 10;
            carry = sum / 10;
            result->size++;
            i++;
        }

        while (i < len1)
        {
            int sum = num1->num_digits[i] + carry;
            result->num_digits[i] = sum % 10;
            carry = sum / 10;
            result->size++;
            i++;
        }

        while (i < len2)
        {
            int sum = num2->num_digits[i] + carry;
            result->num_digits[i] = sum % 10;
            carry = sum / 10;
            result->size++;
            i++;
        }

        if (carry && result->size < MAX) 
        {
            result->num_digits[i] = 1;
            result->size++;
        }

        else if (result->size >= MAX && carry)
        {
            printf("Overflow occurred during addition! Limiting result to %d digits.\n", MAX);
            result->size = MAX;
        }
        result->is_neg = num1->is_neg;
    }
}

void subtraction(BigInt *num1, BigInt *num2, BigInt *result)
{
    memset(result, 0, sizeof(BigInt));
    bool sign_is_different = num1->is_neg ^ num2->is_neg;
    BigInt temp_num2 = *num2;
    temp_num2.is_neg = !temp_num2.is_neg;

    if (sign_is_different)
    {
        addition(num1, &temp_num2, result);
        result->is_neg = temp_num2.is_neg;
    }
    else
    {
        if(num1->is_neg)
        {
            result->is_neg = true;
        }
        else
        {
            result->is_neg = false;
        }

        if (modulus_of_first_is_greater_than_modulus_of_second(num1, num2) <= 0)
        {
            
            BigInt *temp = num1;
            num1 = num2;
            num2 = temp;
            if(num1->is_neg)
            {
                result->is_neg = false;
            }
            else
            {
                result->is_neg = true;
            }
        }
        
        int borrow = 0;
        for (int i = 0; i < MAX; i++)
        {
            int diff = num1->num_digits[i] - num2->num_digits[i] - borrow;
            if (diff < 0)
            {
                borrow = 1;
                diff += 10;
            }
            else
            {
                borrow = 0;
            }
            result->num_digits[i] = diff;
        }
        result->size = number_of_digits_in(result);
    }
}

void multiplication(BigInt *num1, BigInt *num2, BigInt *result)
{
    memset(result, 0, sizeof(BigInt));
    result->is_neg = num1->is_neg ^ num2->is_neg;
    int mult_overflow = 0;

    for (int i = 0; i < num1->size; i++)
    {
        int carry = 0;
        for (int j = 0; j < num2->size; j++)
        {
            int product = num1->num_digits[i] * num2->num_digits[j] + result->num_digits[i + j] + carry;
            if ((i + j) < MAX)  
            {
                result->num_digits[i + j] = product % 10;
            }
            else if(!mult_overflow)
            {
                mult_overflow = 1;
            }
            carry = product / 10;
        }
        if (carry && (i + num2->size) < MAX)  
        {
            result->num_digits[i + num2->size] += carry;
        }
        else if(carry)
        {
            mult_overflow = 1;
        }
    }
    result->size = number_of_digits_in(result);

    if ((result->size >= MAX && mult_overflow) || mult_overflow)
    {
        printf("Overflow occurred during multiplication! Limiting result to %d digits.\n", MAX);
        result->size = MAX;
    }
}

void print(BigInt *num)
{
    if (num->is_neg)
    {
        printf("-");
    }
    for (int i = num->size - 1; i >= 0; i--)
    {
        printf("%d", num->num_digits[i]);
    }
    printf("\n");
}

int main()
{
    BigInt n1, n2, sum, diff, prod;

    char s1[MAX+1], s2[MAX+1];
    printf("Enter Number1 : ");
    scanf("%s",&s1);
    printf("Enter Number2 : ");
    scanf("%s",&s2);
    printf("\n");
    if(strlen(s1) > MAX || strlen(s2) > MAX)
    {
        printf("Please enter number of max size of 1024 bits or 309 digits!\n");
        exit(1);
    }

    initialize(&n1,s1);
    initialize(&n2,s2);

    
    int flag = 1, option;

    while(flag)
    {
        printf("List of Operations you can perform: \n");
        printf("1.Addition\n");
        printf("2.Subtraction\n");
        printf("3.Multiplication\n");
        printf("4.Exit\n");
        printf("Enter Option number: ");
        scanf("%d", &option);
        printf("\n");
        switch(option)
        {
            case 1:
                addition(&n1, &n2, &sum);
                printf("Sum: ");
                print(&sum);
                printf("\n");
                break;

            case 2:
                subtraction(&n1, &n2, &diff);
                printf("Difference: ");
                print(&diff);
                printf("\n");
                break;

            case 3:
                multiplication(&n1, &n2, &prod);
                printf("Product: ");
                print(&prod);
                printf("\n");
                break;
                
            case 4:
                flag=0;
                printf("\n");
                break;

            default:
                printf("Your Option number is invalid");
                printf("\n");
        }
    }

    return 0;
}
