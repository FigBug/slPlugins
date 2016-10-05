if [ ! -f "plot.dat" ]
then
echo
echo "plot.dat does not exist!"
echo "run one of the ex_gen files to produce a plot"
echo
exit 1
fi

gnuplot wave.plt

echo "plot.png created!"