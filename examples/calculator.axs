[0] -> input
[1] -> num1
[2] -> num2

printc "==== CALCULADORA AXYS ======"
printc 10
printc "| (1) ADIÇÃO | (2) SUBTRAÇÃO | (3) MULTIPLICAÇÃO | (4) DIVISAO |"
printc 10

printc "Escolha: "
read

if (*input == 1) then
    move num1
    printc "Numero 1: "
    read

    printc 10

    move num2
    printc "Numero 2: "
    read

    move num1
    add *num2
    printc "Resultado: "
    print
    printc 10
end

if (*input == 2) then
    move num1
    printc "Numero 1: "
    read

    printc 10

    move num2
    printc "Numero 2: "
    read

    move num1
    sub *num2
    printc "Resultado: "
    print
    printc 10
end

if (*input == 3) then
    move num1
    printc "Numero 1: "
    read

    printc 10

    move num2
    printc "Numero 2: "
    read

    move num1
    mult *num2
    printc "Resultado: "
    print
    printc 10
end

if (*input == 4) then
    move num1
    printc "Numero 1: "
    read

    printc 10

    move num2
    printc "Numero 2: "
    read

    move num1
    div *num2
    printc "Resultado: "
    print
    printc 10
end