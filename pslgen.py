#!/usr/bin/env python
import re
import signal
import subprocess
import os
from enum import Enum
from numpy import linspace, cos, sin

VIEWER = False


class Timeout:

    def __init__(self, seconds=1, error_message='Timeout'):
        self.seconds = seconds
        self.error_message = error_message

    def handle_timeout(self, signum, frame):
        raise TimeoutError(self.error_message)

    def __enter__(self):
        signal.signal(signal.SIGALRM, self.handle_timeout)
        signal.setitimer(signal.ITIMER_REAL, self.seconds)

    def __exit__(self, type, value, traceback):
        signal.setitimer(signal.ITIMER_REAL, 0)


class _Getch:

    def __init__(self):
        # try:
        #     self.impl = _GetchWindows()
        # except ImportError:
        self.impl = _GetchUnix()

    def __call__(self):
        return self.impl()


class _GetchUnix:

    def __init__(self):
        import tty, sys

    def __call__(self):
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch


# class _GetchWindows:
#     def __init__(self):
#         import msvcrt
#
#     def __call__(self):
#         import msvcrt
#         return msvcrt.getch()

getch = _Getch()


def timeout(sec):
    if sec is None:
        sec = 1
    try:
        with Timeout(sec):
            return getch()
    except:
        pass
    return None


CMDS = []
ACTIONS = []


class Tk(Enum):
    NULL = 0,
    HELP = 1,
    QUIT = 2,
    SAVE = 3,
    LOAD = 4,
    SHAPE = 5,
    CMD = 6
    STRING = 7,
    POINT = 8,
    LIST = 9,
    DELETE = 10,
    BOOL = 11


def parser(string):
    help_re = "((help|\\?)( \\w+)?)"
    exit_re = "((quit|exit)(\\(\\))?)"
    save_re = "((save|:w)( \\w+)?)"
    load_re = "((load|open|:o)( \\w+)?)"
    shape_re = "((shape|size)( \\d+(\\.\\d+)?)?( \\d+(\\.\\d+)?)?)"
    point_re = "((point|p)( -?\\d+(\\.\\d+)?)?( -?\\d+(\\.\\d+)?)?)"
    circle_re = "((circle|c)( -?\\d+(\\.\\d+)?)?( -?\\d+(\\.\\d+)?)?( -?\\d+(\\.\\d+)?)?( \\d+)?( t| f)?)"
    rect_re = "((rectangle|rect|r)( -?\\d+(\\.\\d+)?)?( -?\\d+(\\.\\d+)?)?( -?\\d+(\\.\\d+)?)?( -?\\d+(\\.\\d+)?)?( t| f)?)"
    tri_re = "((triangle|tri|t)( -?\\d+(\\.\\d+)?)?( -?\\d+(\\.\\d+)?)?( -?\\d+(\\.\\d+)?)?( -?\\d+(\\.\\d+)?)?( -?\\d+(\\.\\d+)?)?( -?\\d+(\\.\\d+)?)?( t| f)?)"
    list_re = "((list|l))"
    delete_re = "((delete|d)( \\d+)?)"
    view_re = "((gen|pdf|view))"
    regexes = [
        help_re, exit_re, save_re, load_re, shape_re, point_re, circle_re,
        rect_re, tri_re, list_re, delete_re, view_re
    ]
    parser = re.compile("^\s*(" + "|".join(regexes) + ")\s*$")
    result = parser.search(string)
    if result is None:
        return (None, string)
    groups = result.groups()
    if groups[1] != None:
        return ([(Tk.HELP, groups[2]), (Tk.STRING, groups[3])], string)
    if groups[4] != None:
        return ([(Tk.QUIT, groups[4])], string)
    if groups[7] != None:
        return ([(Tk.SAVE, groups[8]), (Tk.STRING, groups[9])], string)
    if groups[10] != None:
        return ([(Tk.LOAD, groups[11]), (Tk.STRING, groups[12])], string)
    if groups[13] != None:
        return ([(Tk.SHAPE, groups[14]), (Tk.POINT, groups[15]),
                 (Tk.POINT, groups[17])], string)
    if groups[19] != None:
        return ([(Tk.CMD, groups[20]), (Tk.POINT, groups[21]),
                 (Tk.POINT, groups[23])], string)
    if groups[25] != None:
        return ([(Tk.CMD, groups[26]), (Tk.POINT, groups[27]),
                 (Tk.POINT, groups[29]), (Tk.POINT, groups[31]),
                 (Tk.POINT, groups[33]), (Tk.BOOL, groups[34])], string)
    if groups[35] != None:
        return ([(Tk.CMD, groups[36]), (Tk.POINT, groups[37]),
                 (Tk.POINT, groups[39]), (Tk.POINT, groups[41]),
                 (Tk.POINT, groups[43]), (Tk.BOOL, groups[45])], string)
    if groups[46] != None:
        return ([(Tk.CMD, groups[47]), (Tk.POINT, groups[48]),
                 (Tk.POINT, groups[50]), (Tk.POINT, groups[52]),
                 (Tk.POINT, groups[54]), (Tk.POINT, groups[56]),
                 (Tk.POINT, groups[58]), (Tk.BOOL, groups[60])], string)
    if groups[61] != None:
        return ([(Tk.LIST, groups[62])], string)
    if groups[63] != None:
        return ([(Tk.DELETE, groups[64]), (Tk.POINT, groups[65])], string)
    if groups[66] != None:
        return ([(Tk.SAVE, groups[67])], string)
    return (None, string)


def highlight(string):
    token, src = parser(string)
    if token is None:
        return string
    new_str = " " * (len(string) - len(string.lstrip()))
    colors = {
        Tk.HELP: "\033[1;95m",
        Tk.QUIT: "\033[1;91m",
        Tk.SAVE: "\033[1;92m",
        Tk.LOAD: "\033[1;96m",
        Tk.SHAPE: "\033[1;93m",
        Tk.CMD: "\033[1;94m",
        Tk.STRING: "\033[32m",
        Tk.POINT: "\033[33m",
        Tk.LIST: "\033[1;95m",
        Tk.DELETE: "\033[1;91m",
        Tk.BOOL: "\033[34m"
    }
    for tk in token:
        if tk[1] is None:
            continue
        new_str += colors[tk[0]] + tk[1] + "\033[0m"
    new_str += (" " * (len(string) - len(string.rstrip())))
    return new_str


def prompt():
    global CMDS
    print("> ", end='')
    string = ""
    key = 0
    index = -1
    tmp_string = string
    while key != 13 and key not in (4, 5):
        print("\033[2K\033[G> {}".format(highlight(string)), end='', flush=True)
        key = ord(getch())
        if key == 127 and string:
            string = string[:-1]
        elif key == 27:
            ch = timeout(0.05)
            if ch:
                ch = ord(ch)
            if ch == 91:
                ch = timeout(0.05)
                if ch:
                    ch = ord(ch)
                if ch == 65:
                    if index == -1:
                        tmp_string = string
                    index = index - 1 if index > 0 else len(CMDS) - 1
                    if index == -1:
                        string = tmp_string
                    else:
                        string = CMDS[index]
                elif ch == 66:
                    if index == -1:
                        tmp_string = string
                    index = index + 1 if 0 < index < len(CMDS) - 1 else -1
                    if index == -1:
                        string = tmp_string
                    else:
                        string = CMDS[index]
        else:
            index = -1
            string += chr(key)
    print()
    CMDS.append(string)
    return string


def avg(pts):
    avg_pt = [0, 0]
    for pt in pts:
        avg_pt[0] += pt[0]
        avg_pt[1] += pt[1]
    return (avg_pt[0] / len(pts), avg_pt[1] / len(pts))


def generate_pslg():
    width = 10
    height = 10
    points = []
    edges = []
    holes = []
    for act in ACTIONS:
        if act[0] == 0:
            for pt in points:
                pt = (pt[0] / width * act[1], pt[1] / height * act[2])
            width = act[1]
            height = act[2]
        elif act[0] == 1:
            points.append((act[1], act[2]))
        elif act[0] == 2:
            pts = []
            for theta in linspace(0.0, 6.283184, act[4]):
                points.append((act[3] * cos(theta), act[3] * sin(theta)))
                pts.append(points[-1])
                edges.append((len(points) - 1, len(points)))
            if act[5]:
                holes.append(avg(pts))
            edges = edges[:-1]
        elif act[0] == 3:
            points.append((act[1], act[2]))
            points.append((act[1], act[2] + act[4]))
            points.append((act[1] + act[3], act[2]))
            points.append((act[1] + act[3], act[2] + act[4]))
            n = len(points)
            edges.append((n - 4, n - 3))
            edges.append((n - 4, n - 2))
            edges.append((n - 3, n - 1))
            edges.append((n - 2, n - 1))
            if act[5]:
                holes.append(
                    avg([points[-4], points[-3], points[-2], points[-1]]))
        elif act[0] == 4:
            points.append((act[1], act[2]))
            points.append((act[3], act[4]))
            points.append((act[5], act[6]))
            n = len(points)
            edges.append((n - 3, n - 2))
            edges.append((n - 3, n - 1))
            edges.append((n - 2, n - 1))
            if act[7]:
                holes.append(avg([points[-3], points[-2], points[-1]]))
    return (width, height), points, edges, holes


def generate_pdf():
    # print("\033[1m     Generating PDF\033[0m")
    # print("\033[1m========================\033[0m")
    size, pts, edges, holes = generate_pslg()
    min_pt = (1000000, 1000000)
    for pt in pts:
        min_pt = (min(min_pt[0], pt[0]), min(min_pt[1], pt[1]))
    for pt in holes:
        min_pt = (min(min_pt[0], pt[0]), min(min_pt[1], pt[1]))
    with open(".pslg.svg", 'w') as file:
        file.write('<svg width="{}" height="{}">'.format(
            size[0] * 100, size[1] * 100))
        file.write(
            '<rect x="0" y="0" width="{}" height="{}" fill="white" />'.format(
                size[0] * 100, size[1] * 100))
        for edg in edges:
            file.write(
                '<line x1="{}" y1="{}" x2="{}" y2="{}" stroke="black" />'.
                format(
                    int((pts[edg[0]][0] - min_pt[0]) * 100),
                    int((pts[edg[0]][1] - min_pt[1]) * 100),
                    int((pts[edg[1]][0] - min_pt[0]) * 100),
                    int((pts[edg[1]][1] - min_pt[1]) * 100)))
        for pt in pts:
            file.write('<circle cx="{}" cy="{}" r="2" color="black" />'.format(
                int((pt[0] - min_pt[0]) * 100), int((pt[1] - min_pt[1]) * 100)))
        for hl in holes:
            file.write('<circle cx="{}" cy="{}" r="3" stroke="red" fill="red" />'.format(
                int((hl[0] - min_pt[0]) * 100), int((hl[1] - min_pt[0]) * 100)))
        file.write('</svg>')
    subprocess.run(
        ['rsvg-convert', '-f', 'pdf', '-o', '.pslg.pdf', '.pslg.svg'])
    global VIEWER
    if not VIEWER:
        subprocess.Popen(['zathura', '.pslg.pdf'])
        VIEWER = True


def display_help(target):
    print("HELP:{}".format(target))


def save(file):
    print("\033[1m     Generating PSLG\033[0m")
    print("\033[1m=========================\033[0m")
    size, pts, edges, holes = generate_pslg()
    from pprint import pprint
    pprint(pts)
    pprint(edges)
    pprint(holes)


def load(file):
    print("LOAD:{}".format(file))


def fmt_action(act):
    if act[0] == 0:
        return "\033[1;93mResize\033[0m ({},{})".format(act[1], act[2])
    elif act[0] == 1:
        return "\033[1;94mPoint\033[0m ({},{})".format(act[1], act[2])
    elif act[0] == 2:
        return "\033[1;94mCircle\033[0m ({},{}) r:{} n:{} v:{}".format(
            act[1], act[2], act[3], act[4], "T" if act[5] else "F")
    elif act[0] == 3:
        return "\033[1;94mRect\033[0m ({},{}) w:{} h:{} v:{}".format(
            act[1], act[2], act[3], act[4], "T" if act[5] else "F")
    elif act[0] == 4:
        return "\033[1;94mTri\033[0m ({},{}) -- ({},{}) -- ({},{}) v:{}".format(
            act[1], act[2], act[3], act[4], act[5], act[6],
            "T" if act[7] else "F")


def print_actions():
    print("\033[1m     Action History\033[0m")
    print("\033[1m========================\033[0m")
    for i, act in enumerate(ACTIONS):
        print("\033[1;96m{}: {}".format(i + 1, fmt_action(act)))


def handle_action(tokens):
    global ACTIONS
    if tokens[0][0] == Tk.HELP:
        display_help(tokens[1][1])
    elif tokens[0][0] == Tk.SAVE and len(tokens) > 1 and tokens[1][1]:
        save(tokens[1][1])
    elif tokens[0][0] == Tk.SAVE and tokens[0][1] in ("gen", "pdf", "view"):
        generate_pdf()
    elif tokens[0][0] == Tk.LOAD and tokens[1][1]:
        load(tokens[1][1])
    elif tokens[0][0] == Tk.SHAPE and tokens[1][1] and tokens[2][1]:
        ACTIONS.append([0, float(tokens[1][1]), float(tokens[2][1])])
    elif tokens[0][0] == Tk.LIST:
        print_actions()
    elif tokens[0][0] == Tk.DELETE and tokens[1][1]:
        del ACTIONS[int(tokens[1][1]) - 1 if tokens[1][1] != '0' else 0]
    elif tokens[0][0] == Tk.CMD:
        if tokens[0][1] in ("point", "p") and tokens[1][1] and tokens[2][1]:
            ACTIONS.append([1, float(tokens[1][1]), float(tokens[2][1])])
        elif tokens[0][1] in (
                "cirlce",
                "c") and tokens[1][1] and tokens[2][1] and tokens[3][1]:
            ACTIONS.append([
                2,
                float(tokens[1][1]),
                float(tokens[2][1]),
                float(tokens[3][1]),
                int(tokens[4][1] if tokens[4][1] else 25),
                tokens[5][1] is not None and tokens[5][1] == ' t'
            ])
        elif tokens[0][1] in (
                "rectangle", "rect", "r"
        ) and tokens[1][1] and tokens[2][1] and tokens[3][1] and tokens[4][1]:
            ACTIONS.append([
                3,
                float(tokens[1][1]),
                float(tokens[2][1]),
                float(tokens[3][1]),
                float(tokens[4][1]), tokens[5][1] is not None and
                tokens[5][1] == ' t'
            ])
        elif tokens[0][1] in (
                "triangle", "tri",
                "t") and tokens[1][1] and tokens[2][1] and tokens[3][
                    1] and tokens[4][1] and tokens[5][1] and tokens[6][1]:
            ACTIONS.append([
                4,
                float(tokens[1][1]),
                float(tokens[2][1]),
                float(tokens[3][1]),
                float(tokens[4][1]),
                float(tokens[5][1]),
                float(tokens[6][1]), tokens[7][1] is not None and
                tokens[7][1] == ' t'
            ])
    if VIEWER and tokens[0][0] != Tk.SAVE:
        generate_pdf()


def main():
    global ACTIONS
    action = Tk.NULL
    while action != Tk.QUIT:
        tokens, _ = parser(prompt())
        action = tokens[0][0]
        handle_action(tokens)
    if os.path.isfile('.pslg.svg'):
        os.remove('.pslg.svg')
    if os.path.isfile('.pslg.pdf'):
        os.remove('.pslg.pdf')


if __name__ == "__main__":
    main()
