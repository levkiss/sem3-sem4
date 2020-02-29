program

var f1,f2,k,tmp,i : int;

begin

f1 := 1;
f2 := 1;

read(k);

for i := 1 to k do
begin
tmp := f1 + f2;
f1 := f2;
f2 := tmp;
end;

write(f1);

end

