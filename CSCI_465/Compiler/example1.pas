program simpleProgram;
var 
    arr : array [1..5] of integer;
    x, y : integer;
    result : integer;
    c : char; 
    sum : integer;  
BEGIN
    writeln("Hello Dr. Reza");
    x := 9;
    y := 6;
    c := 'L';  
    IF x > y then
    begin
        result := (x+y) * 6;
    end
    else
    begin
        result := x * 5;
    end;
    writeln('Result is : ');
    writeln(result);
    arr[1] := 10;
    arr[2] := 5;
    sum := arr[1] + arr[2];
    writeln('Enter your favorite integer: ');
    readln(x);
    writeln(x);
    writeln('\nGood Bye and ');
    writeln('Happy Holidays!');
    write(c);
end.