#include<stdio.h>
#include<string.h>
#define MAX_DIGITS 309


    typedef struct 
    {
    int digits[MAX_DIGITS]; // Stores the number in reverse order (LSB first)
    int length;
    int sign; // 1 for positive -1 for negative
    }BigInt;

    BigInt sub(BigInt*, BigInt*); //Function prototype

    BigInt create(char* num) {
    BigInt result;
    result.length = 0;
    result.sign = 1; // Taking default as positive

    if (num[0] == '-') {
        result.sign = -1;
        num++; // Removing the '-' sign in array by incrementing the pointer
    }

    result.length = strlen(num);

    for (int i = 0; i < result.length; i++) {
        result.digits[i] = num[result.length - i - 1] - '0';
    }

    return result;
}


    void print(BigInt *num) {
    if (num->sign == -1) {
        printf("-");
    }
    for (int i = num->length - 1; i >= 0; i--) {
        printf("%d", num->digits[i]);
    }
    printf("\n");
}

    int compare(BigInt *num1, BigInt *num2) {
        int status=0;
        if (num1->length > num2->length) 
            status=1; 
        else if (num1->length < num2->length) 
            status = -1; 
        else
        {
        for (int i = num1->length - 1; i >= 0&&status==0; i--) {
            if (num1->digits[i] > num2->digits[i]) 
                status = 1; 
          
            else if (num1->digits[i] < num2->digits[i]) 
                status = -1; 
        }
        }
        if(status==0)
        status=1;

        return status;
    }

    BigInt add(BigInt *num1, BigInt *num2)
    {
        BigInt result;
        int carry=0,i;
        if(num1->sign==num2->sign)
        {
        if(num1->length>=num2->length)
        {
            result.length=num1->length;
        }
        else
        {
            result.length=num2->length;
        }
        for(i=0;i<result.length||carry;i++)
        {   int digitSum=carry;
            if(i<num1->length) digitSum+=num1->digits[i];
            if(i<num2->length) digitSum+=num2->digits[i];
            result.digits[i]=digitSum%10;
            carry=digitSum/10;
        }
        result.length = i;
        if(num1->sign==1)
        result.sign=1;
        else
        result.sign=-1;
        }
        else{
        BigInt temp = *num2;
        temp.sign *= -1;
        result = sub(num1, &temp);
        if(compare(num1, num2) > 0)
        {
            result.sign=num1->sign;
        }
        else
        {
            result.sign=num2->sign;
        }
        }
        while (result.length > 1 && result.digits[result.length - 1] == 0) {
        result.length--;
        }   
        return result;
    }   

    BigInt sub(BigInt *num1, BigInt *num2) {
    BigInt result;
    int borrow = 0;

    if (num1->sign != num2->sign) {
        // Subtracting if one is negative is equivalent to adding
        BigInt temp = *num2;
        temp.sign *= -1;
        result = add(num1, &temp);
    } else {
        // If both numbers have the same sign
        if (compare(num1, num2) >= 0) {
            // Absolute value of num1 is greater than num2
            result.sign = num1->sign;
            for (int i = 0; i < num1->length; i++) {
                int digitDiff = num1->digits[i] - borrow;
                if (i < num2->length) digitDiff -= num2->digits[i];

                if (digitDiff < 0) {
                    digitDiff += 10;
                    borrow = 1;
                } else {
                    borrow = 0;
                }

                result.digits[i] = digitDiff;
            }
            result.length = num1->length;
        } else {
            // Absolute value of num1 is lesser than num2
            result = sub(num2, num1); // calling sub function with interchanged arguments
            result.sign = -1*num1->sign; // Resulting sign is opposite
        }
    }

    // Remove leading zeros
    while (result.length > 1 && result.digits[result.length - 1] == 0) {
        result.length--;
    }

    return result;
}

    BigInt multiply(BigInt *num1, BigInt * num2) {
    BigInt result;
    memset(result.digits, 0, sizeof(result.digits));
    int max=0;
    for (int i = 0; i < num1->length; i++) {
        int carry = 0;

        for (int j = 0; j < num2->length || carry; j++) { //multiplying each digit of num1 with each digit of num2
            int digitProduct = result.digits[i + j] + num1->digits[i] * (j < num2->length ? num2->digits[j] : 0) + carry;

            result.digits[i + j] = digitProduct % 10;
            carry = digitProduct / 10;

            if((i+j)>max)
            {
                max=i+j;
            }
        }
    }
    result.length = num1->length + num2->length;
    while(result.length!=max+1)
    {
        result.length--;
    }
    // Remove leading zeros
    while (result.length > 1 && result.digits[result.length - 1] == 0) {
        result.length--;
    }
                                               //No '-' sign if product is zero
    result.sign = ((num1->sign == num2->sign)|| (result.digits[result.length - 1] == 0)) ? 1 : -1;
    return result;
}

    int main() {
    char num1[MAX_DIGITS+2], num2[MAX_DIGITS+2]; //+1 incase of negative sign and +1 for null character

    printf("Enter first number: ");
    scanf("%s", num1);
    printf("Enter second number: ");
    scanf("%s", num2);

    if((num1[0]=='-' && strlen(num1)>MAX_DIGITS+1)||(num2[0]=='-' && strlen(num2)>MAX_DIGITS+1))
    {
        printf("Number of digits should be maximum %d\n", MAX_DIGITS);
    }
    else if((num1[0]!='-' && strlen(num1)>MAX_DIGITS)||(num2[0]!='-' && strlen(num2)>MAX_DIGITS))
    {
        printf("Number of digits should be maximum %d\n", MAX_DIGITS);
    }
    else
    {
    BigInt a = create(num1);
    BigInt b = create(num2);

    printf("Addition: ");
    BigInt sum = add(&a, &b);
    if(sum.length>MAX_DIGITS)
    {
        printf("Size overflow in addition\n");
    }
    else
    {
        print(&sum);
    }

    printf("Subtraction: ");
    BigInt diff = sub(&a, &b);
    if(diff.length>MAX_DIGITS)
    {
        printf("Size overflow in subtraction\n");
    }
    else
    {
        print(&diff);
    }

    printf("Multiplication: ");
    BigInt prod = multiply(&a, &b);
    if(prod.length>MAX_DIGITS)
    {
        printf("Size overflow in multiplication\n");
    }
    else
    {
        print(&prod);
    }
    return 0;
}
    }
   

