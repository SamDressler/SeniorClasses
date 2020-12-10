program simpleProgram;
var 
    x, y : integer;
    result : integer;
    c : char;
BEGIN
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
end.