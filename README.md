# Epidemiologic chart simulation

## Description

This program allows you to visualize the process of a disease and its comparison to the real data in a html chart using the following parameters

- `--real-data <path to CSV file with real data>` (feel free to use data.csv file in this repo for reference)
- `--infectious_days <int value>`
- `--infections_per_day <float value>`
- `--output <path to the output file location including the type>` (for example chart.html)

Resulting in the following output:
![output chart](https://github.com/skokcmd/Epidemiologic-simulation/blob/main/dummy_output.png?raw=true)

## How to use

1. Clone this repository
2. Go to the cloned repository and compile the program using `make` command
3. Run the following command with adjusted data
   `./epidemic-modelling --real-data data.csv --infectious_days 8 --infections_per_day 0.3 --output chart.html`

### Notes

The output can be only generated as a file with `.html` extension.
