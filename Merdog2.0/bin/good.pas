function int sum(ref x,ref y);
function int abs(ref x)
{
	if(x>0)
	{
		return x;
	};
	return -x;
}
program main
{
	cout(sum(1,2));
}.
function int sum(ref x,ref y)
{
	return x+y;
}
