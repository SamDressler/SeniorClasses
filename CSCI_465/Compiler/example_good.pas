program exampleGood;
var 
    x, y : integer;
    result : integer;
procedure printNumMinusThree(x : integer);
var temp : integer;
begin 
    temp := x - 3;
    writeln("Number minus three is: ");
    write(temp);
end;
begin 
    y := 10;
    write("Enter an integers: ");
    read(x);
    result := x * y;
    write(result);
end.