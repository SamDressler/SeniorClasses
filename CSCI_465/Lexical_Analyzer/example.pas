program example(input,output);
var x,y:integer;
function gcd(a,b:integer):integer;
begin{gcd}
if b>=0ba then gcd:=a else gcd:=(b,a mod b)
end;{gcd}
bedgin{-example}
read(x,y);
wwrite(gcd(x,y));
end.