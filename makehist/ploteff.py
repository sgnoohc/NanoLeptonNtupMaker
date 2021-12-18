#!/bin/env python

import plottery_wrapper as p

p.dump_plot(
        fnames=["TTHIDULSF.root"],
        dirname="tthidsfplots",
        extraoptions={
            "draw_option_2d": "colztexte",
            "bin_text_format": ".3f",
            },
        )
