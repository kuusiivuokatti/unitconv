### config.md

The program attempts to find a file called config, first from $XDG_CONFIG_HOME/.unitconv/, then from $HOME/.unitconv/ if not specified with the -f or --file argument.


config is a semicolon-separated file with the following structure
- value;from;to;


Where value is the value that is being used as a multiplier, from is the unit we are converting from and to is the unit we are converting to. For example, we could convert from miles to kilometers and specify the following in the config file
- 1.609344;m;km;

In case a reverse conversion for is found (for example the program is called with "unitconv 1 km m"), a division operation is performed. In the above example's case program would return
- 1/1.609344
