#!/usr/bin/env python3
"""Script to display PMT pulse output"""

import argparse
import os
import pathlib
import sys

import numpy as np
import matplotlib.pyplot as plt


def parse_args(args: list[str]) -> argparse.Namespace:
    """
    Parses the command-line arguments.

    Args:
        args: List of strings to parse as command-line arguments.
    Returns:
        A namespace with the parsed arguments.
    """
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument('-r', type=int, default=0, help='run number')
    parser.add_argument('-e', type=int, default=0, help='event number')

    return parser.parse_args(args=args)


def main(args: list[str] = None) -> None:
    """
    Runs the command-line interace.

    Args:
        args: List of strings to parse as command-line arguments. Defaults to sys.argv if set to None.
    """
    if args is None:
        args = sys.argv[1:]

    cli_options = parse_args(args)
    run = cli_options.r
    event = cli_options.e

    pulse_dir = pathlib.Path(f'ATLTileCalTBpulse_Run{run}/Ev{event}')

    plt.figure(f'ATLTileCalTB Run {run} Event {event}')
    plt.xlabel('global time [ns]')
    plt.ylabel('PMT output [a.u.]')
    plt.grid(True)

    for pulse_file in sorted(os.listdir(pulse_dir)):
        full_pulse_file = pulse_dir.joinpath(pulse_file)
        with open(full_pulse_file, encoding='utf-8') as real_pulse_file:
            label = real_pulse_file.readline().rstrip()[2:]
        data = np.loadtxt(full_pulse_file)

        datapoints = len(data)
        sampling = 0.5
        max_x = int(datapoints * sampling)

        plt.plot(np.linspace(0, max_x, num=datapoints), data, label=label)

    plt.legend()
    plt.tight_layout()
    plt.show()


if __name__ == '__main__':
    main()
