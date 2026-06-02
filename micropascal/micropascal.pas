program testeLexico;
var
	x, y: integer;
	z: real;
begin
	x := 10;
	y := 20;
	z := 3.14;

	if x < y then
		x := x + 1
	else
		x := x - 1;

	while x <= y do
	begin
		x := x + 2;
		y := y - 1;
	end;

	if x <> y then
		z := z * 2.0 / 4.0;

	if x >= y then
		x := x;
end.
