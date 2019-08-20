# To run:
## On windows, linux
prompt> mpirun -n 2 ex5 brain.inp

## On AWS instances
 mpirun --allow-run-as-root -np 8  --mca btl_base_warn_component_unused 0  -mca btl_vader_single_copy_mechanism none ex5 brain.inp

# To Plot:
prompt> gnuplot gnuplot.script

## On windows:
-start x, at prompt type: startxwin
-cygstart plot.png