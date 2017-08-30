# -*- coding: utf-8 -*-
import matplotlib.pyplot as splot
from os.path import join
import re


class plotable:
    """ Decorator providing a consitent interface for plotting

    The decorated function is shown to the screen or/and saved to a
    chosen directory. By providing a single interface for a script
    or even multiple scripts, it decreases the work needed to manually
    show and/or save plots for later LaTeX-handling
    """
    def __init__(self, savepath, show=True, saveas=None, *args, **kwargs):
        self.savepath = savepath
        self.show = show
        self.saveas = saveas
        self.args = args
        self.kwargs = kwargs
        self.keywords = {'savepath', 'show', 'saveas'}

    def __call__(self, func):
        def decorator(*args, **kwargs):
            fnKwargs = {k: v for k, v in kwargs.items()
                        if k not in self.keywords}
            dcKwargs = {k: v for k, v in kwargs.items()
                        if k in self.keywords}
            res = func(*args, **fnKwargs)
            if ('show' in dcKwargs and dcKwargs['show'] is True):
                splot.show()
            elif ('show' not in dcKwargs) and self.show:
                splot.show()

            if 'saveas' in dcKwargs or self.saveas is not None:
                path = dcKwargs['saveas'] if 'saveas' in dcKwargs else self.saveas
                splot.savefig(join(self.savepath, path), dpi=1200)
            return res
        return decorator


def tag(identifier, msg):
    return "<{id}>{msg}</{id}>".format(id=identifier, msg=msg)


def untag(tagged_msg):
    match = re.search(r"<(.+?)>([\s\S]*?)</\1>", tagged_msg)
    if match is None:
        raise RuntimeError("Malformed tag message: {}".format(tagged_msg))
    tag = match.group(1)
    msg = match.group(2)
    return tag, msg


def untag_all(tagged_msg):
    match = re.findall(r"<(.+?)>([\s\S]*?)</\1>", tagged_msg)
    return {tag: msg for tag, msg in match}
