program pr;
procedure Alpha(a : integer; b : integer);
var x : integer;
begin
   x := (a + b ) * 2;
end;
begin
	Alpha();          { 0 arguments —> ERROR }
	Alpha(1);         { 1 argument  —> ERROR }
	Alpha(1, 2, 3);   { 3 arguments —> ERROR }
end.
