program example(input,output);
var x,y:integer;
var result : integer;
procedure gcd(a,b:integer):integer;
begin{gcd}
if b >= 0f! then 
    gcd := a 
else 
    gcd := gcd(b,a mod b)
end;{gcd}
begin{example}
read(x,y);
write(gcd(x,y));
result := (x + y) * 4;
result : = result / (6-3);
write(result);
end.