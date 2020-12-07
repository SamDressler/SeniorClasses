program exampleGood;
var x, y : integer;
var result : integer;
begin 
    x := 123;
    y := 10;
    write('Enter an integer: ');
    read(x);
    result := x * y + x;
    write(result);
end.