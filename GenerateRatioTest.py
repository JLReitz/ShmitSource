#! /usr/bin/python3

value = 1
for i in range(pow(2, 4)):

    coef = []
    print_line = '<'
    # Iterate over bits in i
    for j in range(3):
        negative = ((i >> j) & 0x1) == 1

        coef.append(-value if negative is True else value)
        print_line += f'{coef[j]}, '

        if (value == 10):
            value = 1
        else:
            value = value + 1

    num1 = coef[0]
    den1 = coef[1]
    val = coef[2]

    result_num = num1
    result_den = den1 * val

    if result_den < 0 and result_num != 0:
        result_den = -result_den
        result_num = -result_num

    print_line += f'{result_num}, {result_den} > ())); /* ({num1}/{den1}) / {val} = {result_num}/{result_den} */ \\'
    print(print_line)
